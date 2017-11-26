C_FILES = ricksmain.c
LIBS = fuse
DIR = prueba

mkfs.bwfs:
	gcc $(C_FILES) `pkg-config --cflags --libs $(LIBS)` -o mount.bwfs

mount:
	./mount.bwfs $(DIR) -o allow_other -o nosuid -d -s

umount:
	fusermount -u $(DIR)
