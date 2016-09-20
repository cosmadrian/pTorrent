#include "torrent.h"
#include "../error/error.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


T_LIST*
t_list_init(){
    T_LIST* list = (T_LIST*)calloc(1,sizeof(T_LIST));
    if(list){
        list->id = inc_id();
        return list;
    }
    else 
        return NULL;
}

be_node*
t_list_get_by_label(T_LIST** list, char* name){
        return NULL;
}

int
t_list_add(T_LIST** list, be_node* node, char* label){
    return E_OK;
}


int
t_list_remove(T_LIST** list, be_node* node){
    return E_OK;
}

int
t_list_print_names(T_LIST** list){
    return 0;
}

int
inc_id(){
    static int id = 0;
    return id++;
}
