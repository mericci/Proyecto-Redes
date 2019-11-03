#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}


int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  int play;
  play = 1;
  
  while(play)
  { //Recibimos un mensaje del servidor
    int msg_code = client_receive_id(server_socket);
    if(msg_code == 3){
      char * message = client_receive_payload(server_socket);
      printf("%s", message);
      free(message);
      char * response = get_input();
      client_send_message(server_socket, 4, response);
    }
    if(msg_code == 6)
    {
      char * message = client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);
    }
    else if(msg_code == 7)
    {
        char * message = client_receive_payload(server_socket);
        printf("%s", message);
        free(message);
    }
    else if(msg_code == 8)
    {
      char * message = client_receive_payload(server_socket);
      printf("%s", message);
      free(message);
    }




  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  /*
  while (1){
    int msg_code = client_receive_id(server_socket);

    if (msg_code == 1) { //Recibimos un mensaje del servidor
      char * message = client_receive_payload(server_socket);
    	printf("%s", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

      printf("Ingrese su mensaje: ");
      char * response = get_input();
      client_send_message(server_socket, 4, response);
    }
    else if(msg_code == 6)
    {
      char * message = client_receive_payload(server_socket);
    	printf("%s", message);
      free(message);
    }
    else if(msg_code == 7)
    { 
        char * message = client_receive_payload(server_socket);
        printf("%s", message);
        free(message);
      
    }
    else if(msg_code == 8)
    {


    }
    
  }
  

  

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

      printf("Ingrese su mensaje: ");
      char * response = get_input();

      client_send_message(server_socket, option, response);
    }*/
    printf("------------------\n");
  }
  //}

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}

