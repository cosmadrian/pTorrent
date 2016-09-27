#ifndef __TORRENT_H__
#define __TORRENT_H__

#include "bendecoder/bencode.h"
#include "tracker/tracker.h"

#define MAX_TORRENTS 25

//TODO keep a file with some statistics

enum T_STATUS { DOWNLOADING, PAUSED, STOPPED, SEEDING };

typedef struct t_list{
    be_node *torrent;
    char label[32];
    int id;
    enum T_STATUS t_status;
    int downloaded;
    TRACKER* tracker;
    //TODO peer list

} T_LIST;

T_LIST* t_list_init();
void t_list_clear(T_LIST*);
T_LIST* t_list_get_by_label(T_LIST*, char* );
int t_list_add(T_LIST*, be_node*, char*);
int t_list_remove(T_LIST*);
int t_list_print_names(T_LIST*);

void tr_send_req(T_LIST*);
void tr_recv_res(T_LIST*);

int inc_id();

#endif
