#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

// OPTION LINK LIST
typedef struct account {
    char addr[100];
    struct account *next;
} account;
account *list = NULL;

account *searchAndAdd(char addr[]) {
    if (list == NULL) {
        list = malloc(sizeof(account));
        sprintf(list->addr, "%s", addr);
        list->next = NULL;
    } else {
        account *tmp = list;
        int isHas = 0;
        while (tmp != NULL) {
            if (strcmp(tmp->addr, addr) == 0)
                isHas = 1;
            tmp = tmp->next;
        }

        if (!isHas) {
            account *new_acc = malloc(sizeof(account));
            sprintf(new_acc->addr, "%s", addr);
            new_acc->next = NULL;
            list->next = new_acc;
        }
    }
    return list;
}

int main() {
    // open socket
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    SOCKADDR_IN saddr, caddr;
    int clen = sizeof(caddr);
    if(s < 0)
    {
        printf("Could not open socket\n");
    }
    else
    {
        // set up saddr
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(5000);
        saddr.sin_addr.s_addr = 0;
        int slen = sizeof(saddr);

        // bind
        int error = bind(s, (SOCKADDR *) &saddr, sizeof(saddr));
        if(error == -1)
        {
            printf("Can not bind\n");
        }
        else
        {
            while (1) {
                // do something
                char buffer[1024] = {0};
                // recvfrom
                int r = recvfrom(s, buffer, sizeof(buffer), 0, (SOCKADDR *) &caddr, &clen);
                printf("Da nhan: %s\n", buffer);
                // so sanh va lam theo de bai
                if(r < 0)
                {
                    perror("\nError: ");
                    exit(0);
                }
                if(strcmp(buffer, "X") == 0)
                {
                    char massage[]= "Thoat khoi server";
                    sendto(s, massage, sizeof(massage), 0, (SOCKADDR *)&caddr, clen);
                }
                else
                {
                    sendto(s,buffer,strlen(buffer),0,  (SOCKADDR *) &caddr, clen);
                    printf("Truyen nguoc lai: %s\n", buffer);
                }
            }
            close(s);
        }
    }
}