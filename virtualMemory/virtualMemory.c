#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int fileNumber = 0, nextPage = 0;
int realPages[10], virtualPages[10000];
FILE *realFiles[10], *virtualFiles[10000];

int getMem(int size){

  char intNumber[12];
  char *filename = (char *)calloc(21,sizeof(char));

  sprintf(intNumber, "%d", fileNumber);

  FILE *new;
  new = fopen(intNumber, "a");

  sprintf(intNumber, "%d", size);

  fputs(intNumber, new);
  fputs("\n", new);

  fclose(new);
  sprintf(intNumber, "%d", fileNumber);
  new = fopen(intNumber, "a+");
  if(fileNumber<10){
    realPages[fileNumber] = fileNumber;
    realFiles[fileNumber] = new;
  }
  virtualPages[fileNumber] = fileNumber;
  virtualFiles[fileNumber] = new;
  fileNumber++;
  return (fileNumber-1);
}

int search(int file){
  for(int i = 0;i<10;i++){
    if(realPages[i] == file){
      return i;
    }
  }
  return -1;
}

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

int main(){
  int a = getMem(7);
  int b = getMem(5);
  int c = getMem(5);
  int c1 = getMem(5);
  int c2 = getMem(5);
  int c3 = getMem(5);
  int c4 = getMem(5);
  int c5 = getMem(5);
  int c6 = getMem(5);
  int c7 = getMem(5);
  int c8 = getMem(5);
  int c9 = getMem(5);
  int d = setValue(a, "hola\n", 4);
  int x = setValue(c9, "si", 2);
  for(int i =0;i<fileNumber;i++){
    printf("Virtual: %d\n", virtualPages[i]);
  }
  for(int i = 0;i<10;i++){
    printf("Real %i\n", realPages[i]);
  }
  int d2 = setValue(a, "holi\n", 5);
  char *temp = (char *)calloc(5,sizeof(char));
  temp = getValue(a);
  printf("%s\n", temp);

  int d3 = setValue(c8, "holi\n", 5);
  return 0;
}
