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
  char * IP;
  int PORT;

  int L_FLAG = 0;
  if (argc == 6) {
    //tiene l flag

    L_FLAG = 1;
    
    if (strcmp(argv[1], "-l") == 0) {
      if (strcmp(argv[2], "-i") == 0) {
        PORT = atoi(argv[5]);
        IP = argv[3];

      } else {
        PORT = atoi(argv[3]);
        IP = argv[5];
      }

    } else if (strcmp(argv[3], "-l") == 0) {
      if (strcmp(argv[1], "-i") == 0) {
        PORT = atoi(argv[5]);
        IP = argv[2];

      } else {
        PORT = atoi(argv[2]);
        IP = argv[5];
      }

    } else if (strcmp(argv[5], "-l") == 0) {
      if (strcmp(argv[1], "-i") == 0) {
        PORT = atoi(argv[4]);
        IP = argv[2];

      } else {
        PORT = atoi(argv[2]);
        IP = argv[4];
      }

    }


  }

  else {
    //no tiene l flag
    if (strcmp(argv[1], "-i") == 0) {
      IP = argv[2];
      PORT = atoi(argv[4]);
    } else {
      IP = argv[4];
      PORT = atoi(argv[2]);
    }
  }


  

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);
    
    if (msg_code == 1) { //Recibimos un mensaje del servidor
      char * message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      
      printf("Ingrese su mensaje: ");
      char * response = get_input();

      client_send_message(server_socket, option, response);
    }

    if (msg_code == 2) { //Recibimos un mensaje que proviene del otro cliente
      char * message = client_receive_payload(server_socket);
      printf("El otro cliente dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      
      printf("Ingrese su mensaje: ");
      char * response = get_input();

      client_send_message(server_socket, option, response);
    }
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
