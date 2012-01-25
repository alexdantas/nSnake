/**
 * @file client.c
 *
 * Implementacao das funcoes relacionadas ao clienthandler.
 */

#include <stdio.h>
#include <stdlib.h>     /* atoi() realpath()                         */
#include <string.h>     /* memset()                                  */
#include <errno.h>      /* errno                                     */
#include <unistd.h>     /* fcntl()                                   */
#include <fcntl.h>      /* fcntl() O_RDWR                            */
#include <netdb.h>      /* gethostbyname() send() recv()             */
#include <sys/stat.h>   /* stat() S_ISDIR()                          */
#include <limits.h>     /* realpath()                                */

#include "client.h"
#include "http.h"
#include "verbose_macro.h"

#define H_ANSWER     0
#define H_ERROR      1
#define PROTOCOL     "HTTP/1.0"

#define PACKAGE_NAME PACKAGE"/"VERSION

/** Recebe a mensagem atraves de recv() de uma maneira nao-bloqueante
 *
 *  @return 0 caso a mensagem esteja sendo recebida, -1 em caso de erro
 *          e 1 se a mensagem terminou de ser recebida.
 */
int receive_message(struct clienthandler_t* h)
{
  char buffer[BUFFER_SIZE];
  int  buffer_size = BUFFER_SIZE;
  int  retval;

  retval = recv(h->client, buffer, buffer_size, 0);
  if (retval == -1)
    if ((errno != EWOULDBLOCK) && (errno != EAGAIN))
      return -1;

  if (retval == 0)
    return 1;

  buffer[retval] = '\0';
  append(h->request, buffer, h->request_size);

  return 0;
}


/** Inicializa o clienthandler_t 'h' e associa seus servicos ao cliente
 *  representado por 'clientsckt'.
 *
 *  Nessa funcao utiliza-se alocacao dinamica de memoria para criar um
 *  novo clienthandler_t.
 *  @return 0 em caso de sucesso, -1 em caso de erro ao efetuar malloc().
 *  @note Essa funcao supoe que o socket 'clientsckt' esta devidamente
 *        conectado ao cliente.
 */
int handler_init(struct clienthandler_t* h, int sck, char* rootdir)
{
  if (h == NULL)
    return -1;

  h->client = sck;
  h->state = MSG_RECEIVING;

  memset(&(h->request),       '\0', BUFFER_SIZE);
  memset(&(h->answer),        '\0', BUFFER_SIZE);
  memset(&(h->fileerror),     '\0', BUFFER_SIZE);
  memset(&(h->filepath),      '\0', BUFFER_SIZE);
  memset(&(h->filestatusmsg), '\0', BUFFER_SIZE);

  h->request_size = BUFFER_SIZE * 3;
  h->output = NULL;
  h->filep  = NULL;

  h->answer_size = -1;

  strncpy(h->filepath, rootdir, BUFFER_SIZE);
  h->filestatus = -1;
  h->filesize   = -1;

  return 0;
}


/** Checa se o arquivo descrito dentro de 'h' existe ou nao e atribui
 *  a 'h' a mensagem de erro correspondente.
 *
 *  @return 0 caso nao haja erro e -1 em algum erro
 */
int file_check(struct clienthandler_t* h, char* rootdir, int rootdirsize)
{
  struct stat st;
  char   errormsg[BUFFER_SIZE];
  int    errornum;
  int    retval;


  if (realpath(h->filepath, h->filepath) == NULL)
  {
    perror("Error at realpath()");
    return -1;
  }

  if (strncmp(h->filepath, rootdir, rootdirsize) != 0)
  {
    sprintf(h->filestatusmsg, "Forbidden");
    h->filestatus = FORBIDDEN;
    return -1;
  }

  retval = stat(h->filepath, &st);
  if (retval == -1)
  {
    switch (errno)
    {
    case EACCES:
      sprintf(errormsg, "Search permission denied on this directory");
      errornum = FORBIDDEN;
      break;
    case ENOTDIR:
      sprintf(errormsg, "A component of the path is not a directory");
      errornum = BAD_REQUEST;
      break;
    case ENOMEM:
      sprintf(errormsg, "Out of Memory");
      errornum = SERVER_ERROR;
      break;
    default:
      sprintf(errormsg, "File Not Found");
      errornum = NOT_FOUND;
      break;
    }
    sprintf(h->filestatusmsg, "%s", errormsg);
    h->filestatus = errornum;

    return -1;
  }

  if (S_ISDIR(st.st_mode))
  {
    int length = strlen(h->filepath);

    if (h->filepath[length] != '/')
    {
      append(h->filepath, "/", BUFFER_SIZE);
    }
    append(h->filepath, "index.html", BUFFER_SIZE);

    //AGORA PRECISAMOS CHECAR NOVAMENTE!
    //TODO TODO TODO BUG TODO

    retval = stat(h->filepath, &st);
    if (retval == -1)
    {
      switch (errno)
      {
      case EACCES:
        sprintf(errormsg, "Search permission denied on this directory");
        errornum = FORBIDDEN;
        break;
      case ENOTDIR:
        sprintf(errormsg, "A component of the path is not a directory");
        errornum = BAD_REQUEST;
        break;
      case ENOMEM:
        sprintf(errormsg, "Out of Memory");
        errornum = SERVER_ERROR;
        break;
      default:
        sprintf(errormsg, "File Not Found");
        errornum = NOT_FOUND;
        break;
      }
      sprintf(h->filestatusmsg, "%s", errormsg);
      h->filestatus = errornum;

      return -1;
    }
  }

  // TODO TODO TODO TODO
  // Checar por arquivo fora do range
  h->filestatus = OK;
  sprintf(h->filestatusmsg, "OK");
  h->filesize   = st.st_size;
  h->filelastm  = st.st_mtime;

  return 0;
}


int start_sending_msg(struct clienthandler_t* h)
{
  if (h->output == NULL)
    return -1;

  h->size_left = strlen(h->output);
  h->size_sent = 0;

  return 0;
}


/** Continua enviando para h->client a mensagem apontada por h->output
 *  atraves de sockets nao-bloqueantes.
 *
 *  @return O numero de caracteres enviados em caso de sucesso.
 *          Se houver algum erro fatal, retorna -1. Se o socket for
 *          bloquear, retorna -2.
 */
int keep_sending_msg(struct clienthandler_t* h)
{
  int retval = send(h->client, h->output + h->size_sent, h->size_left, 0);

  if (retval == -1)
  {
    if ((errno != EWOULDBLOCK) && (errno != EAGAIN))
    {
      LOG_WRITE("Erro de conexao!");
      return -1;
    }
    return -2;
  }
  else
  {
    h->size_sent += retval;
    h->size_left -= retval;
  }

  return retval;
}


/** Abre o arquivo em 'h' para ser enviado
 *
 *  @return 0 em sucesso, -1 em caso de erro.
 */
int start_sending_file(struct clienthandler_t* h)
{
  h->filep = fopen(h->filepath, "r");
  if (h->filep == NULL)
    return -1;

  return 0;
}


/** Fecha o arquivo aberto para o 'h'.
 *
 *  @return 0 em sucesso e -1 em caso de erro.
 */
int stop_sending_file(struct clienthandler_t* h)
{
  int retval = fclose(h->filep);
  if (retval == EOF)
    return -1;
  h->filep = NULL;
  return 0;
}


/** Pega um pedaco do arquivo e guarda no buffer dentro de 'h'.
 *
 *  @return 0 se pegar todo o pedaco do arquivo de uma vez, 1 se o arquivo
 *          terminou de ser lido e -1 em caso de erro.
 */
int get_file_chunk(struct clienthandler_t* h)
{
  int retval;

  if (h->filep == NULL)
    return -1;

  memset(&(h->filebuff), '\0', BUFFER_SIZE);
  retval = fread(h->filebuff, sizeof(char), BUFFER_SIZE - 1, h->filep);

  h->size_left = strlen(h->filebuff);
  h->size_sent = 0;

  if (retval < BUFFER_SIZE - 1)
  {
    if (feof(h->filep))
      return 1;

    if (ferror(h->filep))
    {
      perror("Error at ferror()");
      return -1;
    }
  }
  return 0;
}


/** Constroi e armazena em 'h->fileerror' uma pagina HTML contendo
 *  o erro ocorrido.
 */
void build_error_html(struct clienthandler_t* h)
{
  get_http_status_msg(h->filestatus, h->filestatusmsg, BUFFER_SIZE);

  snprintf(h->fileerror, BUFFER_SIZE,
           "<html>\n<head>\n<title>Error %d</title>\n</head>\n"
           "<body>\n<h3>Error %d - %s</h3>\n</body>\n</html>",
           h->filestatus, h->filestatus, h->filestatusmsg);

  h->fileerrorsize = strlen(h->fileerror);
}


/** Separa as partes uteis da request HTTP enviada pelo usuario.
 *
 *  @todo Por enquanto so mexemos com filename. Implementar metodo e versao
 *  @return 0 se tudo der certo, -1 caso a request contenha algum metodo
 *          nao-implementado.
 */
int parse_request(struct clienthandler_t* h)
{
  char buff[BUFFER_SIZE];
  char *method;
  char *filename;
  char *http_version;

  strncpy(buff, h->request, BUFFER_SIZE);

  method = strtok(buff, " ");
  filename = strtok(NULL, " ");
  http_version = strtok(NULL, "\r\n");

  if (strcmp(method, "GET") != 0)
    return -1;
  if ((strcmp(http_version, "HTTP/1.0") != 0) &&
      (strcmp(http_version, "HTTP/1.1") != 0))
    return -1;

  strncat(h->filepath, filename, BUFFER_SIZE - strlen(h->filepath));

  return 0;
}


/** Constroi e atribui o header HTTP ao 'buf' (respeitando 'bufsize').
 *
 *  A mensagem e construida de acordo com os parametros.
 *  @todo Remover valores arbitrarios dos buffers.
 *  @return O numero de caracteres efetivamente atribuidos a 'buf'.
 */
int build_header(struct clienthandler_t* h)
{
  int n;

  //~ char last_modif[BUFFER_SIZE];
  //LIDAR COM O TEMPO!!!!!!
  //strftime (timebuf)


  if (h->filestatus == OK)
  {
    n = snprintf(h->answer, h->answer_size, "%s %d %s\r\n"
                                            "Server: %s\r\n"
                                            "Content-Type: %s\r\n"
                                            "Content-Length: %d\r\n"
                                            //~ "Last-Modified: %s"
                                            "Connection: close\r\n"
                                            "\r\n",
                                            PROTOCOL, h->filestatus, h->filestatusmsg,
                                            PACKAGE_NAME,
                                            "text/html",
                                            h->filesize);
  }
  else
  {
    n = snprintf(h->answer, h->answer_size, "%s %d %s\r\n"
                                            "Server: %s\r\n"
                                            "Content-Type: %s\r\n"
                                            "Content-Length: %d\r\n"
                                            "Connection: close\r\n"
                                            "\r\n",
                                            PROTOCOL, h->filestatus, h->filestatusmsg,
                                            PACKAGE_NAME,
                                            "text/html",
                                            h->fileerrorsize);
  }

  return n;
}
