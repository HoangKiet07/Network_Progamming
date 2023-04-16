#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //socket()
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // khoi tao dia chi server
    int portNum = atoi(argv[2]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(portNum);

    // connect()
    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1) {
        printf("connect() failed\n");
        return 1;
    }
    printf("connect() success\n");

    // receive()
    char buf[256];
    int k = recv(client, buf, sizeof(buf), 0);
    if(k < sizeof(buf)) 
        buf[k] = '\0';
    printf("%s\n", buf);
    // send()
    while(1) {
        printf("Nhap ban tin: ");
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, sizeof(buf), 0);
        if(strncmp(buf, "exit", 4) == 0) return 1; 
    }
    close(client);
}