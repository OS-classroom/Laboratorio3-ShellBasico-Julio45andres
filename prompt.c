#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "parser.h"
#include "psinfo.h"

#define TAM 100
#define NUM_PROCS 50
/*
typedef struct String {
    char *val;
    void (*equals) ()
}*/

/*
 * TODO: Implementar las funciones mas dificiles: mytime y myps
 * Ojo: no implementar mypause
 * Ya estan mypwd, myecho, psinfo y myexit
 */

bool equals(char *x, char *y);
void usage(char *use);
void invoke(char *proc_path, char **params, int background);
char* f_pwd();
void f_copy(char *source, char *destination);

int main(void){
    char ** items;
    bool end = false;
    int num, background;
    char expresion[TAM];
    char *cmd;

    while(!end){
        /*Wait for input*/
        printf("Bash&Roll 🤘 > ");
        fgets(expresion, TAM, stdin);

        num = separaItems(expresion, &items, &background);
        if(num == 0) continue;
        cmd = items[0]; /*El comando que se desea ejecutar*/

        if(equals(cmd, "pwd")){
            char *cwd;
            cwd = f_pwd();
            if(cwd != NULL){
                printf("%s\n", cwd);
            }
            free(cwd);
        }
        else if(equals(cmd, "cp")){
            if(num < 3){
                usage("cp <src> <dest>");
                continue;
            }
            f_copy(items[1], items[2]);
        }
        else if(equals(cmd, "echo")){
            if(num < 2){
                usage("echo <msg>");
                continue;
            }
            printf("%s\n", items[1]);
        }
        else if(equals(cmd, "clear")){
            printf("\033[2J"); //Limpia la pantalla.
        }
        else if(equals(cmd, "kill")){
            invoke("/bin/kill", items, background);
        }
        else if(equals(cmd, "time")){
            printf("time");
        }
        else if(equals(cmd, "psinfo")){
            Proc procs[NUM_PROCS];
            print_info(procs, num, items);
            // char *cwd;
            // cwd = f_pwd();
            // if(cwd != NULL){
            //     printf("%s\n", cwd);
            // }
            // else{
            //     strncpy(cwd, mypath[1], TAM*10);
            // }
            // snprintf(proc_path, TAM, "%s/%s", cwd, cmd);
            // invoke(proc_path, items, background);
            // printf("\n");
        }
        else if(equals(cmd, "grep")){
            invoke("/bin/grep", items, background);
        }
        else if(equals(cmd, "exit")){
            end = true;
        }
    }
}

void invoke(char *proc_path, char **params, int background){
    pid_t pid;
    int status;
    if ((pid = fork()) == -1) perror("fork error");
    else if (pid == 0) {
        if(background == 1){
            setpgid(pid, 0);
            //printf("\nOn background[%d]", getpid());
            execvp(proc_path, params);
            printf("Return not expected. Must be an execv error.n\n");
        }
        else{
            execv(proc_path, params);
            printf("Return not expected. Must be an execv error.n\n");
        }
    }
    wait(&status);
}

bool equals(char *x, char *y){
    if(sizeof(x)/sizeof(char) > TAM) return false;
    if(sizeof(y)/sizeof(char) > TAM) return false;

    if(strncmp(x, y,TAM) == 0) return true;
    else return false;
}

void usage(char *use){
    printf("Usage: %s\n", use);
}

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