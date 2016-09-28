#include <stdio.h>
#include <string.h>
#include "tracker.h"
#include "../torrent.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "../../error/error.h"
#include "../bendecoder/bencode.h"
#include <stdlib.h>

void
tr_dump(TRACKER* tr){
    printf("== DUMP TRACKER ==\n\n");
    printf("Domain (announce): %s\n",tr->domain);
    printf("IP: %s\n",tr->ip);
    printf("Port: %d\n",tr->port);
    printf("File Descriptor: %d\n",tr->fd);
    printf("Params: %s\n", tr->params);
}

int
tr_clear(TRACKER* tr){
    int i;
    close(tr->fd);
    free(tr);
    return E_OK;
}

static 
int get_hostip(char * hostname , char* ip) {
    struct hostent *he;     
    struct in_addr **addr_list;     
    if ((he = gethostbyname(hostname)) == NULL) { 
        herror("gethostbyname");         
        return 1;
    }     
    addr_list = (struct in_addr **) he->h_addr_list;
    if(*addr_list != NULL){
        strcpy(ip , inet_ntoa(**addr_list));
        return 0;
    }
    return 1;
}

static int 
connect_tracker(char* ip, int port){
    struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("socket");
        return sockfd;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_aton(ip, &serv_addr.sin_addr);
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return sockfd;
    }
    printf("Connected to tracker at %s:%d.\n",ip,port);

    return sockfd;
}

TRACKER*
tr_init(char* announce_url){
    unsigned int i, k = 0;
    TRACKER* tr = (TRACKER*) calloc(1, sizeof(TRACKER));
    strcpy(tr->domain, announce_url);
    memset(tr->params, 0, sizeof(tr->params));

    char proto[8];
    char host[64];
    memset(proto, 0, 8);
    memset(host, 0, 64);
    for(i = 0; i < strlen(announce_url) && i < 7; i++){
        if(announce_url[i] != '/' && announce_url[i+1] != '/'){
            proto[k++] = announce_url[i];
        }else break;
    }
    i += 3;
    k = 0;
    //TODO refactor
    for( ; i < strlen(announce_url); i++){
        if(announce_url[i] != ':' && announce_url[i] != '/'){
            host[k++] = announce_url[i];
        }else if(announce_url[i] == ':'){
            tr->port = 0;
            i++;
            for(;i<strlen(announce_url);i++){
                if(announce_url[i] != '/'){
                    tr->port = tr->port*10 + announce_url[i] - '0';
                } else break;
            }
            break;
        }else if(announce_url[i] == '/'){
            tr->port = 80;
            break;
        }
    }
    k = 0;
    int flag = 0;
    for(;i < strlen(announce_url); i++){
        if(announce_url[i] == '?')
            flag = 1;
        if(flag)
            tr->params[k++] = announce_url[i];
    }
    
    get_hostip(host, tr->ip);
    tr->fd = connect_tracker(tr->ip, tr->port);
    return tr;
}

static int
get_info_hash(be_node* meta, char* hash){
    int i;
    for(i=0; meta->val.d[i].val; i++){
        if(strcmp(meta->val.d[i].key, "info") == 0){
            int offset = 0;
            be_encode(meta->val.d[i].val, hash, &offset);
            return offset;
        }
    }
}

// TODO SHA1 the info_hash and curl_easy_escape it

void 
tr_send_res(struct t_list* tor){

}

void
tr_recv_res(struct t_list* tor){
    
}


