#ifndef __TRACKER_H__
#define __TRACKER_H__

#define MAX_PARAMS 16


typedef struct {
    char domain[128];
    int num_params;
    char* params[MAX_PARAMS];
    
    char ip[16];
    int port;
    int fd;

} TRACKER;

int tr_clear(TRACKER*);
TRACKER* tr_init(char* ); // get file_descriptor, ip / domain name
void tr_dump(TRACKER*);


#endif
