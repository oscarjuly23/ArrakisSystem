/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* Harkonen.c
* @Proposit: Escaneig i eliminació de processos actius al sistema Arrakis
************************************************/

/*
    LLIBRERIES
*/

//Llibreries del sistema
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//Llibreries del nostre sistema
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

//Llibreries propies


/*
    CONSTANTS
*/
#define WELCOME "\nStarting Harkonen . . .\n"
#define SCAN "\nScanning pids . . .\n"
#define ERROR_ARG "Error, número d'arguments incorrecte\n"
#define ERROR_PIPE "Error en crear la pipe\n"
#define ERROR_FORK "Error Fork\n"
#define FOUND "Fremen executant-se:\n"
#define FINAL "\nExiting...\n"
#define print(x) write(1, x, strlen(x))

/***********************************************
*
* @Finalitat: Tancar el nostre sistema Atreides.
* @Parametres: -
* @Retorn: -
*
************************************************/
void close_harkonen()
{
    print(FINAL);
    exit(0);
}

/*
    MAIN
*/

// arg[1] = Temps entre cada KILL
int main(int argc, char *argv[])
{
  if (argc == 2)
  {
    //Quan rebem SIGINT volem prendre aquest nou comportament.
    signal(SIGINT, close_harkonen);
    print(WELCOME);

    print(SCAN);

    while (1)
    {
      char buffer[256];
      char frase[100];
      time_t t;
      int fd[2]; // Pipe entrada/sortida
      int p = 0;
      int pids_fremen[100];

      char *args[] = {
        "/bin/bash",
        "-c",
        "ps -u oscar.julian | grep Fremen | awk {'print$1'}",
        NULL
      };

      // Per a passar la resposta del fill al pare farem una pipe
      if (pipe(fd) == -1) {
        print(ERROR_PIPE);
        exit(-1);
      }

      int pid = fork();
      switch (pid) {
        case (-1):  // ERROR
          print(ERROR_FORK);
          exit(1);
        case (0):   // CHILD PROCESS
          //To capture the output of the child process, its standard output must instead be routed into the pipe. This can be arranged using the dup2 command
          dup2(fd[1], 1);
          close(fd[0]);
          //close(fd[1]);
          execvp(args[0], args);
          break;
        default:    // PARENT
          wait(0);
          close(fd[1]);
          read(fd[0], buffer, 256);
          break;
      }
      //print(buffer);

      char *first = strtok(buffer, "\n");
      if(first == NULL) {
        close_harkonen();
      }
      pids_fremen[0] = atoi(first);
      p++;

      while (1) {
        first = strtok(NULL, "\n");
        if(first == NULL) {
          break;
        } else {
          pids_fremen[p] = atoi(first);
          p++;
        }
      }

      /* Intializes random number generator */
      srand((unsigned) time(&t));
      int j = rand() % p;

      sprintf(frase, "kill -2 %d", pids_fremen[j]); // -9 = SIGKILL no es pot reconfigurar --> Li enviem un SIGINT
      char *argv1[] = {
        "/bin/bash",
        "-c",
        frase,
        NULL
      };

      int pid2 = fork();
      switch (pid2) {
        case (-1):  // ERROR
          print(ERROR_FORK);
          exit(1);
        case (0):   // CHILD PROCESS
          execvp(argv1[0], argv1);
          break;
        default:    // PARENT
          wait(0);
          sprintf(frase, "killing pid %d\n", pids_fremen[j]);
          print(frase);
          print(SCAN);
          break;
      }

      bzero(buffer, 256);
      sleep(atoi(argv[1]));
    }
  } else
  {
    print(ERROR_ARG);
  }

  return 0;
}
