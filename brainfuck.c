#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"

#define MAXCELLS 30000
int strnchr(char str[], char chr, int n){
    char *adr;
    int cur_adr = 0;
    if (strchr(str, chr) == NULL)
        return NULL;
    for(int c = 0;c < n; c++){
        adr = strchr(str, chr);
        cur_adr = cur_adr + (adr - str + 1);
        str = str + (adr - str) + 1;
    }
    return cur_adr - 1;
}

struct Br_counter {
    int open_br;
    int close_br;
};

int br_is_balanced(struct Br_counter br){
    return br.open_br == br.close_br;
};

int main( int argc, char *argv[] ){

    FILE *infile;
    char *buffer;
    long numbytes;

	//open file
    infile = fopen(argv[1], "r");

	//if file not exists 
    if(infile == NULL){
        perror("File error");
        puts("Usage: brainfuck [filename]");
        exit(EXIT_FAILURE);
    }
	
	//get number of bytes
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
	
    // set to beginning of the file 
    fseek(infile, 0L, SEEK_SET);

	//alloc memory for file text 
    buffer = (char*)calloc(numbytes, sizeof(char));

	//alloc error
    if(buffer == NULL)
        return 1;

	//read file contents
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);
    numbytes = (int)numbytes;

	//init memory
    int carriage = 0;
    char mem[MAXCELLS];
    memset(mem, 0, sizeof(mem));
    int loops = 0;

	// count number of loops
    struct Br_counter b_count;
    for(int i = 0; i < (int)numbytes; i++){
        if(buffer[i] == '[')
            b_count.open_br++;
        if(buffer[i] == ']')
            b_count.close_br++;
    }

	//if braces not balanced
    if(!br_is_balanced(b_count)){
        errno = "Braces not balanced";
        fprintf(stderr, "Syntax error: %s", errno);
        exit(EXIT_FAILURE);
    }

	//make loop stack
    Stack* loop_stack = make_new_stack(b_count.open_br);

    struct Br_counter b_local;

	//main loop
    for(int pointer = 0; pointer < (int)numbytes; pointer++){
        switch(buffer[pointer]){
        case '+':
            mem[carriage]++;
            break;
        case '-':
            mem[carriage]--;
            break;
        case '>':
            carriage++;
            break;
        case '<':
            carriage--;
            break;
        case '.':
            putchar(mem[carriage]);
            break;
        case ',':
            mem[carriage] = getch();
            break;
        case '[':
            if(mem[carriage]==0){
                b_local.close_br = 0;
                b_local.open_br = 0;
                int checking = 1;
                while(checking){
                    ++pointer;
                    if (buffer[pointer] == '[')
                        b_local.open_br++;
                    if (buffer[pointer] == ']'){
                        b_local.close_br++;
                        if(!br_is_balanced(b_local))
                            --checking;
                    }
                }
            }
            if(mem[carriage]!=0){
                stack_push(loop_stack, pointer);
            }
            break;
        case ']':
            if(mem[carriage]!=0){

                pointer = stack_pop(loop_stack);
				
                stack_push(loop_stack, pointer);
            }
            if(mem[carriage]==0){
                stack_pop(loop_stack);
            }
            break;
        }

    }
    puts("\nProgram finished, press any key to exit");
    getch();
    return 0;
}
