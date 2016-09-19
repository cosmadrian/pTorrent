#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "input_handler/input_handler.h"
#include "command/command.h"
#include "error/error.h"

#define BUF_SIZE 1024
#define S_SIZE 256
#define MAX_INPUT 32
#define MAX_ARGZ 10

int sflag = 0, pflag = 0, lflag = 0;

static void
help(){
    printf("pTorrent - uTorrent clone \n\n\
Downloads torrents from the command line.\n\
Options:\n\
    -s -- start the program with all torrents stopped\n\
    -p -- start the program with all torrents paused\n\
    -h -- print this help message\n\
\n\
Author: Cosma Adrian aka NoNy\n\n");
}

static void 
usage(char* pname){
    printf("Usage: %s [-s] [-p] [-h] [-l <torrent-name>]\n\n", pname);
}


int 
main(int argc, char** argv){
    
    int c;
    char torrent_name[S_SIZE];

    while((c = getopt(argc, argv, "sphl:")) != -1){
        switch(c){
            case 's':
                sflag = 1;
                break;
            case 'p':
                pflag = 1;
                break;
            case 'l':
                lflag = 1;
                strncpy(torrent_name, optarg, S_SIZE);
                break;
            case 'h':
                help();
                return 0;
            default:
                usage(argv[0]);
                return -1;
        }
    }

    char buffer[BUF_SIZE];
    int r, max_fd;
    fd_set read_fds, tmp_fds;

    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

    FD_SET(STDIN_FILENO, &read_fds);
    max_fd = STDIN_FILENO;

    for(;;) {
        tmp_fds = read_fds;
        memset(buffer, 0, MAX_INPUT);
        if(select(max_fd + 1, &tmp_fds, NULL, NULL, NULL) >= 0){
            
            if(FD_ISSET(STDIN_FILENO, &tmp_fds)) {
                if(fgets(buffer, MAX_INPUT, stdin) == NULL){
                    printf("Error fetching input.\n");
                    continue;
                } else {
                    buffer[strlen(buffer)-1] = '\0';
                }
                char* argz[MAX_ARGZ];
                int i=0;
                for(i=0;i<MAX_ARGZ;i++) argz[i] = '\0';

                
                int arg_count = 0;
                if((r = process(buffer, argz, &arg_count)) < 0){
                    print_error(r);
                    continue;
                }

                void (*cmd)(int, char**) = NULL;
                if((r = get_command_info(argz[0], arg_count - 1, &cmd)) >= 0){
                    (*cmd)(arg_count, argz);
                } else {
                    print_error(r);
                    continue;
                }
            } 
        }
    }

    return 0;
}
