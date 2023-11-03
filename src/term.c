#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "commands.h"

typedef void (*CommandFunction)(void);

typedef struct {
    const char *name;
    CommandFunction func;
} Command;

Command commands[10];

// UTIL
// ***************************

void to_lower(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

char **split(char *string, char *seperators, int *count)
{
    // get the length of the string
    int len = strlen(string);

    // use count to keep a count of the number of substrings
    *count = 0;

    // We make one pass of the string to first determine how many substrings 
    // we'll need to create, so we can allocate space for a large enough array 
    // of pointer to strings.  The variable i will keep track of our current 
    // index in the string
    int i = 0;
    while (i < len)
    {
        // skip over the next group of separator characters
        while (i < len)
        {
            // keep incrementing i until the character at index i is NOT found in the 
            // separators array, indicating we've reached the next substring to create 
            if (strchr(seperators, string[i]) == NULL)
                break;
            i++;
        }

        // skip over the next group of substring (i.e. non-separator characters), 
        // we'll use old_i to verify that we actually did detect non-separator 
        // characters (perhaps we're at the end of the string)
        int old_i = i;
        while (i < len)
        {
            // increment i until the character at index i IS found in the separators 
            // array, indicating we've reached the next group of separator 
            // character(s)
            if (strchr(seperators, string[i]) != NULL)
                break;
            i++;
        }

        // if we did encounter non-seperator characters, increase the count of 
        // substrings that will need to be created  
        if (i > old_i) *count = *count + 1;
    }

    // allocate space for a dynamically allocated array of *count* number of 
    // pointers to strings
    char **strings = malloc(sizeof(char *) * *count);

    // we'll make another pass of the string using more or less the same logic as 
    // above, but this time we'll dynamically allocate space for each substring 
    // and store the substring into this space
    i = 0;

    // buffer will temporarily store each substring, string_index will keep track 
    // of the current index we are storing the next substring into using the 
    // dynamically allocated array above
    char buffer[16384];
    int string_index = 0;
    while (i < len)
    {
        // skip through the next group of separators, exactly the same as above
        while (i < len)
        {
            if (strchr(seperators, string[i]) == NULL)
                break;
            i++;
        }

        // store the next substring into the buffer char array, use j to keep 
        // track of the index in the buffer array to store the next char
        int j = 0;
        while (i < len)
        {
            if (strchr(seperators, string[i]) != NULL)
                break;

            buffer[j] = string[i];
            i++;
            j++;
        }

        // only copy the substring into the array of substrings if we actually 
        // read in characters with the above loop... it's possible we won't if 
        // the string ends with a group of separator characters!
        if (j > 0)
        {
            // add a null terminator on to the end of buffer to terminate the string
            buffer[j] = '\0';

            // calculate how much space to allocate... we need to be able to store 
            // the length of buffer (including a null terminator) number of characters 
            int to_allocate = sizeof(char) *
                (strlen(buffer) + 1);

            // allocate enough space using malloc, store the pointer into the strings 
            // array of pointers at hte current string_index
            strings[string_index] = malloc(to_allocate);

            // copy the buffer into this dynamically allocated space 
            strcpy(strings[string_index], buffer);

            // advance string_index so we store the next string at the next index in 
            // the strings array
            string_index++;
        }
    }

    // return our array of strings  
    return strings;
}

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

// ***************************

void add_command(const char *name, CommandFunction func) {
    for (long unsigned int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (commands[i].name == NULL) {
            commands[i].name = name;
            commands[i].func = func;
            break;
        }
    }
}

int handle_command(char *buf) {
    // add later
    //int count_strings = 0;
    //char **split_string = split(buf, " ", &count_strings);
    to_lower(buf);
    for (long unsigned int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
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
    add_command("help", help_command);
    add_command("ls", list_command);

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
