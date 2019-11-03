#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

void print_cards(char * message) {
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
  printf("       Card 1       ");
  //son 5 espacios
  printf("     ");
  printf("       Card 2       \n");
  for (int i = 0; i < 10; i++) {
    //card1
    for (int j = 0; j < words_position[i]; j++) {
      printf("-");
    }
    char * word = words[i];
    for (int j = 0; j < words_length[i]; j++) {
      printf("%c", word[j]);
    }
    int restantes = 20 - words_position[i] - words_length[i];
    for (int j = 0; j < restantes; j++) {
      printf("-");
    }
    printf("     ");
    //card2
    for (int j = 0; j < words_position[i + 10]; j++) {
      printf("-");
    }
    word = words[i + 10];
    for (int j = 0; j < words_length[i + 10]; j++) {
      printf("%c", word[j]);
    }
    restantes = 20 - words_position[i + 10] - words_length[i + 10];
    for (int j = 0; j < restantes; j++) {
      printf("-");
    }
    printf("\n");
    for (int j = 0; j < 20; j++) {
      printf("-");
    }
    printf("     ");
    for (int j = 0; j < 20; j++) {
      printf("-");
    }
    printf("\n");

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


  

  // Se prepara el socket
  
  
  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while(1) {
    printf("1) Comenzar juego\n");
    printf("2) Abandonar\n");
    printf("Ingerese numero de la opcion: ");
    char * response = get_input();
    if (response[0] == '1') {
    while (1) {
    //COMIENZO EL JUEGO
    int server_socket = prepare_socket(IP, PORT);
    char * cards_message;
    int id;
    char * empty_message = "";
    client_send_message(server_socket, 1, empty_message);
    
    int msg_code = client_receive_id(server_socket);
    
    if (msg_code == 2) {
      //CONNECTION ESTABLISHED
      printf("Se ha establecido exitosamente la conexion con el servidor\n");
      
    }
    if (msg_code == 3) {
      //ASK NICKNAME
      printf("Ingrese su nickname o ingrese:\n");
      char * response = get_input();
      client_send_message(server_socket, 4, response);
      printf("Se ha enviado el nickname al servidor\n");
      printf("Esperando al oponente\n");
      
    }
   
    if (msg_code == 5) {
      //OPPONENT FOUND
      
    }
    if (msg_code == 6) {
      //SEND ID'S

      
    }
    if (msg_code == 7) {
      //START GAME

      
    }
    if (msg_code == 8) {
      //SCORES
    }
    if (msg_code == 9) {
      //SEND CARDS
      cards_message = client_receive_payload(server_socket);
      print_cards(cards_message);
      printf("ingrese palabra: ");
      char * response = get_input();
  
      client_send_message(server_socket, 10, response);
      printf("\n");
    

    }
    

    if (msg_code == 11) {
      //RESPONSE WORD
      char * message = client_receive_payload(server_socket);
      int correcto = message[0];
      int intentos_restantes = message[1];
      if (correcto == 0) {
        //incorrecta
        printf("respuesta incorrecta\n");
        if (intentos_restantes == 0) {
          printf("No te quedan intentos\n");
        } else {
          printf("Te quedan %c intentos\n", intentos_restantes);
          printf("ingrese palabra: ");
          char * response = get_input();
          client_send_message(server_socket, 10, response);
          printf("\n");
        }

      } else {
        //correcta
        printf("Respuesta correcta\n");
      }

    }
    if (msg_code == 12) {
      //ROUND WINNER/LOOSER
      char * message = client_receive_payload(server_socket);
      int winner_id = message[0];
      
      if (winner_id == 0) {
        printf("Empate\n");
      } else {
        if (winner_id == id) {
          printf("Ganaste\n");
        } else {
          printf("Perdiste\n");
        }
      }
    }
    if (msg_code == 13) {
      //END GAME
    }
    if (msg_code == 14) {
      //GAME WINNER/LOOSER
    }
    if (msg_code == 15) {
      //ASK NEW GAME
    }
    
    if (msg_code == 17) {
      //DISCONNECT
    }
    if (msg_code == 20) {
      //ERROR BAD PACKAGE
    }
   
    
    printf("------------------\n");
  }

    } else if (response[0] == '2') {
      printf("Chao\n");
      break;
    } else {
      printf("No existe esa opcion\n");
    }

  }
  

  // Se cierra el socket
  
  close(server_socket);

  return 0;
}
