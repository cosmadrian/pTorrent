#include <stdio.h>
#include "error.h"


void
print_error(enum E_CODE err_no){
    size_t i;
    size_t err_len = sizeof(ERROR_S) / sizeof(ERROR_S[0]);
    for (i = 0; i < err_len; i++) {
        if(ERROR_S[i].e_code == err_no){
            printf("(%d)%s\n",ERROR_S[i].e_code ,ERROR_S[i].e_msg);
            return;
        }
    }
    printf("Invalid error code!\n");
}
