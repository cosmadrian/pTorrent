#include <stdio.h>
#include "error.h"


void
print_error(enum E_CODE err_no){
    int i;
    for (i = 0; i < N_ERRORS; i++) {
        if(ERROR_S[i].e_code == err_no){
            printf("(%d)%s\n",ERROR_S[i].e_code ,ERROR_S[i].e_msg);
            return;
        }
    }
    printf("Invalid error code!\n");
}
