#ifndef __TRACKER_H__
#define __TRACKER_H__

struct _tracker{
    char domain[128];
    char ip[16];
    int port;
    int fd;
    char params[2048];

};
typedef struct _tracker TRACKER;

int tr_clear(TRACKER*);
TRACKER* tr_init(char* );
void tr_dump(TRACKER*);


#endif
