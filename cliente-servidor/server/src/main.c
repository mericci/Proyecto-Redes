#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;
  char * juego1 =  "/ $$$$$$$  /$$$$$$   /$$$$$$$  /$$$$$$$  /$$       /$$$$$$$$\n";
  char * juego2 =  "| $$__  $$| $$__  $$| $$__  $$| $$__  $$| $$      | $$_____/\n";
  char * juego3 =  "| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n";
  char * juego4 =  "| $$  | $$| $$  | $$| $$$$$$$ | $$$$$$$ | $$      | $$$$$$$\n";
  char * juego5 =  "| $$  | $$| $$  | $$| $$__  $$| $$__  $$| $$      | $$____/\n";
  char * juego6 =  "| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n";
  char * juego7 =  "| $$$$$$$/|  $$$$$$/| $$$$$$$/| $$$$$$$/| $$$$$$$$| $$$$$$$$\n";
  char * juego8 =  "|_______/ |_______/ |_______/ |_______/ |_________/ ________/\n";
  char * juego = "Chupala Martín";
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);
  int game_start = 1;
  int score_player1 = 0;
  int score_player2 = 0;
  int round = 0;
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  // Le enviamos al primer cliente un mensaje de bienvenida
  char * nickname = "Ingrese su Nickname: ";
  // preguntamos el nickname al cliente 1
  server_send_message(sockets_array[0], 3, nickname);
  // Esperamos respuesta del cliente 1
  int nickname1 = server_receive_id(sockets_array[0]);
  char * NICK1 = server_receive_payload(sockets_array[0]);
  // preguntamos el nickname al cliente 2
  server_send_message(sockets_array[1], 3, nickname);
  // Esperamos respuesta del cliente 1
  int nickname2 = server_receive_id(sockets_array[1]);
  char * NICK2 = server_receive_payload(sockets_array[1]);
  server_send_message(sockets_array[0], 6, "1");
  server_send_message(sockets_array[1], 6, "2");
  server_send_message(sockets_array[0], 7, juego1);
  server_send_message(sockets_array[1], 7, juego1);
  server_send_message(sockets_array[0], 7, juego2);
  server_send_message(sockets_array[1], 7, juego2);
  server_send_message(sockets_array[0], 7, juego3);
  server_send_message(sockets_array[1], 7, juego3);
  server_send_message(sockets_array[0], 7, juego4);
  server_send_message(sockets_array[1], 7, juego4);
  server_send_message(sockets_array[0], 7, juego5);
  server_send_message(sockets_array[1], 7, juego5);
  server_send_message(sockets_array[0], 7, juego6);
  server_send_message(sockets_array[1], 7, juego6);
  server_send_message(sockets_array[0], 7, juego7);
  server_send_message(sockets_array[1], 7, juego7);
  server_send_message(sockets_array[0], 7, juego8);
  server_send_message(sockets_array[1], 7, juego8);
  while (1) {
    server_send_message(sockets_array[0], 8, "Puntajes: \n");
    server_send_message(sockets_array[1], 8, "Puntajes: \n");
    server_send_message(sockets_array[0], 8, NICK1);
    server_send_message(sockets_array[1], 8, NICK1);
    server_send_message(sockets_array[0], 8, ": \n ");
    server_send_message(sockets_array[1], 8, ": \n ");
    server_send_message(sockets_array[0], 8, score_player1);
    server_send_message(sockets_array[1], 8, score_player1);
    server_send_message(sockets_array[0], 8, NICK2);
    server_send_message(sockets_array[1], 8, NICK2);
    server_send_message(sockets_array[0], 8, ": \n");
    server_send_message(sockets_array[1], 8, ": \n");
    server_send_message(sockets_array[0], 8, score_player2);
    server_send_message(sockets_array[1], 8, score_player2);
  }




  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  /*
  int my_attention = 0;

  while (1)
  {
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[my_attention]);

    if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    {

      char * client_message = server_receive_payload(sockets_array[my_attention]);
      printf("El cliente %d dice: %s\n", my_attention+1, client_message);

      // Le enviaremos el mismo mensaje invertido jeje
      char * response = revert(client_message);

      // Le enviamos la respuesta
      server_send_message(sockets_array[my_attention], 1, response);
    }
    else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      printf("Servidor traspasando desde %d a %d el mensaje: %s\n", my_attention+1, ((my_attention+1)%2)+1, client_message);

      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;

      server_send_message(sockets_array[my_attention], 2, client_message);
    }*/
  printf("------------------\n");
  //}

  return 0;
}
