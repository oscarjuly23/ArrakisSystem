/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* Atreides.c
* @Proposit: Main del server Atreides
************************************************/

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>

//Llibreries del nostre sistema
#include <string.h>
#include <signal.h>

//Llibreries propies
#include "../../HeaderFiles/read.h"
#include "../../HeaderFiles/server.h"

/*
    CONSTANTS
*/
#define WELCOME "\nSERVIDOR ATREIDES\nLlegit el fitxer de configuració\n"
#define ERROR_ARG "Error, número d'arguments incorrecte\n"
#define ERROR_CREATE_SKT "ERROR durant la creació del socket.\n"
#define print(x) write(1, x, strlen(x))

/*
    MAIN
*/

// arg[1] = Fitxer de text de configuració
int main(int argc, char *argv[])
{
  //Comprovem que ens passen l'arxiu de configuració per arguments.
  if (argc == 2)
  {
    //Llegim fitxer de text de configuració
    ConfigFileAtreides ConfigFileAtreides = READ_llegeixConfigAtreides (argv[1]);

    print(WELCOME);

    //Obrim el nostre Server Atreides
    if (SERVER_start_server(ConfigFileAtreides.port, ConfigFileAtreides.ip) < 0)
    {
      print(ERROR_CREATE_SKT);
      exit(-1);
    }

    //Quan rebem SIGINT volem prendre aquest nou comportament.
    signal(SIGINT, SERVER_close_server);
    print("Esperant connexions...\n\n");

    //Un cop ho tenim tot a punt executem el nostre servidor Atreides.
    SERVER_exec_server();

  } else
  {
    print(ERROR_ARG);
  }

  return 0;
}
