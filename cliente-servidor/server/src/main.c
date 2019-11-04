#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"

void dobble(int sockets_array[2])
{
  char * juego1 =  "/$$$$$$$    /$$$$$$  /$$$$$$$  /$$$$$$$  /$$       /$$$$$$$$\n";
  char * juego2 =  "| $$__  $$ /$$__  $$| $$__  $$| $$__  $$| $$      | $$_____/\n";
  char * juego3 =  "| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n";
  char * juego4 =  "| $$  | $$| $$  | $$| $$$$$$$ | $$$$$$$ | $$      | $$$$$\n";
  char * juego5 =  "| $$  | $$| $$  | $$| $$__  $$| $$__  $$| $$      | $$__/\n";
  char * juego6 =  "| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n";
  char * juego7 =  "| $$$$$$$/|  $$$$$$/| $$$$$$$/| $$$$$$$/| $$$$$$$$| $$$$$$$$\n";
  char * juego8 =  "|_______/ |_______/ |_______/ |_______/ |________/|________/\n";

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
}

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;
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
  
  


  while(1)
  {
    //TITULO
    dobble(sockets_array);

    server_send_message(sockets_array[0], 6, "Tu ID es: 1\n");
    server_send_message(sockets_array[1], 6, "Tu ID es: 2\n");

    //marcador actual
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

    
    //server_send_message(sockets_array[0], 9, tablero);
    //server_send_message(sockets_array[1], 9, tablero);

  
  
  
  printf("------------------\n");
  }

  return 0;
}