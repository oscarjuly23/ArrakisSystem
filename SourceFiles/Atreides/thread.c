/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* thread.c
* @Proposit: Módul encarregat de contenir les funcions i procediments respectius als threads del sistema.
************************************************/

//Llibreries pròpies
#include "../../HeaderFiles/thread.h"

/*
    PROCEDIMENTS & FUNCIONS
*/

/***********************************************
*
* @Finalitat: Cada Thread (cada client) enviarà accions al nostre servidor.
* @Parametres:    in: arg = arguments del thread
* @Retorn: -
*
************************************************/

static void* threadFunc(void *arg)
{
  int state = 1;
  ThreadArg* aux = (ThreadArg*) arg;
  ThreadArg threadArg = *aux;

  while((state%10) == 1)
  {
    // Esperem a rebre una comanda d'algun client. Acabarem quan tinguem un LOGOUT
    state = SERVER_checkCommand(threadArg.id,(state/10));
  }

  close(threadArg.id);

  return 0;
}

/***********************************************
*
* @Finalitat: Crear un Thread per a cada client Fremen que es connecti al servidor Atreides.
* @Parametres:    in: fd_client = FD del client al que li donem un Thread del sistema.
* @Retorn: -
*
************************************************/
void THREAD_clientThread(int fd_client)
{
  ThreadArg arg;
  arg.id = fd_client;

  int estat = pthread_create(&arg.tid, NULL, threadFunc, (void*) &arg);
  sleep(SLEEP_TIME);

  if (estat < 0)
  {
    print(ERROR_THREAD);
  }
}
