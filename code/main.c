#include <stdio.h>
#include <stdlib.h>

void enmascarar_c(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long tamanio);

float enmascarar_asm(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long tamanio);

int main(int argc, char *argv[]){
    FILE *img1,*img2,*mask,*imgOut,*imgOut2;
    long size;
    unsigned char *buffer1,*buffer2,*buffer3;

    img1 = fopen("../img/pacman_1.rgb","rb");
    img2 = fopen("../img/pacman_2.rgb","rb");
    mask = fopen("../img/pacman_mask.rgb","rb");
    imgOut = fopen("salida_c.rgb","wb");
    imgOut2 = fopen("salida_asm.rgb","wb");

    int alto = 300;
    int ancho = 300;
    size = ancho*alto*3;

    buffer1 = (char*) malloc(size);
    buffer2 = (char*) malloc(size);
    buffer3 = (char*) malloc(size);

    size_t final = fread(buffer1,1,size,img1);
    final = fread(buffer2,1,size,img2);
    final = fread(buffer3,1,size,mask);

    enmascarar_asm(buffer1,buffer2,buffer3,size);
    
    fwrite(buffer1,1,size,imgOut2);

    enmascarar_c(buffer1,buffer2,buffer3,size);

    fwrite(buffer1,1,size,imgOut);

    free(buffer1);
    free(buffer2);
    free(buffer3);

    fclose(img1);
    fclose(img2);
    fclose(mask);
    fclose(imgOut);
    fclose(imgOut2);
    
}

void enmascarar_c(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long size){
    
    unsigned char ch;
    
    for(long i=0;i<size;i+=3){
        ch = (unsigned char)mascara[i];
        //Modifico los pixeles
        if((unsigned int)ch == 255){            
            buffer1[i] = buffer2[i];
            buffer1[i+1] = buffer2[i+1];
            buffer1[i+2] = buffer2[i+2];
        }
    }
}