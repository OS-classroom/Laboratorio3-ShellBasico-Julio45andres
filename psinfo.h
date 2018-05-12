typedef struct Procs {
    char *pid;
    char **psinfo;
} Proc;

void app_usage(char *filename);
int searchInFile(char *filename, char *s);
char ** get_status(char *pid);
char * trim(int left_offset, char *line);
void print_info(Proc *procs, int argc, char *argv[]);

