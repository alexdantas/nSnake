/**
 * @file server.c
 *
 * Implementacao das funcoes basicas para automatizar a inicializacao de um
 * servidor inet.
 */

#include <stdio.h>      /* perror()                                  */
#include <errno.h>      /* errno                                     */
#include <string.h>     /* memset() memcpy ()                        */
#include <unistd.h>     /* fcntl()                                   */
#include <sys/types.h>  /* socket() bind() getsockopt()              */
#include <sys/socket.h> /* socket() bind() getsockopt()              */
#include <arpa/inet.h>  /* htons() htonl ()                          */
#include <fcntl.h>      /* fcntl()                                   */
#include <netdb.h>      /* gethostbyname()                           */

#include "server.h"
#include "verbose_macro.h"


/** Efetua todo o trabalho de iniciar o servidor.
 *
 *  Essa funcao age como um modelo padrao do que deve ser feito ao iniciar
 *  um servidor.
 *  Pode-se usar cada uma dessas funcoes separadamente da maneira que
 *  se preferir.
 *
 *  @note O servidor vai possuir porta reusavel para outras conexoes,
 *        seu socket vai ser nao-bloqueante e vai funcionar na
 *        porta especificada.
 *
 *  @return Um socket pronto para conexao em sucesso, -1 em caso de erro.
 */
int server_start (int port_number)
{
  int listener;
  int retval;

  listener = new_inet_socket();
  if (listener == -1)
  {
    perror("Error at socket()");
    return -1;
  }

  LOG_WRITE("Socket Created");

  retval = set_reusable_port(listener);
  if (retval == -1)
  {
    perror("Error at setsockopt()");
    return -1;
  }

  LOG_WRITE("Reusable Port Set");

  retval = bind_inet_address (listener, port_number);
  if (retval == -1)
  {
    perror("Error at bind()");
    return -1;
  }

  printf("Address binded to port %d\n", port_number);

  listen(listener, 10);

  LOG_WRITE("Ready to accept connections");

  retval = socket_set_nonblocking(listener);
  if (retval == -1)
  {
    return -1;
  }

  LOG_WRITE("Socket set as nonblocking");

  return listener;
}


/** Cria um socket pronto voltado ao protocolo TCP/IP.
 *
 *  @return O mesmo que socket() - um socket pronto para ser usado
 *          ou -1 em caso de erro.
 */
int new_inet_socket ()
{
  int sck;

  sck = socket (PF_INET, SOCK_STREAM, 0);

  return sck;
}


/** Remove o erro 'address already in use', permitindo que diferentes
 *  conexoes usem a mesma porta.
 *
 */
int set_reusable_port (int sckt)
{
  int yes = 1;

  return (setsockopt (sckt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes)));
}



/** Inicia e efetua o bind() no endereco de um servidor para internet
 *  (segundo o protocolo TCP/IP).
 *
 *  @param sckt   O listener socket do servidor.
 *  @param port   A porta que o servidor vai aceitar conexoes.
 *
 *  @return   O mesmo que bind().
 */
int bind_inet_address (int sckt, int port)
{
  struct sockaddr_in server_addr;
  int n;

  server_addr.sin_family = AF_INET;
  server_addr.sin_port   = htons (port);
  server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  memset (&(server_addr.sin_zero), '\0', 8);

  n = bind (sckt, (struct sockaddr*) &server_addr, sizeof (server_addr));
  return n;
}



/** Guarda o endereco IP do host 'host_name' no 'buffer' - na notacao
 *  estilo "172.0.0.11". Escreve ate 'bsize' bytes.
 *
 *  @return O mesmo que gethostbyname() - 0 em caso de sucesso e -1
 *  em caso de erro.
 */
int get_ip_addr (char* buffer, size_t bsize, char* host_name)
{
  // Contem informacoes sobre o host
  struct hostent* my_host;
  struct in_addr* my_host_address;

  my_host = gethostbyname (host_name);
  if (my_host == NULL)
    return -1;

  // Finalmente pegamos o endereco IP, em network byte order
  my_host_address = (struct in_addr *)my_host->h_addr_list[0];

  // Convertemos de network byte order para string de estilo "127.0.0.1"
  strncpy (buffer, inet_ntoa (*my_host_address), bsize);

  return 0;
}



/** Sets the socket specified to non-blocking mode.
 *
 *  @todo Entender o que esses operadores bitwise fazem.
 *  @return 0 on success, -1 on error (perror() is called).
 */
int socket_set_nonblocking (int sck)
{
  int error;


  error = fcntl (sck, F_GETFL, NULL);
  if (error == -1)
  {
    perror ("Error at fcntl() on socket_set_nonblocking");
    return error;
  }

  error |= O_NONBLOCK;

  error = fcntl (sck, F_SETFL, error);
  if (error == -1)
  {
    perror ("Error at fcntl() on socket_set_nonblocking");
    return error;
  }

  return 0;
}

