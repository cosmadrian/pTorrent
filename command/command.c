#include <stdio.h>
#include "command.h"
#include <string.h>
#include <stdlib.h>
#include "../error/error.h"
#include "../bittorrent/torrent.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern T_LIST* torrent_list;

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
    char* filename = argv[argc-1];
    
    struct stat file_status;
    if(stat(filename, &file_status) == -1){
        printf("Problem with torrent file: %s\n", filename);
        return;
    }

    FILE *fp = fopen(filename, "r");
    char* buffer = (char*) calloc(1, file_status.st_size*sizeof(char));
    fread(buffer, 1, file_status.st_size, fp);
    fclose(fp);

    be_node *node = be_decode(buffer, file_status.st_size);
    t_list_add(&torrent_list, node, filename);
    printf("Torrent %s loaded!\n", filename);
}

void 
c_pause(int argc, char** argv){
    printf("Pausing %s ...\n", argv[argc-1]);
}

void
c_remove(int argc, char** argv){
    be_node* node = t_list_get_by_label(torrent_list, argv[argc-1]);
    t_list_remove(torrent_list, node);
    be_free(node);
}

void
c_stop(int argc, char** argv){
    printf("Stopping %s ... \n", argv[argc-1]);
}

void
c_list(int argc, char** argv){
    if(strncmp(argv[argc-1],"all",3) == 0){
        t_list_print_names(torrent_list);
    } else {
        printf("?!\n");
    }
}

void
c_show(int argc, char** argv){
    be_node *node = t_list_get_by_label(torrent_list, argv[argc-1]);
    if(!node){
        printf("Torrent %s not within the list.\n",argv[argc-1]);
        return;
    }

    be_dump(node);
}

void
c_help(int argc, char** argv){
    printf("Available commands: \n");
    int i;
    for(i = 0; i < CMDS_LEN; i++){
        printf("%d. %s (%d)\n",i + 1,COMMAND_S[i].cmd_msg, COMMAND_S[i].args);
    }
}

void
c_quit(int argc, char** argv){
    printf("Bye!\n");
    exit(0);
}
