#include <stddef.h>
int tcp_server_create(const char * port_c);
void error_handling(char * message);
int read_full(int sd, void * buf, size_t nbytes);
int write_v(int sd, void * buf, size_t n, size_t size);

