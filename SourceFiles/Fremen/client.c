/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* client.c
* @Proposit: Módul encarregat de contenir les funcions i procediments respectius al socket Client (Fremen) del nostre sistema.
************************************************/

//Llibreries pròpies
#include "../../HeaderFiles/client.h"

/*
    PROCEDIMENTS & FUNCIONS
*/

/***********************************************
*
* @Finalitat: Controlar la connexió al servidor Atreides.
* @Parametres:    in: ip = ip del servidor llegit del fitxer de configuració.
                  in: port = port del servidor llegit del fitxer de configuració.
* @Retorn: Retorna la resposta de connexió amb el servidor Atreides.
*
************************************************/
int connect_to_server(char *ip, int port)
{
  struct sockaddr_in s_addr;  // Posteriorment configurem alguns paràmetres del Socket
  int socket_conn = -1;

  //El nostre file descriptor sense connexió
  if ((socket_conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
      print(ERROR_CREATE_SKT);

  } else
  {
      memset(&s_addr, 0, sizeof(s_addr));
      s_addr.sin_family = AF_INET;
      s_addr.sin_port = htons(port);
      s_addr.sin_addr.s_addr = inet_addr(ip);

      //Excecutem el Three Way Handshake si el destí ens ho permet.
      if (connect(socket_conn, (void *) &s_addr, sizeof(s_addr)) < 0)
      {
          print(ERROR_CONN_SKT);
          close(socket_conn);
          socket_conn = -1;
      }
  }

  return socket_conn;
}

/***********************************************
*
* @Finalitat: Realitzar el Login del usuari en el sistema.
* @Parametres:    in: configFile = estructura de dades de la configuració del servidor.
                      trama = trama que hem de enviar a Atreides.
* @Retorn:  Retorna el FD del servidor al qui li fem el connect.
*
************************************************/
int CLIENT_loginAction(ConfigFileFremen configFile, Trama trama)
{
  int fd_server = connect_to_server(configFile.ip, configFile.port);
  //print("\nCONNEXIO SERVER\n");

  if (fd_server >= 0)
  {
    //Reassignem el Ctrl+C (SIGINT)
    signal(SIGINT, ACTIONHANDLER_exit_program);
    signal(SIGALRM, ACTIONHANDLER_clean_photos);
    alarm(configFile.temps);

    // Enviem la trama sencera en text pla al servidor.
    //print("\nESCRIC TRAMA AL SERVER\n");
    write(fd_server, &trama, sizeof(trama));
  }

  return fd_server;
}
