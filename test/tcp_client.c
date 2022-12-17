#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

typedef struct sockaddr_in SOCKADDR_IN;

int main() {
    // open socket
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN saddr;

    // set up saddr
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // connect
    if (connect(sfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) {
        close(sfd);
        printf("Khong the ket noi may chu.\n");
        exit(-1);
    }

    // Nhan tin nhan dau tien (neu server gui hello)
    char buffer[1024] = {0};
    memset(buffer, 0, sizeof(buffer));
    recv(sfd, buffer, 1024, 0);
    if(buffer[strlen(buffer)-1] =='\n')
    {
        buffer[strlen(buffer)-1]=0;
    }

    // do something theo debai
    char command[20] = {0};
    strcpy(command,"pwd");
    char buffer_pwd[1024]= {0};
    memset(buffer_pwd, 0, sizeof(buffer_pwd));
    while (1) {

        // pwd -  do something theo debai
        memset(buffer_pwd, 0, sizeof(buffer_pwd));
        // send 
        send(sfd, command, sizeof(command), 0);
        // recv
        recv(sfd, buffer_pwd, sizeof(buffer_pwd), 0);
        // trim data
        if(buffer_pwd[strlen(buffer_pwd)-1] =='\n')
        {
            buffer_pwd[strlen(buffer_pwd)-1]=0;
        }
        printf("#%s#", buffer_pwd);

        // trao doi du lieu
        fgets(buffer, 1024, stdin);
        send(sfd, buffer, sizeof(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(sfd, buffer, 1024, 0);
        if(buffer[strlen(buffer)-1] =='\n')
            {
                buffer[strlen(buffer)-1]=0;
            }
        printf("%s\n", buffer);
    }
    close(sfd);
}
