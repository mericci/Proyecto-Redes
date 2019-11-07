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

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);
  int game_start = 1;
  int score_player1 = 0;
  int score_player2 = 0;
  int round = 1;
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  char * NICK1;
  char * NICK2;
  char * empty_message = "";
  // Le enviamos al primer cliente un mensaje de bienvenida
  //char * nickname = "Ingrese su Nickname: ";
  // preguntamos el nickname al cliente 1
  
  int msg_code1 = server_receive_id(sockets_array[0]);
  int msg_code2 = server_receive_id(sockets_array[1]);

  if(msg_code1 == 1) server_send_message(sockets_array[0], 2, empty_message);
  if(msg_code2 == 1) server_send_message(sockets_array[1], 2, empty_message);

  server_send_message(sockets_array[0], 3, empty_message); // o agregar nickname
  server_send_message(sockets_array[1], 3, empty_message); // o agregar 

  msg_code1 = server_receive_id(sockets_array[0]);
  msg_code2 = server_receive_id(sockets_array[1]);
  if(msg_code1 == 4) NICK1 = server_receive_payload(sockets_array[0]);
  if(msg_code2 == 4) NICK2 = server_receive_payload(sockets_array[1]);
  printf("%s     v/s    %s\n",NICK1,NICK2);

  
  server_send_message(sockets_array[0], 5, NICK1); //enviamos nombre del contrincante
  server_send_message(sockets_array[1], 5, NICK2); // enviamos nombre del contrincante

  server_send_message(sockets_array[0], 6, 1);
  server_send_message(sockets_array[1], 6, 2);

    

  while(1)
  {
    
    //TITULO
    dobble(sockets_array);

    server_send_message(sockets_array[0], 7, round);
    server_send_message(sockets_array[1], 7, round);

    //marcador actual
    /*
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
    server_send_message(sockets_array[1], 8, score_player2);*/

    //char * score_1 = score_player1 + '0';
    //char * score_2 = score_player2 + '0';
    
    int both = score_player1 & score_player2;

    //server_send_message(sockets_array[0], 8, both);

    
    //server_send_message(sockets_array[0], 9, tablero);
    //server_send_message(sockets_array[1], 9, tablero);

  
  
  
  printf("------------------\n");
  }

  return 0;
}