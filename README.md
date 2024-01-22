# Arrakis System
Este repositorio contiene la implementación de Arrakis System, un sistema de arquitectura distribuida diseñado para abordar la concurrencia de procesos y la multitarea mediante el uso de sockets, threads y otros mecanismos del núcleo del sistema operativo. La implementación se llevó a cabo como parte de un proyecto asociado con La Salle BCN entre noviembre de 2021 y julio de 2022.
## Descripción del Proyecto
El sistema consta de dos componentes principales: Fremen y Atreides. Fremen es un programa diseñado para ser replicado y distribuido en dispositivos con acceso a una red, operando en modo de línea de comandos y capaz de ejecutar comandos del sistema operativo Linux. Atreides actúa como servidor, estableciendo la conectividad entre múltiples instancias de Fremen mediante sockets.
## Diseño & Estructura
### Diagramas
El sistema consta de un servidor Atreides y clientes Fremen que se conectan a él. Fremen está diseñado para ser replicado y distribuido en varios dispositivos que tengan acceso a una red. Funciona en modo de línea de comandos y puede ejecutar funciones esenciales, así como comandos del sistema operativo Linux. El servidor mantiene una lista actualizada de clientes conectados y usuarios registrados.
### Estructuras de Datos
Cada cliente Fremen tiene un archivo de configuración que se lee y almacena en una estructura de datos llamada ConfigFile. Atreides también tiene una estructura de datos para almacenar información sobre su configuración. Se utilizan File Descriptors para las operaciones de entrada y salida, Signals para manejar eventos del sistema, Forks para clonar procesos y Sockets para la comunicación entre Fremen y Atreides.
### Recursos del Sistema
- File Descriptors: Se utilizan para operaciones de entrada/salida y comunicación entre procesos.
- Signals: Se usan para cambiar el comportamiento de SIGINT y SIGALRM en el cliente Fremen y el servidor Atreides.
- Forks: Se utilizan para crear procesos secundarios al ejecutar comandos del sistema.
- Threads: Se generan para manejar las conexiones de los clientes en el servidor Atreides.
- Sockets: Facilitan la comunicación entre el servidor y los clientes.
- Pipes: Permiten la comunicación entre procesos hijo y padre.

## Transferencia de Fotos y Seguridad
El sistema permite la transferencia de fotos entre Fremen y Atreides. Los usuarios pueden enviar imágenes al servidor para su almacenamiento y solicitar fotos almacenadas. Se implementan protocolos de comunicación para garantizar la integridad de las imágenes mediante la verificación de MD5SUM.

## Programa Adicional - Harkonen
Se añade un programa adicional llamado Harkonen, que simula ataques de programas intrusos a los procesos activos en el sistema Arrakis. Además, se implementa una función de limpieza local de archivos en Fremen para eliminar archivos JPEG descargados del servidor Atreides a intervalos regulares.

## Conclusiones y Propuestas
Desde un punto de vista teórico, la práctica permitió poner a prueba los conocimientos adquiridos de manera incremental. A nivel personal, la asignatura fue interesante y desafiante, y la estructura de aprendizaje incremental resultó beneficiosa. Aunque la experiencia de enfrentarse solo a la práctica fue intensa, se considera valiosa. A pesar de no completar todo como se deseaba, la práctica fue entretenida y educativa. Se destaca la utilidad de los checkpoints y el feedback proporcionado por el profesorado y los becarios.

## Estructura del Proyecto
- ConfigFiles: Contiene los archivos de configuración tanto de Fremen como de Atreides, con varias configuraciones de Fremen para diferentes clientes. También incluye dos configuraciones de Atreides y una de Fremen1 por si hay problemas con el socket, permitiendo probar otro puerto sin tener que reiniciar el servidor.
- HeaderFiles: Contiene todos los archivos de cabecera del proyecto.
- SourceFiles: Contiene los archivos fuente del proyecto, estructurados en carpetas para el cliente Fremen, el servidor Atreides, y también para Harkonen. Los archivos compartidos se encuentran en la carpeta "All".
- _Lists: Contiene las listas requeridas por el sistema, como la lista de usuarios registrados en Atreides (la lista se crea una vez ejecutado Atreides).
- _AtreidesIMG: Carpeta de contenidos de Atreides. Almacena las imágenes que recibe.
- _FremenIMG: Con carpetas separadas para cada Fremen, contiene las imágenes que tienen y que cada cliente puede solicitar. (Se han incluido imágenes de ejemplo en los clientes).

## Ejecución
1. Conectarse al servidor 'Montserrat' de LaSalle.
2. Aceder a la carpeta donde se tenga guardado el archivo 'G8_F4.tar' entregado.
3. Descomprimir el archivo 'tar' desde la misma carpeta con el siguiente comando:
tar xvf G8_F4.tar
4. Acceder a la carpeta 'ArrakisSistema' que se ha descomprimido.
5. Ejecutar el comando make clean desde esta carpeta.
6. Ejecutar el comando make desde esta carpeta.
7. Ahora se pueden ejecutar los componentes necesarios:  
CLIENTE: 'Fremen ConfigFiles/configFremen1.dat'  
SERVIDOR: 'Atreides ConfigFiles/configAtreides.dat'  
HARKONEN 'Harkonen 20'  

Nota: Al comprimir/descomprimir el proyecto, hemos observado que a veces las carpetas vacías del proyecto desaparecen. Si no existen, deben crearse tanto '_AtreidesIMG' como '_Lists'.

##
@author: Oscar Julian  
@date: Julio 2022
