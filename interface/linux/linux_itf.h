



#ifndef __LINUX_ITF_H 
#define __LINUX_ITF_H

int LINUX_itf_connect(int fd, char *ipv4, int port);
int LINUX_itf_bind(char *ipv4, int port, int *out);
int LINUX_itf_listen(int fd);
int LINUX_itf_accept(int fd, char *ipv4, int size, int *port);



#endif



