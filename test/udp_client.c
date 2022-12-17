/*UDP Echo Client*/
#include <stdio.h> /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int main()
{
    // open socket
    int sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKADDR_IN saddr;

    // set up saddr
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(5000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int slen = sizeof(saddr);

    {
        while (1)
        {
            // communicate with server
            printf("Insert string to send:");
            char buffer[1024]={0};
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, 1024, stdin);
            if(buffer[strlen(buffer)-1] == '\n')
            {
                buffer[strlen(buffer)-1]=0;
            }

            printf("Da dinh gui: %s\n", buffer);
            // sendto
            int bytes_sent = sendto(sfd, buffer, sizeof(buffer), 0, (SOCKADDR *)&saddr, sizeof(saddr));
            if (bytes_sent < 0)
            {
                perror("Error: ");
                return 0;
            }
            printf("Da gui xong\n");
            memset(buffer, 0, sizeof(buffer));
            // recvfrom
            int r = recvfrom(sfd, buffer, sizeof(buffer), 0, (SOCKADDR *) &saddr, &slen);
            if(r>0)
            {
                printf("Da nhan: %s\n", buffer);
            }
            else{
                printf("Chua nhan duoc\n");
            }
            if(strcmp(buffer, "Thoat khoi server")==0)
            {
                close(sfd);
                return 0;
            }
        }
    }
            
    close(sfd);
    return 0;
}