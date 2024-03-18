




#include "main.h"
#include <stdio.h>



#include "log.h"
#include "pool2.h"
#include "tcp.h"


int main()
{
	
    log_test();
    pool_test();
    tcp_test("192.168.100.220", 8080);
   
    
	printf("OK,my name is acs!\n");
	return 0;
}


