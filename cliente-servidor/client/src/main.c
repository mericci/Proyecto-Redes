#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

int ROUND_WINNER = 0;
int ROUND_NUMBER = 5;


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

int negative_transformation(int n) {
  if (n >= 0) {
    return n;
  }
  else {
    int dif = 128 - (n * (-1));
    int m = 128 + dif;
    return m; 
  }
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
  int play;
  play = 1;
  
  while(1)
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
      client_send_message(server_socket, 1, empty_message);
      while(1) //while(play)
      {         
        
        int msg_code = client_receive_id(server_socket);
        

        if (msg_code == 2) {
          //CONNECTION ESTABLISHED
          client_receive_payload(server_socket);
          printf("Se ha establecido exitosamente la conexion con el servidor\n");
        }

        if(msg_code == 3){
          //char * message = client_receive_payload(server_socket);
          //printf("%s", message);
          //free(message);
          client_receive_payload(server_socket);
          printf("Ingrese su Nickname: ");
          char * response = get_input();
          nombre = response;
          client_send_message(server_socket, 4, response);
          printf("Se ha enviado el nickname al servidor\n");
          printf("Esperando al oponente\n");
        }

        if (msg_code == 5) {
          //OPPONENT FOUND
          char * message = client_receive_payload(server_socket);
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
          id = message[0];
          
          printf("Tu ID es: %d\n", id);
          free(message);
          
        }
        /*
        else if(msg_code == 7)
        {
            char * message = client_receive_payload(server_socket);
            printf("########## PARTIDA: %s ##########", message);
            free(message);
        }*/
        if (msg_code == 7) {
          //START GAME
          char * message = client_receive_payload(server_socket);
          game_number = message[0];
          printf("########## PARTIDA: %d ##########\n\n", game_number);
          free(message);
        
          
        }

        if (msg_code == 8) {
          //SCORES
          char * message = client_receive_payload(server_socket);
          int my_score = message[0];
          int opponent_score = message[1];
          printf("SCORE:\n");
          printf("   %s: %d\n",nombre, my_score);
          printf("   %s: %d\n",rival, opponent_score);

    
        }

        if (msg_code == 9) {
          //SEND CARDS
          cards_message = client_receive_payload(server_socket);
          int current_position = 0;
          print_cards(cards_message);
          printf("Ingrese palabra o ingrese 9 para salir: ");
          char * response = get_input();
          if (response[0] == '9') {
            client_send_message(server_socket, 17, response);
          }
            else
          {
            client_send_message(server_socket, 10, response);
            printf("\n");
          }
         
        }

        if (msg_code == 11) {
          //RESPONSE WORD
          char * message = client_receive_payload(server_socket);
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
    
        }

        if (msg_code == 12) {
          //ROUND WINNER/LOOSER
          if (ROUND_NUMBER == 5) {
            ROUND_NUMBER = 1;
          }
          else {
            ROUND_NUMBER++;
          }
          char * message = client_receive_payload(server_socket);
          int winner_id = message[0];
          printf("RONDA: %d\n", ROUND_NUMBER);
          
          if (winner_id == 0) {
            printf("Empate\n");
          } else {
            if (winner_id == id) {
              printf("Ganaste la ronda\n");
            } else {
              printf("Perdistela ronda\n");
            }
          }
        }

        if (msg_code == 13) {
          //END GAME
          char * message = client_receive_payload(server_socket);
          int numero_partida = message[0];
          printf("Se ha terminado la partida numero %d\n", numero_partida);
        }

        if (msg_code == 14) {
          //GAME WINNER/LOOSER
          char * message = client_receive_payload(server_socket);
          int winner_id = message[0];
          if (winner_id == id) {
            printf("Ganaste la partida\n");
          } else {
            printf("Perdiste la partida\n");
          }
        }

        if (msg_code == 15) {
          //ASK NEW GAME
          client_receive_payload(server_socket);
          while (1) {
            printf("Deseas jugar otra partida?\n1)Si\n2)No\nIngrese numero: ");
            char * response = get_input();
            if (response[0] == '1') {
              char * res = malloc(sizeof(char));
              res[0] = 1;
              client_send_message(server_socket, 16, res);
              free(res);
              break;
            } else if (response[0] == '2') {
              char * res = malloc(sizeof(char));
              res[0] = 0;
              client_send_message(server_socket, 16, res);
              free(res);
              break;
            } else {
              printf("Opcion invalida\n");
            }
          }
          
          
        }
        
        if (msg_code == 17) {
          //DISCONNECT
          client_receive_payload(server_socket);
          printf("Te has desconectado del servidor");
          close(server_socket);
          break;
        }

        if (msg_code == 20) {
          //ERROR BAD PACKAGE
          client_receive_payload(server_socket);
          printf("Se ha enviado un paquete con ID erroneo al servidor\n");
          close(server_socket);
          break;

        }
      
        //printf("------------------\n");
      }
      free(rival);
    
    }
    else if (response[0] == '2') {
      printf("Haz abandonado la partida\n");
      break;
    } 
    else {
      printf("No existe esa opcion\n");
    }
  }


  return 0;


}