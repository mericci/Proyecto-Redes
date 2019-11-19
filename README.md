# Proyecto-Redes

Grupo compuesto por

1. Felipe Hanckes - 14619644
2. Martín Ricci - 14619482
3. Nicolas Vielva - 14619474
4. Dinko Yoma - 14634465

La estructura del proyecto contiene una carpeta llamada **cliente-servidor** dentro de la cual se separa las funcionalidades del cliente en la carpeta **client** de las del servidor en la carpeta **server**, cada una de estas carpetas presenta su propio *Makefile*, las cuales se ejecutan en el terminal comenzando con *./client* y *./server* respectivamente, junto a a definición del IP y puerto tal como sale en el enunciado, donde soporta cualquier orden de estos, además discrimina si se ingresa o no el parametro -l.
 
Cada carpeta tiene un archivo **main.c**, en las cuales se desarrolla el programa principal tanto del cliente como del servidor mediante el envio de mensajes, los cual se maneja con las funciones de los archivos **comunication.c** y **comunication.h** (tanto cliente como servidor tienen sus propios archivos con este nombre), para lo que es el manejo de la conexión se usan los archivos **conection.c** y **conection.h**, ambos se encuentran en cada una de las carpetas. Finalmente la carpeta **server** tiene además los archivos **util.c** y **util.h** que presenta funciones utiles para el manejo del programa, como por ejemplo la creación de las tarjetas.

El diseño utilizado en el desarrollo de este proyecto se basa en el que aparece al final del enunciado.

Los supuestos a considerar se tiene que para poder desarrollarse correctamente el programa se entregaran en algun momento y en orden los tipos de mensajes necearios según el diseño, si esto no es así, por ejemplo, si el cliente tiene que mandar un mensaje con id = 1, el servidor esperara hasta que le llegue ese id, si no es ese id el que llega, mandara un mensaje con id = 20 hasta que llegue el mensaje correcto.