#ifndef __ERROR_H__
#define __ERROR_H__

#define N_ERRORS 4

enum E_CODE {
    E_OK = 0,
    E_UNKNOWN_CMD = -1,
    E_INVALID_ARGS = -2,
    E_TORRENT_NOT_FOUND = -3,
};

static const struct {
    enum E_CODE e_code;
    const char *e_msg;

} ERROR_S [] = {
    { E_OK, "All good." },
    { E_UNKNOWN_CMD, "Unknown command." },
    { E_INVALID_ARGS, "Invalid command arguments." },
    { E_TORRENT_NOT_FOUND, "Requested torrent not found."}
};

void
print_error(enum E_CODE);


#endif
