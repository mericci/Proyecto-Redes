#include "conection.h"

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port){
  // Se define la estructura para almacenar info del socket del servidor al momento de su creación
 

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes


  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)


  // Se guardan el puerto e IP en la estructura antes definida

  // Se le asigna al socket del servidor un puerto y una IP donde escuchar

  // Se coloca el socket en modo listening

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes


  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.


  // Se aceptan a los primeros 2 clientes que lleguen. "accept" retorna el n° de otro socket asignado para la comunicación

}
