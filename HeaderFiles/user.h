/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
*
* User.h
************************************************/

#ifndef _USER_H
#define _USER_H

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//Llibreries del nostre sistema
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <time.h>

//Llibreries pròpies
#include "read.h"

/*
    TIPUS PROPIS
*/

typedef struct
{
  char *name;
  int codiPostal;
  int id;
} Connected;

/*
    CONSTANTS
*/

#define FILE_NOT_FOUND "\nNo s'ha pogut trobar el fitxer.\n"
#define PATH_LISTS "./_Lists/registeredAtreides.txt"
#define print(x) write(1, x, strlen(x))

/*
    PROCEDIMENTS & FUNCIONS
*/

void USER_update_listUsers(char *nom, char *codiP, int id);
int USER_checkUser(char *nameUser);
int USER_generateID(User *users, int n_clients);
Connected *USER_listConnected(int n_connected, char *name, int codiP, int id);
Connected *USER_removeConnected(int n_connected, int id);


#endif /* Atreides.h */
