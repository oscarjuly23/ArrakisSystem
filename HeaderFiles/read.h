/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
*
* read.h
************************************************/

#ifndef _READ_H_
#define _READ_H_

#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 1

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//Llibreries del nostre sistema
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
    CONSTANTS
*/

#define ERROR_FILE "Error a l'obrir fitxer de configuració, no s'ha trobat el fitxer indicat!\n"
#define ERROR_FILE_REG "Error a l'obrir fitxer de usuaris registrats al sistema.\n"
#define ERROR_PIPE "Error en crear la pipe\n"
#define ERROR_FORK "Error Fork\n"
#define PATH_LISTS "./_Lists/registeredAtreides.txt"
#define MAX_SIZE 240

#define print(x) write(1, x, strlen(x))

/*
    TIPUS PROPIS
*/

typedef struct
{
  int temps;
  char *ip;
  int port;
  char *directori;
} ConfigFileFremen;

typedef struct
{
  char *ip;
  int port;
  char *directori;
} ConfigFileAtreides;

typedef struct
{
  int numParams;
  char **token;
} Command;

typedef struct
{
  char *name;
  int codiPostal;
  int id;
} User;

typedef struct
{
  char origen[15];
  char tipus;
  char data[240];
} Trama;

typedef struct
{
  char *filename;
  int size;
  char *md5sum;
} Photo;

/*
    PROCEDIMENTS & FUNCIONS
*/

char* READ_readUntil(int fd, char end);
ConfigFileFremen READ_llegeixConfigFremen (char *fitxerEntrada);
ConfigFileAtreides READ_llegeixConfigAtreides (char *fitxerEntrada);
Command READ_splitCommand(char *action);
User* READ_listUsers();
int READ_countListUsers();
int READ_receivePhoto(int fd_desti, char *trama, char *file);
int READ_sendPhoto(int fd_client_, char *pathPhoto);
int READ_fileSize(char *filename);
char *READ_fileMD5SUM(char *filename);

#endif /* read.h */
