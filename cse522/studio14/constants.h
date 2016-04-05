#define SUCCESS 0
#define INSUFFICIENT_ARGS 1
#define SOCK_CREATE_FAIL 2
#define BIND_FAIL 3
#define LISTEN_FAIL 4
#define ACCEPT_FAIL 5
#define UNLINK_FAIL 6

#define MIN_ARG_COUNT 2
#define SOCK_FILE_ARG_IDX 1

#define BACKLOG_SIZE 4
#define UNIX_PATH_MAX 108
#define MAX_MSG_LEN 128

typedef int SOCK;
