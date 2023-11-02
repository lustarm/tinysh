#include <stdio.h>
#include <stdlib.h>

void quit_command() {
    exit(0);
}

void clear_command() {
    printf("\e[2J\e[1;1H");
}

