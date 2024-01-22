/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* actionHandler.c
* @Proposit: Módul encarregat de comprobar les comandes introduides i enviar-les al servidor.
************************************************/

//Llibreries pròpies
#include "../../HeaderFiles/actionHandler.h"

//Variables Globals
int fd_server = 0;
UserClient userClient;
Photo photo;
int t;
int e_photo = 0;
char md5[100];
char *directori;

/*
    PROCEDIMENTS & FUNCIONS
*/

/***********************************************
*
* @Finalitat: Sortir del sistema Fremen
* @Parametres:    in: trama = rebem trama
* @Retorn: -
*
************************************************/
void ACTIONHANDLER_exit_program()
{
  print(STYLE_NO_BOLD);
  // Si estavem connectats avisem al servidor que ens hem desconnectat.
  if(fd_server > 0)
  {
    Trama tramaS;
    bzero(tramaS.origen, 15);
    bzero(tramaS.data, 240);
    tramaS.tipus = '\0';

    char id[3];
    sprintf(id, "%d", userClient.id);

    //token = (char**)realloc(token, sizeof(char*) * (nArgs + 2));
    tramaS.tipus = 'Q';
    strcpy(tramaS.data, userClient.name);
    strcat(tramaS.data, "*");
    strcat(tramaS.data, id);

    write(fd_server, &tramaS , sizeof(tramaS));
    close(fd_server);
  }
  print(LOGOUT_CLIENT);
  exit(0);
}

/***********************************************
*
* @Finalitat: Sortir del sistema Fremen
* @Parametres:    in: trama = rebem trama
* @Retorn: -
*
************************************************/
void ACTIONHANDLER_clean_photos()
{
  signal(SIGALRM, ACTIONHANDLER_clean_photos);
  alarm(t);
  //print("ALARMA!!");

  char frase[100];
  sprintf(frase, "rm %s/*", directori);
  char *argv1[] = {
    "/bin/bash",
    "-c",
    //"/dev/null",
    frase,
    NULL
  };

  int pid = fork();
  switch (pid) {
    case (-1):  // ERROR
      print(ERROR_FORK);
      exit(1);
    case (0):   // CHILD PROCESS
      execvp(argv1[0], argv1);
      break;
    default:    // PARENT
      wait(0);
      break;
  }
}

/***********************************************
*
* @Finalitat: Comprovar que el número de paràmentres de la comanda introduïda són els correctes
* @Parametres:    in: numArgs = nombre de paràmetres de la comanda que ha introduit l'usuari.
                  in: correctArgs = nombre correcte dels paràmetres que ha de tenir aquesta comanda.
* @Retorn:        out: 1 = el nombre de paràmetres de la comanda és el correcte.
                  out: 0 = el nombre de paràmetres de la comanda no és el correcte.
*
************************************************/
int checkNParams(int numArgs, int correctArgs)
{
  //Segons la comanda necessitem un número de paràmentres concret
  if (numArgs < correctArgs)
  {
    print(LOW_PARAMS);
    return 0;
  } else if(numArgs > correctArgs)
  {
    print(MUCH_PARAMS);
    return 0;
  } else
  {
    return 1;
  }
}

/***********************************************
*
* @Finalitat: Comprovar que una cadena de chars és numèrica i que sigui de 5 dígits (codi postal)
* @Parametres:    in: parameter = cadena que volem comprobar.
                  in: opt = opt és 1 per CP i 0 per altres
* @Retorn:        out: 1 = tots els caràcters de la cadena són numérics i en tenim 5.
                  out: 0 = algun dels caràcters de la cadena no és numéric o no en tenim 5.
*
************************************************/
int checkNum(char *parameter, int opt)
{
  long unsigned int i;
  for (i = 0; i < strlen(parameter); i++)
  {
    if(!isdigit(parameter[i]))
    {
      print(ERROR_NUM);
      return 0;
    }
  }
  if ((opt == 1) && (i != 5))
  {
    print(ERROR_CODEP);
    return 0;
  }

  return 1;
}

/***********************************************
*
* @Finalitat: Comprovar que una cadena de chars és tota alphabetica
* @Parametres:    in: parameter = cadena que volem comprobar.
* @Retorn:        out: 1 = tots els caràcters de la cadena són alphabetics.
                  out: 0 = algun dels caràcters de la cadena no és alphabetic.
*
************************************************/
int checkString(char *parameter)
{
  for (long unsigned int i = 0; i < strlen(parameter); i++)
  {
    if(!isalpha(parameter[i]))
    {
      print(ERROR_ALPHA);
      return 0;
    }
  }
  return 1;
}

/***********************************************
*
* @Finalitat: Comprovar que una foto té extensió
* @Parametres:    in: file = nom del fitxer demanat
* @Retorn:        out: 1 = extensió vàlida
                  out: 0 = extensió inexistent o invàlida.
*
************************************************/
int checkExt(char *file)
{
  if(strstr(file, ".jpg") || strstr(file, ".jpeg") || strstr(file, ".png"))
  {
    return 1;
  }
  print(ERROR_EXT);
  return 0;
}

/***********************************************
*
* @Finalitat: Rebre i descomprimir trama de search.
* @Parametres:    in: trama = trama completa rebuda
                      codiP = Codi Postal demandat en la comanda
* @Retorn: -
*
************************************************/
void searchReceived(Trama trama, char *codiP)
{
  Trama trama1;
  char frase[300];
  char buffer[240];
  bzero(frase, 100);
  bzero(buffer, 240);

  bzero(trama1.origen, 15);
  bzero(trama1.data, 240);
  trama1.tipus = '\0';

  //print(trama.data);

  char *nPers = strtok(trama.data, "*");
  sprintf(frase, "Hi ha %s persones humanes a %s\n", nPers, codiP);
  print(frase);
  bzero(frase, 300);

  if (atoi(nPers) != 0) {
    for (int i = 0; i < atoi(nPers); i++) {

      bzero(frase, 300);

      char *token = strtok(NULL, "*");
      if(token == NULL) {
        read(fd_server, &trama1, sizeof(trama1));
        //print(trama1.data);
        print(buffer);
        bzero(buffer, 240);
        strcpy(buffer, frase);

        char *token1 = strtok(trama1.data, "*");
        strcpy(frase, token1);
        strcat(frase, " ");
        token1 = strtok(NULL, "*");
        strcat(frase, token1);
      } else
      {
        strcpy(frase, token);
        strcat(frase, " ");
        token = strtok(NULL, "*");
        strcat(frase, token);
      }
      strcat(frase, "\n");

      if ((strlen(buffer) + strlen(frase))+3  < 240)
      {
        strcat(buffer, frase);
      }
    }
    print(buffer);
  }
}

/***********************************************
*
* @Finalitat: Rebre i descomprimir trama de photo.
* @Parametres:    in: trama = trama completa rebuda
                      confiFile = fitxer de configuració del client
* @Retorn: -
*
************************************************/
void photoReceived(Trama trama, ConfigFileFremen configFile)
{
  char frase[100];
  bzero(frase, 100);

  photo.filename = strtok(trama.data, "*");
  photo.size = atoi(strtok(NULL, "*"));
  photo.md5sum = strtok(NULL, "*");
  strcpy(md5, photo.md5sum);

  bzero(frase, 100);
  sprintf(frase, "./%s/%s.jpg",configFile.directori, photo.filename);

  open(frase, O_RDWR | O_TRUNC | O_CREAT, 00666);
}
/***********************************************
*
* @Finalitat: Comprovar que seguim tenint connexió amb Atreides.
* @Parametres:    in: fd_server = FD del servidor que estavem loginats.
*                 in: buff = resposta que obtenim, si tinguessim connexió.
* @Retorn:        out: fd_server = seguim amb el mateix FD d'Atreides.
*                 out: 0 = No tenim connexió, posem el FD a '0'.
*
************************************************/
int checkConnetion(int fd_server, char *cp, ConfigFileFremen configFile)
{
  Trama trama1;
  int con = read(fd_server, &trama1, sizeof(trama1));

  if(con <= 0)
  {
    print(ERROR_CONNECTING);
    close(fd_server);
    return 0;
  }

  if (trama1.tipus == 'L')
  {
    searchReceived(trama1, cp);
  }
  else if (trama1.tipus == 'I')
  {
    print(PHOTO_SENT);
  } else if (trama1.tipus == 'F')
  {
    photoReceived(trama1, configFile);
  } else if (trama1.tipus == 'R')
  {
    e_photo = 1;
    print("No hi ha foto registrada.");
  }

  return fd_server;
}

/***********************************************
*
* @Finalitat: Comprovar que la comanda és pròpia i enviar-la al servidor.
* @Parametres:    in: nArgs = número d'arguments de la comanda.
+                 in: token = matriu que conté cada paràmetre de la comanda en una posició.
* @Retorn:        out: 1 = la comanda introduïda és pròpia del nostre sistema.
*                 out: 0 = la comanda introduïda no és pròpia del nostre sistema.
*
************************************************/
int checkOurCommand(int nArgs, char **token, ConfigFileFremen configFile, char *command)
{
  Trama tramaS;
  Trama tramaR;
  char frase[100];

  bzero(tramaS.origen, 15);
  bzero(tramaS.data, 240);
  tramaS.tipus = '\0';

  strcpy(tramaS.origen, "FREMEN");

  bzero(tramaR.origen, 15);
  bzero(tramaR.data, 240);
  tramaR.tipus = '\0';

  //strcasecmp: Compara primera paraula (primera posició de token) de forma case insensitive amb les nostres comandas pròpies
  if (!strcasecmp(token[0], "LOGIN"))
  {
    // Comprovem que els arguments de la comanda són correctes.
    if (checkNParams(nArgs, 3) && checkNum(token[2], 1) && checkString(token[1]))
    {
      // Un cop hem comprovat que la comanda es correcte farem el login al sistema
      if (fd_server > 0)
      {
        // Si ja hem fet 'login' no permetrem que pugui fer un altre cop login.
        print(LOGIN_REPEAT);

      } else {
        userClient.name = token[1];
        userClient.codiPostal = atoi(token[2]);

        tramaS.tipus = 'C';
        strcpy(tramaS.data, token[1]);
        strcat(tramaS.data, "*");
        strcat(tramaS.data, token[2]);

        fd_server = CLIENT_loginAction(configFile, tramaS);

        if (fd_server > 0)
        {
          // Després de fer la acció de login esperem a rebre el ID del servidor.
          read(fd_server, &tramaR, sizeof(tramaR));

          userClient.id = atoi(tramaR.data);
          sprintf(frase, "Benvingut %s. Tens ID %d.\n", token[1], userClient.id);
          print(frase);

          print(WELCOME_USER);

        } else
        {
          //No hem pogut connectar amb el servidor per a fer el login
          print(LOGIN_KO);
        }
      }

    } else
    {
      //Els paràmetres de la comanda 'login' no són correctes
      print(ERROR_LOGIN_ARG);
    }

    return 1;

  } else if (!strcasecmp(token[0], "SEARCH"))
  {
    if (checkNParams(nArgs, 2) && checkNum(token[1], 1))
    {

      if (fd_server > 0)
      {
        char id[3];
        sprintf(id, "%d", userClient.id);

        tramaS.tipus = 'S';
        strcpy(tramaS.data, userClient.name);
        strcat(tramaS.data, "*");
        strcat(tramaS.data, id);
        strcat(tramaS.data, "*");
        strcat(tramaS.data, token[1]);

        // Si hem iniciat sessió, li passem la comanda al servidor.
        write(fd_server, &tramaS, sizeof(tramaS));

        // Comprovem que tinguem connexió amb el servidor i rebem resposta
        fd_server = checkConnetion(fd_server, token[1], configFile);
        // TENEMOS QUE CONTAR LOS BYTES IGUAL QUE EN LAS FOTOS

      } else
      {
        //Encara no hem fet login al sistema
        print(NO_LOGIN);
      }
    } else
    {
      //Els paràmetres de la comanda 'search' no són correctes
      print(ERROR_SEARCH_ARG);
    }

    return 1;

  } else if (!strcasecmp(token[0], "SEND"))
  {
    if (checkNParams(nArgs, 2) && checkExt(token[1]))
    {
      if (fd_server > 0)
      {
        char filePath[100];
        sprintf(filePath, "%s/%s", configFile.directori, token[1]);
        int size = READ_fileSize(filePath);
        char *md5sum = READ_fileMD5SUM(filePath);

        if (!size)
        {
          print(PHOTO_NOT_FOUND);
        } else
        {
          tramaS.tipus = 'F';

          char sizePhoto[8];
          sprintf(sizePhoto, "%d", size);
          // char a = '0'+size;
          // frase[0] = a;
          // frase[1] = '\0';
          strcpy(tramaS.data, token[1]);
          strcat(tramaS.data, "*");
          //strcat(tramaS.data, frase);
          strcat(tramaS.data, sizePhoto);
          strcat(tramaS.data, "*");
          strcat(tramaS.data, md5sum);
          //print(tramaS.data);

          // Si hem iniciat sessió, li passem la comanda al servidor.
          write(fd_server, &tramaS, sizeof(tramaS));
          //sleep(1);
          bzero(tramaS.data, MAX_SIZE);

          // Un cop enviada la primera trama, enviem las següents
          tramaS.tipus = 'D';
          int fp = open(filePath, O_RDWR);
          if (fp > 0)
          {
            for (int i = 0; i < (size / MAX_SIZE); i++) {
              read(fp, &tramaS.data, MAX_SIZE);
              write(fd_server, &tramaS, sizeof(tramaS));
              bzero(tramaS.data, MAX_SIZE);
            }
            bzero(tramaS.data, MAX_SIZE);
            read(fp, &tramaS.data, (size % MAX_SIZE));
            write(fd_server, &tramaS, sizeof(tramaS));

            close(fp);
          }

          print(PHOTO_SENT);

        }
      } else
      {
        print(NO_LOGIN);
      }
    } else {
      print(ERROR_SEND_ARG);
    }
    return 1;

  } else if (!strcasecmp(token[0], "PHOTO"))
  {
    if (checkNParams(nArgs, 2) && checkNum(token[1],0))
    {
      if (fd_server > 0)
      {
        tramaS.tipus = 'P';
        strcpy(tramaS.data, token[1]);

        // Si hem iniciat sessió, li passem la comanda al servidor.
        write(fd_server, &tramaS, sizeof(tramaS));

        e_photo = 0;

        // Comprovem que tinguem connexió amb el servidor i rebem resposta
        fd_server = checkConnetion(fd_server, 0, configFile);
        //sleep(1);

        if (!e_photo) {
          bzero(frase, 100);
          sprintf(frase, "./%s/%s.jpg", configFile.directori, token[1]);

          for (int i = 0; i < ((photo.size / MAX_SIZE)+1); i++)
          {
            read(fd_server, &tramaR, sizeof(tramaR));
            int fp = open(frase, O_RDWR | O_APPEND , 00666);
            if (fp > 0)
            {
              if (i == 1) {
                write(fp, &tramaR.data, (photo.size % MAX_SIZE));
                bzero(tramaR.data, 240);
              } else {
                write(fp, &tramaR.data, sizeof(tramaR.data));
                bzero(tramaR.data, 240);
              }
            }
            close(fp);
          }

          bzero(frase, 100);
          sprintf(frase, "./%s/%s.jpg", configFile.directori, token[1]);
          // char *md5sum = READ_fileMD5SUM(frase);
          // print(md5sum);
          // print(md5);

          // if (!strcasecmp(md5sum, md5))
          // {
            print(PHOTO_DOWNLOAD);
          // } else
          // {
          //   print("ERROR md5sum no cumpleix");
          // }
        }


      } else {
        print(NO_LOGIN);
      }
    } else
    {
      print(ERROR_PHOTO_ARG);
    }

    return 1;

  } else if (!strcasecmp(token[0], "LOGOUT"))
  {
    if (checkNParams(nArgs, 1))
    {
      // Si rebem logout sortirem del sistema.
      ACTIONHANDLER_exit_program();
    } else {
      print(ERROR_LOGOUT_ARG);
    }

    return 1;

  }

  // Si no és cap comanda retornem un '0'.
  return 0;
}

/***********************************************
*
* @Finalitat: Tractar cada comanda que introdueix l'usuari al nostre sistema.
* @Parametres:    in: action = cadena que llegim que ens ha introduit l'usuari al sistema.
                  ConfigFileFremen = Fitxer de configuració llegit
* @Retorn: -
*
************************************************/
void ACTIONHANDLER_actionHandler(char *action, ConfigFileFremen configFile)
{
  t = configFile.temps;
  directori = configFile.directori;
  //Primer separem la nostre comanda en una matriu de strings per a poder tractar-la.
  Command comanda = READ_splitCommand(action);
  // Volem printar els displays del sistema sense negreta
  print(STYLE_NO_BOLD);

  // Primerament mirarem que la comanda que ens han introduit és una de les nostres.
  if(comanda.numParams > 0)
  {
    //print("\nLLEGEIXO COMANDA\n");
    if (!checkOurCommand(comanda.numParams, comanda.token, configFile, action))
    {
      // Si la comanda no es pròpia, mirarem si és una comanda del sistema.
      int pid = fork();

      switch(pid) {
          case -1:
            print(ERROR_FORK);
            break;

            // FILL
            case 0:
            // Si la comanda tampoc es del sistema no podem realitzar cap acció.
              if(execvp(comanda.token[0], comanda.token) < 0)
              {
                print(ERROR_ACTION);
                exit(0);
              }
            break;

          // PARE
          default:
            wait(0);
            break;
        }
      }
    }
}
