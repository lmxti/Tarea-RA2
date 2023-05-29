# Tarea-RA2
## COMUNICACIÓN DE DATOS Y REDES (2023-1) SECC 2 RA2 
Esta tarea consiste en implementar una conexión de servidor y cliente utilizando sockets, junto con la implementación de un juego de Battleship que se ejecuta en el servidor y permite que el cliente juegue al conectarse.

## Requisitos
Compilador de C++ compatible.
Instrucciones de compilación y ejecución

**Abre una terminal y navega hasta el directorio donde se encuentran los archivos fuente.**

## Ejecuta el siguiente comando para compilar el servidor:
` g++ Game.cpp Server.cpp -o Servidor`

## Ejecuta el siguiente comando para compilar el cliente:
` g++ Client.cpp -o Cliente`

## Inicia el servidor ejecutando el siguiente comando:
` ./Servidor [puerto]`

**Reemplaza [puerto] por el número de puerto en el que deseas que el servidor escuche las conexiones. Por ejemplo:**
` ./Servidor 1234`

## Inicia el cliente ejecutando el siguiente comando:
`./Cliente [dirección IP] [puerto]`

**Reemplaza [dirección IP] por la dirección IP del servidor y [puerto] por el número de puerto en el que el servidor está escuchando las conexiones. Por ejemplo:**
`./Cliente 127.0.0.1 1234`

Sigue las instrucciones mostradas en la terminal para jugar al juego de Battleship. El cliente se conectará al servidor y podrás realizar tus movimientos.

## Estructura del proyecto
- **Game.cpp**: Archivo fuente que contiene la implementación del juego de Battleship.
- **Game.h**: Archivo de encabezado que contiene la declaración de las clases, estructuras y funciones utilizadas en la implementación del juego de Battleship
- **Server.cpp**: Archivo fuente que contiene la implementación del servidor que maneja la conexión con el cliente y el juego.
- **Client.cpp**: Archivo fuente que contiene la implementación del cliente que se conecta al servidor y juega al juego de Battleship.
- **README.md**: Archivo de documentación que proporciona instrucciones y detalles sobre el proyecto.
