#include <stdio.h>


int main(){
/*

enmascarar_c(unsigned char *a, unsigned char *b, unsigned char *c, unsigned char *mask, int cant){

Revisar cada pixel de la mascara y si es blanco reemplazar en img1 por el pixel de img2
    for(int i=0, i<cant, i+=3){
        if(c[i]==0){
            a[i]=b[i];
            a[i+1]=b[i+1];
            a[i+2]=b[i+2];
        }
    }
    return salida_c.rgb;
}

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

Solicitar datos:

img1 = cargarImagen1...
img2 = cargarImagen2...
mask = cargarMascara...
int ancho;
int alto;
cant = alto*ancho*3; (rgb cada color utiliza un byte)

img_c = enmascarar_c(&img1, &img2, &mask, cant);
img_asm = enmascarar_asm(&img1, &img2, &mask, cant);

*/
}