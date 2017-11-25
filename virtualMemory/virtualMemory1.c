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
  new = fopen(intNumber, "w");

  sprintf(intNumber, "%d", size);
  //fputs("0 ", new);
  fputs(intNumber, new);
  fputs("\n", new);

  fclose(new);
  sprintf(intNumber, "%d", fileNumber);
  new = fopen(intNumber, "w+");
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
  void *data;
  char *size = (char*)calloc(20, sizeof(char));
  int place = search(numPage);
  if(place == -1){
    char * virtualFile = (char *)calloc(20, sizeof(char));
    sprintf(virtualFile, "%d", numPage);
    FILE *virtual = fopen(virtualFile, "w+");
    place = nextPage%10;
    fclose(realFiles[place]);
    realFiles[place] = virtual;
    realPages[place] = numPage;
    nextPage++;
  }
  fgets(size, 20, realFiles[place]);
  numSize = atoi(size);
  fgets(data, numSize, realFiles[place]);
  return data;
}

int setValue(int numPage, void *value, int cant){
  //const char s[2] = " ";
  int place = search(numPage);
  int numSize;
  char * data = (char *)calloc(4096, sizeof(char));
  char * size = (char *)calloc(20, sizeof(char));
  data = value;
  printf("%d\n", place);
  if(place == -1){
    char * virtualFile = (char *)calloc(20, sizeof(char));
    sprintf(virtualFile, "%d", numPage);
    FILE *virtual = fopen(virtualFile, "w+");
    place = nextPage%10;
    fclose(realFiles[place]);
    realFiles[place] = virtual;
    realPages[place] = numPage;
    nextPage++;
  }
  fgets(size, 20, realFiles[place]);
  //char *start, *end;
  //start = strtok(size, s);
  //end = strtok(NULL, s);

  //numSize = atoi(end) - atoi(start);
  numSize = atoi(size);
  if(numSize>cant){
    fputs(size, realFiles[place]);
    fputs(data, realFiles[place]);
    fclose(realFiles[place]);
    char*temp = (char*)calloc(12,sizeof(char));
    sprintf(temp, "%d", place);
    realFiles[place] = fopen(temp, "w+");
  }
  return 1;
}

int main(){
  int a = getMem(5);
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
  int d = setValue(a, "hola", 4);
  int x = setValue(c9, "si", 2);
  printf("%d %d %d\n", a,b,c9);
  for(int i = 0;i<10;i++){
    printf("%i\n", realPages[i]);
  }
  printf("%s\n", (char *)getValue(a));
  int d2 = setValue(a, "holi", 4);
  return 0;
}
