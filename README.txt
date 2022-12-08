Pràctica 'Arrakis System'
Sistemes Operatius, Curs 2021-22
@author Oscar Julian Ponte (oscar.julian)

Estructura:
El projecte consta d'un makefile i està estructurat en diferents carpetes:
  - ConfigFiles: Conté els arxius de confiuració tant de Fremen com d'Atreides, consta de diferents Fremen per als diferents clients.
    També tenim dos d'Atreides i de Fremen1 per si falla el socket no haver d'esperar que es reinicii i provar un altre port.
  - HeaderFiles: Conté tots els fitxers de capçalera del projecte.
  - SourceFiles: Conté tots els arxius de codi font del projecte, estructurats en carpetes per al client Fremen, per al server Atreides, i també per Harkonen.Tenim els compartits dins la carpeta All.
  - _Lists: Conté les llistes que requereixen el sistema, com la d'usuaris registrats a Atreides (la llista es crea un cop executem Atreides).
  - _AtreidesIMG: Carpeta de continguts d'Atreides. En aquest cas emmagatzema les imatges que rep.
  - _FremenIMG: Amb carpetes separades per a cada Fremen, conté les imatges que tenen i que poden demanar cada client. (Ja hem posat les imatges d'exemple en els clients)

Execució:
  1. Connectar-nos al servidor 'Montserrat' de LaSalle.
  2. Accedir a la carpeta on tinguem guardat l'arxiu 'G8_F4.tar' entregat.
  3. Descomprimir arxiu 'tar' des de la mateixa carpeta mitjançant comanda:
    - Descomprimir: tar xvf G8_F4.tar
  4. Accedir a la carpeta 'ArrakisSistema' que hem descomprimit.
  5. Realitzar comanda 'make clean' des d'aquesta carpeta.
  6. Realitzar comanda 'make' des d'aquesta carpeta.
  7. Ja podem executar el que necessitem:
    - Per executar client: Fremen ConfigFiles/configFremen1.dat
    - Per executar servidor: Atreides ConfigFiles/configAtreides.dat
    - Per executar harkonen: Harkonen 20


**Al comprimir/descomprimir el projecte ens hem adonat de que de vegades les carpetes buïdes del projecte desapareixen. Si no existeixen s'han de crear tant '_AtreidesIMG' com '_Lists'.
