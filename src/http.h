/**
 * @file http.h
 *
 * Definicao dos procedimentos relacionados a parsear HTTP.
 */

#ifndef HTTP_H_DEFINED
#define HTTP_H_DEFINED


/* Informacoes e coisas sobre o protocolo HTTP */

/** Os status codes HTTP.
 *
 * 1xx - Informational Message
 * 2xx - Success
 * 3xx - Redirection
 * 4xx - Client Error
 * 5xx - Server Error
 */
enum status_codes
{
  NONE              = -1,

  OK                = 200,

  BAD_REQUEST       = 400,
  FORBIDDEN         = 403,
  NOT_FOUND         = 404,

  SERVER_ERROR      = 500
};



#endif /* HTTP_H_DEFINED */
