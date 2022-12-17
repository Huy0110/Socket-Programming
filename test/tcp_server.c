#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FAIL -1
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int main() {
    // open socket
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN saddr, caddr;
    int clen = sizeof(caddr);

    // set up saddr
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = 0;

    // bind
    int error = bind(sfd, (SOCKADDR *) &saddr, sizeof saddr);
    if (error == -1)
        abort();

    // listen
    listen(sfd, 10);

    // accept
    int cfd = accept(sfd, (SOCKADDR *) &caddr, &clen);
    if (cfd != FAIL) {

        // send fist massage
        char *welcome = "Welcome to TCP server\nType a command\n";
        send(cfd, welcome, strlen(welcome), 0);

        while (1) {
            // do something
            char buffer[1024] = {0};
            memset(buffer, 0, sizeof(buffer));

            // receive data
            recv(cfd, buffer, sizeof buffer, 0);

            // trim() data
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = 0;
            
            // xac nhan data
            printf("Da nhan: %s\n", buffer);

            // dieu kien thoat
            if (strncmp(buffer, "exit", 4) == 0)
                break;
            
            // do something for debai
            sprintf(buffer + strlen(buffer), " > out.txt");
            system(buffer);

            // doc file
            FILE *taptin = NULL;
            taptin = fopen("out.txt", "r");
            if (taptin != NULL) {
                char data[500] = "";
                char newLine[500] = "";
                while (fgets(newLine, sizeof(newLine), taptin)!=NULL) {
                    strcat(data,newLine);
                }

                // send data
                send(cfd,data, sizeof(data), 0);
            }
            fclose(taptin);
        }
        close(cfd);
    }
    close(sfd);
}
