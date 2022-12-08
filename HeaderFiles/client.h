/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
*
* client.h
************************************************/

#ifndef _CLIENT_H_
#define _CLIENT_H_

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>

//Llibreries del nostre sistema
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

//Llibreries pròpies
#include "read.h"
#include "actionHandler.h"

/*
    CONSTANTS
*/

#define ERROR_CREATE_SKT "ERROR durant la creació del socket.\n"
#define ERROR_CONN_SKT "ERROR de connexió amb el servidor.\n"
#define print(x) write(1, x, strlen(x))

/*
    PROCEDIMENTS & FUNCIONS
*/

int CLIENT_loginAction(ConfigFileFremen configFile, Trama trama);

#endif /* client.h */
