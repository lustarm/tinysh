#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// will probably find a way to use this later
int get_build_dir(){
    DIR *dir = opendir("./build/");
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
        printf("%s\n", entry->d_name);
    }
clean:
    closedir(dir);
    return 0;
}

