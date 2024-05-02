/*
 Author: Kumar Shubham

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/*
   the IP address "192.168.2.1", you would get:

    Part 1: 192 (11000000 in binary)
    Part 2: 168 (10101000 in binary)
    Part 3: 2 (00000010 in binary)
    Part 4: 1 (00000001 in binary)

     Combining these binary numbers gives you 11000000101010000000001000000001 in binary, which is 3232236033 in decimal
*/



int main(int argc, char *argv[]) {
    char ip_addr[] = "192.168.2.1";
    int mask = 24;
    struct in_addr  network_addr;
    uint32_t ip_val;
    memset(&network_addr, 0, sizeof(network_addr));

    inet_pton(AF_INET, ip_addr, &ip_val);
    printf("ip %s in numerical form: %u\n", ip_addr, ip_val);

    return 0;
}
