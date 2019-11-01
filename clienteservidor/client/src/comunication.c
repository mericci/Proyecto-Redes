#include "comunication.h"

int client_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje

}

char * client_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  
}

void client_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo. 
  //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.
  
  // Se arma el paquete

  // Se envía el paquete
  
}