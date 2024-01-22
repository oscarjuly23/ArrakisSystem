/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* Fremen.c
* @Proposit: Main del client Fremen
************************************************/

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Llibreries propies
#include "../../HeaderFiles/read.h"
#include "../../HeaderFiles/actionHandler.h"

/*
    CONSTANTS
*/

#define STYLE_BOLD "\033[1m"
#define WELCOME "\nBenvingut a Fremen\n"
#define ERROR_ARG "Error, número d'arguments incorrecte. Has d'indicar el fitxer de configuració!\n"
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
    ConfigFileFremen configFileFremen = READ_llegeixConfigFremen (argv[1]);
    print(WELCOME);

    while (1)
    {
      //Volem printar els inputs del usuari en negreta
      print(STYLE_BOLD);
      write(1, "\n$  ", strlen("$  "));

      //Segons la comanda que ens han introduit, farem una acció o una altre.
      ACTIONHANDLER_actionHandler(READ_readUntil(0, '\n'), configFileFremen);
    }
  } else
  {
    print(ERROR_ARG);
  }

  return 0;
}
