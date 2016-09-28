#include <stdio.h>
#include "command.h"
#include <string.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include "../error/error.h"
#include "../bittorrent/torrent.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../bittorrent/tracker/tracker.h"

extern T_LIST* torrent_list;

int 
get_command_info(char* cmd_name, int arg_count, void (**p)(int,char**)){
    size_t i;
    size_t cmd_len = sizeof(COMMAND_S) / sizeof(COMMAND_S[0]);

    for(i = 0; i < cmd_len; i++){
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
    int r;
    
    printf("Loading %s ...\n",argv[argc-1]);
    char* filename = argv[argc-1];
    
    struct stat file_status;
    if(stat(filename, &file_status) == -1){
        printf("Problem with torrent file: %s\n", filename);
        return;
    }

    FILE *fp = fopen(filename, "r");
    char* buffer = (char*) calloc(1, file_status.st_size*sizeof(char));
    if(!fread(buffer, 1, file_status.st_size, fp)){
        printf("Problems reading file.\n");
        fclose(fp);
        free(buffer);
        return;
    }
    fclose(fp);

    be_node *node = be_decode(buffer, file_status.st_size);
    if((r = t_list_add(torrent_list, node, filename)) < 0){
        be_free(node);
        print_error(r);
        return;
    }
    printf("Torrent %s loaded!\n", filename);
}

void 
c_pause(int argc, char** argv){
    printf("Pausing %s ...\n", argv[argc-1]);
}

void
c_remove(int argc, char** argv){
    T_LIST* node = t_list_get_by_label(torrent_list, argv[argc-1]);
    if(!node){
        printf("Label not found.\n");
        return;
    }
    t_list_remove(node);
    printf("Torrent removed.\n");
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
    T_LIST *node = t_list_get_by_label(torrent_list, argv[argc-1]);
    if(!node){
        printf("Torrent %s not within the list.\n",argv[argc-1]);
        return;
    }

    be_dump(node->torrent);
    tr_dump(node->tracker);
}

void
c_help(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){
    printf("Available commands: \n");
    size_t i;
    size_t cmd_len = sizeof(COMMAND_S) / sizeof(COMMAND_S[0]);
    for(i = 0; i < cmd_len; i++){
        printf("%zu. %s (%d)\n",i + 1,COMMAND_S[i].cmd_msg, COMMAND_S[i].args);
    }
}

void
c_quit(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){
    printf("Bye!\n");
    
    int i;
    for(i = 0; i < MAX_TORRENTS; i++){
        if(torrent_list[i].id > 0){
            // TODO save its stat
            t_list_remove(&torrent_list[i]);
        }
    }
    
    exit(0);
}

extern char peer_id[20];

void
c_peerid(__attribute__((unused)) int argc, __attribute__((unused)) char** argv){
    printf("Your peer id is: \n");
    size_t i;
    for(i = 0; i < SHA_DIGEST_LENGTH; i++)
        printf("0x%02x ", peer_id[i]);
    printf("\n");
}
