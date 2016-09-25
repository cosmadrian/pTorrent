#include "torrent.h"
#include "../error/error.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


T_LIST*
t_list_init(){
    T_LIST* list = (T_LIST*)calloc(1, MAX_TORRENTS * sizeof(T_LIST));
    memset(list, 0, MAX_TORRENTS * sizeof(T_LIST));
    if(list){
        return list;
    }
    else 
        return NULL;
}

void
t_list_clear(T_LIST* list){
    if(list){
        free(list);
    }
}

be_node*
t_list_get_by_label(T_LIST* list, char* name){
    int i;
    for(i = 0; i < MAX_TORRENTS; i++){
        if(list[i].id > 0)
            if(strncmp(list[i].label, name, strlen(list[i].label)) == 0){
                return list[i].torrent;
            }
    }
    return NULL;
}

int
t_list_add(T_LIST* list, be_node* node, char* label){
    int i;
    for(i = 0; i < MAX_TORRENTS; i++){
        if(list[i].id == 0){
            list[i].torrent = node;
            strncpy(list[i].label,label,strlen(label));
            list[i].id = inc_id();
            return E_OK;
        }
    }
    return E_MAX_TORRENTS;
}


int
t_list_remove(T_LIST* list, be_node* node){
    int i;
    for(i=0;i<MAX_TORRENTS;i++){
        if(list[i].id > 0 && list[i].torrent == node){
            be_free(list[i].torrent);
            memset(&list[i], 0, sizeof(T_LIST));
            return E_OK;
        }
    }

    return E_TORRENT_NOT_FOUND;
}

// TODO
int
t_list_print_names(T_LIST* list){
    int i;
    for(i = 0; i < MAX_TORRENTS; i++){
        if(list[i].id > 0){
            int downloaded = 0;
            int size = 0;
            printf("%d. %s (%d / %d)\n", i + 1, list[i].label, downloaded, size);
        }
    }
    fflush(stdout);
    return E_OK;
}

int
inc_id(){
    static int id = 0;
    return ++id;
}
