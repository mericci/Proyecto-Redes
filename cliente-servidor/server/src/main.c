#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"
#include <pthread.h>
#include "util.h"

int ROUND_SCORE_1;
int ROUND_SCORE_2;
int FINISHED_1 = 0;
int FINISHED_2 = 0;

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

struct arg_struct {
    int client_socket;
    int sockets_array[2];
    int game_start;
    int score_player1;
    int score_player2;
    unsigned char * cards;
    char * answer;
};

void *game_function(void *arguments) {

  struct arg_struct *args = arguments;
  char * message_1 = malloc(sizeof(char));
  char * message_2 = malloc(sizeof(char) * 2);
  message_1[0] = args->game_start;
  
  

  if (args->client_socket == args->sockets_array[0]) {
    //player1
    //server_send_message(args->sockets_array[0], 7, message_1);
    message_2[0] = args->score_player1;
    message_2[1] = args->score_player2;
    server_send_message_2bytes(args->sockets_array[0], 8, message_2);
    server_send_cards(args->sockets_array[0], args->cards);
    int numero_intento = 1;
    char * response;
    ROUND_SCORE_1 = 0;
    while (numero_intento <= 3) {
      int id = server_receive_id(args->sockets_array[0]);
      if (id == 10) {
        //SEND WORD
        response = server_receive_payload(args->sockets_array[0]);
        if (strcmp(response, args->answer) == 0) {
          if (numero_intento == 1) {
            ROUND_SCORE_1 = 5;
          }
          else if (numero_intento == 2) {
            ROUND_SCORE_1 = 3;
          }
          else {
            ROUND_SCORE_1 = 1;
          }
          message_2[0] = 1;
          message_2[1] = 3 - numero_intento;
          server_send_message_2bytes(args->sockets_array[0], 11, message_2);
          break;
        }
        else {
          
          message_2[0] = 0;
          message_2[1] = 3 - numero_intento;
          int intentos_restantes = 3 - numero_intento;
          message_2[1] = intentos_restantes;
          server_send_message_2bytes(args->sockets_array[0], 11, message_2);
          if (numero_intento < 3) {
            server_send_cards(args->sockets_array[0], args->cards);
          }
        }
      } 
      else if(id == 17){
        server_send_message(args -> sockets_array[0], 17, message_1);
        server_send_message(args -> sockets_array[1], 17, message_1);
      }
      else
      {
        numero_intento--;
        server_send_message(args -> sockets_array[0], 20, message_1);
      }
      numero_intento++;

    }
    FINISHED_1 = 1;

  }

  else{
    //player2
    //server_send_message(args->sockets_array[1], 7, message_1);
    message_2[0] = args->score_player2;
    message_2[1] = args->score_player1;
    server_send_message_2bytes(args->sockets_array[1], 8, message_2);
    server_send_cards(args->sockets_array[1], args->cards);
    int numero_intento = 1;
    char * response;
    ROUND_SCORE_2 = 0;
    while (numero_intento <= 3) {
      int id = server_receive_id(args->sockets_array[1]);
      if (id == 10) {
        //SEND WORD
        response = server_receive_payload(args->sockets_array[1]);
        if (strcmp(response, args->answer) == 0) {
          if (numero_intento == 1) {
            ROUND_SCORE_2 = 5;
          }
          else if (numero_intento == 2) {
            ROUND_SCORE_2 = 3;
          }
          else {
            ROUND_SCORE_2 = 1;
          }
          message_2[0] = 1;
          message_2[1] = 3 - numero_intento;
          server_send_message_2bytes(args->sockets_array[1], 11, message_2);
          break;

        }
        else {
          
          message_2[0] = 0;
          message_2[1] = 3 - numero_intento;
          server_send_message_2bytes(args->sockets_array[1], 11, message_2);
          if (numero_intento < 3) {
            server_send_cards(args->sockets_array[1], args->cards);
          }
          
        }
      } 
      else if(id == 17)
      {
        server_send_message(args -> sockets_array[0], 17, message_1);
        server_send_message(args -> sockets_array[1], 17, message_1);
      }
      else {
        numero_intento--;
        server_send_message(args -> sockets_array[0], 20, message_1);
      }
      numero_intento++;

    }
    FINISHED_2 = 1;

  }
  free(message_1);
  free(message_2);

  return NULL;

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

  char * empty_message = "";
  char * NICK1;
  char * NICK2;
  // Se obtiene el socket del primer cliente
  int server_socket = prepare_socket(IP, PORT);
  PlayersInfo * players_info_1= get_client(server_socket);
  printf("Se obtuvo la info del primer cliente\n");

  int msg_code1;
  while(1)
  {
    msg_code1 = server_receive_id(players_info_1->socket_c1);
    if (msg_code1 == 1) {
      server_receive_payload(players_info_1->socket_c1);
      server_send_message(players_info_1->socket_c1, 2, empty_message);
      server_send_message(players_info_1->socket_c1, 3, empty_message);
      break;
    }
    else{
      server_receive_payload(players_info_1->socket_c1);
      server_send_message(players_info_1->socket_c1, 20, empty_message);
      printf("Error en el cliente\n");
    }
  }
  while(1)
  {
    msg_code1 = server_receive_id(players_info_1->socket_c1);
    if (msg_code1 == 4) {
      NICK1 = server_receive_payload(players_info_1->socket_c1);
      printf("Nickname 1: %s\n", NICK1);
      break;
    }
    else {
      server_receive_payload(players_info_1->socket_c1);
      server_send_message(players_info_1->socket_c1, 20, empty_message);
      printf("Error en el cliente\n");
    }
  }
  
  //Se obtiene el socket del segundo cliente
  PlayersInfo * players_info_2 = get_client(server_socket);
  printf("Se obtuvo la info del segundo cliente\n");
  int msg_code2;
  
  while(1)
  {
    msg_code2 = server_receive_id(players_info_2->socket_c1);
    if (msg_code2 == 1) {
      server_receive_payload(players_info_2->socket_c1);
      server_send_message(players_info_2->socket_c1, 2, empty_message);
      server_send_message(players_info_2->socket_c1, 3, empty_message); 
      break;
    }
    else {
      server_receive_payload(players_info_2->socket_c1);
      server_send_message(players_info_2->socket_c1, 20, empty_message);
      printf("Error en el cliente\n");
    }
  }
  while(1)
  {
  msg_code2 = server_receive_id(players_info_2->socket_c1);
  if (msg_code2 == 4) {
    NICK2 = server_receive_payload(players_info_2->socket_c1);
    printf("Nickname 2: %s\n", NICK2);
    break;
  }
  else {
    server_receive_payload(players_info_2->socket_c1);
    server_send_message(players_info_2->socket_c1, 20, empty_message);
    printf("Error en el cliente\n");
  }
  }


  int game_start = 1;
  int score_player1 = 0;
  int score_player2 = 0;
  int round = 1;
  int sockets_array[2] = {players_info_1->socket_c1, players_info_2->socket_c1};
  printf("%s     v/s    %s\n",NICK1,NICK2);


  server_send_message(sockets_array[0], 5, NICK2); //enviamos nombre del contrincante
  server_send_message(sockets_array[1], 5, NICK1); // enviamos nombre del contrincante
  
  //envio los ids
  char * message_1 = malloc(sizeof(char));
  char * message_2 = malloc(sizeof(char) * 2);
  message_1[0] = 1;
  server_send_message(sockets_array[0], 6, message_1);
  message_1[0] = 2;
  server_send_message(sockets_array[1], 6, message_1);

  //COMIENZA EL JUEGO

  my_init();
  FILE * archivo = fopen("../../palabras.txt", "r");
  char ** leidas;
  leidas = leer_palabras(archivo);

  while(1) {
    //cada ronda recorre esto
    if (round == 1) {
      message_1[0] = game_start;
      server_send_message(sockets_array[0], 7, message_1);
      server_send_message(sockets_array[1], 7, message_1);
    }
    

    unsigned char * cards = crear_cartas(leidas);
    char * answer = correct_answer(cards);

    pthread_t thread_id1;
    struct arg_struct args1;
    args1.client_socket = sockets_array[0];
    args1.sockets_array[0] = sockets_array[0];
    args1.sockets_array[1] = sockets_array[1];
    args1.game_start = game_start;
    args1.score_player1 = score_player1;
    args1.score_player2 = score_player2;
    args1.cards = cards;
    args1.answer = answer;

    pthread_t thread_id2;
    struct arg_struct args2;
    args2.client_socket = sockets_array[1];
    args2.sockets_array[0] = sockets_array[0];
    args2.sockets_array[1] = sockets_array[1];
    args2.game_start = game_start;
    args2.score_player1 = score_player1;
    args2.score_player2 = score_player2;
    args2.cards = cards;
    args2.answer = answer;


    printf("Before Thread\n"); 
    pthread_create(&thread_id1, NULL, &game_function, (void *)&args1 );
    pthread_create(&thread_id2, NULL, &game_function, (void *)&args2 );
    while ((FINISHED_1 == 0) || (FINISHED_2 == 0)) {
      //usleep(100);
    }
    printf("After Thread\n");
    FINISHED_1 = 0;
    FINISHED_2 = 0;

    score_player1 += ROUND_SCORE_1;
    score_player2 += ROUND_SCORE_2;
    int round_winner_id = 0;
    if (ROUND_SCORE_1 > ROUND_SCORE_2) {
      round_winner_id = 1;
    }
    else if (ROUND_SCORE_2 > ROUND_SCORE_1) {
      round_winner_id = 2;
    }
    char * round_winner_message = malloc(sizeof(char));
    round_winner_message[0] = round_winner_id;
    server_send_message(sockets_array[0], 12, round_winner_message);
    server_send_message(sockets_array[1], 12, round_winner_message);
    ROUND_SCORE_1 = 0;
    ROUND_SCORE_2 = 0;

        

    round++;

    if (round > 5) {
      message_1[0] = game_start;
      server_send_message(sockets_array[0], 13, message_1);
      server_send_message(sockets_array[1], 13, message_1);
      int winner_id = 0;

      if (score_player1 > score_player2) {
        winner_id = score_player1;
      }
      else if (score_player2 > score_player1) {
        winner_id = score_player2;
      }
      message_1[0] = winner_id;
      server_send_message(sockets_array[0], 14, message_1);
      server_send_message(sockets_array[1], 14, message_1);

      server_send_message(sockets_array[0], 15, message_1);
      server_send_message(sockets_array[1], 15, message_1);
      char * response_payload_1;
      char * response_payload_2;
      //printf("ACA\n");

      int response_1;
      int response_2;

      int msg_code;
      while(1)
      { 
        msg_code = server_receive_id(players_info_1->socket_c1);
        if(msg_code == 16)
        {
          response_payload_1 = server_receive_payload(sockets_array[0]);
          response_1 = response_payload_1[0];
          break;
        }
        else
        {
          server_receive_payload(sockets_array[0]);
          server_send_message(players_info_1->socket_c1, 20, empty_message);
          printf("Error en el cliente\n");
        }
      }
      while(1)
      { 
        msg_code = server_receive_id(players_info_2->socket_c1);
        if(msg_code == 16)
        {
          response_payload_2 = server_receive_payload(sockets_array[1]);
          response_2 = response_payload_2[0];
          break;
        }
        else
        {
          server_receive_payload(sockets_array[1]);
          server_send_message(players_info_2->socket_c1, 20, empty_message);
          printf("Error en el cliente\n");
        }
      }
      printf("response 1: %d\n", response_1);
      printf("response 2: %d\n", response_2);


      if ((response_1 == 1) && (response_2 == 1)) {
        game_start++;
        round = 1;
      }
      else{
        server_send_message(sockets_array[0], 17, message_1);
        server_send_message(sockets_array[1], 17, message_1);
        break;
      }


    }

  }

  
  

  while(1) {
    usleep(30000);
  }

  

  return 0;
}