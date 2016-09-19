#ifndef __COMMAND_H__
#define __COMMAND_H__

#define CMDS_LEN 7


int get_command_info(char*, int, void (**)(int, char**));
void c_load(int,char**);
void c_remove(int, char**);
void c_pause(int, char**);
void c_stop(int, char**);
void c_list(int, char**);
void c_show(int, char**);
void c_help(int, char**);

enum COMMAND_E {
    C_LOAD = 1, C_REMOVE = 2, C_PAUSE = 3, C_STOP = 4, C_LIST = 5, C_SHOW = 6, C_HELP = 7
};

static const struct {
    enum COMMAND_E cmd_no;
    const char* cmd_msg;
    int args;
    void (*f_ptr)(int, char**);

} COMMAND_S [] = {
    { C_LOAD, "load" , 1, &c_load},
    { C_REMOVE, "remove", 1, &c_remove },
    { C_PAUSE, "pause", 1, &c_pause},
    { C_STOP, "stop", 1, &c_stop },
    { C_LIST, "list", 1, &c_list },
    { C_SHOW, "show", 1, &c_show},
    { C_HELP, "help" , 1, &c_help}
};
#endif
