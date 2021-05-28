#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    FILE *img1,*img2,*mask,*imgOut;
    long size;
    char *buffer1,*buffer2,*buffer3,*bufferSalida;

    img1 = fopen("../img/space1.rgb","rb");
    img2 = fopen("../img/space2.rgb","rb");
    mask = fopen("../img/spacemask.rgb","rb");
    imgOut = fopen("space_salida.rgb","wb");

    int alto = 2160;
    int ancho = 3840;
    size = ancho*alto*3;

    buffer1 = (char*) malloc(size);
    buffer2 = (char*) malloc(size);
    buffer3 = (char*) malloc(size);

    size_t final = fread(buffer1,1,size,img1);
    final = fread(buffer2,1,size,img2);
    final = fread(buffer3,1,size,mask);

    unsigned char ch;
    
    for(long i=0;i<size;i+=3){
        ch = (unsigned char)buffer3[i];
        printf("%u",(unsigned int)ch);
        //Modifico los pixeles
        if((unsigned int)ch == 255){            
            buffer1[i] = buffer2[i];
            buffer1[i+1] = buffer2[i+1];
            buffer1[i+2] = buffer2[i+2];
        }
    }

    fwrite(buffer1,1,size,imgOut);

    free(buffer1);
    free(buffer2);
    free(buffer3);

    fclose(img1);
    fclose(img2);
    fclose(mask);
    fclose(imgOut);
    
}