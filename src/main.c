/**
 * @file main.c
 *
 * A logica e funcao principal do programa.
 */

#include <stdio.h>
#include <stdlib.h>     /* atoi()                                    */
#include <string.h>     /* memset()                                  */
#include <errno.h>      /* errno                                     */
#include <unistd.h>     /* fcntl()                                   */
#include <fcntl.h>      /* fcntl() O_RDWR                            */
#include <netdb.h>      /* gethostbyname() send() recv()             */
#include <sys/stat.h>   /* stat() S_ISDIR()                          */
#include <limits.h>     /* realpath()                                */

#include "client.h"
#include "server.h"
#include "http.h"
#include "verbose_macro.h"

#define MAX_CLIENTS  10
#define BUFFER_SIZE  256




/** Lida com os argumentos passados pela linha de comando: port 'port number'
 *  e 'root directory'.
 */
int handle_args(int argc, char* argv[])
{
  int port;

  if (argc != 3)
  {
    printf("Usage: servw [port_number] [root_directory]\n");
    return -1;
  }

  port = atoi(argv[1]);
  if ((port < 0) || (port > 65535))
  {
    printf("Invalid port number %d! Choose between 0 and 65535!\n", port);
    return -1;
  }

  return 0;
}


/** Retorna o maior numbero, 'a' ou 'b'.
 */
int bigger(int a, int b)
{
  if (a > b)
    return a;
  else
    return b;
}


/** Adiciona 'orig' a string 'dest' a partir do final, respeitando o limite
 *  de 'size'.
 *
 *  @note 'size' e o tamanho TOTAL de 'dest' - nao pode ser o tamanho restante!
 */
int append(char* dest, char* orig, size_t size)
{
  int origsize = strlen(orig);
  int destsize = strlen(dest);
  int remaining = size - destsize;

  if (origsize < remaining)
  {
    strncat(dest, orig, remaining);
    return 0;
  }
  else
    return -1;
}

int find_crlf(char* where)
{
  if (where == NULL)
    return -1;
  if (strstr(where, "\r\n\r\n") == NULL)
    return 0;
  else
    return 1;
}


int get_http_status_msg(int status, char* buff, size_t buffsize)
{
  switch (status)
  {
  case OK:
    strncpy(buff, "OK", buffsize);
    break;

  case BAD_REQUEST:
    strncpy(buff, "Bad Request", buffsize);
    break;
  case FORBIDDEN:
    strncpy(buff, "Forbidden", buffsize);
    break;
  case NOT_FOUND:
    strncpy(buff, "Not Found", buffsize);
    break;

  case SERVER_ERROR:
    strncpy(buff, "Server Error", buffsize);
    break;

  default:
    strncpy(buff, "Unknown Status Code", buffsize);
    return -1;
  }

  return 0;
}


/** Cria um daemon atraves de fork(), 'matando' o processo pai e atribuindo
 *  stdout para 'logfile' e stderr para 'errfile'.
 *
 *  Caso haja um erro, essa funcao interrompe a execucao do programa e
 *  exibe a mensagem de erro em stderr.
 *  Isso porque ela depende do perror() especifico de cada funcao chamada.
 *
 */
void daemonize(FILE* logfile, char* logname, FILE* errfile, char* errlogname)
{
  pid_t pid;
  pid_t sid;
  int   retval;


  pid = fork();
  if (pid == -1)
  {
    perror("Error at fork()");
    exit(EXIT_FAILURE);
  }

  if (pid > 0)
    exit(EXIT_SUCCESS);

  logfile = freopen(logname, "w", stdout);
  if (logfile == NULL)
  {
    perror("Error at freopen()");
    exit(EXIT_FAILURE);
  }

  errfile = freopen(errlogname, "w", stderr);
  if (logfile == NULL)
  {
    perror("Error at freopen()");
    exit(EXIT_FAILURE);
  }

  retval = dup2(STDIN_FILENO, open("/dev/null", O_RDWR));
  if (retval == -1)
    perror("Error at dup2()");

  sid = setsid();
  if (sid == -1)
  {
    perror("Error at setsid()");
    exit(EXIT_FAILURE);
  }
}


int main(int argc, char *argv[])
{
  FILE *logfile = NULL;
  FILE *errfile = NULL;

  int  port_number;
  char rootdir[BUFFER_SIZE];
  int  rootdirsize;

  int  listener    = -1;
  int  maxfds;
  int  max_clients = MAX_CLIENTS;
  int  cur_clients = 0;

  struct clienthandler_t* handler[max_clients];
  fd_set readfds;
  fd_set writefds;
  fd_set clientfds;

  int retval;
  int i;


  retval = handle_args(argc, argv);
  if (retval == -1)
    exit (EXIT_FAILURE);


  /* Inicializar daemon */
  //daemonize(logfile, "servw.log", errfile, "servwERR.log");

  /* Inicializar servidor */
  port_number = atoi(argv[1]);
  /*
  if (rootdir[rootdirsize] == '/')
    rootdir[rootdirsize] = '\0'; */

  memset(rootdir, '\0', BUFFER_SIZE);
  rootdirsize = BUFFER_SIZE;
  if (argv[2][0] == '/')
  {
    strncpy(rootdir, argv[2], rootdirsize - 1);
    rootdirsize -= strlen(argv[2]);
  }
  else
  {
    strncpy(rootdir, getenv("PWD"), rootdirsize - 1);
    rootdirsize -= strlen (getenv("PWD"));
    rootdir[strlen(rootdir)] = '/';
    rootdirsize--;
    strncat(rootdir, argv[2], rootdirsize - 1);
    rootdirsize -= strlen (argv[2]);
  }

  if (realpath(rootdir, rootdir) == NULL)
  {
    perror("Error at realpath()");
    exit(EXIT_FAILURE);
  }

  printf("Root directory set to: %s\n", rootdir);

  listener = server_start(port_number);
  if (listener == -1)
    exit(EXIT_FAILURE);


  /* Inicializar select() */
  /* Depois garantir que (max_clients < FD_SETSIZE) */
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&clientfds);

  maxfds = listener;


  /* Inicializar clienthandlers */
  for (i = 0; i < max_clients; i++)
    handler[i] = NULL;

  LOG_WRITE("Inicializacao completa!");


  /* Main Loop */
  while (1)
  {
    readfds  = clientfds;
    writefds = clientfds;

    FD_SET  (listener, &readfds);

    retval = select(maxfds + 1, &readfds, &writefds, NULL, NULL);

    /* nova conexao */
    if (FD_ISSET (listener, &readfds))
    {
      LOG_WRITE("\n *** \nTem um cliente tentando se conectar!");
      int new_client;

      if (cur_clients < max_clients)
      {
        new_client = accept(listener, NULL, NULL);

        if (new_client == -1)
          perror("Erro em accept()");
        else
        {
          for (i = 0; i < max_clients; i++)
            if (handler[i] == NULL)
              break;

          handler[i] = malloc(sizeof(struct clienthandler_t));
          retval = handler_init(handler[i], new_client, rootdir);
          if (retval != -1)
          {
            cur_clients++;
            maxfds = bigger(new_client, maxfds);
            FD_SET(new_client, &clientfds);
            LOG_WRITE("Nova conexao de cliente aceita!");
          }
        }
      }
    }


    /* Maquina de estados dos clienthandlers */
    for (i = 0; i < max_clients; i++)
    {
      if (handler[i] == NULL)
        continue;

      switch (handler[i]->state)
      {
      case MSG_RECEIVING:
        /** @todo @bug @warning
         *  Caso o cliente conecte mas tenha um 'lag', o FD_ISSET do readfds
         *  nao vai dar.
         *  Porem, se o cliente terminar de mandar a mensagem, vai acontecer
         *  a mesma coisa.
         *  Como diferenciar o fato de que o cliente pode estar numa conexao
         *  lenta com o fato de que o cliente pode ter terminado de enviar?
         */
        if (FD_ISSET(handler[i]->client, &readfds))
        {
          retval = receive_message(handler[i]);
          if (retval == -1)
          {
            LOG_WRITE("Erro de conexao com cliente!");
            handler[i]->state = FINISHED;
          }
          if (retval == 1)
          {
            LOG_WRITE("Cliente desconectou");
            handler[i]->state = FINISHED;
          }
        }
        else
        {
          if (find_crlf(handler[i]->request))
            handler[i]->state = MSG_RECEIVED;

        }
        break;

      case MSG_RECEIVED:
        LOG_WRITE("Mensagem recebida!");
        handler[i]->request_size = strlen(handler[i]->request);
        handler[i]->state = FILE_PROCESSING;
        break;

      case FILE_PROCESSING:
        LOG_WRITE("Processando request...");
        retval = parse_request(handler[i]);
        if (retval == -1)
        {
          // TODO TODO TODO LIDAR COM COISAS ESTRANHAS
        }
        retval = file_check(handler[i], rootdir, strlen(rootdir));
        if (retval == 0)
        {
          build_header(handler[i]);
          handler[i]->output = handler[i]->answer;
          handler[i]->state = HEADER_SENDING;
        }
        else
        {
          build_error_html(handler[i]);
          build_header(handler[i]);
          handler[i]->output = handler[i]->answer;
          handler[i]->state = ERROR_HEADER_SENDING;
        }

        start_sending_msg(handler[i]);
        LOG_WRITE("Arquivo processado!");
        break;

      case HEADER_SENDING:
        LOG_WRITE("Enviando Headers...");
        if (FD_ISSET(handler[i]->client, &writefds))
        {
          retval = keep_sending_msg(handler[i]);
          if (retval == 0)
            handler[i]->state = HEADER_SENT;
          if (retval == -1)
            handler[i]->state = FINISHED;
        }
        break;

      case HEADER_SENT:
        LOG_WRITE("Header enviado");
        handler[i]->output = handler[i]->filebuff;
        start_sending_file(handler[i]);
        handler[i]->state = FILE_SENDING;
        handler[i]->need_file_chunk = 1;
        handler[i]->filesentsize = 0;
        break;

      case FILE_SENDING:
        if (FD_ISSET(handler[i]->client, &writefds))
        {
          LOG_WRITE("Enviando arquivo");

          if (handler[i]->need_file_chunk == 1)
          {
            retval = get_file_chunk(handler[i]);
            if (retval == -1)
            {
              LOG_WRITE("Erro na leitura do arquivo!");
              handler[i]->state = FINISHED;
              break;
            }

            start_sending_msg(handler[i]);
            handler[i]->need_file_chunk = 0;
          }

          retval = keep_sending_msg(handler[i]);
          if (retval == -1)
          {
            LOG_WRITE("Erro de conexao!");
            handler[i]->state = FINISHED;
            break;
          }
          if (retval == 0)
          {
            handler[i]->need_file_chunk = 1;
            LOG_WRITE("Peguei pedaco do arquivo");
          }
          else //sending
            handler[i]->filesentsize += retval;

          if (handler[i]->filesentsize >= handler[i]->filesize)
            handler[i]->state = FILE_SENT;
        }
        break;

      case FILE_SENT:
        LOG_WRITE("Arquivo enviado");
        stop_sending_file(handler[i]);
        handler[i]->state = FINISHED;
        break;

      case ERROR_HEADER_SENDING:
        if (FD_ISSET(handler[i]->client, &writefds))
        {
          LOG_WRITE("Enviando Header de erro");
          retval = keep_sending_msg(handler[i]);
          if (retval == 0)
            handler[i]->state = ERROR_HEADER_SENT;
          else if (retval == -1)
            handler[i]->state = FINISHED;
          else
          {
            //sending...
          }
        }
        break;

      case ERROR_HEADER_SENT:
        LOG_WRITE("Header de erro enviado");
        handler[i]->output = handler[i]->fileerror;
        start_sending_msg(handler[i]);
        handler[i]->state = ERROR_SENDING;
        break;

      case ERROR_SENDING:
        if (FD_ISSET(handler[i]->client, &writefds))
        {
          LOG_WRITE("Enviando arquivo de erro");
          retval = keep_sending_msg(handler[i]);
          if (retval == 0)
            handler[i]->state = ERROR_SENT;
          if (retval == -1)
            handler[i]->state = FINISHED;

          handler[i]->fileerrorsize -= retval;

          if (handler[i]->fileerrorsize <= 0)
            handler[i]->state = ERROR_SENT;
        }
        break;

      case ERROR_SENT:
        LOG_WRITE("Arquivo de erro enviado");
        handler[i]->state = FINISHED;
        break;

      case FINISHED:
        FD_CLR(handler[i]->client, &clientfds);
        if (maxfds == handler[i]->client)
        {
          //~ printf("Vixe");
          //~ int j, a = 0;
          //~ for (j = 0; j < max_clients; j++)
          //~ {
            //~ if ((handler[j] != NULL) && (handler[j] != handler[i]))
              //~ a = handler[j]->client;
//~
            //~ maxfds = bigger(a, maxfds);
          //~ }
          maxfds = listener;
        }
        close(handler[i]->client);

        cur_clients--;
        free(handler[i]);
        handler[i] = NULL;
        LOG_WRITE("Cliente desconectou\n");
        break;

      default:
        break;
      }
    }
  }

  return 0;
}
