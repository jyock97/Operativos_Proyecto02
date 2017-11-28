#include "virtualMemory.h"

int fileNumber = 0, nextPage = 0;
int realPages[10], virtualPages[10000];
FILE *realFiles[10];

//funcion que pide mememoria para la variable, pide size bytes
int getMem(int size){
  char intNumber[12];
  sprintf(intNumber, "%d", fileNumber);

  FILE *new;
  new = fopen(intNumber, "a");

  sprintf(intNumber, "%d", size);

  fputs(intNumber, new);
  fputs("\n", new);

  fclose(new);
  sprintf(intNumber, "%d", fileNumber);
  new = fopen(intNumber, "a+");
  if(fileNumber<10){//Si son las primeras 10 paginas, las guarda en real, o sino, solo en virtual
    realPages[fileNumber] = fileNumber;
    realFiles[fileNumber] = new;
  }
  virtualPages[fileNumber] = fileNumber;
  fileNumber++;
  return (fileNumber-1);
}

//Busca si una pagina se encuentra en memoria real
int search(int file){
  for(int i = 0;i<10;i++){
    if(realPages[i] == file){
      return i;
    }
  }
  return -1;
}

//Busca el valor que tiene asignado la variable pedida
void *getValue(int numPage){
  int numSize;
  char *value = (char*)calloc(4096, sizeof(char));
  char *value2 = (char*)calloc(4096, sizeof(char));
  int place = search(numPage);
  if(place == -1){
    char * virtualFile = (char *)calloc(20, sizeof(char));
    sprintf(virtualFile, "%d", numPage);
    FILE *virtual = fopen(virtualFile, "a+");
    place = nextPage%10;
    fclose(realFiles[place]);
    realFiles[place] = virtual;
    realPages[place] = numPage;
    nextPage++;
  }
  fgets(value, 4096, realFiles[place]);
  //////LEE TODO//////////////////////
  fgets(value2, 4096, realFiles[place]);
  fgets(value2, 4096, realFiles[place]);
  strcat(value,value2);
  /////////////LEE TODO///////////
  const char s[2] = "\n";
  char *token;

  /* get the first token */
  token = strtok(value, s);

  /* walk through other tokens */
  token = strtok(NULL, s);
  //numSize = atoi(value);
  //void *data = calloc(1,numSize);
  //fgets(data, numSize, realFiles[place]);
  rewind(realFiles[place]);
  return token;
}
//funcion que setea un valor a una variable asignada, las entradas:
//la pagina a cual escribirle, el valor a escribir y cuantos bytes escribe
int setValue(int numPage, void *value, int cant){
  int place = search(numPage);
  int numSize;
  char * size = (char *)calloc(4096, sizeof(char));
  char * data = (char *)calloc(4096, sizeof(char));
  data = value;
  if(place == -1){
    char * virtualFile = (char *)calloc(20, sizeof(char));
    sprintf(virtualFile, "%d", numPage);
    FILE *virtual = fopen(virtualFile, "a+");
    place = nextPage%10;
    fclose(realFiles[place]);
    realFiles[place] = virtual;
    realPages[place] = numPage;
    nextPage++;
  }
  //fgets(size, 20, realFiles[place]);

  //////////////LEE TODO/////////////////////////
  char *value1 = (char*)calloc(4096, sizeof(char));
  char *value2 = (char*)calloc(4096, sizeof(char));
  fgets(value1, 4096, realFiles[place]);
  fgets(value2, 4096, realFiles[place]);
  strcat(value1,value2);
  //////////////LEE TODO//////////////////////
  const char s[2] = "\n";

  size = strtok(value1, s);
  strcat(size, "\n");
  strcat(size, data);
  numSize = atoi(size);

  if(numSize>cant){
    fputs(size, realFiles[place]);
  }
  rewind(realFiles[place]);
  return 1;
}
