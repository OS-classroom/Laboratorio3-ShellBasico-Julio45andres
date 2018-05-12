#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define TAM 100

char* f_pwd();
void f_copy(char *source, char *destination);

char * f_pwd(){
    size_t alloc_size = sizeof(char) * TAM*10;
    char *cwd = malloc(alloc_size);
    if (getcwd(cwd, alloc_size) != NULL)
        return cwd;
    else{
        perror("getcwd() error");
        return NULL;
    }
}

void f_copy(char *source, char *destination){
    size_t alloc_size = sizeof(char) * TAM*20;

    char *cwd;
    char *src_path = malloc(alloc_size);
    char *dest_path = malloc(alloc_size);
    char *content = malloc(alloc_size);
    cwd = f_pwd();
    if(cwd == NULL){
        printf("Can't read the file path");
    }

    snprintf(src_path, TAM*20, "%s/%s", cwd, source);
    int fd = open(src_path, O_RDONLY);

    if(fd < 0){
        printf("Source file don't exist\n");
        return;
    }
    if(read(fd, content, alloc_size) < 0){
        printf("Could'n read source file\n");
        return;
    }
    close(fd);
    snprintf(dest_path, TAM*10, "%s/%s", cwd, destination);
    printf("File copied at %s\n", dest_path);
    fd = open(dest_path, O_RDWR | O_APPEND | O_CREAT, 0644);
    if(write(fd, content, alloc_size) < 0){
        printf("Could'n copy to destination file\n");
        return;
    }
    close(fd);
    free(cwd);
    free(src_path);
    free(dest_path);
}