// client UDP 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>

#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1" // localhost 
#define PORT_NO 9002
#define NET_BUF_SIZE 64
#define cipherKey 'X'
#define sendrecvflag 0

FILE *fp;

// funtion to clear buffer 
void clearBuf(char *b) {
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

// function for decryption 
char Decrypt(char ch) {
    return ch ^ cipherKey;
}

// function to receive file 
int recvFile(char *buf, int s) {
    int i;
    char ch;
    char decrypted[s];
    for (i = 0; i < s; i++) {
        ch = buf[i];
        ch = Decrypt(ch);
        decrypted[i] = ch;
        if (ch == EOF) {
            printf("\n$$$\n");
            return 1;
        } else {
            printf("%c", ch);
        }
    }
    if (fwrite(decrypted, sizeof(char), s, fp) != s) {
        perror("Write File Error");
        exit(1);
    }
    return 0;
}

// driver code 
int main() {
    int sockfd, nBytes;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    char net_buf[NET_BUF_SIZE];


    // socket()
    sockfd = socket(AF_INET, SOCK_DGRAM,
                    IP_PROTOCOL);

    if (sockfd < 0)
        printf("\nfile descriptor not received!!\n");
    else
        printf("\nfile descriptor %d received\n", sockfd);
    while (1) {
        printf("\nPlease enter file name to receive:\n");
        scanf("%s", net_buf);
        sendto(sockfd, net_buf, NET_BUF_SIZE,
               sendrecvflag, (struct sockaddr *) &addr_con,
               addrlen);

        printf("\n---------Data Received---------\n");
        fp = fopen(basename(net_buf), "wb");
        while (1) {
            // receive
            clearBuf(net_buf);
            nBytes = recvfrom(sockfd, net_buf, NET_BUF_SIZE,
                              sendrecvflag, (struct sockaddr *) &addr_con,
                              &addrlen);
            // process
            if (recvFile(net_buf, NET_BUF_SIZE)) {
                printf("\n####\n");
                break;
            }
        }
        printf("\n-------------------------------\n");
        fclose(fp);
    }
    return 0;
} 
