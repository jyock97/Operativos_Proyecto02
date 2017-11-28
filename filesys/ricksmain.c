
#define FUSE_USE_VERSION 31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fuse.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

static const int BLOCK_SIZE = 4096; // 4Kb
char *rootPath = ".";

enum FILE_T{
    IS_DIR,
    IS_FILE,
    OTHER
};

struct s_blockFile{
    char *pathName;
    int size;
    int freeSpace;
    FILE *fd;
    struct s_blockFile *next;
};

struct s_file{
    char *pathName;
    enum FILE_T type;
    mode_t mode;
    size_t size;
    struct s_blockFile *blocks;
    struct s_file *next;
};

int sock, useSock;  //file descriptor del socket
int port = 5000;             //numero del puerto
char ipAddr[16] = "127.0.0.1";
struct sockaddr_in servAddr; //direccion el cliente

struct s_file *files = NULL;
struct s_file *endFile = NULL;
struct s_blockFile *blocks = NULL;
struct s_blockFile *endBlock = NULL;
int freeBlockCount = 0;
int lenFreeBlocks = 0;
int lenFiles = 0;

/**
 * Funcion la cual se conecta con el servidor(tron)
**/
void initClient(){

    sock = socket(AF_INET, SOCK_STREAM,0);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ipAddr);
    servAddr.sin_port = htons(port);
    if(connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) != -1){

      useSock = sock;
      printf("Se realiza conexion\n");
    }else{
      useSock = -1;
    }
}

/**
 * Funcion la cual envia un mensaje por red.
**/
void sendMessage(char *msg) {
    write(useSock, msg, strlen(msg));
}

/*
 * Funcion la cual invierte el orden de na linea de caracters
 * Entrada:
 *      string -> puntero a la linea de caracteres
*/
void strReverse(char *string){

    int len;
    char temp;

    len = strlen(string);

    for (int start = 0, end = len - 1; start <= end; start++, end--) {
        temp = string[start];
        string[start] = string[end];
        string[end] = temp;
    }
}

/**
 * Funcion la cual dado un numero, retorna el numero de digitos que este
 * presenta
 * Entrada:
 *      x -> Numero el cual sera procesado
**/
int intLen(int x){
    int temp, cont;

    temp = 9;
    cont = 1;
    while (x > temp) {
        cont ++;
        temp *= 10;
        temp += 9;
    }
    return cont;
}

/**
 * Funcion la cual se encarga de crear un bloque de memoria; este bloque
 * es en realidad un archivo al cual unicamente se le puede escribir
 * 4Kb de memoria
**/
struct s_blockFile *makeBlock(){
    struct s_blockFile *bf;
    int len, rootLen, temp;

    if (!blocks) {

        bf = malloc(sizeof(struct s_blockFile));
        len = intLen(freeBlockCount);
        rootLen = strlen(rootPath);
        bf -> pathName = malloc(rootLen + len + 1);
        bf -> pathName[rootLen+len] = '\0';
        temp = freeBlockCount;
        strcpy(bf -> pathName, rootPath);
        for (int i = rootLen + len - 1; i >= rootLen; i--) {
            bf -> pathName[i] = (temp % 10) + '0';
            temp /= 10;
        }
        freeBlockCount++;

    } else {
        bf = blocks;
        blocks = blocks -> next;
        lenFreeBlocks--;

        if (!blocks) {
            endBlock = NULL;
        }
    }
    bf -> size = 0;
    bf -> freeSpace = BLOCK_SIZE;
    bf -> fd = NULL;
    bf -> next = NULL;
    fclose(fopen(bf -> pathName, "w"));
    //printf("%s\n", bf -> pathName);

    return bf;
}

/**
 * Funcion la cual se encarga de marcar una lista de bloques como libres
 * para ser utilizados cuando se crean nuevos bloques.
 * Entrada:
 *      b -> Lista de bloques a ser liberados.
**/
void freeBlocks(struct s_blockFile *b) {
    printf("<freeBlocks>\n");
    if (!endBlock) {
        blocks = b;
        endBlock = b;
        lenFreeBlocks = 1;

    } else {
        endBlock -> next = b;
    }
    while (endBlock -> next) {
        endBlock = endBlock -> next;
        lenFreeBlocks++;
    }

}

/**
 * Funcion la cual se encarga de crear un nuevo archivo.
 * Entrada:
 *      name -> Nombre del archivo.
 *      type -> Tipo del archivo.
 *      mode -> Permisos del archivo.
**/
int makeFile(const char *name, enum FILE_T type, mode_t mode){
    printf("makeFile <%s>\n", name);

    struct s_file *f = calloc(1, sizeof(struct s_file));

    f -> pathName = malloc(strlen(name) + 1);
    strcpy(f -> pathName, name);
    f -> type = type;
    f -> blocks = makeBlock();
    f -> mode = mode;

    if (!endFile) {
        files = f;
        endFile = f;

    } else {
        endFile -> next = f;
        endFile = f;
    }
    lenFiles++;
    if(useSock != -1){
      sendMessage("000");

    }
    return 0;
}

/**
 * Funcion la cual se encarga de buscar y retornar el archivo asociado
 * al pathName.
 * Entrada:
 *      pathName -> Parametro mediante el cual se realizara la busqueda.
**/
struct s_file *searchFile(const char *pathName){
    printf("busco <%s>\n", pathName);

    struct s_file *f = NULL;
    struct s_file *temp;

    temp = files;
    while (temp != NULL && f == NULL) {
        printf("\t%s\n", temp -> pathName);
        if (!strcmp(temp -> pathName, pathName)) {
            f = temp;

        } else {
            temp = temp -> next;
        }
    }

    return f;
}

/**
 * Funcion la cual se encarga de abrir todos los bloques de un archivo.
 * Entrada:
 *      f -> Archivo el cual se desea abrir.
**/
void openFile(struct s_file *f){

    struct s_blockFile *bf;

    bf = f -> blocks;
    while (bf != NULL) {
        bf -> fd = fopen(bf -> pathName, "a+");
        bf = bf -> next;
    }
}

/**
 * Funcion la cual se encarga de escribir un string en un archivo.
 * Entrada:
 *      f -> Archivo al cual se va a escribir.
 *      string -> String el cual se desea escribir.
**/
int writeFile(struct s_file *f, const char *string){
    printf("\tescribir <%s>\n", string);

    struct s_blockFile *bptr;
    int len;

    len = strlen(string);

    bptr = f -> blocks;
    while (bptr -> freeSpace - len < 0) {
        if (!bptr -> next)
            break;
        bptr = bptr -> next;
    }
    if (bptr -> freeSpace - len < 0) {
        bptr -> next = makeBlock();
        bptr = bptr -> next;
    }

    if(bptr -> fd == NULL)
        bptr -> fd = fopen(bptr -> pathName, "a");

    if (bptr -> fd == NULL) {
        printf("\t\tError al abrir\n");
        return -1;
    }
    fprintf(bptr -> fd, "%s", string);

    fclose(bptr -> fd);
    bptr -> fd = NULL;
    bptr -> size += len;
    bptr -> freeSpace -= len;
    f -> size += len;
    printf("se escribe\n");
    if(useSock != -1){
      sendMessage("000");

    }
    return 0;
}

/**
 * Funcion la cual se encarga de buscar y eliminar un string de un
 * archivo.
 * Entrada:
 *      f -> Archivo al cual se va a eliminar.
 *      string -> String el cual se desea eliminar.
**/
void deleteInFile(struct s_file *f, char *string){
    printf("<deleteInFile <%s>\n", string);
    char s[256];
    int bFinish;
    FILE *tf;
    struct s_blockFile *bf;

    bFinish = 0;
    bf = f -> blocks;
    tf = NULL;
    while (bf && !bFinish) {
        bf -> fd = fopen(bf -> pathName, "r");
        tf = fopen(".temp", "w");
        while (fgets(s, 256, bf -> fd)) {
            if (!strcmp(s, string)) {
                bFinish = 1;

            } else {
                fprintf(bf -> fd, "%s", s);
            }
        }
        fclose(bf -> fd);
        fclose(tf);
        bf -> fd = NULL;
        tf = NULL;
        rename(".temp", bf -> pathName);
    }
}

/**
 * Funcion la cual se encarga de eliminar un archivo y liberar los bloques.
**/
void deleteFile(const char *pathName){
    printf("deleteFile <%s>\n", pathName);

    struct s_file *f, *temp;

    f = files;
    if (!strcmp(f -> pathName, pathName)) {
        files = f -> next;
        temp = f;

    } else {
        temp = f -> next;
        while (strcmp(temp -> pathName, pathName)) {
            f = temp;
            temp = temp -> next;
        }
        f -> next = temp -> next;
    }
    if (temp == endFile) {
        endFile = f;
    }
    free(temp -> pathName);
    freeBlocks(temp -> blocks);
    free(temp);
}

/**
 * Funcion la cual cambia el nombre de un archivo.
 * Entrada:
 *      f -> Archivo al cual se le va a cambiar el nombre.
 *      pathName -> Nuevo nombre del archivo.
**/
void renameFile(struct s_file *f, const char *pathName){
    printf("<renameFile>\n");
    char *s;

    s = malloc(strlen(pathName) + 1);
    strcpy(s, pathName);
    free(f -> pathName);
    f -> pathName = s;
}

/**
 *
**/
int eraseFile(const char *pathName){
    char *s, *dir, *path;
    int len, bFinish;
    struct s_file *f1, *f2;
    struct s_blockFile *bf;
    FILE *tf;

    f1 = searchFile(pathName);
    if (!f1) {
        return -ENOENT;     //El archivo no existe
    }

    s = malloc((strlen(pathName) + 2));
    strcpy(s, pathName);

    len = strlen(pathName);
    s[len] = '\n';
    s[len+1] = '\0';
    for (int i = len; i >= 0; i--) {
        if (s[i] == '/') {
            s[i] = '\0';
            break;
        }
        dir = &s[i];
    }
    if (s[0] == '\0') {
        path = "/";

    } else {
        path = s;
    }

    f2 = searchFile(path);

    deleteInFile(f2, dir);
    deleteFile(pathName);

    free(s);
    lenFiles--;
    return 0;
}

/**
 *
**/
int rGetattr(const char *pathName, struct stat *stbuf){
    printf("getattr <%s>\n", pathName);

    struct s_file *f = searchFile(pathName);
    if (!f) {
        return -ENOENT;
    }

    memset(stbuf, 0, sizeof(struct stat));
    if (f -> type == IS_DIR) {
        stbuf->st_mode = S_IFDIR | 0775;
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();

    } else if (f -> type == IS_FILE) {
        stbuf->st_mode = S_IFREG | 0664;
        stbuf->st_uid = getuid();
        stbuf->st_gid = getgid();
        stbuf->st_size = f -> size;

    } else {
        return -ENOENT;
    }
    return 0;
}

/**
 *
**/
int rOpen(const char *path, struct fuse_file_info *fi){

    struct s_file *f;
    struct s_blockFile *bptr;

    f = searchFile(path);
    if (!f) {
        return -ENOENT;
    }

    bptr = f -> blocks;
    while (bptr) {
        bptr -> fd = fopen(bptr -> pathName, "a+");
        bptr = bptr -> next;
    }
}

/**
 *
**/
int rRead(const char *pathName, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    printf("read <%s> <%lu>\n", pathName, size);

    int len, totalSize;
    char *string, *ptr;
    struct s_file *f;
    struct s_blockFile *bptr;

    f = searchFile(pathName);
    if (!f) {
        return -ENOENT;
    }
    totalSize = 0;
    bptr = f -> blocks;
    string = malloc(size+1);
    ptr = buf;
    while (bptr != NULL && size > 0) {
        if (!bptr) {
            bptr -> fd = fopen(bptr -> pathName, "r");
        }
        while (fgets(string, size, bptr -> fd) && size > 0) {
            len = strlen(string);
            if (size - len < 0) {
                len = size;
            }
            memcpy(ptr, string, len);
            size -= len;
            totalSize += len;
            ptr+=len;
        }
        fclose(bptr -> fd);
        bptr = bptr -> next;
    }
    free(string);
    printf("<<%d>>\n", totalSize);
    if(useSock != -1){
      sendMessage("000");

    }
    return totalSize;
}

/**
 *
**/
int rWrite(const char *pathName, const char *string, size_t size, off_t t_off, struct fuse_file_info *_ffi){
    printf("write <%s>\n", pathName);

    struct s_file *f;

    f = searchFile(pathName);
    if (!f)
        return -ENOENT;

    writeFile(f, string);
    return strlen(string);
}

/**
 *
**/
int rRename(const char *pathName, const char *pathNewName){
    printf("====================================\nrename <%s> -> <%s>\n", pathName, pathNewName);
    char *s, *s2, *path, *dir, *path2, *dir2;
    int len;
    struct s_file *f, *f1, *f2;

    f = searchFile(pathName);
    if (!f)
        return -ENOENT;

    s = malloc((strlen(pathName) + 2) * sizeof(char));
    strcpy(s, pathName);

    len = strlen(pathName);
    s[len] = '\n';
    s[len+1] = '\0';
    for (int i = len; i >= 0; i--) {
        if (s[i] == '/') {
            s[i] = '\0';
            break;
        }
        dir = &s[i];
    }
    if (s[0] == '\0') {
        path = "/";

    } else {
        path = s;
    }

    s2 = malloc((strlen(pathNewName) + 2) * sizeof(char));
    strcpy(s2, pathNewName);

    len = strlen(pathNewName);
    s2[len] = '\n';
    s2[len+1] = '\0';
    for (int i = len; i >= 0; i--) {
        if (s2[i] == '/') {
            s2[i] = '\0';
            break;
        }
        dir2 = &s2[i];
    }
    if (s2[0] == '\0') {
        path2 = "/";

    } else {
        path2 = s2;
    }

    f2 = searchFile(path2);
    if (!path2)
        return -ENOENT;

    f1 = searchFile(path);
    deleteInFile(f1, dir);

    if (searchFile(pathNewName)) {
        deleteFile(pathNewName);
    }
    writeFile(f2, dir2);
    renameFile(f, pathNewName);

    free(s);
    free(s2);
    return 0;
}

/**
 *
**/
int rMkdir(const char *pathName, mode_t mode){
    printf("====================================\nmkdir <%s>\n", pathName);

    char *s, *dir, *path;
    int len;
    struct s_file *f;

    f = searchFile(pathName);
    if (f) {
        return -EEXIST;     //El archivo existe
    }

    s = malloc((strlen(pathName) + 2) * sizeof(char));
    strcpy(s, pathName);

    len = strlen(pathName);
    s[len] = '\n';
    s[len+1] = '\0';
    for (int i = len; i >= 0; i--) {
        if (s[i] == '/') {
            s[i] = '\0';
            break;
        }
        dir = &s[i];
    }
    if (s[0] == '\0') {
        path = "/";

    } else {
        path = s;
    }

    f = searchFile(path);
    if (!f) {
        return -ENOENT;
    }
    //printf("path: <%s> -- dir: <%s>\n", path, dir);
    if (writeFile(f, dir) != 0)
        return -EPERM;

    makeFile(pathName, IS_DIR, mode);

    free(s);
    return 0;
}

/**
 *
**/
int rRmdir(const char *pathName){
    printf("====================================\nrmdir <%s>\n", pathName);
    return eraseFile(pathName);
}

/**
 *
**/
int rOpendir(const char *path, struct fuse_file_info *fi){
    printf("====================================\nopendir <%s>\n", path);

    struct s_file *f;

    f = searchFile(path);
    if (!f) {
        return -ENOENT;
    }

    openFile(f);

    return 0;
}

/**
 *
**/
int rReaddir(const char *pathName, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
    printf("====================================\nreaddir <%s>\n", pathName);

    struct s_file *f;
    struct s_blockFile *bptr;
    char string[256];
    int len;

    f = searchFile(pathName);

    if (f -> type != IS_DIR)
        return -ENOTDIR;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    bptr = f -> blocks;
    while (bptr != NULL) {
        if (bptr -> fd == NULL)
            bptr -> fd = fopen(bptr -> pathName, "r");

        while (fgets(string, 256, bptr -> fd)) {
            printf("<%s>\n", string);
            len = strlen(string);
            string[len-1] = '\0';
            filler(buf, string, NULL, 0);
        }
        bptr = bptr -> next;
    }
    return 0;
}

/**
 *
**/
int rStatfs(const char *pathName, struct statvfs *stat){
    printf("====================================\nstatfs <%s>\n", pathName);

    stat -> f_bsize = BLOCK_SIZE;
    stat -> f_frsize = 0;
    stat -> f_blocks = freeBlockCount;
    stat -> f_bfree = lenFreeBlocks;
    stat -> f_bavail = 0;
    stat -> f_files = lenFiles;
    stat -> f_ffree = 0;
    stat -> f_namemax = 256;

    return 0;
}

/**
 *
**/
int rFsync(const char *pathName, int t_int, struct fuse_file_info *fi){
    printf("====================================\nfsync <%s>\n", pathName);
    return 0; //siempre se guarda en disco, por lo que esta de más esta operacion
}

/**
 *
**/
int rAccess(const char *pathName, int mode){
    printf("====================================\naccess <%s>\n", pathName);
    return 0;
    struct s_file *f;

    f = searchFile(pathName);
    if (!f)
        return -ENOENT;

    printf("<%d - %d>\n", mode, f -> mode);
    if (f -> mode != mode)
        return -EACCES;

    return 0;
}

/**
 *
**/
int rCreate(const char *pathName, mode_t mode, struct fuse_file_info *fi){
    printf("====================================\ncreate <%s>\n", pathName);

    char *s, *dir, *path;
    int len;
    struct s_file *f;

    f = searchFile(pathName);
    if (f) {
        return -EEXIST;     //El archivo existe
    }

    s = malloc((strlen(pathName) + 2) * sizeof(char));
    strcpy(s, pathName);

    len = strlen(pathName);
    s[len] = '\n';
    s[len+1] = '\0';
    for (int i = len; i >= 0; i--) {
        if (s[i] == '/') {
            s[i] = '\0';
            break;
        }
        dir = &s[i];
    }
    if (s[0] == '\0') {
        path = "/";

    } else {
        path = s;
    }

    f = searchFile(path);
    if (!f) {
        return -ENOENT;
    }

    if (writeFile(f, dir) != 0)
        return -EPERM;

    makeFile(pathName, IS_FILE, mode);


    free(s);
    return 0;
}

/**
 *
**/
int rUnlink(const char *pathName){
    printf("====================================\nunlink <%s>\n", pathName);
    return eraseFile(pathName);;
}

struct fuse_operations fOperations = {
    .getattr    = rGetattr,     //Optener los atributos [directorio o archivo]
    .open       = rOpen,        //Abrir archivo
    .read       = rRead,        //Leer de un archivo
    .write      = rWrite,       //Escribir en un archivo
    .rename     = rRename,    //Renombrar un archivo
    .opendir    = rOpendir,
    .readdir    = rReaddir,     //Leer un directorio
    .rmdir      = rRmdir,       //Borrar un directorio
    .mkdir      = rMkdir,       //Crear un directorio
    .statfs     = rStatfs,                                    //-0-
    .fsync      = rFsync,                                     //-0-
    .access     = rAccess,
    .create     = rCreate,      //Crear un archivo
    .unlink     = rUnlink       //Eliminar un archivo
};

int main(int argc, char *argv[]){
    // rootPath = malloc(strlen(argv[1] + 2)); //no puedo crear los "bloques" directamente
    // strcpy(rootPath, argv[1]);              //en el path donde se monta el filesystem
    // strcat(rootPath, "/");                  //ya que se encicla

    initClient();
    makeFile("/", IS_DIR, 777);
    printf("\t\t\"%s\"\n", files -> pathName);

    return fuse_main(argc, argv, &fOperations, NULL);
}
