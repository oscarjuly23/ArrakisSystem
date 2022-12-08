/***********************************************
*
* Pràctica 'Arrakis System'
* Sistemes Operatius, Curs 2021-22
*
* @author Oscar Julian Ponte (oscar.julian)
*
* user.c
* @Proposit: Módul encarregat de contenir les funcions i procediments respectius al control d'usuaris del sistema.
************************************************/

//Llibreries pròpies
#include "../../HeaderFiles/user.h"

//Variables Globals
Connected *connected;

/*
    PROCEDIMENTS & FUNCIONS
*/

/***********************************************
*
* @Finalitat: Guardar en el fitxer els usuaris que s'han connectat algun cop (registrat) al sistema.
* @Parametres:  in: token = comanda de login completa que ha introduit l'usuari
* @Retorn: -
*
************************************************/
void USER_update_listUsers(char *nom, char* codiP, int id)
{
  int file = open (PATH_LISTS, O_WRONLY | O_APPEND | O_CREAT, 00666 );

  if (file == -1)
  {
      print(FILE_NOT_FOUND);
  } else
  {
    char aux[128] = "";
    int bytes = sprintf(aux, " %s %s %d\n", nom, codiP, id);

    write(file, aux, bytes);
    }

  close(file);
}

/***********************************************
*
* @Finalitat: Comrpovar i generar un ID únic per cada client
* @Parametres:    in: *users = Array de usuaris registrats
                  in: n_clients = numero de clients registrats
* @Retorn:        out: id_client = ID únic per al client
*
************************************************/

int USER_generateID(User *users, int n_clients)
{
  time_t t;
  // Generarem un número random entre 100 i 899 per al ID de cada client
  srand((unsigned) time(&t));
  int id_client = rand() % (899);
  id_client = id_client + 100;

  //Hem de comproar que l'ID creat no existeix
  for (int i = 0; i < n_clients+1; i++)
  {
    if (id_client == users[i].id)
    {
      USER_generateID(users, n_clients);
    }
  }

  return id_client;
}

/***********************************************
*
* @Finalitat: Comrpovar si l'usuari ja està registrat
* @Parametres:    in: nameUser = nom del usuari que inicia sessió
* @Retorn:        out: 1 = Usuari nou
*                 out: 0 = Usuari registrat
*
************************************************/
int USER_checkUser(char *nameUser)
{
    char *current;
    int fd = open (PATH_LISTS, O_RDONLY );

    if (fd == -1)
    {
        print(FILE_NOT_FOUND);
    } else
    {
      int id = 0;
      //Llegim el primer nom del fitxer
      READ_readUntil(fd, ' ');

      //Si existeix comprobem
      while (strlen(current = READ_readUntil(fd, ' ')) > 0)
      {
          //Si trobem que un nom coincideix li assignarem el mateix ID (sigui minuscula o majuscula)
          if (!strcasecmp(current,nameUser))
          {
            READ_readUntil(fd, ' ');
            current = READ_readUntil(fd, ' ');

            id = atoi(current);

            close(fd);
            free(current);
            return id;
          }
      }
    }

    free(current);
    close(fd);

  return 0;
}

/***********************************************
*
* @Finalitat: Afegeix l'usuari a la llista d'usuaris connectats
* @Parametres:    in: n_connectes = numero d'usuaris connectats
*                 in: name = nom del usuari que es connecta
*                 in: codiP = codiPostal del usuari que es connecta
*                in: id = ID del usuari que es connecta
* @Retorn: Retorna la llista d'usuaris connectats actualitzada
*
************************************************/
Connected *USER_listConnected(int n_connected, char *name, int codiP, int id)
 {
   if (!n_connected)
   {
     connected = malloc(sizeof(Connected));
   } else
   {
     connected = realloc(connected, sizeof(Connected) * (n_connected+1));
   }
   connected[n_connected].name = name;
   connected[n_connected].codiPostal = codiP;
   connected[n_connected].id = id;

   return connected;
 }

  /***********************************************
 *
 * @Finalitat: Esborra un element de la strucura d'usuaris connectats
 * @Parametres:    in: array = llista d'usuaris connectats
                   in: size = numero d'usuaris connectat
                   in: a = Index on esta l'element que es vol eliminar
 * @Retorn: Retorna la llista d'usuaris connectats actualitzada
 *
 ************************************************/
 Connected* remove_element(Connected* array, int size, int a)
{
  Connected* temp = malloc((size - 1) * sizeof(Connected));

    if (a != 0)
    {
      memcpy(temp, array, a * sizeof(Connected));
    }

    if (a != (size - 1))
    {
      memcpy(temp+a, array+a+1, (size - a - 1) * sizeof(Connected));
    }

    free (array);
    return temp;
}

 /***********************************************
 *
 * @Finalitat: Esborra l'usuari de la llista d'usuaris connectats quan fa logout
 * @Parametres:    in: n_connectes = numero d'usuaris connectats
                   in: id = ID del usuari que es desconnecta
 * @Retorn: Retorna la llista d'usuaris connectats actualitzada
 *
 ************************************************/
 Connected *USER_removeConnected(int n_connected, int id)
  {
    for (int i = 0; i < n_connected+1; i++)
    {
      if (connected[i].id == id)
      {
        connected = remove_element(connected,n_connected,i);
      }
    }

    return connected;
  }
