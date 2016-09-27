#include <stdio.h>
#include <ctype.h>
#include "../error/error.h"
#include "../command/command.h"
#include <string.h>
#include <limits.h>
#include <stdlib.h>

extern int trie_init_f;
static void isolate_args(char*, char**, int*);
static void edit_distance(char**);
static int edit_distance_h(const char*,const char*, int , int);

int
process(char* buffer, char** argz, int* argc){
    isolate_args(buffer, argz, argc);

    if(argz[0] != NULL)
        edit_distance(&argz[0]);
    else
        return E_UNKNOWN_CMD;

    if(argz[0] == NULL){
        return E_UNKNOWN_CMD;
    }
    return E_OK;
}

static void
isolate_args(char* buffer, char** argv, int* argc){
    int c;
    char *p, *start_of_word;
    enum STATES { DULL, IN_STRING, IN_WORD} state = DULL;

    *argc = 0;
    for(p = buffer; *argc < 9 && *p != '\0'; p++){
        c = (unsigned char) *p;
        switch(state){
            case DULL:
                if(isspace(c))
                    continue;
                
                if(c == '"'){
                    state = IN_STRING;
                    start_of_word = p + 1;
                    continue;
                }
                
                state = IN_WORD;
                start_of_word = p;
                continue;
            case IN_STRING:
                if(c == '"'){
                    *p = '\0';
                    argv[*argc] = start_of_word;
                    *argc += 1;
                    state = DULL;
                }
                continue;
            case IN_WORD:
                if(isspace(c)){
                    *p = '\0';
                    argv[*argc] = start_of_word;
                    *argc += 1;
                    state = DULL;
                }
                continue;
        }
    }
    if(state != DULL && *argc < 9){
        argv[*argc] = start_of_word;
        *argc += 1;
    }
}

static void
edit_distance(char** buffer){
    int min = INT_MAX, i;
    const char* p = NULL;
    for(i = 0; i < CMDS_LEN; i++){
        int dist = edit_distance_h(*buffer, COMMAND_S[i].cmd_msg, strlen(*buffer), strlen(COMMAND_S[i].cmd_msg));
        if(dist < min){
            min = dist;
            p = (char*) COMMAND_S[i].cmd_msg;
        }
    }
    if(min > 2){
        *buffer = NULL;
        return;
    }

    if(min > 0){
        printf("Command \"%s\" interpreted as \"%s\".\n",*buffer, p);
    }
    *buffer = p;
}

#define MIN(a,b) (((a)<(b))?(a):(b))

static int
edit_distance_h(const char* str1, const char* str2, int m, int n){
    if(m == 0) return n;
    if(n == 0) return m;

    if(str1[m-1] == str2[n-1]) return edit_distance_h(str1, str2, m - 1, n - 1);

    return 1 + MIN(edit_distance_h(str1,str2,m,n-1), MIN(edit_distance_h(str1,str2,m-1,n), edit_distance_h(str1,str2,m-1,n-1)));
}
