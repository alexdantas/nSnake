/**
 * @file client.h
 *
 * Definicao de procedimentos relacionados a servir clientes.
 */


#ifndef CLIENT_H_DEFINED
#define CLIENT_H_DEFINED


#define BUFFER_SIZE  256


/** O responsavel por fornecer o servico ao cliente.
 *
 *  Recebe a request, efetua os ajustes necessarios e envia a resposta.
 *  Cada clienthandler_t equivale a um cliente. Ou seja, cada clienthandler_t
 *  tem que possuir todos os recursos necessarios para atender um cliente
 *  independentemente entre si.
 */
struct clienthandler_t
{
  int  client;                   /**< Socket do cliente servido. */
  int  state;                    /**< Estado em que se encontra o handler */
  char request[BUFFER_SIZE * 3]; /**< Toda a request HTTP solicitada pelo cliente. */
  int  request_size;             /**< Tamanho de caracteres que 'request' suporta. */
  char filepath[BUFFER_SIZE];    /**< Localizacao do arquivo que o cliente solicitou. */
  int  filestatus;               /**< Indica se o arquivo existe ou qual erro esta associado a ele.
                                   *  Seus valores sao os mesmos da especificacao HTTP (status codes). */
  char filestatusmsg[BUFFER_SIZE]; /**< Mensagem equivalente ao status do arquivo. */
  FILE* filep;                   /**< Arquivo que o cliente pede */
  int   filesize;                /**< Tamanho do arquivo solicitado*/
  int   filesentsize;            /**< Tamanho que ja foi enviado do arquivo como um todo */
  time_t filelastm;              /**< Data de ultima modificacao do arquivo */
  char filebuff[BUFFER_SIZE];    /**< Buffer onde serao guardadas partes temporarias do arquivo */
  char  answer[BUFFER_SIZE];     /**< Header a ser enviado como resposta ao cliente, antes do arquivo */
  int   answer_size;             /**< O tamanho total do header */

  /** Quanto do arquivo (caso exista) ainda precisa ser enviado ao cliente. */
  int size_left;
  /** Quanto do arquivo (caso exista) ja foi enviado ao cliente. */
  int size_sent;

  /** Se houver algum erro relacionado ao arquivo, sua mensagem estara
   *  aqui (no formato de pagina HTML) para ser enviada ao cliente. */
  char fileerror[BUFFER_SIZE];

  int fileerrorsize;

  /** Ponteiro que vai indicar o que vai ser enviado - o erro ou o header */
  char *output;

  int need_file_chunk;           /** Flag que indica se precisa pegar um pedaco do arquivo. */
};


/** Todos os estados possiveis do clienthandler_t
 */
enum states { FINISHED, MSG_RECEIVING, FILE_PROCESSING, HEADER_SENDING, ERROR_HEADER_SENDING, FILE_SENDING, ERROR_SENDING,
                        MSG_RECEIVED,                   HEADER_SENT,    ERROR_HEADER_SENT,    FILE_SENT,     ERROR_SENT   };


int  receive_message(struct clienthandler_t* h);
int  handler_init(struct clienthandler_t* h, int sck, char* rootdir);
int  file_check(struct clienthandler_t* h, char* rootdir, int rootdirsize);
int  start_sending_msg(struct clienthandler_t* h);
int  keep_sending_msg(struct clienthandler_t* h);
int  start_sending_file(struct clienthandler_t* h);
int  stop_sending_file(struct clienthandler_t* h);
int  get_file_chunk(struct clienthandler_t* h);
void build_error_html(struct clienthandler_t* h);
int  parse_request(struct clienthandler_t* h);
int  build_header(struct clienthandler_t* h);


#endif /* CLIENT_H_DEFINED */
