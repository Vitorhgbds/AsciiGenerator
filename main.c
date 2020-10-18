#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings
#include <math.h>

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height, totalSize;
    RGB* img;
} Img;
void resize(int** originalPic, int** reducedPic, Img* origiPic, Img* reduPic);


char* chooseCharacter(int grayScale);

// Protótipos
void load(char* name, Img* pic);

/**
 * 
*/
void resize(int** originalPic, int** reducedPic, Img* origiPic, Img* reduPic){
    int linePosOriginalPic = 0;
    int columPosOriginalPic = 0;

    float widthRelation = (origiPic->width / (float) reduPic->width);
    float heightRelation = (origiPic->height / (float) reduPic->height);

    for(int i = 0; i < reduPic->height; i++){
        linePosOriginalPic = round(i * heightRelation);
        for(int j = 0; j < reduPic->width; j++){
            columPosOriginalPic = round(j * widthRelation);
            reducedPic[i][j] = originalPic[linePosOriginalPic][columPosOriginalPic];
        }
    }
}

char* chooseCharacter(int grayScale){
    if( grayScale >= 0 && grayScale < 51){
        return ".";
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
    pic->totalSize = pic->width * pic->height;
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
   // if(argc == 1) {
    //    printf("loader [img]\n");
    //    exit(1);
    //}

    load("british_summer.jpg", &pic);

    Img reducedPic;
    reducedPic.height = (int)pic.height/2;
    reducedPic.width = (int)pic.width/2;
    reducedPic.totalSize = reducedPic.height * reducedPic.width;
    reducedPic.img = malloc(sizeof(RGB) * reducedPic.totalSize);
    
    printf("%ld\n",sizeof(reducedPic));
    printf("%ld\n",sizeof(pic));
    char* picture[(int)pic.height / 2][(int) pic.width / 2];

    //instanciando a matriz da imagem original dinâmicamente
    int** pxs = malloc(pic.height * sizeof(int*));
    
    for(int i = 0; i < (pic.height); i++){
        pxs[i] = malloc(pic.width * sizeof(int));
    }
    //fim da alocação dinâmica

    //instanciando a matriz da imagem reduzida dinâmicamente
    int** reducedPxs = malloc(((int)pic.height / 2) * sizeof(int*));
    for(int i = 0; i < ((int)pic.height / 2); i++){
        reducedPxs[i] = malloc(((int)pic.width / 2) * sizeof(int));
    }
    //fim da alocação dinâmica

    //fazendo a média ponderada da escala de cinza
    for(int i = 0, k = 0; i < pic.height; i++){
        for(int j = 0; j < pic.width; j++,k++){
            pxs[i][j] = (int)((0.3 * pic.img[k].r) + (0.59 * pic.img[k].g) + (0.11 * pic.img[k].b));
        }
    }

    resize(pxs, reducedPxs, &pic, &reducedPic);
    
    for(int i = 0; i < ((int)pic.height / 2); i++){
        for(int j = 0; j < ((int)pic.width / 2); j++){
            picture[i][j] = chooseCharacter(reducedPxs[i][j]);
        }
    }





    //KISS = Keep it simple stupid


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
    fprintf(arq,"\tpre  {\n \t\tcolor: white;\n\t\tfont-family: Courier;\n\t\tfont-size: 8px;\n\t}\n");
    fprintf(arq,"</style>\n");
    fprintf(arq,"<pre>\n");

    int sum = 0;
    for(int line = 0; line + 5 <= (pic.height); line += 5) {
        for (int column = 0; column + 4 <= (pic.width); column += 4) {
            for (int sectorLine = line; sectorLine < line + 5; sectorLine++) {
                for (int sectorColumn = column; sectorColumn < column + 4; sectorColumn++) {
                    sum += pxs[sectorLine][sectorColumn];
                }
            }
            fprintf(arq,chooseCharacter((int) round(sum / 20.0f)));
            sum = 0;
        }
        fprintf(arq,"\n");
    }
    fprintf(arq,"</pre>\n");
    fprintf(arq,"</body>\n");
    fprintf(arq, "</html>\n");

    fclose(arq);
    free(pic.img);
}
