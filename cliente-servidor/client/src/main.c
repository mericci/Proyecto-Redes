#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
#include <pthread.h>



int ROUND_WINNER = 0;
int ROUND_NUMBER = 1;
int L_FLAG = 0;

int cards_size(unsigned char * cards) {
  int size = 0;
  int current_position = 0;
  int word_index = 0;
  while (word_index < 20) {
    int word_length = cards[current_position];
    size += (word_length + 2);
    current_position += (word_length + 2);
    word_index++;
  }
  return size;
}


void cards_log(FILE* fp, unsigned char * cards) {
  int size = cards_size(cards);
  fprintf(fp, "Size: %d Payload: ", size);
  int current_position = 0;
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
    year = actual_time -> tm_year + 1900; 
    //Segun man de localtime, debiera ser el año desde 1900, pero aca es desde 1815.
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


void print_cards(unsigned char * message) {
  int current_position = 0;
  char ** words = malloc(sizeof(char*) * 20);
  int * words_length = malloc(sizeof(int) * 20);
  int * words_position = malloc(sizeof(int) * 20);
  for (int i = 0; i < 20; i++) {
    words_length[i] = message[current_position];
    current_position ++;
    words[i] = malloc(sizeof(char) * words_length[i]);
    char * word = words[i];
    for (int j = 0; j < words_length[i]; j++) {
      word[j] = message[current_position];
      current_position ++;
    }
    words_position[i] = message[current_position];
    current_position ++;
  }
  printf("                Card 1                ");
  //son 5 espacios
  printf("      ");
  printf("                Card 2            \n");
  for (int i = 0; i < 10; i++) {
    //card1
    for (int j = 0; j < words_position[i]; j++) {
      printf("- ");
    }
    char * word = words[i];
    for (int j = 0; j < words_length[i]; j++) {
      printf("%c ", word[j]);
    }
    int restantes = 20 - words_position[i] - words_length[i];
    for (int j = 0; j < restantes; j++) {
      printf("- ");
    }
    printf("     ");
    //card2
    for (int j = 0; j < words_position[i + 10]; j++) {
      printf("- ");
    }
    word = words[i + 10];
    for (int j = 0; j < words_length[i + 10]; j++) {
      printf("%c ", word[j]);
    }
    restantes = 20 - words_position[i + 10] - words_length[i + 10];
    for (int j = 0; j < restantes; j++) {
      printf("- ");
    }
    printf("\n");
    for (int j = 0; j < 20; j++) {
      printf("- ");
    }
    printf("     ");
    for (int j = 0; j < 20; j++) {
      printf("- ");
    }
    printf("\n");

  }

  //FREE
  for (int i = 0; i < 20; i++) {
    free(words[i]);
  }
  free(words_length);
  free(words_position);
  free(words);


}


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
  int play = 1;
  
  
  
  while(play)
  {
    printf("1) Comenzar juego\n");
    printf("2) Abandonar\n");
    printf("Ingerese numero de la opcion: ");
    char * response = get_input();
    if (response[0] == '1') {
      int server_socket = prepare_socket(IP, PORT);
      unsigned char * cards_message;
      int id;
      int game_number;
      char * nombre = malloc(40);
      char * rival;
      char * empty_message = "";
      //client_send_message(server_socket, 20, empty_message);
      client_send_message(server_socket, 1, empty_message);
      record_in_log(0, 1, empty_message);
      while(1) //while(play)
      {         
        int msg_code = client_receive_id(server_socket);
        if (msg_code == 2) {
          //CONNECTION ESTABLISHED
          free(client_receive_payload(server_socket));
          record_in_log(1, 2, empty_message);
          printf("Se ha establecido exitosamente la conexion con el servidor\n");
        }

        if(msg_code == 3){
          free(client_receive_payload(server_socket));
          record_in_log(1, 3, empty_message);
          printf("Ingrese su Nickname: ");
          char * response = get_input();
          for (int i = 0; i < strlen(response); i++) {
            nombre[i] = response[i];
          }
          client_send_message(server_socket, 4, response);
          record_in_log(0, 4, response);
          printf("Se ha enviado el nickname al servidor\n");
          printf("Esperando al oponente\n");
          free(response);
        }

        if (msg_code == 5) {
          //OPPONENT FOUND
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 5, message);
          printf("%s se ha unido a la partida\n", message);
          int a = strlen(message);

          rival = malloc(sizeof(char) * strlen(message)); 
          for (int i = 0; i < strlen(message); i++) {
            rival[i] = message[i];
          }
          free(message);
        }

        if (msg_code == 6) {
          //SEND ID'S
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 6, message);
          id = message[0];
          
          printf("Tu ID es: %d\n", id);
          free(message);
          
        }
        
        if (msg_code == 7) {
          //START GAME
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 7, message);
          game_number = message[0];
          printf("\n");
          printf("/$$$$$$$    /$$$$$$  /$$$$$$$  /$$$$$$$  /$$       /$$$$$$$$\n");
          printf("| $$__  $$ /$$__  $$| $$__  $$| $$__  $$| $$      | $$_____/\n");
          printf("| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n");
          printf("| $$  | $$| $$  | $$| $$$$$$$ | $$$$$$$ | $$      | $$$$$\n");
          printf("| $$  | $$| $$  | $$| $$__  $$| $$__  $$| $$      | $$__/\n");
          printf("| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n");
          printf("| $$$$$$$/|  $$$$$$/| $$$$$$$/| $$$$$$$/| $$$$$$$$| $$$$$$$$\n");
          printf("|_______/ |_______/ |_______/ |_______/ |________/|________/\n");

          printf("\n########## PARTIDA: %d ##########\n\n", game_number);
          if(ROUND_NUMBER < 6) printf("RONDA: %d\n", 1);
          free(message);
        
          
        }

        if (msg_code == 8) {
          //SCORES
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 8, message);
          int my_score = message[0];
          int opponent_score = message[1];
          printf("SCORE:\n");
          printf("   %s: %d\n",nombre, my_score);
          printf("   %s: %d\n",rival, opponent_score);
          free(message);    
        }

        if (msg_code == 9) {
          //SEND CARDS
          cards_message = client_receive_payload(server_socket);
          record_in_log(1, 9, cards_message);
          int current_position = 0;
          print_cards(cards_message);
          printf("Ingrese palabra o ingrese 9 para salir: ");
          char * response = get_input();
          if (response[0] == '9') {
            client_send_message(server_socket, 17, response);
            record_in_log(0, 17, response);
            
          }
            else
          {
            client_send_message(server_socket, 10, response);
            record_in_log(0, 10, response);
            printf("\n");
          }
          free(response);
          free(cards_message);
         
        }

        if (msg_code == 11) {
          //RESPONSE WORD
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 11, message);
          int correcto = message[0];
          int intentos_restantes = message[1];
          if (correcto == 0) {
            //incorrecta
            printf("Respuesta incorrecta\n");
            printf("Intentos restantes: %d\n", intentos_restantes);
            
    
          } else {
            //correcta
            printf("Respuesta correcta\n");
          }
          free(message);
    
        }

        if (msg_code == 12) {
          //ROUND WINNER/LOOSER

          char * message = client_receive_payload(server_socket);
          record_in_log(1, 12, message);
          int winner_id = message[0];
          printf("\n");
          printf("/$$$$$$$    /$$$$$$  /$$$$$$$  /$$$$$$$  /$$       /$$$$$$$$\n");
          printf("| $$__  $$ /$$__  $$| $$__  $$| $$__  $$| $$      | $$_____/\n");
          printf("| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n");
          printf("| $$  | $$| $$  | $$| $$$$$$$ | $$$$$$$ | $$      | $$$$$\n");
          printf("| $$  | $$| $$  | $$| $$__  $$| $$__  $$| $$      | $$__/\n");
          printf("| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$      | $$\n");
          printf("| $$$$$$$/|  $$$$$$/| $$$$$$$/| $$$$$$$/| $$$$$$$$| $$$$$$$$\n");
          printf("|_______/ |_______/ |_______/ |_______/ |________/|________/\n");
          printf("\n");
          if (ROUND_NUMBER == 5) {
            ROUND_NUMBER = 1;
          }
          else {
            ROUND_NUMBER++;
            printf("RONDA: %d\n", ROUND_NUMBER);
          }
          
          if (winner_id == 0) {
            printf("Empate\n");
          } else {
            if (winner_id == id) {
              printf("Ganaste la ronda\n");
            } else {
              printf("Perdistela ronda\n");
            }
          }
          free(message);
        }

        if (msg_code == 13) {
          //END GAME
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 13, message);
          int numero_partida = message[0];
          printf("Se ha terminado la partida numero %d\n", numero_partida);
          free(message);
        }

        if (msg_code == 14) {
          //GAME WINNER/LOOSER
          char * message = client_receive_payload(server_socket);
          record_in_log(1, 14, message);
          int winner_id = message[0];
          if (winner_id == id) {
            printf("Ganaste la partida\n");
          } else if (winner_id == 0) {
            printf("Empate\n");
          }
          else {
            printf("Perdiste la partida\n");
          }
          free(message);
        }

        if (msg_code == 15) {
          //ASK NEW GAME
          free(client_receive_payload(server_socket));
          record_in_log(1, 15, empty_message);
          while (1) {
            printf("Deseas jugar otra partida?\n  1)Si\n  2)No\nIngrese numero: ");
            char * response = get_input();
            if (response[0] == '1') {
              char * res = malloc(sizeof(char));
              res[0] = 1;
              client_send_message(server_socket, 16, res);
              record_in_log(0, 16, res);
              free(res);
              break;
            } else if (response[0] == '2') {
              char * res = malloc(sizeof(char));
              res[0] = 0;
              client_send_message(server_socket, 16, res);
              record_in_log(0, 16, res);
              free(res);
              break;
            } else {
              printf("Opcion invalida\n\n");
            }
            free(response);
          }
          
          
        }
        
        if (msg_code == 17) {
          //DISCONNECT
          free(client_receive_payload(server_socket));
          record_in_log(1, 17, empty_message);
          printf("\nTe has desconectado del servidor\n\n");
          play = 0;
          close(server_socket);
          break;
        }

        if (msg_code == 20) {
          //ERROR BAD Pbreak;ACKAGE
          /*
          Obs:

          considerar cuando se tiene que mandar msg code = 1, no entrar al while
          considerar para caso que se está jugando
           */
          free(client_receive_payload(server_socket));
          record_in_log(1, 20, empty_message);
          printf("Se ha enviado un paquete con ID erroneo al servidor\n");
        }
      
      }
      free(rival);
      free(nombre);
    
    }
    else if (response[0] == '2') {
      printf("Haz abandonado la partida\n");
      break;
    } 
    else {
      printf("No existe esa opcion\n");
    }
    free(response);
  }


  return 0;


}