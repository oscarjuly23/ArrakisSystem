/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* server.c
* @Proposit: Módul encarregat de contenir les funcions i procediments respectius al socker Servidor (Atreides) del nostre sistema.
************************************************/

//Llibreries pròpies
#include "../../HeaderFiles/server.h"

//Variables Globals
User *users; // Array de la estructura d'Usuaris
Connected *connected; // Array de la estructura de usuaris connectats
Photo photo; // Estructura de photo que s'envien client-servidor
char md5[100]; // Guarda el md5 de la photo
int nTrames; // Numero de trames rebudes (per al send)
int fd_socket;
int fd_client;
int n_clients;
int n_connected;

/*
    PROCEDIMENTS & FUNCIONS
*/

/***********************************************
*
* @Finalitat: Tancar el nostre sistema Atreides.
* @Parametres: -
* @Retorn: -
*
************************************************/
void SERVER_close_server()
{
    free(users);
    free(connected);
    close(fd_client);
    close(fd_socket);
    exit(0);
}

/***********************************************
*
* @Finalitat: Crear i configurar el nostre servidor Atreides.
* @Parametres:    in: ip = ip del servidor llegit del fitxer de configuració.
                  in: port = port del servidor llegit del fitxer de configuració.
* @Retorn:        out: 0 = Servidor inicialitzar correctament
                  out: -1 = No s'ha pogut inicialitzar el servidor
*
************************************************/
int SERVER_start_server(int port, char *ip)
{
    struct sockaddr_in s_addr; // Posteriorment configurem alguns paràmetres del Socket

    //El nostre file descriptor sense connexió
    if ((fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        print(ERROR_CREATE_SKT);
        return -1;
    }

    bzero(&s_addr, sizeof (s_addr));
    //Configurem diversos paràmetres del Socket:
    s_addr.sin_family = AF_INET;  // Familia del socket
    s_addr.sin_port = htons (port); // Nº port en Big Endian
    s_addr.sin_addr.s_addr = inet_addr(ip); // @IP

    //Indiquem a l'SO que esperem  informació per aquest port
    if (bind (fd_socket, (struct sockaddr*) &s_addr, sizeof (s_addr)) < 0) // Text d'IP a binari(ipv4/6)
    {
        print(ERROR_BIND_SKT);
        close(fd_socket);
        return -1;
    }

    //Obrim el port del socket i deixem un número de 20 com a màxim de connexions a acceptar per el nostre servidor
    if ((listen(fd_socket, 20)) < 0)
    {
      print(ERROR_LISTEN_SKT);
      close(fd_socket);
      return -1;
    }

    // Carreguem el llistat d'usuaris registrats al sistema a la memòria.
    users = READ_listUsers();
    n_clients = READ_countListUsers();
    n_connected = 0;

    return 0;
}

/***********************************************
*
* @Finalitat: Executar el nostre servidor un cop està correctament configurat
* @Parametres: -
* @Retorn: -
*
************************************************/
void SERVER_exec_server()
{
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof(c_addr);

    while (1)
    {
      //print(WAITING_CONN);

      // Creem un nou file descriptor amb la connexió activa.
      fd_client = accept(fd_socket, (struct sockaddr*) &c_addr, &c_len);

      // Farem un Thread per a cada client que es connecta al nostre servidor Atreides.
      THREAD_clientThread(fd_client);
    }

    close(fd_client);
}

/***********************************************
*
* @Finalitat: Segons el id_client agafar en nom del usuari
* @Parametres:    in: id_client = id del client connectat
* @Retorn:        out: name = Nom del usuari
*
************************************************/
char *checkName(int id_client)
{
  char *name;

  for (int a = 0; a <= n_clients; a++) {
    if (users[a].id == id_client)
    {
      name = users[a].name;
    }
  }
  return name;
}

/***********************************************
*
* @Finalitat: Manegar tots els tipus de comandes pròpies que rep el nostre servidor.
* @Parametres:    in: fd_client = FD del client que ens envia la comanda.
                      id_client = ID del client logejat
* @Retorn:        out: 0 = si la comanda rebuda és de Logout
                       1 = si la comanda rebuda no és de Logout
*
************************************************/
int SERVER_checkCommand(int fd_client_, int id_client)
{
  Trama tramaS; // Trama a enviar
  Trama tramaR; // Trama que rebem
  char frase[100]; // Frase a mostrar 'Rebut ...'

  bzero(tramaS.origen, 15);
  bzero(tramaS.data, 240);
  tramaS.tipus = '\0';

  bzero(tramaR.origen, 15);
  bzero(tramaR.data, 240);
  tramaR.tipus = '\0';

  strcpy(tramaS.origen, "ATREIDES");


  // Llegim la trama sencera en text pla que rebem.
  read(fd_client_, &tramaR, sizeof(tramaR));

  // Comprovem quina de les nostres comandas propies ens ha arribat.
  if (tramaR.tipus == 'C')
  {
    // Separem la trama amb cada paràula.
    char *nom = strtok(tramaR.data, "*");
    char *codiP = strtok(NULL, "*");

    bzero(frase, 100);
    sprintf(frase, "Rebut login %s %s\n", nom, codiP);
    print(frase);

    // Comprobarem si el client que està iniciant ja ha iniciat anteriorment al sistema
    int userRegistered = USER_checkUser(nom);

    if (!userRegistered)
    {
      // Si és un nou client li generarem un nou ID.
      id_client = USER_generateID(users, n_clients);

      //Un cop tenim ja el ID podem actualitzar la llista d'usuaris
      USER_update_listUsers(nom, codiP, id_client);

      //Ademés d'actualitzar la llista ho guardem en la memòria princial.
      if(!n_clients)
      {
        users = malloc(sizeof(User));
      } else
      {
        users = realloc(users, sizeof(User) * (n_clients + 1));
      }
      users[n_clients].name = nom;
      users[n_clients].codiPostal = atoi(codiP);
      users[n_clients].id = id_client;

      n_clients++;

    } else
    {
      // Si ja està registrat li donarem el mateix ID i no l'hem de tornar a registrar.
      id_client = userRegistered;
    }
    //Després de tenir la llista d'usuaris registrats, farem la de connectads
    connected = USER_listConnected(n_connected, nom, atoi(codiP), id_client);
    n_connected++;

    bzero(frase, 100);
    sprintf(frase, "\nAssignat a ID %d.\n", id_client);
    print(frase);

    // Aquesta trama mai donarà error. Des de la F1 hem hagut de controlar que les trames que enviem al servidor des de Fremen siguin correctes.
    // Si aquestes dades que rep Atreides són correctes, el Login sempre es fa satisfactoriament, sempre enviarem trama correcta.

    tramaS.tipus = 'O';
    char id_[3];
    sprintf(id_, "%d", id_client);
    strcpy(tramaS.data, id_);

    write(fd_client_, &tramaS, sizeof(tramaS));

    print(ANSWER);
    print(WAITING_CONN);

    return (id_client * 10) + 1;

  } else if (tramaR.tipus == 'S')
  {
    char id_[3];
    int nTrames = 0;
    char tramaAUX[240];

    // Separem la trama amb cada paràula.
    char *nom = strtok(tramaR.data, "*");
    char *id = strtok(NULL, "*");
    char *codiP = strtok(NULL, "*");

    bzero(frase, 100);
    sprintf(frase, "Rebut search %s de %s %s\n", codiP, nom, id);
    print(frase);

    // Realitzem el SEARCH del Codi Postal que ens han introduit.
    int total = 0;
    users = READ_listUsers();
    for (int i = 0; i < n_clients; i++) {
      if (users[i].codiPostal == atoi(codiP))
      {
        total++;
      }
    }

    bzero(frase, 100);
    sprintf(frase, "Feta la cerca\nHi ha %d persones humanes a %s\n", total, codiP);
    print(frase);
    bzero(frase, 100);

    tramaS.tipus = 'L';
    sprintf(tramaS.data, "%d*", total);

    for (int a = 0; a <= n_clients; a++) {
      if(users[a].codiPostal == atoi(codiP))
      {
        sprintf(id_, "%d", users[a].id);
        sprintf(frase, "%s %s\n", users[a].name, id_);
        print(frase);
        bzero(frase, 100);

        bzero(tramaAUX, 240);
        sprintf(tramaAUX, "%s*%s*", users[a].name, id_);

        if ((strlen(tramaS.data) + strlen(tramaAUX))  < 240)
        {
          strcat(tramaS.data, tramaAUX);
        } else {
          //print(tramaS.data);
          write(fd_client_, &tramaS, sizeof(tramaS));
          bzero(tramaS.data, 240);
          strcpy(tramaS.data, tramaAUX);
          nTrames++;
        }
      }
    }
    //print(tramaS.data);
    write(fd_client_, &tramaS, sizeof(tramaS));

    print(ANSWER);
    print(WAITING_CONN);

    return (id_client * 10) + 1;

  } else if (tramaR.tipus == 'F')
  {
    bzero(frase, 100);
    sprintf(frase, "./_AtreidesIMG/%d.jpg", id_client);

    // Crearem la nova foto que guardarem al servidor o reemplaçarem la que hi ha
    int fp = open(frase, O_RDWR | O_TRUNC | O_CREAT, 00666);
    if (fp > 0)
    {
      // Separem la trama amb cada paràula.

      photo.filename = strtok(tramaR.data, "*");
      photo.size = atoi(strtok(NULL, "*"));
      photo.md5sum = strtok(NULL, "*");
      strcpy(md5, photo.md5sum);

      //print(photo.md5sum);

      nTrames = (photo.size / MAX_SIZE);

      // Agafem el nom del client que ens envia la comanda
      char *name = checkName(id_client);

      // Rebem la primera trama
      bzero(frase, 100);
      sprintf(frase, "Rebut send %s de %s %d\n", photo.filename, name, id_client);
      print(frase);

    } else {
      print("ERROR al rebre photo.\n");
    }

    return (id_client * 10) + 1;

    } else if (tramaR.tipus == 'D')
    {

      bzero(frase, 100);
      sprintf(frase, "./_AtreidesIMG/%d.jpg", id_client);
      int fp = open(frase, O_RDWR | O_APPEND , 00666);

      if (fp > 0)
      {

        if (nTrames > 0) {
          write(fp, &tramaR.data, sizeof(tramaR.data));
        } else {
          write(fp, &tramaR.data, (photo.size % MAX_SIZE));
        }

        close(fp);
      } else {
        print(PHOTO_NOT_FOUND);
      }

      if (nTrames == 0) {
        bzero(frase, 100);
        sprintf(frase, "./_AtreidesIMG/%d.jpg", id_client);
        char *md5sum = READ_fileMD5SUM(frase);

        if (!strcasecmp(md5sum, md5))
        {
          bzero(frase, 100);
          sprintf(frase, "Guardada com %d.jpg\n", id_client);
          print(frase);
          print(WAITING_CONN);
        } else
        {
          print("ERROR md5sum no cumpleix");
        }
      }
      nTrames--;

      return (id_client * 10) + 1;

  } else if (tramaR.tipus == 'P')
  {
    // Agafem el nom del client que ens envia la comanda
    char *name = checkName(id_client);

    // Separem la nostre comanda en una matriu amb cada paràula.
    char *requested_id = strtok(tramaR.data, "\0");

    // Rebem la primera trama
    bzero(frase, 100);
    sprintf(frase, "Rebut photo %s de %s %d\n", requested_id, name, id_client);
    print(frase);

    char filePath[100];
    sprintf(filePath, "./_AtreidesIMG/%s.jpg", requested_id);
    int size = READ_fileSize(filePath);
    char *md5sum = READ_fileMD5SUM(filePath);

    if (!size)
    {
      print(PHOTO_NOT_FOUND);
      tramaS.tipus = 'R';
      strcat(tramaS.data, "IMATGE KO");

      print(ANSWER);
      print(WAITING_CONN);
      write(fd_client_, &tramaS, sizeof(tramaS));
    } else
    {
      tramaS.tipus = 'F';

      char sizePhoto[8];
      sprintf(sizePhoto, "%d", size);
      strcpy(tramaS.data, requested_id);
      strcat(tramaS.data, "*");
      strcat(tramaS.data, sizePhoto);
      strcat(tramaS.data, "*");
      strcat(tramaS.data, md5sum);
      //print(tramaS.data);

      write(fd_client_, &tramaS, sizeof(tramaS));
      //sleep(1);
      bzero(tramaS.data, MAX_SIZE);

      // Un cop enviada la primera trama, enviem las següents
      tramaS.tipus = 'D';
      int fp = open(filePath, O_RDWR);
      if (fp > 0)
      {
        for (int i = 0; i < (size / MAX_SIZE)+1; i++) {
            read(fp, &tramaS.data, MAX_SIZE);
            write(fd_client_, &tramaS, sizeof(tramaS));
            //sleep(0.10);
        }
        // read(fp, &tramaS.data, (size % MAX_SIZE));
        // write(fd_client_, &tramaS, sizeof(tramaS));

        close(fp);
      }
      bzero(frase, 100);
      sprintf(frase, "Enviament %s.jpg\n", requested_id);
      print(frase);

      print(ANSWER);
      print(WAITING_CONN);
    }

  } else if (tramaR.tipus == 'Q')

  {
    // Separem la trama amb cada paràula.
    char *nom = strtok(tramaR.data, "*");
    char *id = strtok(NULL, "*");

    bzero(frase, 100);
    sprintf(frase, "Rebut logout de %s %s\n", nom, id);
    print(frase);

    //Quan rebem logout, hem de borrar l'usuari de la llista de connectats
    connected = USER_removeConnected(n_connected, id_client);
    n_connected--;

    print(LOGOUT_SERVER);

    return (id_client * 10);
  }

  return (id_client * 10) + 1;
}
