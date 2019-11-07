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
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
        strtok(line, "\n");
        strcpy(lectura[leyendo], line);
        leyendo+=1;
    }

    for (int z = 0; z < 1001; z++)
    {
       printf("sss %s ddddd ---- %i \n", lectura[z], z);
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
    int* random_numbers = malloc(19 * sizeof(int));
    random_numbers = random_function(1001, 19); 

    for (int i = 0; i < 19; i++)
    {
       strcpy(elegidas[i], lectura[random_numbers[i]]);
       printf("elegida:  %s, %i\n", elegidas[i], i);
    }

    int* random_position = malloc(20 * sizeof(int));
    random_position = random_function(20,20); 

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
            printf("chosem:%s, pos: %i \n", elegidas[chosen], random_position[i]);

        }
    }

    int * largo = malloc(20 * sizeof(int));

    for (int i = 0; i < 20; i++)
    {
        if (definitivo_orden[i][0] == '1')
        {
            printf("quede 1\n");
            largo[i] = 10;
        }
        else
        {
            largo[i] = atoi(&definitivo_orden[i][0]);
        }    
        printf("quede 2 %c\n", definitivo_orden[i][0]);
    }

    int* posiciones = malloc(20 * sizeof(int));

    int asignadas = 0;
    int random_pos = 0;
    int actual = 0;
    while (asignadas < 20)
    {
        random_pos = random_number(20 - largo[actual]);
        posiciones[actual] = random_pos;
        printf("%s, comienza en %i\n", definitivo_orden[actual], posiciones[actual]);
        actual+=1;
        asignadas+=1;
    }

    int largo_total = 20 + 20;
    for (int i = 0; i < 20; i++)
    {
        largo_total+= largo[i];
    }
    printf("%i\n", largo_total);

    //char respuesta[largo_total + 2];
    unsigned char * respuesta = malloc(sizeof(char) * (largo_total + 1));

    int id = 0;
    //respuesta[0] = id;
    //respuesta[1] = largo_total;
    respuesta[0] = largo_total;
    int ingresar_en = 1;

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

    for (int i = 0; i < largo_total + 2; i++)
    {
        if (i==1)
        {
            printf("--mm %i\n" , (int)respuesta[i]);
        }
        else
        {
            printf("%c\n", respuesta[i]);
        }
        
        
    }
    
    

    
    return respuesta;


}