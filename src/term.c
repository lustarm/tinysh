#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "commands.h"

typedef void (*CommandFunction)(void);

typedef struct {
    const char *name;
    CommandFunction func;
} Command;

Command commands[10];

void removeCRLF(char *input) {
    int len = strlen(input);
    int j = 0;

    for (int i = 0; i < len; i++) {
        if (input[i] != '\r' && input[i] != '\n') {
            input[j] = input[i];
            j++;
        }
    }

    input[j] = '\0';
}

void add_command(const char *name, CommandFunction func) {
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (commands[i].name == NULL) {
            commands[i].name = name;
            commands[i].func = func;
            break;
        }
    }
}

int handle_command(char *buf) {
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (commands[i].name != NULL && strncmp(buf, commands[i].name, strlen(commands[i].name)) == 0) {
            commands[i].func();
            return 0;
        }
    }

    printf("ERROR: Unknown Command\n");
    return 1;
}

int start_term(){
    // add commands
    add_command("quit", quit_command);
    add_command("clear", clear_command);

    // main logic loop
    while(1){
        printf("tinyshell> ");
        char buf[1024];
        fgets(buf, 1024, stdin);
        removeCRLF(buf);
        handle_command(buf);
    }
    return 0;
}
