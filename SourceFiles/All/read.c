/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* read.c
* @Proposit: Módul encarregat de contenir les funcions i procediments respectius a la lectura de fitxers o comandes.
************************************************/

//Llibreries pròpies
#include "../../HeaderFiles/read.h"


/*
    PROCEDIMENTS & FUNCIONS
*/

/***********************************************
*
* @Finalitat: Llgir fins a un caràcter dessitjat.
* @Parametres:    in: fd = filedescriptor del que volem llegir.
                  in: end = caràcter fins al qual volem llegir.
* @Retorn: Retorna cadena de caràcters que hem llegit del fd fins al char 'end'.
*
************************************************/
char* READ_readUntil(int fd, char end)
{
    int i = 0, size;
    char c = '\0';
    char *string = (char*)malloc(sizeof(char));

    while (1)
    {
        size = read(fd, &c, sizeof(char));

        if (c != end && size > 0)
        {
            string = (char*)realloc(string, sizeof(char) * (i + 2));
            string[i++] = c;
        } else
        {
            break;
        }
    }

    string[i] = '\0';
    return string;
}

/***********************************************
*
* @Finalitat: Llegir tota la informació de configuració que tenim en el fitxer de configuració de Fremen.
* @Parametres:    in: fitxerEntrada = nom del fitxer de configuració de Fremen.
* @Retorn: Retorna l'estructura amb la informació del fitxer de configuració de Fremen guardada.
*
************************************************/
ConfigFileFremen READ_llegeixConfigFremen (char *fitxerEntrada)
{
  int fd = open(fitxerEntrada, O_RDONLY);
  ConfigFileFremen configFileFremen;

  //Comprovem que s'hagi trobat el fitxer
  if (fd > 0)
  {
    //LLegim fins a trobar un salt de línia
    char *n = READ_readUntil(fd, '\n');
    configFileFremen.temps = atoi(n);

    n = READ_readUntil(fd, '\n');
    configFileFremen.ip = n;

    n = READ_readUntil(fd, '\n');
    configFileFremen.port = atoi(n);

    n = READ_readUntil(fd, '\n');
    configFileFremen.directori = n;

    close(fd);
    //free(n);

  } else {
    print(ERROR_FILE);
    exit(-1);
  }

  //Retornem la configuració que hem llegit del fitxer
  return configFileFremen;
}

/***********************************************
*
* @Finalitat: Llegir tota la informació de configuració que tenim en el fitxer de configuració de Atreides.
* @Parametres:    in: fitxerEntrada = nom del fitxer de configuració de Atreides.
* @Retorn: Retorna la estructura amb la informació del fitxer de configuració de Atreides guardada.
*
************************************************/
ConfigFileAtreides READ_llegeixConfigAtreides (char *fitxerEntrada)
{
  int fd = open(fitxerEntrada, O_RDONLY);
  ConfigFileAtreides configFileAtreides;

  //Comprovem que s'hagi trobat el fitxer
  if (fd > 0)
  {
    //LLegim fins a trobar un salt de línia
    char *n = READ_readUntil(fd, '\n');
    configFileAtreides.ip = n;

    n = READ_readUntil(fd, '\n');
    configFileAtreides.port = atoi(n);

    n = READ_readUntil(fd, '\0');
    configFileAtreides.directori = n;

    close(fd);
    free(n);

  } else
  {
    print(ERROR_FILE);
    exit(-1);
  }

  //Retornem la configuració que hem llegit del fitxer
  return configFileAtreides;
}

/***********************************************
*
* @Finalitat: Dividir la comanda que ens entra en una matriu de strings
* @Parametres:    in: action = string amb la comanda que introdueix l'usuari
* @Retorn: Retorna la comanda separada en una matriu i tabé el número d'arguments que té.
*
************************************************/
Command READ_splitCommand(char *action)
{

  Command comanda;
  char *action2 = strdup(action);
  comanda.numParams = 0;
  comanda.token = (char**)malloc(sizeof(char*));

  //Utilitzem 'strtok' per a separar el input del nostre usuari, cada paraula en una posició de la nostre matriu 'token'
  comanda.token[comanda.numParams] = strtok(action2, " ");

  while (comanda.token[comanda.numParams] != NULL)
  {
    comanda.numParams++;
    comanda.token = (char**)realloc(comanda.token, sizeof(char*) * (comanda.numParams+1));
    comanda.token[comanda.numParams] = strtok(NULL, " ");
  }
  // Un cop arribat aquest punt ja tenim omplenada la matriu token amb una paraula en cada posició
  return comanda;
}

/***********************************************
*
* @Finalitat: Llegir la llista de usuaris registrats al sistema.
* @Parametres: -
* @Retorn: Retorna la llista d'Users emplenada amb els registrats del .txt
*
************************************************/
User* READ_listUsers ()
{
  User *users;
  char c = ' ';

  int fd = open(PATH_LISTS, O_RDONLY | O_CREAT, 77777);
  int size = read(fd, &c, sizeof(char));

  //Comprovem que s'hagi trobat el fitxer
  if (fd > 0)
  {
    // Comprovem que l'arxiu té algun camp que llegir
    if (size > 0)
    {
      int i = 0;
      users = malloc(sizeof(User));
      users[i].name = READ_readUntil(fd, ' ');
      users[i].codiPostal = atoi(READ_readUntil(fd, ' '));
      users[i].id = atoi(READ_readUntil(fd, '\n'));

      while (1)
      {
        // Comprovem que queden més usuaris per llegir.
        size = read(fd, &c, sizeof(char));
        if (size > 0)
        {
          i++;
          users = realloc(users, sizeof(User) * (i+1));
          users[i].name = READ_readUntil(fd, ' ');
          users[i].codiPostal = atoi(READ_readUntil(fd, ' '));
          users[i].id = atoi(READ_readUntil(fd, '\n'));
        } else
        {
          // Si ens quedem snese users sortim del bucle.
          break;
        }
      }
    }

    close(fd);

  } else
  {
    print(ERROR_FILE_REG);
    exit(-1);
  }

  return users;
}

/***********************************************
*
* @Finalitat: Actualitzar cuants clients hi han registrats a Atreides.
* @Parametres: -
* @Retorn: Retorna el número total de clients registrats.
*
************************************************/
int READ_countListUsers ()
{
  char c = ' ';
  int i = 0;

  int fd = open(PATH_LISTS, O_RDONLY | O_CREAT, 77777);
  int size = read(fd, &c, sizeof(char));

  //Comprovem que s'hagi trobat el fitxer
  if (fd > 0)
  {
    // Comprovem que l'arxiu té algun camp que llegir
    if (size > 0)
    {
       READ_readUntil(fd, '\n');

      while (1)
      {
        // Comprovem que queden més usuaris per llegir.
        size = read(fd, &c, sizeof(char));
        if (size > 0)
        {
          i++;
          READ_readUntil(fd, '\n');
        } else
        {
          i++;
          // Si ens quedem snese users sortim del bucle.
          break;
        }
      }
    }

    close(fd);

  } else
  {
    print(ERROR_FILE_REG);
    exit(-1);
  }

  return i;
}

/***********************************************
*
* @Finalitat:
* @Parametres:    in:
* @Retorn:        out: 1 =
*
************************************************/
int READ_fileSize(char *filename)
{
  // Primer de tot mirarem que existeixi la foto
  int fp = 0;
  char c = ' ';
  int size = 0;
  int sizePhoto = 0;

  fp = open(filename, O_RDONLY);

  if (fp > 0)
  {
    // Mirarem el size total de la imatge
    while (1)
    {
      size = read(fp, &c, sizeof(char));
      if (size > 0)
      {
        sizePhoto++;
      } else
      {
        break;
      }
    }
    close(fp);
  }
  return sizePhoto;
}

/***********************************************
*
* @Finalitat:
* @Parametres:    in:
* @Retorn:        out: 1 =
*
************************************************/
char *READ_fileMD5SUM(char *filename)
{
  int fd[2]; // Pipe entrada/sortida
  char frase[100];
  int j = 0;
  char c = '\0';
  char* md5sum = (char*)malloc(sizeof(char));

  // Primer de tot mirarem que existeixi la foto
  int fp = open(filename, O_RDONLY);

  if (fp > 0)
  {
    if (pipe(fd) == -1) {
      print(ERROR_PIPE);
      exit(-1);
    }

    sprintf(frase, "md5sum %s", filename);
    char *args[] = {
      "/bin/bash",
      "-c",
      frase,
      "| awk {'print$1'}",
      NULL
    };

    int pid = fork();
    switch (pid) {
      case (-1):  // ERROR
        print(ERROR_FORK);
        exit(1);
      case (0):   // CHILD PROCESS
        //To capture the output of the child process, its standard output must instead be routed into the pipe. This can be arranged using the dup2 command
        dup2(fd[1], 1);
        close(fd[0]);
        execvp(args[0], args);
        break;
      default:    // PARENT
        wait(0);
        close(fd[1]);
        for (int i = 0; i <= 32; i++) {
          read(fd[0], &c, sizeof(char));
          md5sum = (char*)realloc(md5sum, sizeof(char) * (j + 2));
          md5sum[i] = c;
          j++;
        }
        md5sum[j - 1] = '\0';
        close(fd[0]);
        break;
    }
    close(fp);
  }

  //print(md5sum);
  return md5sum;
}
