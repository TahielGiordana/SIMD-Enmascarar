#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void enmascarar_c(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long tamanio);

float enmascarar_asm(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long tamanio);

int main(int argc, char *argv[]){

    // Validamos cantidad de argumentos
	if (argc != 4) {
		printf("Error: Deben ingresarse 3 parametros: imagen1, imagen2 y mascara \n");
		return 1;
	}

    //Recibimos la ruta de las imagenes pasadas por argumento
    char* ruta_imagen1 = argv[1];
    char* ruta_imagen2 = argv[2];
    char* ruta_mascara = argv[3];

    FILE *img1,*img2,*mask,*imgOut,*imgOut2;
    long size;
    unsigned char *buffer1,*buffer2,*buffer3;

    img1 = fopen(ruta_imagen1,"rb");
    img2 = fopen(ruta_imagen2,"rb");
    mask = fopen(ruta_mascara,"rb");
    imgOut = fopen("salida_c.rgb","wb");
    imgOut2 = fopen("salida_asm.rgb","wb");

    //Obtenemos el largo tomando la ultima posicion del archivo, luego retornamos a la primera posicion 
    fseek(img1, 0, SEEK_END);
    size = ftell(img1);
    rewind(img1); 
 
    buffer1 = (char*) malloc(size);
    buffer2 = (char*) malloc(size);
    buffer3 = (char*) malloc(size);

    size_t final = fread(buffer1,1,size,img1);
    final = fread(buffer2,1,size,img2);
    final = fread(buffer3,1,size,mask);

    //Creamos variables para medir el tiempo transcurrido 
    clock_t inicio_c, fin_c, inicio_asm, fin_asm;
    int segundos_c;
	int segundos_asm;
 
	inicio_asm = clock();
    enmascarar_asm(buffer1,buffer2,buffer3,size);
    fin_asm = clock();
    segundos_asm = (int)(fin_asm - inicio_asm);

    fwrite(buffer1,1,size,imgOut2);
 
    inicio_c = clock();
    enmascarar_c(buffer1,buffer2,buffer3,size);
    fin_c = clock();
    segundos_c = (int)(fin_c - inicio_c);

    fwrite(buffer1,1,size,imgOut);

    free(buffer1);
    free(buffer2);
    free(buffer3);

    fclose(img1);
    fclose(img2);
    fclose(mask);
    fclose(imgOut);
    fclose(imgOut2);

    printf("Total de posiciones: %ld\nTiempo transcurrido C: %d\nTiempo transcurrido ASM: %d\n(Expresado en unidades de Clock)\n", size, segundos_c, segundos_asm);
    
    return 0;
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
