#include <stdio.h>
#include "command.h"
#include <string.h>
#include "../error/error.h"

int 
get_command_info(char* cmd_name, int arg_count, void (**p)(int,char**)){
    int i;
    for(i = 0; i < CMDS_LEN; i++){
        if(strcmp(cmd_name, COMMAND_S[i].cmd_msg) == 0){
            if(COMMAND_S[i].args != arg_count)
                return E_INVALID_ARGS;
            *p = COMMAND_S[i].f_ptr;
            return E_OK;
        }
    }

    return E_UNKNOWN_CMD;
}

void
c_load(int argc, char** argv){
    printf("Loading %s ...\n",argv[argc-1]);
}

void 
c_pause(int argc, char** argv){
    printf("Pausing %s ...\n", argv[argc-1]);
}

void
c_remove(int argc, char** argv){
    printf("Removing %s ... \n", argv[argc-1]);
}

void
c_stop(int argc, char** argv){
    printf("Stopping %s ... \n", argv[argc-1]);
}

void
c_list(int argc, char** argv){
    printf("Listing ... \n");
}

void
c_show(int argc, char** argv){
    printf("Showing ... \n");
}

void
c_help(int argc, char** argv){
    printf("Helping.. \n");
}
