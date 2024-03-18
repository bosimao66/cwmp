



#include "main.h"
#include <stdio.h>
#include <stdlib.h>



#include "log.h"
#include "pool2.h"
#include "tcp.h"


int main(int argn, char *argv[])
{
    int port;
    log_init(NULL, "debug/log2.txt");
    pool_init();

    if(argn >= 2)port = atoi(argv[1]);
	tcp_client_test("192.168.100.220", port, "192.168.100.220", 8080);
	
	printf("OK! my name is cpe\n");
	return 0;
}


