#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char** leer_palabras(FILE* archivo);

unsigned char * crear_cartas(char** lectura);

int* random_function( int max, int quantity);

void my_init();

int random_number(int max);