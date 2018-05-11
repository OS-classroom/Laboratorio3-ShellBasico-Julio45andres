/*
* Julián Muñoz M 
* SO y lab 2018 -1
* UdeA
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

void usage(char *filename);
int searchInFile(char *filename, char *s);
void print_status(char *tgid);

int main (int argc, char *argv[]){

    // int result;
    if(argc < 2 || argc > 2){
        usage(argv[0]);
        exit(1);
    }

 /*    printf("%s", argv[1]);

    char str[30] = "VmData";
    char *fname = "/proc/";
    strncat(fname, argv[1], 5);
    strncat(fname, "/status", 10); */

    /* if(result == -1){
        perror("Error");
        exit(1);
    } */

    print_status(argv[1]);

    return 0;
}

void usage(char *app){
    printf("Usage: %s [PID]\n\n", app);
}

void print_status(char *tgid){
    printf("%s\n", tgid);
    int salto = 0;

    char path[40], line[100], *p;
    FILE *statusf;

    snprintf(path, 40, "/proc/%s/status", tgid);
    printf("/proc/%s/status\n", tgid);
    statusf = fopen(path, "r");
    if(!statusf)
        return;
    
    while(fgets(line, 100, statusf)){
        if(strncmp(line, "Name:", 5) == 0)
        {
            salto = 6;
        }
        if(strncmp(line, "Groups:", 7) == 0){
            salto = 8;
            printf("groups:\n)");
        } 
        if(salto == 0)
            continue;
        
        
        // Ignorar Name:
        p = line + salto;
        // Ignorar espacio en blanco
        while(isspace(*p)) ++p;
        printf("%s", p);
        salto = 0;
        // break;
    }

    fclose(statusf);
}

int searchInFile2(char *filename, char *s){
    FILE *fp;
    int findResutl = 0;
    char temp[512];

    // Abre el archivo y lo asigna a la variable fp
    if((fp = fopen(filename, "r")) == NULL){
        return -1;
    }

    // Busca la cadena en el archivo e imprime la linea completa que contiene 
    // dicha cadena.
    while(fgets(temp, 512, fp) != NULL){
        if(strstr(temp, s) != NULL){
            printf("\n%s\n", temp);
            findResutl++;
        }
    }

    // Cierra el archivo si todavia está abierto.
    if(fp){
        fclose(fp);
    }   
    return 0;
}
