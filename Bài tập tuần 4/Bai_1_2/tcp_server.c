#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    //socket()
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // khoi tao dia chi server
    int portNum = atoi(argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNum);

    // bind() 
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        printf("error");
        return 1;
    }
    // listen()
    if(listen(listener, 5) == -1) {
        printf("error");
        return 1;
    }

    printf("Waiting for clients...\n");

    // accept()
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if(client == -1) {
        printf("accept() failed\n");
        return 1;
    }
    printf("Client IP: %s:%d\n",inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // send()
    char buf[256];
    char *helloFile = argv[2];
    FILE *fp1 = fopen(helloFile, "r");
    char *msg = fgets(buf, sizeof(buf), fp1);
    send(client, msg, sizeof(msg), 0);
    fclose(fp1);

    FILE *fp2 = fopen(argv[3], "w");
    
    // receive()
    while(1) {  
    int k = recv(client, buf, sizeof(buf), 0);
    if(k < sizeof(buf))
        buf[k] = '\0';
    
    fprintf(fp2, "%s", buf);
    if(strncmp(buf, "exit", 4) == 0) 
        break;
    }
    fclose(fp2);
    close(listener);
}