/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
*
* actionHandler.h
************************************************/

#ifndef _ACTIONHANDLER_H_
#define _ACTIONHANDLER_H_

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

//Llibreries del nostre sistema
#include <string.h>
#include <strings.h>

//Llibrerias pròpies
#include "read.h"
#include "client.h"

/*
    CONSTANTS
*/

#define STYLE_NO_BOLD "\033[22m"
#define ERROR_LOGIN_ARG "ERROR en la comanda, els paràmetres de la comanda no són correctes.\n"
#define ERROR_SEARCH_ARG "ERROR en la comanda, --> SEARCH <CodiPostal>\n"
#define ERROR_PHOTO_ARG "ERROR en la comanda, --> PHOTO <ID>\n"
#define ERROR_SEND_ARG "ERROR en la comanda, --> SEND <filename.ext>\n"
#define ERROR_LOGOUT_ARG "ERROR en la comanda, --> LOGOUT\n"
#define LOW_PARAMS "Comanda KO. Falta paràmetres\n"
#define MUCH_PARAMS "Comanda KO. Massa paràmetres\n"
#define ERROR_ACTION "Introdueix una acció vàlida. No s'ha pogut executar la comanda.\n"
#define ERROR_CODEP "El Codi Postal ha de tenir 5 xifres!\n"
#define ERROR_NUM "El paràmetre ha de ser numéric!\n"
#define ERROR_ALPHA "El nom només pot contenir lletres!\n"
#define ERROR_EXT "Has de posar una extensió correcte.\n"
#define LOGIN_REPEAT "Ja has iniciat sessió!\n"
#define LOGIN_KO "No s'ha pogut fer Login\n"
#define NO_LOGIN "Has d'iniciar sessió abans!\n"
#define ERROR_CONNECTING "ERROR Atreides no podem trobar Atreides.\nHem tancat la teva sessió, sisplau torna a iniciar.\n"
#define WELCOME_USER "Ara estas connectat a Atreides.\n"
#define PHOTO_SENT "Foto enviada amb èxit a Atreides.\n"
#define PHOTO_NOT_FOUND "No hi ha cap foto registrada.\n"
#define PHOTO_DOWNLOAD "Foto descarregada\n"
#define LOGOUT_CLIENT "Desconnectat d'Atreides. Dew!\n\n"
#define NO_PHOTO "No hem rebut cap Photo des de Atreides.\n"
#define MAX_MD5SUM 32
#define print(x) write(1, x, strlen(x))

typedef struct
{
  char *name;
  int codiPostal;
  int id;
} UserClient;

/*
    PROCEDIMENTS & FUNCIONS
*/

void ACTIONHANDLER_actionHandler(char *action, ConfigFileFremen configFileFremen);
void ACTIONHANDLER_exit_program();
void ACTIONHANDLER_clean_photos();

#endif /* actionHandler.h */
