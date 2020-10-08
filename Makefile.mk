# Makefile para Windows

PROG = loader.exe
FONTES = main.c lib\SOIL\image_DXT.c lib\SOIL\image_helper.c lib\SOIL\SOIL.c lib\SOIL\stb_image_aug.c
OBJETOS = $(FONTES:.c=.o)
CFLAGS = -g # -Wall -g  # Todas as warnings, infos de debug
LDFLAGS =  -lopengl32 -lm
CC = gcc

$(PROG): $(OBJETOS)
	gcc $(CFLAGS) $(OBJETOS) -o $@ $(LDFLAGS)

clean:
	-@ del $(OBJETOS) $(PROG)