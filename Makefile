CC = gcc.exe 
CFLAGS = -std=c99 -static -I ./include/
RAUDIOFLAGS = -DRAUDIO_STANDALONE -DSUPPORT_MODULE_RAUDIO -DSUPPORT_FILEFORMAT_MP3 -DSUPPORT_FILEFORMAT_WAV -DSUPPORT_FILEFORMAT_OGG -DSUPPORT_FILEFORMAT_FLAC -DSUPPORT_FILEFORMAT_XM -DSUPPORT_FILEFORMAT_MOD
SRC = wcmp.c ./lib/raudio.c

final: buildall


build: wcmp.o raudio.o ./lib/pdcurses.a
	@echo "Linking wcmp and libraries"
	$(CC) $(CFLAGS) wcmp.o raudio.o ./lib/pdcurses.a -o wcmp.exe 

wcmp.o: wcmp.c
	@echo "Building Main"
	$(CC) $(CFLAGS) -c wcmp.c

raudio.o: ./lib/raudio.c
	@echo "Building raudio"
	$(CC) $(CFLAGS) -c ./lib/raudio.c $(RAUDIOFLAGS)

clean:
	@echo "Removing build objects"
	rm *.o
