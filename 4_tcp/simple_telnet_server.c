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
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN saddr, caddr;
    int clen = sizeof(caddr);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = 0;

    int error = bind(sfd, (SOCKADDR *) &saddr, sizeof saddr);
    if (error == -1)
        abort();

    listen(sfd, 10);

    int cfd = accept(sfd, (SOCKADDR *) &caddr, &clen);
    if (cfd != FAIL) {
        char *welcome = "Welcome to TCP server\nType a command\n";
        send(cfd, welcome, strlen(welcome), 0);

        while (1) {
            char buffer[1024] = {0};
            memset(buffer, 0, sizeof(buffer));
            recv(cfd, buffer, sizeof buffer, 0);
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = 0;
            printf("Da nhan: %s\n", buffer);
            if (strncmp(buffer, "exit", 4) == 0)
                break;
            sprintf(buffer + strlen(buffer), " > out.txt");
            system(buffer);

            FILE *taptin = NULL;
            taptin = fopen("out.txt", "r");
            if (taptin != NULL) {
                char data[500] = "";
                char newLine[500] = "";
                while (fgets(newLine, sizeof(newLine), taptin)!=NULL) {
                    strcat(data,newLine);
                }
                send(cfd,data, sizeof(data), 0);
            }
            fclose(taptin);
        }
        close(cfd);
    }
    close(sfd);
}
