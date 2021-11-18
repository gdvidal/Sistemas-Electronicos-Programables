#include <stdio.h>
#include <stdlib.h>
#include <time.h> 


void imprimir_tablero(int tablero[10][10], int perder);
void colocar_bombas(int minas[10][10], int bum);

#define FILA 10
#define COLUMNA 10

int main()
{
  int buscaminas[FILA][COLUMNA] = {0}, i, j;
  int nivel, bombas;
  int gameStatus, aciertos = 0;
  int victoria=0;
  float puntaje;

 system("cls");
 printf("\n\tBienvenido al mitico Buscaminas!\n\n");
 printf("~Ingresa el nivel del juego:\n%d.-Nivel 1 (10 bombas)\n", 1);
 printf("%d.-Nivel 2 (20 bombas)\n%d.-Nivel 3 (30 bombas)\n%d.-Nivel 4 (35 bombas)\n%d.-Nivel 5 (40 bombas)\n", 2, 3, 4, 5);

 scanf("%d", &nivel);

 switch(nivel){

     case 1: bombas = 10;
             break;

     case 2: bombas = 20;
             break;

     case 3: bombas = 30;
             break;

     case 4: bombas = 35;
             break;

     case 5: bombas = 40;
             break;

     default: bombas= 15;
              break;

 }

 srand(time(NULL));  //generacion de numeros aleatorios
 colocar_bombas(buscaminas, bombas);

 do{
     system("cls");
     printf("\nSoldado! Recuerda tener precaucion con las bombas!\n\n");

     imprimir_tablero(buscaminas, gameStatus);

     printf("\n");

     printf(" Indica tu posicion soldado (i,j):");
     scanf("%d%d",&i, &j);

     if(buscaminas[i][j] == 3){
         gameStatus = -1;

         system("cls");
         imprimir_tablero(buscaminas, gameStatus);
         printf("\t\t\t\t\t Perdiste! El juego ha terminado!");
     }
     else
     {
         buscaminas[i][j] = 1;
         aciertos++;

     }
     if(aciertos == (100 - bombas) )
     {
         victoria = 1;
         system("cls");
         imprimir_tablero(buscaminas, gameStatus);
         printf("\t\t\t\t\t Has ganado soldado!, felicidades!!");

     }


 }while(gameStatus != -1 || victoria == 1);

 puntaje = ((float)aciertos);
 printf("\nEstadisticas del juego:\n\n");
 printf("Tus aciertos del juego fueron %.2f\n", puntaje);

}

void colocar_bombas(int minas[10][10], int bomb)
{
     int i, fila, columna;

     for(i = 1; i <= bomb; i++)
     {
           fila = rand() % 10;
           columna = rand() % 10;

           minas[fila][columna] = 3;
     }
}

void imprimir_tablero(int tablero[10][10], int perder)
{
     int i, j;

     for(i = 0; i <= FILA -1; i++)  //encabezado
       printf("%d ", i);

     printf("\n");

     for(i = 0; i <= COLUMNA - 1; i++)
       printf("--");

     printf("\n");

     for(i = 0; i <= FILA -1; i++)
     {
           for(j = 0; j <= COLUMNA - 1; j++){

             if(tablero[i][j] == 1)
               printf("%d ", tablero[i][j]);
             else if((tablero[i][j] == 3) && (perder == -1))
               printf("%c ", '*');
             else
               printf("%c ", '#');
           }

           printf("| %d", i);
           printf("\n");

     }
}
