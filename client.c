#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define SERV_PORT 10010

int main(int argc, char** argv)
{
        int sockfd;
        struct sockaddr_in servaddr;
        sockfd = socket (AF_INET, SOCK_STREAM, 0);
        char sendline[MAXLINE], recvline[MAXLINE];
        uint32_t op_a, op_b, ans;
        char op, valid;

        if (argc != 2)
        {
          perror("Invalid Usage: ./client12 IP address\n");
          printf("ie. ./client12 127.0.0.1\n");
          exit(2);
        }

        if (sockfd == -1)
        {
                perror("Problem in creating the socket");
                exit(2);
        }

        else
        {
                printf("Socket Creation Successful\n");
        }


        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr= inet_addr(argv[1]);
        servaddr.sin_port =  htons(SERV_PORT);

        if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0)
        {
                perror("Problem in connecting to the server");
                exit(2);
        }

          printf("Please enter an operation sign('+','-','x','/'): ");
          while((scanf("%c", &op) != 0) && (op != 43 && op!= 45 && op != 120 && op != 47)) {
            printf("Please enter an operation sign within the requirements.\n");
          }

          printf("Please enter operand A(unsigned integer): \n");
          while(scanf("%u", &op_a) != 1) {
            printf("Please enter an unsigned integer! \n");
            exit(2);
          }

          printf("Please enter operand B(unsigned integer): \n");
          while(scanf("%u", &op_b) != 1) {
            printf("Please enter an unsigned integer! \n");
            exit(2);
          }

          while(getchar() != '\n');
          memcpy(sendline, &op, 1);
          memcpy(sendline + 1, &op_a, 4);
          memcpy(sendline + 5, &op_b, 4);
          send(sockfd, sendline, MAXLINE, 0);
          printf("Message sent: %u %c %u\n", op_a, op, op_b);

          if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
            perror("The server terminated prematurely");
            exit(3);
          }else {
            memcpy(&ans, recvline + 9, 4);
            valid = recvline[13];

            if (valid  == 1) {
              printf("Data received from server: %u %c %u = %u. \n", op_a, op, op_b, ans);
            }else {
              ans = 0;
              printf("Data received from server: %u %c %u = %u. \n", op_a, op, op_b, ans);
            }

          }

        return 1;
}
