#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"
#include <pthread.h>
#include "util.h"
#include <time.h>

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
    int L_FLAG;
    char* NICK1;
    char* NICK2;
    unsigned char * payload;
};

void *record_in_log(char * message, int socket, int messageId, char* nick1, char* nick2, int ptj1, int ptj2){
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
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", day,month,year,hour,minute,second, message);
    }
    if(messageId == 1){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s Package: 1 0\n", day,month,year,hour,minute,second, message);
    }
    if(messageId == 2){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package: 2 0 \n", day,month,year,hour,minute,second, message,socket);
    }
    if(messageId == 3){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package: 3 0\n", day,month,year,hour,minute,second, message,socket);
    }
    if(messageId == 4){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package: 4 3 %s\n", day,month,year,hour,minute,second, message,socket, nick1);
    }
    if(messageId == 5){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package: 5 3 %s\n", day,month,year,hour,minute,second, message, socket, nick1);
    }
    if(messageId == 6){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package: 6 1 %d \n", day,month,year,hour,minute,second, message, socket, ptj1);
    }
    if(messageId == 7){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %s Package: 7 1 %d\n", day,month,year,hour,minute,second, message,socket, ptj1);
    }
    if(messageId == 8){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d. 8 2 %d %d\n", day,month,year,hour,minute,second, message, socket, ptj1 ,ptj2);
    }
    if(messageId == 10){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package %d %s\n", day,month,year,hour,minute,second, message,socket, messageId, nick1);
    }
    if(messageId == 11){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package: 11 2 %d %d\n", day,month,year,hour,minute,second, message,socket, ptj1, ptj2);
    }
    if(messageId == 12){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", day,month,year,hour,minute,second, message);
    }
    if(messageId == 13){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", day,month,year,hour,minute,second, message);
    }
    if(messageId == 14){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", day,month,year,hour,minute,second, message);
    }
    if(messageId == 15){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", day,month,year,hour,minute,second, message);
    }
    if(messageId == 16){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s %d Package 16 1 %d\n", day,month,year,hour,minute,second, message, socket, ptj1);
    }
    if(messageId == 17  ){
        fprintf(fp, "[%d-%d-%d %d:%d:%d] %s\n", day,month,year,hour,minute,second, message);
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
    //server_send_message(args->sockets_array[0], 7, message_1);
    message_2[0] = args->score_player1;
    message_2[1] = args->score_player2;
    server_send_message_2bytes(args->sockets_array[0], 8, message_2);
    if (args->L_FLAG == 1){
        char * l = "[SERVER] [PKGE OUT] Sended score to socket ";
        record_in_log(l, args->sockets_array[0], 8, args->NICK1, args->NICK2, message_2[0], message_2[1]);
    }
    server_send_cards(args->sockets_array[0], args->cards);
    if (args->L_FLAG == 1){
        int year, month, day, hour, minute, second;
        time_t now = time(0);
        struct tm *actual_time = localtime(&now);
        year = actual_time -> tm_year + 1900; //Segun man de localtime, debiera ser el año desde 1900, pero aca es desde 1815.
        month = actual_time -> tm_mon + 1;
        day = actual_time -> tm_mday;
        hour = actual_time -> tm_hour;
        minute = actual_time -> tm_min;
        second = actual_time -> tm_sec;
        int payload_length = args->payload[1];
        FILE * fp;
        fp = fopen("log.txt", "a");
        fprintf(fp, " [%d-%d-%d %d:%d:%d] [SERVER] [PKGE OUT] Sended card to socket %d Package ", day,month,year,hour,minute,second, args->sockets_array[0]);
        for(int i=0; i< payload_length + 2; i++){
          if (i==0 || i==1){
            fprintf(fp, "%d ", args->payload[i]);
          }else{
            printf("Valor de i en el primer i %d\n", i );
            int aux = args->payload[i];
            int suma = i;
            fprintf(fp, "%d ", args->payload[i]);
            suma += 1;
            i++;
            for (int j=i; j<i+aux; j++){
              fprintf(fp, "%c", args->payload[j]);
              suma += 1;
            }
            fprintf(fp, " %d ", args->payload[suma]);
            i = suma;
            }
         }
        fprintf(fp, "\n");
        fclose(fp);
    }
    int numero_intento = 1;
    char * response;
    ROUND_SCORE_1 = 0;
    while (numero_intento <= 3) {
      int id = server_receive_id(args->sockets_array[0]);
      if (id == 10) {
        //SEND WORD
        response = server_receive_payload(args->sockets_array[0]);

        if (args->L_FLAG == 1){
            char * l = "[SERVER] [PKGE IN] Answer from socket ";
            record_in_log(l, args->sockets_array[0], 10, response, args->NICK1, 0, 0);
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
          if (args->L_FLAG == 1){
              char * l = "[SERVER] [PKGE OUT] ResponseWord sended to socket ";
              record_in_log(l, args->sockets_array[0], 11, response, args->NICK1, message_2[0], message_2[1]);
          }
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
      else {
        //DESCONECCION
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
    if (args->L_FLAG == 1){
        char * l = "[SERVER] [PKGE OUT] Sended Score to socket ";
        record_in_log(l, args->sockets_array[1], 8, args->NICK1, args->NICK2, message_2[0], message_2[1]);
    }
    server_send_cards(args->sockets_array[1], args->cards);
    if (args->L_FLAG == 1){
        int year, month, day, hour, minute, second;
        time_t now = time(0);
        struct tm *actual_time = localtime(&now);
        year = actual_time -> tm_year + 1900; //Segun man de localtime, debiera ser el año desde 1900, pero aca es desde 1815.
        month = actual_time -> tm_mon + 1;
        day = actual_time -> tm_mday;
        hour = actual_time -> tm_hour;
        minute = actual_time -> tm_min;
        second = actual_time -> tm_sec;
        int payload_length = args->payload[1];
        FILE * fp;
        fp = fopen("log.txt", "a");
        fprintf(fp, " [%d-%d-%d %d:%d:%d] [SERVER] [PKGE OUT] Sended card to socket %d Package ", day,month,year,hour,minute,second, args->sockets_array[1]);
        for(int i=0; i< payload_length + 2; i++){
          if (i==0 || i==1){
            fprintf(fp, "%d ", args->payload[i]);
          }else{
            printf("Valor de i en el primer i %d\n", i );
            int aux = args->payload[i];
            int suma = i;
            fprintf(fp, "%d ", args->payload[i]);
            suma += 1;
            i++;
            for (int j=i; j<i+aux; j++){
              fprintf(fp, "%c", args->payload[j]);
              suma += 1;
            }
            fprintf(fp, " %d ", args->payload[suma]);
            i = suma;
            }
         }
        fprintf(fp, "\n");
        fclose(fp);
    }
    int numero_intento = 1;
    char * response;
    ROUND_SCORE_2 = 0;
    while (numero_intento <= 3) {
      int id = server_receive_id(args->sockets_array[1]);
      if (id == 10) {
        //SEND WORD
        response = server_receive_payload(args->sockets_array[1]);

        if (args->L_FLAG == 1){
            char * l = "[SERVER] [PKGE IN] Answer from socket ";
            record_in_log(l, args->sockets_array[0], 10, response, args->NICK1, 0, 0);
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
          if (args->L_FLAG == 1){
              char * l = "[SERVER] [PKGE OUT] ResponseWord sended to socket ";
              record_in_log(l, args->sockets_array[0], 11, response, args->NICK1, message_2[0], message_2[1]);
          }
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
      else {
        //DESCONECCION
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
  if (L_FLAG == 1){
      char * l = "[SERVER] [SETUP] socket creation succeded.";
      record_in_log(l, 0 , 0, NICK1, NICK2, 0, 0);
      l = "[SERVER] [CONNECTION] starting connections";
      record_in_log(l, 0 , 0, NICK1, NICK2, 0, 0);
  }
  PlayersInfo * players_info_1= get_client(server_socket);

  printf("Se obtuvo la info del primer cliente\n");
  int msg_code1 = server_receive_id(players_info_1->socket_c1);
  server_receive_payload(players_info_1->socket_c1);
  if (msg_code1 == 1) {
    if (L_FLAG == 1){
      char * l = "[SERVER] [PKGE IN] AskConnection received. ";
      record_in_log(l, players_info_1->socket_c1 , 1, NICK1, NICK2, 0, 0);
    }
    server_send_message(players_info_1->socket_c1, 2, empty_message);
    if (L_FLAG == 1){
      char * l = "[SERVER] [PKGE OUT] ConnectionEstablished sended to socket ";
      record_in_log(l, players_info_1->socket_c1 , 2, NICK1, NICK2, 0, 0);
    }
    server_send_message(players_info_1->socket_c1, 3, empty_message);
    if (L_FLAG == 1){
      char * l = "[SERVER][PKGE OUT] AskNickname sended to socket ";
      record_in_log(l, players_info_1->socket_c1 , 3, NICK1, NICK2, 0, 0);
    }
  }
  else {
    printf("Error en el cliente\n");
  }
  msg_code1 = server_receive_id(players_info_1->socket_c1);
  if (msg_code1 == 4) {
    NICK1 = server_receive_payload(players_info_1->socket_c1);
    if (L_FLAG == 1){
        char * l = "[SERVER][PKGE IN] ReturnNickname received from socket ";
        record_in_log(l, players_info_1->socket_c1 , 4, NICK1, NICK1, 0, 0);
    }
    printf("Nickname 1: %s\n", NICK1);
  }
  else {
    printf("Error en el cliente\n");
  }

  //Se obtiene el socket del segundo cliente
  PlayersInfo * players_info_2 = get_client(server_socket);
  printf("Se obtuvo la info del segundo cliente\n");
  int msg_code2 = server_receive_id(players_info_2->socket_c1);
  server_receive_payload(players_info_2->socket_c1);
  if (msg_code2 == 1) {
    if (L_FLAG == 1){
      char * l = "[SERVER] [PKGE IN] AskConnection received from socket ";
      record_in_log(l, players_info_2->socket_c1, 1, NICK2, NICK2, 0, 0);
    }
    printf("Se obtuvo la info del segundo cliente\n");
    server_send_message(players_info_2->socket_c1, 2, empty_message);
    if (L_FLAG == 1){
      char * l = "[SERVER] [PKGE OUT] ConnectionEstablished sended to socket ";
      record_in_log(l, players_info_2->socket_c1 , 2, NICK2, NICK2, 0, 0);
    }
    server_send_message(players_info_2->socket_c1, 3, empty_message);
    if (L_FLAG == 1){
      char * l = "[SERVER][PKGE OUT] AskNickname sended to socket ";
      record_in_log(l, players_info_2->socket_c1 , 3, NICK2, NICK2, 0, 0);
    }
  }
  else {
    printf("Error en el cliente\n");
  }
  msg_code2 = server_receive_id(players_info_2->socket_c1);
  if (msg_code2 == 4) {
    NICK2 = server_receive_payload(players_info_2->socket_c1);
    if (L_FLAG == 1){
        char * l = "[SERVER][PKGE IN] ReturnNickname received from socket ";
        record_in_log(l, players_info_2->socket_c1 , 4, NICK2, NICK2, 0, 0);
    }
    printf("Nickname 2: %s\n", NICK2);
  }
  else {
    printf("Error en el cliente\n");
  }

  int game_start = 1;
  int score_player1 = 0;
  int score_player2 = 0;
  int round = 1;
  int sockets_array[2] = {players_info_1->socket_c1, players_info_2->socket_c1};
  printf("%s     v/s    %s\n",NICK1,NICK2);
  if (L_FLAG == 1){
      char * l = "[SERVER][PKGE OUT] Oponent Found sended to socket ";
      record_in_log(l, sockets_array[0] , 5, NICK2, NICK2, 0, 0);
      record_in_log(l, players_info_2->socket_c1 , 5, NICK1, NICK1, 0, 0);
  }

  server_send_message(sockets_array[0], 5, NICK2); //enviamos nombre del contrincante
  server_send_message(sockets_array[1], 5, NICK1); // enviamos nombre del contrincante
  //envio los ids
  char * message_1 = malloc(sizeof(char));
  char * message_2 = malloc(sizeof(char) * 2);
  message_1[0] = 1;
  server_send_message(sockets_array[0], 6, message_1);
  if (L_FLAG == 1){
      char * l = "[SERVER][PKGE OUT] SendIDs sended to socket ";
      record_in_log(l, sockets_array[0], 6, NICK1, NICK2, 1, 0);
  }
  message_1[0] = 2;
  server_send_message(sockets_array[1], 6, message_1);
  if (L_FLAG == 1){
      char * l = "[SERVER][PKGE OUT] SendIDs sended to socket ";
      record_in_log(l, sockets_array[1], 6, NICK1, NICK2, 2, 0);
  }
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
      if (L_FLAG == 1){
          char * l = "[SERVER][PKGE OUT] Start Game sended to both players ";
          record_in_log(l, sockets_array[1], 7, NICK1, NICK2, game_start, 0);
      }
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
    args1.L_FLAG = L_FLAG;
    args1.NICK1 = NICK1;
    args1.NICK2 = NICK2;
    args1.payload = cards;

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
    args2.L_FLAG = L_FLAG;
    args2.NICK1 = NICK2;
    args2.NICK2 = NICK1;
    args2.payload = cards;

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
    if (L_FLAG == 1){
        char * l = "[SERVER] [PKGE OUT] Round Winner/Looser sended to both players Package: 12 1 1";
        record_in_log(l, sockets_array[0], 12, NICK1, NICK2, message_2[0], message_2[1]);
    }
    ROUND_SCORE_1 = 0;
    ROUND_SCORE_2 = 0;



    round++;

    if (round > 5) {
      message_1[0] = game_start;
      server_send_message(sockets_array[0], 13, message_1);
      server_send_message(sockets_array[1], 13, message_1);
      if (L_FLAG == 1){
          char * l = "[SERVER] [PKGE OUT] Game Ended sended to both players Package: 13 1 1";
          record_in_log(l, sockets_array[0], 13, NICK1, NICK2, message_2[0], message_2[1]);
      }
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
      if (L_FLAG == 1){
          char * l = "[SERVER] [PKGE OUT] Game Winner/Looser sended to both players Package: 14 1 1";
          record_in_log(l, sockets_array[0], 14, NICK1, NICK2, message_2[0], message_2[1]);
      }
      server_send_message(sockets_array[0], 15, message_1);
      server_send_message(sockets_array[1], 15, message_1);
      if (L_FLAG == 1){
          char * l = "[SERVER] [PKGE OUT] AskNewGame sended to both players Package: 15 0";
          record_in_log(l, sockets_array[0], 15, NICK1, NICK2, message_2[0], message_2[1]);
      }
      char * response_payload_1;
      char * response_payload_2;
      printf("ACA\n");

      int response_1;
      int response_2;
      server_receive_id(sockets_array[0]);
      server_receive_id(sockets_array[1]);
      response_payload_1 = server_receive_payload(sockets_array[0]);
      response_payload_2 = server_receive_payload(sockets_array[0]);
      response_1 = response_payload_1[0];
      response_2 = response_payload_2[0];
      if (L_FLAG == 1){
          char * l = "[SERVER] [PKGE IN] AnswerNewGame received from socket ";
          record_in_log(l, sockets_array[0], 16, NICK1, NICK2, response_1, message_2[1]);
          record_in_log(l, sockets_array[1], 16, NICK1, NICK2, response_1, message_2[1]);
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
