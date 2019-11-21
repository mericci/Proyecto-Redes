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
int L_FLAG = 0;
int DISCONNECTED = 0;

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


void cards_log(FILE* fp, unsigned char * cards) {
  int size = cards[1];
  fprintf(fp, "Size: %d Payload: ", size);
  int current_position = 2;
  int word_index = 0;
  while (word_index < 20) {
    int word_length = cards[current_position];
    fprintf(fp, "%d", word_length);
    current_position++;
    for (int i = 0; i < word_length; i++) {
      fprintf(fp, "%c", cards[current_position]);
      current_position++;
    }
    int word_position = cards[current_position];
    fprintf(fp, "%d", word_position);
    current_position++;
    word_index++;
  }
  fprintf(fp, "\n");

}

void record_in_log(int package_in, int messageId, char* payload){
  if (L_FLAG == 0) {
    return;
  }
  char * message;
  if (package_in == 0) {
    message = "[PKGE OUT]";
  } else {
    message = "[PKGE IN]";
  }
    int year, month, day, hour, minute, second;
    time_t now = time(0);
    struct tm *actual_time = localtime(&now);
    year = actual_time -> tm_year + 1900; //Segun man de localtime, debiera ser el año desde 1900, pero aca es desde 1815.
    month = actual_time -> tm_mon + 1;
    day = actual_time -> tm_mday;
    hour = actual_time -> tm_hour;
    minute = actual_time -> tm_min;
    second = actual_time -> tm_sec;

    FILE * fp;
    fp = fopen("log.txt", "a");
    if(messageId == 0){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n",
        day,month,year,hour,minute,second, message);
    }
    if(messageId == 1){
      //START CONNECTION
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Start Connection ID: 1 Size: 0\n",
        day,month,year,hour,minute,second, message);
    }
    if(messageId == 2){
      //CONNECTION ESTABLISHED
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Connection Established ID: 2 Size: 0 \n",
        day,month,year,hour,minute,second, message);
    }
    if(messageId == 3){
      //ASK NICKNAME
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Ask Nickname ID: 3 Size: 0\n",
        day,month,year,hour,minute,second, message);
    }
    if(messageId == 4){
      //RETURN NICKNAME
      int size = strlen(payload);
      fprintf(fp, 
        "[%d-%d-%d %d:%d:%d] %s Return Nickname ID: 4 Size: %d Payload: %s\n",
        day,month,year,hour,minute,second, message, size, payload);
    }
    if(messageId == 5){
      //OPPONENT FOUND
      int size = strlen(payload);
      fprintf(fp, 
        "[%d-%d-%d %d:%d:%d] %s Opponent Found ID: 5 Size: %d Payload: %s\n",
        day,month,year,hour,minute,second, message, size, payload);
    }
    if(messageId == 6){
      //SEND IDs
      int id = payload[0];
      fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Send IDs ID: 6 Size: 1 Payload: %d\n",
        day,month,year,hour,minute,second, message, id);
    }
    if(messageId == 7){
      //START GAME
      int game_number = payload[0];
      fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Start Game ID: 7 Size: 1 Payload: %d\n",
        day,month,year,hour,minute,second, message, game_number);
    }
    if(messageId == 8){
      //SCORES
      int score_1 = payload[0];
      int score_2 = payload[1];
      fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Scores ID: 8 Size: 2 Payload: %d%d\n",
        day,month,year,hour,minute,second, message, score_1 ,score_2);
    }
    if (messageId == 9) {
      //SEND CARDS
      fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Send Cards ID: 9 ",
        day,month,year,hour,minute,second, message);
      cards_log(fp, payload);
    }
    if(messageId == 10){
      //SEND WORD
      int size = strlen(payload);
      fprintf(fp,
        "[%d-%d-%d %d:%d:%d] %s Send Word ID: 10 Size: %d Payload: %s\n",
        day,month,year,hour,minute,second, message, size, payload);
    }
    if(messageId == 11){
      //RESPONSE WORD
      int response = payload[0];
      int remaining_attempts = payload[1];
      fprintf(fp, 
        "[%d-%d-%d %d:%d:%d] %s Response Word ID: 11 Size: 2 Payload: %d%d\n",
        day,month,year,hour,minute,second, message, response, remaining_attempts);
    }
    if(messageId == 12){
      //ROUND WINNER/LOOSER
      int winner_id = payload[0];
      fprintf(fp, 
      "[%d-%d-%d %d:%d:%d] %s Round Winner/Looser ID: 12 Size: 1 Payload: %d\n",
        day,month,year,hour,minute,second, message, winner_id);
    }
    if(messageId == 13){
      //END GAME
      int game_number = payload[0];
      fprintf(fp, 
        "[%d-%d-%d %d:%d:%d] %s End Game ID: 13 Size: 1 Payload: %d\n",
        day,month,year,hour,minute,second, message, game_number);
    }
    if(messageId == 14){
      //GAME WINNER/LOOSER
      int winner_id = payload[0];
      fprintf(fp,
        "[%d-%d-%d %d:%d:%d] %s Game Winner/Looser ID: 14 Size: 1 Payload: %d\n",
        day,month,year,hour,minute,second, message, winner_id);
    }
    if(messageId == 15){
      //ASK NEW GAME
      fprintf(fp,
        "[%d-%d-%d %d:%d:%d] %s Ask New Game ID: 15 Size: 0\n",
        day,month,year,hour,minute,second, message);
    }
    if(messageId == 16){
      //ANSWER NEW GAME
      int answer = payload[0];
      fprintf(fp,
        "[%d-%d-%d %d:%d:%d] %s Answer New Game ID: 16 Size: 1 Payload: %d\n",
        day,month,year,hour,minute,second, message, answer);
    }
    if(messageId == 17){
      //DISCONNECT
      fprintf(fp,
        "[%d-%d-%d %d:%d:%d] %s Disconnect ID: 17 Size: 0\n",
        day,month,year,hour,minute,second, message);
    }
    if (messageId == 20) {
      //ERROR BAD PACKAGE
      fprintf(fp,
        "[%d-%d-%d %d:%d:%d] %s Error Bad Package ID: 20 Size: 0\n",
        day,month,year,hour,minute,second, message);
    }
    fclose(fp);

}

void *game_function(void *arguments) {

  struct arg_struct *args = arguments;
  char * message_1 = malloc(sizeof(char));
  char * message_2 = malloc(sizeof(char) * 2);
  message_1[0] = args->game_start;
  
  

  if (args->client_socket == args->sockets_array[0]) {
    //player1
    message_2[0] = args->score_player1;
    message_2[1] = args->score_player2;
    server_send_message_2bytes(args->sockets_array[0], 8, message_2);
    record_in_log(0, 8, message_2);
    server_send_cards(args->sockets_array[0], args->cards);
    record_in_log(0, 9, args->cards);
    int numero_intento = 1;
    char * response;
    ROUND_SCORE_1 = 0;
    while (numero_intento <= 3) {
      int id = server_receive_id(args->sockets_array[0]);
      if (id == 10) {
        //SEND WORD
        response = server_receive_payload(args->sockets_array[0]);
        record_in_log(1, 10, response);
        if (DISCONNECTED == 1) {
          break;
        }
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
          record_in_log(0, 11, message_2);
          break;
        }
        else {
          
          message_2[0] = 0;
          message_2[1] = 3 - numero_intento;
          int intentos_restantes = 3 - numero_intento;
          message_2[1] = intentos_restantes;
          server_send_message_2bytes(args->sockets_array[0], 11, message_2);
          record_in_log(0, 11, message_2);
          if (numero_intento < 3) {
            server_send_cards(args->sockets_array[0], args->cards);
            record_in_log(0, 9, args->cards);
          }
        }
        free(response);
      } 
      else if(id == 17){
        free(server_receive_payload(args->sockets_array[0]));
        record_in_log(1, 17, message_1);
        server_send_message(args -> sockets_array[0], 17, message_1);
        record_in_log(0, 17, message_1);
        server_send_message(args -> sockets_array[1], 17, message_1);
        record_in_log(0, 17, message_1);
        DISCONNECTED = 1;
        break;
      }
      else
      {
        if (DISCONNECTED == 0) {
          free(server_receive_payload(args->sockets_array[0]));
          //no se hace log porque no se confia en el mensaje y podria generar un error
          numero_intento--;
          server_send_message(args -> sockets_array[0], 20, message_1);
          record_in_log(0, 20, message_1);
          server_send_cards(args ->sockets_array[0], args->cards);
          record_in_log(0, 9, args->cards);
        }
        
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
    record_in_log(0, 8, message_2);
    server_send_cards(args->sockets_array[1], args->cards);
    record_in_log(0, 9, args->cards);
    int numero_intento = 1;
    char * response;
    ROUND_SCORE_2 = 0;
    while (numero_intento <= 3) {
      int id = server_receive_id(args->sockets_array[1]);
      if (id == 10) {
        //SEND WORD
        response = server_receive_payload(args->sockets_array[1]);
        record_in_log(1, 10, response);
        if (DISCONNECTED == 1) {
          break;
        }
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
          record_in_log(0, 11, message_2);
          break;

        }
        else {
          
          message_2[0] = 0;
          message_2[1] = 3 - numero_intento;
          server_send_message_2bytes(args->sockets_array[1], 11, message_2);
          record_in_log(0, 11, message_2);
          if (numero_intento < 3) {
            server_send_cards(args->sockets_array[1], args->cards);
            record_in_log(0, 9, args->cards);
          }
          
        }
        free(response);
      } 
      else if(id == 17)
      {
        free(server_receive_payload(args->sockets_array[1]));
        record_in_log(1, 17, message_1);
        server_send_message(args -> sockets_array[0], 17, message_1);
        record_in_log(0, 17, message_1);
        server_send_message(args -> sockets_array[1], 17, message_1);
        record_in_log(0, 17, message_1);
        DISCONNECTED = 1;
        break;
      }
      else {
        if (DISCONNECTED == 0){
          free(server_receive_payload(args->sockets_array[1]));
          numero_intento--;
          server_send_message(args -> sockets_array[1], 20, message_1);
          record_in_log(0, 20, message_1);
          server_send_cards(args ->sockets_array[1], args->cards);
          record_in_log(0, 9, args->cards);
  
        }
       
        
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
      free(server_receive_payload(players_info_1->socket_c1));
      record_in_log(1, 1, empty_message);
      server_send_message(players_info_1->socket_c1, 2, empty_message);
      record_in_log(0, 2, empty_message);
      server_send_message(players_info_1->socket_c1, 3, empty_message);
      record_in_log(0, 3, empty_message);
      break;
    }
    else{
      free(server_receive_payload(players_info_1->socket_c1));
      server_send_message(players_info_1->socket_c1, 20, empty_message);
      record_in_log(0, 20, empty_message);
      printf("Error en el cliente\n");
    }
  }
  while(1)
  {
    msg_code1 = server_receive_id(players_info_1->socket_c1);
    if (msg_code1 == 4) {
      NICK1 = server_receive_payload(players_info_1->socket_c1);
      record_in_log(1, 4, NICK1);
      printf("Nickname 1: %s\n", NICK1);
      break;
    }
    else {
      free(server_receive_payload(players_info_1->socket_c1));
      server_send_message(players_info_1->socket_c1, 20, empty_message);
      record_in_log(0, 20, empty_message);
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
      free(server_receive_payload(players_info_2->socket_c1));
      record_in_log(1, 1, empty_message);
      server_send_message(players_info_2->socket_c1, 2, empty_message);
      record_in_log(0, 2, empty_message);
      server_send_message(players_info_2->socket_c1, 3, empty_message); 
      record_in_log(0, 3, empty_message);
      break;
    }
    else {
      free(server_receive_payload(players_info_2->socket_c1));
      server_send_message(players_info_2->socket_c1, 20, empty_message);
      record_in_log(0, 20, empty_message);
      printf("Error en el cliente\n");
    }
  }
  while(1)
  {
  msg_code2 = server_receive_id(players_info_2->socket_c1);
  if (msg_code2 == 4) {
    NICK2 = server_receive_payload(players_info_2->socket_c1);
    record_in_log(1, 4, NICK2);
    printf("Nickname 2: %s\n", NICK2);
    break;
  }
  else {
    free(server_receive_payload(players_info_2->socket_c1));
    server_send_message(players_info_2->socket_c1, 20, empty_message);
    record_in_log(0, 20, empty_message);
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
  record_in_log(0, 5, NICK2);
  server_send_message(sockets_array[1], 5, NICK1); // enviamos nombre del contrincante
  record_in_log(0, 5, NICK1);
  
  //envio los ids
  char * message_1 = malloc(sizeof(char));
  message_1[0] = 1;
  server_send_message(sockets_array[0], 6, message_1);
  record_in_log(0, 6, message_1);
  message_1[0] = 2;
  server_send_message(sockets_array[1], 6, message_1);
  record_in_log(0, 6, message_1);

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
      record_in_log(0, 7, message_1);
      server_send_message(sockets_array[1], 7, message_1);
      record_in_log(0, 7, message_1);
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
    if (DISCONNECTED == 1) {
      return 0;
    }
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
    record_in_log(0, 12, round_winner_message);
    server_send_message(sockets_array[1], 12, round_winner_message);
    record_in_log(0, 12, round_winner_message);
    ROUND_SCORE_1 = 0;
    ROUND_SCORE_2 = 0;
    free(round_winner_message);

        

    round++;

    if (round > 5) {
      message_1[0] = game_start;
      server_send_message(sockets_array[0], 13, message_1);
      record_in_log(0, 13, message_1);
      server_send_message(sockets_array[1], 13, message_1);
      record_in_log(0, 13, message_1);
      int winner_id = 0;

      if (score_player1 > score_player2) {
        winner_id = 1;
      }
      else if (score_player2 > score_player1) {
        winner_id = 2;
      }
      message_1[0] = winner_id;
      printf("score player 1: %d\n", score_player1);
      printf("score player 2: %d\n", score_player2);
      printf("winner_id: %d\n", winner_id);
      server_send_message(sockets_array[0], 14, message_1);
      record_in_log(0, 14, message_1);
      server_send_message(sockets_array[1], 14, message_1);
      record_in_log(0, 14, message_1);

      server_send_message(sockets_array[0], 15, message_1);
      record_in_log(0, 15, message_1);
      server_send_message(sockets_array[1], 15, message_1);
      record_in_log(0, 15, message_1);

      char * response_payload_1;
      char * response_payload_2;

      int response_1;
      int response_2;

      int msg_code;
      while(1)
      { 
        msg_code = server_receive_id(players_info_1->socket_c1);
        if(msg_code == 16)
        {
          response_payload_1 = server_receive_payload(sockets_array[0]);
          record_in_log(1, 16, response_payload_1);
          response_1 = response_payload_1[0];
          free(response_payload_1);
          break;
        }
        else
        {
          free(server_receive_payload(sockets_array[0]));
          server_send_message(players_info_1->socket_c1, 20, empty_message);
          record_in_log(0, 20, empty_message);
          printf("Error en el cliente\n");
        }
      }
      while(1)
      { 
        msg_code = server_receive_id(players_info_2->socket_c1);
        if(msg_code == 16)
        {
          response_payload_2 = server_receive_payload(sockets_array[1]);
          record_in_log(1, 16, response_payload_2);
          response_2 = response_payload_2[0];
          free(response_payload_2);
          break;
        }
        else
        {
          free(server_receive_payload(sockets_array[1]));
          server_send_message(players_info_2->socket_c1, 20, empty_message);
          record_in_log(0, 20, empty_message);
          printf("Error en el cliente\n");
        }
      }
      printf("response 1: %d\n", response_1);
      printf("response 2: %d\n", response_2);


      if ((response_1 == 1) && (response_2 == 1)) {
        game_start++;
        round = 1;
        score_player1 = 0;
        score_player2 = 0;
      }
      else{
        server_send_message(sockets_array[0], 17, message_1);
        record_in_log(0, 17, message_1);
        server_send_message(sockets_array[1], 17, message_1);
        record_in_log(0, 17, message_1);
        break;
      }


    }

  }

  free(message_1);
  free(players_info_1);
  free(players_info_2);
  free(NICK1);
  free(NICK2);

  

  

  

  return 0;
}