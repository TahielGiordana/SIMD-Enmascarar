#include <stdio.h>
#include <stdlib.h>
/*

enmascarar_c(unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *mask, int cant){

enmascarar_asm(unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *mask, int cant){
    
loop:
    cargar una cantidad de bytes de cada imagen en los registros

    mov xmm0, a
    mov xmm1, b
    mov xmm2, c

    xmm0 and xmm2 elimina de img1 los pixeles blancos de la mascara

    not xmm2 invierte la mascara

    xmm1 and xmm2 elimina de img2 los pixeles negros de la mascara

    xmm0 + xmm1 compone la imagen

    modificar ebp para seleccionar los bytes siguientes

    verificar que cont != cant

    jmp loop

return salida_asm.rgb;
};

*/

int main(int argc, char *argv[]){
    FILE *img1 = fopen("pacman_1.rgb","rb");
    FILE *img2 = fopen("pacman_2.rgb","rb");
    FILE *mask = fopen("pacman_mask.rgb","rb");
    FILE *imgOut = fopen("pacman_salida.rgb","wb");

    int i,r,g,b;
    int alto = 300;
    int ancho = 300;

    int size = ancho*alto;

    unsigned char buffer1[size][3];
    unsigned char buffer2[size][3];
    unsigned char buffer3[size][3];

    
    for(i=0;i<size;i++){
        r = 0;
        g = 0;
        b = 0;

        //Obtengo cada pixel de la imagen
        buffer1[i][0] = getc(img1);
        buffer1[i][1] = getc(img1);
        buffer1[i][2] = getc(img1);

        buffer2[i][0] = getc(img2);
        buffer2[i][1] = getc(img2);
        buffer2[i][2] = getc(img2);

        buffer3[i][0] = getc(mask);
        buffer3[i][1] = getc(mask);
        buffer3[i][2] = getc(mask);

        //Modifico los pixeles
        if(buffer3[i][0] != 255){
            
            r = buffer1[i][0];
            g = buffer1[i][1];
            b = buffer1[i][2];
        }
        else{
            
            r = buffer2[i][0];
            g = buffer2[i][1];
            b = buffer2[i][2];
        }

        //Escribo los valores en el nuevo archivo
        putc(r,imgOut);
        putc(g,imgOut);
        putc(b,imgOut);
    }

    fclose(img1);
    fclose(img2);
    fclose(mask);
    fclose(imgOut);

}