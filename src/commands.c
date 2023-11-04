#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void quit_command() {
    exit(0);
}

void clear_command() {
    printf("\e[2J\e[1;1H");
}

void help_command(){
    printf("Help : Shows this screen\n");
    printf("Clear: Clears the screen\n");
    printf("Quit : Quits the program\n");
    printf("Ls   : Lists current directory\n");
}

#include <sys/stat.h>

int is_folder(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return -1;
    }
    return S_ISDIR(st.st_mode);
}

void list_command(){
    DIR *dir = opendir(".");
    if(dir == NULL){
        printf("ERROR: Failed to find directory\n");
        goto clean;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        // skip the . and ..
        if(strncmp(entry->d_name, ".", strlen(".")) == 0 || strncmp(entry->d_name, "..", strlen("..")) == 0){
            continue;
        }
        // print the file name will change later
        if(is_folder(entry->d_name)){
            printf("%s/\n", entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
    }
clean:
    closedir(dir);
}
