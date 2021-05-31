# Trabajo Práctico 2 - Enmascarar con SIMD

#### Organización del Computador 2
#### UNGS 1er Semestre 2021

##### Grupo: Giordana Tahiel, Geiser Andrés

## Índice
  - [Descripción](#descripción)
  - [Requisitos](#requisitos)
  - [Enmascarar](#enmascarar)
    - [Enmascarar C](#enmascarar-c)
    - [Enmascarar ASM](#enmascarar-asm)
    - [Compilación](#compilación)
    - [Ejecución](#ejecución)
    - [Capturas](#capturas)

## Descripción

El objetivo de este trabajo es comparar la performance de las instrucciones SIMD con las instrucciones aritméticas y lógicas de la AL
U, se debe desarrollar una aplicación de linea de comandos que combine dos imágens usando una máscara de selección. Implementaremos dos versiones de esta operación: primero en lenguaje C y luego utilizando instrucciones SIMD en lenguaje ensamblador.

Las imágenes que recibe son en formato rgb, el cual almacena los valores rojo, verde y azul de cada pixel usando tres bytes consecutivos. Estas imágenes deben tener las mismas dimensiones, además la máscara solo debe contener los valores blanco(FFFFFF) y negro(000000) de manera que los pixeles negros de la máscara seleccionen el pixel de la primer imagen, y los pixeles blancos de la máscara seleccione los pixeles de la segunda.

## Requisitos

Para la compilación del código assembler se requiere utilizar [NASM](https://www.nasm.us) y para el código C se requiere [gcc](https://gcc.gnu.org).

Recomendamos el uso de [GraphicsMagick](http://www.graphicsmagick.org/) para convertir las imágenes a procesar en archivos rgb, además de contar con herramientas de edición para las imágenes.

## Enmascarar
Creamos un programa en C encargado de recibir los parámetros y llamar a las funciones.

Comenzamos importando las librerías y declarando las funciones a implementar. Las imágenes se reciben cada una almacenada en un buffer, el tamaño de la imagen corresponde a la cantidad de bytes a leer de cada imagen.
```c
#include <stdio.h>
#include <stdlib.h>

void enmascarar_c(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long tamanio);

void enmascarar_asm(unsigned char *buffer1, unsigned char *buffer2, unsigned char *mascara, long tamanio);
```
Los parámetros de la aplicación son pasados por linea de comandos, se verifica que no falten argumentos y se almacena la ruta de las imágenes.

```c
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
```
Utilizamos variables de tipo FILE para almacenar los archivos. Mediante el método fopen abrimos las imágenes, además creamos los archivos de salida de las imágenes a las cuales se les aplicó la máscara. Luego obtenemos la cantidad de bytes de las imágenes y reservamos suficiente memoria en cada buffer para almacenarlas. Por último copiamos cada imágen al buffer correspondiente mediante fread.

```c
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
```
Tomamos el tiempo que conlleva crear las imágenes con cada función para comparar la performance entre el uso de la ALU y de SIMD.
```c
//Creamos variables para medir el tiempo transcurrido 
    clock_t inicio_c, fin_c, inicio_asm, fin_asm;
    int segundos_c;
    int segundos_asm;    
```

Realizamos el llamado a ambas funciones y escibimos los datos en los archivos de salida controlando el tiempo de ejecución.
```c
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
```
Por último liberamos la memoria de los buffer que ya no utilizamos y cerramos los archivos. Además mostramos por pantalla las estadísticas de cada función.
```c
    free(buffer1);
    free(buffer2);
    free(buffer3);

    fclose(img1);
    fclose(img2);
    fclose(mask);
    fclose(imgOut);
    fclose(imgOut2);
    
    printf("Total de posiciones: %ld\nTiempo transcurrido C: %d\nTiempo transcurrido ASM: %d\n(Expresado en unidades de Clock)\n", size, segundos_c, segundos_asm);
    
    return 0
```

### Enmascarar C

Recorremos cada pixel de la máscara, recordemos que size es igual a la cantidad de bytes del archivo y cada pixel ocupa 3 bytes, por lo tanto en cada ciclo aumentamos i en 3.
Luego vamos comparando los bytes de la máscara con 255(blanco), cómo la máscara solo posee los colores blanco y negro nos basta con revisar el primer byte de ese ciclo para saber el valor de los 2 bytes siguientes. En caso de ser un pixel blanco, se reemplazan los valores de ese pixel en la imagen1 por los valores del pixel en la imagen2. En caso de ser negro el buffer no es alterado.
```c
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
```

### Enmascarar ASM
Al invocar la función desde C los parámetros son almacenados en la pila. Para poder obtener estos valores contamos con el registro **EBP** que apunta a la base de la pila, y el registro **ESP** que apunta al tope de la misma.

Declaramos la función como global, de esta manera el linker vinculará la función con la llamada en C. Para evitar incongruencias en la estructura de la pila comenzamos realizando el **Enter 0,0**. Además almacenamos en los registros las direcciones de los buffers y el tamaño de los archivos.

```asm
section .text

global enmascarar_asm

enmascarar_asm:

    push ebp
    mov ebp,esp
    xor eax,eax
    xor ecx,ecx

    mov eax,[ebp+8]     ;Almacena la direccion del buffer1
    mov ebx,[ebp+12]    ;Almacena la direccion del buffer2
    mov edx,[ebp+16]    ;Almacena la direccion de la mascara
    mov ecx,[ebp+20]    ;Almacena el size
    
    jmp loop
```
Procedemos a recorrer las imágenes, esta vez utilizando instrucciones SIMD. 

Comenzamos revisando si los archivos ya fueron recorridos en su totalidad, y si es así se produce un salto a la etiqueta end para finalizar la función. Procedemos a cargar en los registros MMX los 8 bytes correspondientes de cada imágen mediante el metodo movq. De esta manera el registro mm0 contiene valores de la imagen1, mm1 valores de la imagen2 y mm2 valores de la máscara. Luego aplicamos la instrucción lógica pand sobre la imagen2 y la máscara, filtrando solo los valores que coincidan con un pixel blanco de la máscara. A su vez aplicamos pandn sobre la imagen1 y la máscara, filtrando los valores que coincidan con un pixel negro. Por último unimos los resultados obtenidos mediante la instrucción por y los almacenamos en el buffer1. Como vamos cargando de a 8 bytes aumentamos las direcciones en 8 para apuntar a los siguientes datos, y así mismo decrementamos el contador en 8 hasta haber recorrido toda la imagen.
```asm
loop:

    cmp ecx,0           ;Revisar que queden pixeles por recorrer
    jl end              ;Sino terminar la función

    movq mm0,qword[eax] ;Guardar en el registro mm0 los primeros 8 caracteres del buffer1
    movq mm1,qword[ebx] ;Guardar en el registro mm1 los primeros 8 caracteres del buffer2
    movq mm2,qword[edx] ;Guardar en el registro mm1 los primeros 8 caracteres de la mascara

    pand mm1,mm2

    pandn mm2,mm0
    por mm1,mm2

    movq [eax],mm1      ;Mover al registro eax los caracteres modificados
  
    add eax,8           ;mover eax 8 bytes
    add ebx,8
    add edx,8
    sub ecx,8           ;decremento ecx en 8. (size/8)
    jmp loop
```

### Compilación

Partiendo de la carpeta raíz del proyecto, nos ubicamos en la carpeta **code**.
Para esto ejecutamos la siguiente línea en la terminal:
```
ls code
```
Luego compilamos el código assembler utilizando NASM, indicando la arquitectura de 32 bits:
```
nasm -f elf32 enmascarar.asm -o enmascarar.o
```
Continuamos compilando y linkeando el código C mediante gcc:
```
gcc -m32 -o main enmascarar.o main.c
```
Se incluye ademas un archivo **compilar.sh** para automatizar esta tarea.

### Ejecución

Una vez compilado podremos ejecutar el programa indicando la ruta de las imágenes. En nuestro caso contamos con imágenes de prueba en la carpeta img, por lo tanto un ejemplo sería el siguiente:
```
./main ../space1.rgb ../space2.rgb ../spacemask.rgb
```
De esta manera notamos que se crean los archivos **salida_c.rgb** y **salida_asm.rgb**.

Si contamos con GraphicsMagick podremos visualizar estas imágenes con el comando display, solo hay que pasar las dimensiones y el nombre de la imagen:
```
gm display -size 3840x2160 salida_c.rgb
gm display -size 3840x2160 salida_asm.rgb
```
Una alternativa online para visualizar las imágenes podría ser [Raw Pixels](http://www.rawpixels.net).
### Capturas

Se incluyen diversas imágenes de prueba en el repositorio, para probar el funcionamiento de las implementaciones vamos a ejecutar las pruebas sobre los archivos **space_1_HD.rgb**, **space_2_HD.rgb** y **space_mask_HD.rgb** las cuales se visualizan de la siguiente manera:
