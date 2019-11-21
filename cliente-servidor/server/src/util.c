#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "util.h"

char** leer_palabras(FILE* archivo){
    // LEE EL ARCHIVO DE PALABRAS Y RETORNA LISTA DE STRINGS CON LAS DIFERENTES PALABRAS
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    //creamos arreglo para informacion de archivo
    //hay que liberarlo!

    char ** lectura = malloc(1001 * sizeof(char *));
    int i = 0;
    while (i < 1001)
    {
        lectura[i] = malloc(13 * sizeof(char));   
        i+=1;
    }
    
    int leyendo = 0;

    while ((read = getline(&line, &len, archivo)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        strtok(line, "\n");
        strcpy(lectura[leyendo], line);
        leyendo+=1;
    }

    
    
    
    
    return lectura;

}
void my_init() {
    srand(time(NULL));
}

int random_number(int max){
    int randNum =rand() % max;
    return randNum;

}

int* random_function(int max, int quantity){
    int count = 0;
    int* random_numbers = malloc(quantity * sizeof(int));

     while(count < quantity) {
		int randNum =rand() % max; //Generate a random number
		bool found =false; //assume the random number hasnt been generated
		
		//make sure the random number hasnt already been generated
		for (int i = 0; i < count; i++) {
			if(random_numbers[i] ==randNum) {
				found =true;
				break; //once we have located the number we dont need to continue checking
			}
		}
		
		//we have a new random number
		if(!found) {
			random_numbers[count] =randNum;
			count++;
		}
	}
   return random_numbers;
}

unsigned char * crear_cartas(char** lectura){

    char ** elegidas = malloc(19 * sizeof(char *));
    int i = 0;
    while (i < 19)
    {
        elegidas[i] = malloc(13 * sizeof(char));   
        i+=1;
    }
    //int* random_numbers = malloc(19 * sizeof(int));
    int* random_numbers = random_function(1001, 19); 

    for (int i = 0; i < 19; i++)
    {
       strcpy(elegidas[i], lectura[random_numbers[i]]);
       //printf("elegida:  %s, %i\n", elegidas[i], i);
    }
    

    //int* random_position = malloc(20 * sizeof(int));
    int* random_position = random_function(20,20); 

    char ** definitivo_orden = malloc(20 * sizeof(char *));
    int z = 0;
    while (z < 20)
    {
        definitivo_orden[z] = malloc(13 * sizeof(char));   
        z+=1;
    }

    int contador_1 = 0;
    int contador_2 = 0;

    int pos_1 = 0;
    int pos_2 = 0;

    for (int i = 0; i < 20; i++)
    {
        if (random_position[i] <= 9 && contador_1 == 0)
        {
            if (pos_2 - 10 != random_position[i])
            {
                strcpy(definitivo_orden[random_position[i]], elegidas[18]);
                contador_1 = 1;
                pos_1 = random_position[i];
            }
            
            
        }
        if ( 10 <= random_position[i] && random_position[i] <=19 && contador_2 ==0)
        {
            if (pos_1 + 10 != random_position[i])
            {
                strcpy(definitivo_orden[random_position[i]], elegidas[18]);
                contador_2 = 1;
                pos_2 = random_position[i];
            }
        } 
    }

    int chosen = 0;

    for (int i = 0; i < 20; i++)
    {
        if(random_position[i] != pos_1 && random_position[i] != pos_2 && chosen < 18){

            strcpy(definitivo_orden[random_position[i]], elegidas[chosen]);
            chosen +=1;

        }
    }

    int * largo = malloc(20 * sizeof(int));

    for (int i = 0; i < 20; i++)
    {
        if (definitivo_orden[i][0] == '1')
        {
            //printf("quede 1\n");
            largo[i] = 10;
        }
        else
        {
            largo[i] = atoi(&definitivo_orden[i][0]);
        }    
        //printf("quede 2 %c\n", definitivo_orden[i][0]);
    }

    int* posiciones = malloc(20 * sizeof(int));

    int asignadas = 0;
    int random_pos = 0;
    int actual = 0;
    while (asignadas < 20)
    {
        random_pos = random_number(20 - largo[actual]);
        posiciones[actual] = random_pos;
        //printf("%s, comienza en %i\n", definitivo_orden[actual], posiciones[actual]);
        actual+=1;
        asignadas+=1;
    }

    int largo_total = 20 + 20;
    for (int i = 0; i < 20; i++)
    {
        largo_total+= largo[i];
    }
    //printf("%i\n", largo_total);

    //char respuesta[largo_total + 2];
    unsigned char * respuesta = malloc(sizeof(char) * (largo_total + 2));

    respuesta[0] = 9;
    respuesta[1] = largo_total;
    int ingresar_en = 2;

    for (int i = 0; i < 20; i++)
    {
        respuesta[ingresar_en] = largo[i];
        ingresar_en += 1;
        if (largo[i] == 10)
        {
            for (int j = 0; j < largo[i] + 3; j++)
            {
    
                if (isdigit(definitivo_orden[i][j]) )
                {
                    continue;
                }
                else if( definitivo_orden[i][j] != ',')
                {
                    respuesta[ingresar_en] = definitivo_orden[i][j];
                    //printf("%c  - ingresar en: %i\n", definitivo_orden[i][j], ingresar_en);
                    ingresar_en += 1;
                }
                
                
            }
        }
        else
        {
            for (int j = 0; j < largo[i] + 2; j++)
            {
    
                if (isdigit(definitivo_orden[i][j]) )
                {
                    continue;
                }
                else if( definitivo_orden[i][j] != ',')
                {
                    respuesta[ingresar_en] = definitivo_orden[i][j];
                    //printf("%c  - ingresar en: %i\n", definitivo_orden[i][j], ingresar_en);
                    ingresar_en += 1;
                }
                
                
            }
        }
        
        
        respuesta[ingresar_en] = posiciones[i];
        ingresar_en += 1;
        
        
    }

    free(random_numbers);
    for (int i = 0; i < 19; i++) {
        free(elegidas[i]);
    }
    free(elegidas);
    free(random_position);


    for (int i = 0; i < 20; i++) {
        free(definitivo_orden[i]);
    }
    free(definitivo_orden);
    free(largo);
    free(posiciones);

    
    
    return respuesta;


}


char * correct_answer(unsigned char * cards) {
  int current_position = 2;
  char ** words = malloc(sizeof(char*) * 20);
  int * words_length = malloc(sizeof(int) * 20);
  int * words_position = malloc(sizeof(int) * 20);
  for (int i = 0; i < 20; i++) {
    words_length[i] = cards[current_position];
    current_position ++;
    words[i] = malloc(sizeof(char) * words_length[i]);
    char * word = words[i];
    for (int j = 0; j < words_length[i]; j++) {
      word[j] = cards[current_position];
      current_position ++;
    }
    words_position[i] = cards[current_position];
    current_position ++;
  }
  //char * answer;

  for (int i = 0; i < 10; i++) {
    char * current_word_1 = words[i];
    for (int j = 0; j < 10; j++) {
        char * current_word_2 = words[10 + j];
        if (strcmp(current_word_1, current_word_2) == 0) {
            //answer = malloc(sizeof(char) * words_length[i]);
            char* answer = calloc(words_length[i], sizeof(char));
            for (int k = 0; k < words_length[i]; k++) {
                answer[k] = current_word_1[k];
            }

            printf("\nanswer: %s\n", answer);
            printf("anwer length: %d\n", words_length[i]);
            //FREE
            for (int k = 0; k < 20; k++) {
              free(words[k]);
            }
            free(words_length);
            free(words_position);
            free(words);
            return answer;
        }
    }
  }
  

  return "NoHay";
  

}



