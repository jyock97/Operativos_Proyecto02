C_FILES = main.c
LIBS = gtk+-2.0

all:
	gcc main.c `pkg-config --cflags --libs $(LIBS)` -o eje

run:
	./eje
