C_FILES = main.c
LIBS = fuse

all:
	gcc main.c `pkg-config --cflags --libs $(LIBS)` -o eje

run:
	./eje
