/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
*
* thread.h
************************************************/

#ifndef _THREAD_H_
#define _THREAD_H_

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>

//Llibreries del nostre sistema
#include <string.h>
#include <pthread.h>

//Llibreries pròpies
#include "server.h"

/*
    CONSTANTS
*/

#define ERROR_THREAD "ERROR al crear el Thread del client.\n"
#define SLEEP_TIME 3
#define print(x) write(1, x, strlen(x))

/*
    TIPUS PRÒPIS
*/

typedef struct {
    int id;
    pthread_t tid;
}ThreadArg;

/*
    PROCEDIMENTS & FUNCIONS
*/

void THREAD_clientThread(int fd_client);

#endif /* thread.h */
