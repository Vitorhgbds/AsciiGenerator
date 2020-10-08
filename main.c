#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

char* chooseCharacter(float grayScale);

// Protótipos
void load(char* name, Img* pic);

char* chooseCharacter(float grayScale){
    if( grayScale >= 0 && grayScale < 51){
        return " ";
    } else if(grayScale < 102){
        return "/";
    } else if(grayScale < 153){
        return "f";
    } else if(grayScale < 204){
        return "%%";
    }
    return "@";
}

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{

    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc == 1) {
        printf("loader [img]\n");
        //exit(1);
    }

    load("british_summer.jpg", &pic);
    char* picture[pic.height][pic.width];

    float pxs[pic.height][pic.width];


    for(int i = 0, k = 0; i < pic.height; i++){
        for(int j = 0; j < pic.width; j++,k++){
            pxs[i][j] = (float)((0.3 * pic.img[k].r) + (0.59 * pic.img[k].g) + (0.11 * pic.img[k].b));
        }
    }

    for(int i = 0; i < pic.height; i++){
        for(int j = 0; j < pic.width; j++){
            picture[i][j] = chooseCharacter(pxs[i][j]);
        }
    }

    //for(int i=0; i<pic.width*pic.height; i++) {
    //     pxs[i] = (int) ((0.3 * pic.img[i].r) + (0.59 * pic.img[i].g) + (0.11 * pic.img[i].b));
    //}
    // printf("Primeiros 10 pixels da imagem:\n");
    // for(int i=0; i<pic.width*pic.height; i++) {
    //      pxs[i] = (int) ((0.3 * pic.img[i].r) + (0.59 * pic.img[i].g) + (0.11 * pic.img[i].b));
    // }
    // for(int i = 10; i < 20; i++){
    //     printf("[%d] ", pxs[i]);
    // }
    // printf("\n");

    // Inverte as cores
    for(int i=0; i<pic.width*pic.height; i++) {
        pic.img[i].r = 255 - pic.img[i].r;
        pic.img[i].g = 255 - pic.img[i].g;
        pic.img[i].b = 255 - pic.img[i].b;
    }

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height,
        3, (const unsigned char*) pic.img);

    // Exemplo: gravando um arquivo saida.html
    FILE* arq = fopen("saida.html", "w"); // criar o arquivo: w
    if(arq == NULL) // 0, falso
    {
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    fprintf(arq, "<html>\n");
    fprintf(arq,"<head>");
    fprintf(arq,"</head>\n");
    fprintf(arq,"<body style=\"background: black;\" leftmargin=0 topmargin=0>\n");
    fprintf(arq,"<style>\n");
    fprintf(arq,"\tpre  {\n \t\tcolor: white;\n\t\tfont-family: Courier;\n\t\tfont-size: 1.5px;\n\t}\n");
    fprintf(arq,"</style>\n");
    fprintf(arq,"<pre>\n");
    for(int i = 0; i < pic.height; i++){
        for(int j = 0; j < pic.width;j++){
            fprintf(arq,picture[i][j]);
        }
        fprintf(arq,"\n");
    }
    fprintf(arq,"</pre>\n");
    fprintf(arq,"</body>\n");
    fprintf(arq, "</html>\n");

    fclose(arq);
    free(pic.img);
}
