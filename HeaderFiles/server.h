/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
* @author Josep Franco Rom (josep.franco)
*
*
* server.h
************************************************/

#ifndef _SERVER_H_
#define _SERVER_H_

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>

//Llibreries del nostre sistema
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Llibreries pròpies
#include "thread.h"
#include "user.h"
#include "read.h"

/*
    CONSTANTS
*/

#define ERROR_CREATE_SKT "ERROR durant la creació del socket.\n"
#define ERROR_BIND_SKT "ERROR en el bind del port.\n"
#define ERROR_LISTEN_SKT "ERROR en el listen del port.\n"
#define WAITING_CONN "Esperant connexions...\n\n"
#define LOGOUT_SERVER "Desconnectat d'Atreides.\n\n"
#define ERROR_THREAD "ERROR al crear el Thread del client.\n"
#define ANSWER "Enviada resposta\n"
#define PHOTO_NOT_FOUND "No hi ha cap foto registrada.\n"
#define NO_PHOTO "No hem rebut cap Photo des de Fremen.\n"
#define print(x) write(1, x, strlen(x))

/*
    PROCEDIMENTS & FUNCIONS
*/

void SERVER_close_server();
int SERVER_start_server(int port, char *ip);
void SERVER_exec_server();
int SERVER_checkCommand(int fd_client, int id_client);

#endif /* server.h */
