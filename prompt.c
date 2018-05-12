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
#include "bashnroll.h"

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

int main(void){
    char ** items;
    bool end = false;
    int num, background;
    char expresion[TAM];
    char *cmd;

    printf("       Bash&Roll🤘\n      Julián Muñoz\n");
    printf("Universidad de Antioquia\n Facultad de ingenieria\n");
    printf("          2018\n\n");

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