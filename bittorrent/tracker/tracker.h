#ifndef __TRACKER_H__
#define __TRACKER_H__

typedef struct {
    char ip[4];

} TRACKER;

int tr_send_req(TRACKER*, char*, int); // char* parameters
int tr_recv_res(TRACKER*, char*, int);

#endif
