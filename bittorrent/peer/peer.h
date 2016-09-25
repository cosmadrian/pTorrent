#ifndef __PEER_H__
#define __PEER_H__

typedef struct {
    int id;
    char ip[4];
    int port;
} PEER;

int p_send_msg();
int p_receive_msg();


#endif
