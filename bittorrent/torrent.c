#include "torrent.h"
#include "../error/error.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tracker/tracker.h"
#include <openssl/sha.h>
#include <curl/curl.h>

extern void add_fd(int);

static void
t_get_tracker_info(T_LIST*);

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

static void
t_node_free(T_LIST* list){
    be_free(list->torrent);
    tr_clear(list->tracker);
    memset(list, 0, sizeof(T_LIST));
}

void
t_list_clear(T_LIST* list){
    if(list){
        free(list);
    }
}

T_LIST*
t_list_get_by_label(T_LIST* list, char* name){
    int i;
    for(i = 0; i < MAX_TORRENTS; i++){
        if(list[i].id > 0)
            if(strncmp(list[i].label, name, strlen(list[i].label)) == 0){
                return &list[i];
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
            t_get_tracker_info(&list[i]);
            list[i].id = inc_id();
            
            add_fd(list[i].tracker->fd);
             
            return E_OK;
        }
    }
    return E_MAX_TORRENTS;
}


int
t_list_remove(T_LIST* node){
    t_node_free(node);
    
    return E_OK;
}

int
t_list_print_names(T_LIST* list){
    int i, j = 1;
    for(i = 0; i < MAX_TORRENTS; i++){
        if(list[i].id > 0){
            int downloaded = 0;
            int size = 0;
            printf("%d. %s (%d / %d)\n", j ++, list[i].label, downloaded, size);
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


static void
t_get_tracker_info(T_LIST *node){
    TRACKER* tr = NULL;
    int i;
    be_node* meta = node->torrent;
    for(i=0; meta->val.d[i].val; i++){
        if(strcmp(meta->val.d[i].key,"announce") == 0){
            tr = tr_init(meta->val.d[i].val->val.s);
            break;
        }
    }
    node->tracker = tr;
}

