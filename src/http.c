/**
 * @file http.c
 *
 * Implementacao dos procedimentos relacionados a parsear HTTP.
 *
 * Por ora, criei um parser extremamente simples, para parsear e construir
 * mensagens HTTP de uma forma bem restrita.
 * @todo Tornar o parser mais generalizado. (MUITO TRABALHO)
 */

#include "http.h"

#include <string.h>
#include <ctype.h>


struct httpparser_t
{
  char message[BUFFER_SIZE];
  struct headers_t header;
};

struct headers_t
{
  char *method;
  char *path;
  char *http_version;

  char *host;
  char *user_agent;

  char *status;
  char *content_length;
  char *content_type;
  char *last_modified;
  char *server;
};

#define PROTOCOL "HTTP/1.0"


/** Constroi e atribui o header HTTP ao 'buf' (respeitando 'bufsize').
 *
 *  A mensagem e construida de acordo com os parametros.
 *  @todo Remover valores arbitrarios dos buffers.
 *  @return O numero de caracteres efetivamente atribuidos a 'buf'.
 */
int build_header(char* buf, size_t bufsize, int status, char* status_msg
                 char* cont_type, off_t cont_length, char* last_modif)
{
  int n;

  char status_msg[256];

  //LIDAR COM O TEMPO!!!!!!
  //strftime (timebuf)

//  n = get_status_msg(status, status_msg, 256);
  //if (n == -1)
    //strncpy(status_msg, "Unknown Status Code", 256);
      // FAZER ISSO EM UMA OUTRA FUNCAO QUE VAI ENGLOBAR ESSA E VAI
        // CONSTRUIR TAMBEM TODAS AS OUTRAS COISAS QUE ESSA FUNCAO PRECISA
          // COMO O TIME_T

  n = snprintf(buf, bufsize, "%s %d %s\r\n"
                             "Server: %s\r\n"
                             "Content-Type: %s\r\n"
                             "Content-Length: %d\r\n"
                             "Last-Modified: %s"
                             "Connection: close\r\n"
                             "\r\n",
                             PROTOCOL, status, status_msg,
                             PACKAGE_NAME,
                             cont_type,
                             cont_length,
                             last_modif);
  return n;
}



/** Atribui ao 'buf' a resposta em HTML para o erro 'status'
 *  (respeitando 'bufsize');
 */
int build_error(char* buf, size_t bufsize, int status)
{
  char status_msg[256];
  int n;

  n = get_status_msg(status, status_msg, 256);
  if (n == -1)
    strncpy(status_msg, "Unknown Status Code", 256);

  n = snprintf(buf, bufsize, "<html> <head>"
                             " <title>Error %d</title>"
                             "</head> <body>"
                             " <h2>Error %d - %s</h2>"
                             "</body> </html>",
                             status,
                             status, status_msg);
  return n;
}



/** Processa 'header', armazena o nome RELATIVO do arquivo em 'buf'
 *  (limitado por 'bufsize') e retorna se o 'header' esta incompleto ou nao.
 *
 *  @return Quantos caracteres foram efetivamente copiados para 'buf'
 *          em caso de sucesso; -1 em caso de header/buffer com erros.
 *  @warning O unico metodo suportado e GET.
 */
int get_filename(char* header, char* buf, size_t bufsize)
{
  char* start;
  char* end;
  int   filenamesize;

  if ((header == NULL) || (buf == NULL))
    return -1;

  start = header;

  while(!isblank(*start))
    start++;

  while(isblank(*start))
    start++;

  end = start;

  while(!isblank(*end))
    end++;

  filenamesize = (strlen(start) - strlen(end));

  if (filenamesize > bufsize)
    filenamesize = bufsize;

  strncpy(buf, start, filenamesize);

  return filenamesize;
}


/** Indica se o metodo 'GET' esta presente em 'header'.
 *
 *  @return Nao-zero se o metodo 'GET' esta presente, 0 se nao esta.
 */
int is_get_method(char *header)
{
  if (strncmp(header, "GET ", 4) == 0)
    return 1;
  else
    return 0;
}


/** Diz se o numero de status 'status' e um aviso de erro.
 *
 *  @return 1 se for caso de erro, 0 se for um status normal.
 */
int is_error_status(int status)
{
  switch (status)
  {
  case NOT_FOUND:
  case SERVER_ERROR:

    return 1;
    break;
  default:
    return 0;
    break;
  }
}


/** Atribui a mensagem de status correspondente ao 'status' em 'buf'
 *  (respeitando 'bufsize').
 *
 *  @return O numero de caracteres escritos em 'buf' e -1 em caso de
 *          erros.
 *  @todo adicionar suporte a mais mensagens de erro!
 */
int get_status_msg(int status, char* buf, size_t bufsize)
{
  char* msg;


  if (buf == NULL)
    return -1;

  switch(status)
  {
  case OK:
    msg = "OK";
    break;
  case NOT_FOUND:
    msg = "Not Found";
    break;
  case SERVER_ERROR:
    msg = "Server Error";
    break;

  default:
    return -1;
  }

  strncpy(buf, msg, bufsize);

  return strlen(buf);
}

