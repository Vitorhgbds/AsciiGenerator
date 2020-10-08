# Makefile para Linux e macOS

PROG = loader
FONTES = main.c lib/SOIL/image_DXT.c lib/SOIL/image_helper.c lib/SOIL/SOIL.c lib/SOIL/stb_image_aug.c
OBJETOS = $(FONTES:.c=.o)
CFLAGS = -g -DGL_SILENCE_DEPRECATION # -Wall -g  # Todas as warnings, infos de debug
LDFLAGS =  -lopengl32 -lm

UNAME = `uname`

all: $(TARGET)
	-@make $(UNAME)

Darwin: $(OBJETOS)
	gcc $(OBJETOS) -O3 -Wno-deprecated -framework OpenGL -framework Cocoa -lm -o $(PROG)

Linux: $(OBJETOS)
	gcc $(OBJETOS) -O3 -lGL -lm -o $(PROG)

clean:
	-@ rm -f $(OBJETOS) $(PROG)
