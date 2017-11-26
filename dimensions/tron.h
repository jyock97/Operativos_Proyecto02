#ifndef TRON_H
#define TRON_H

extern "C" {
#include "virtualMemory.h"
#include "mythreads/my_mutex.h"
#include "mythreads/my_thread.h"
#include "mythreads/scheduler.h"
}
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctime>
#include <fcntl.h>
#include <pthread.h>

void initServer();
void *receive();
void * tron();


#endif
