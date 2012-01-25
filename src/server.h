/**
 * @file server.h
 *
 * Definicao das funcoes basicas para automatizar a inicializacao de um
 * servidor inet.
 */


#ifndef SERVER_H_DEFINED
#define SERVER_H_DEFINED


int server_start (int port_number);
int new_inet_socket ();
int set_reusable_port (int sckt);
int bind_inet_address (int sckt, int port);
int get_ip_addr (char* buffer, size_t bsize, char* host_name);
int socket_set_nonblocking (int sck);


#endif /* SERVER_H_DEFINED */
