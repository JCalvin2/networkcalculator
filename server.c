#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <math.h>

#define MAXLINE 1024
#define SERV_PORT 10010

int main()
{
        int listenfd, connfd, n;
        socklen_t clilen;
        char buf[MAXLINE];
        struct sockaddr_in cliaddr, servaddr;

        char op, valid;
        unsigned op1, op2, result;


        listenfd = socket(AF_INET, SOCK_STREAM, 0);

        if(listenfd == -1)
        {
                perror("Error in creating the socket\n");
                exit(2);
        }

        else
        {
                printf("Socket Creation Successful\n");
        }

        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);

        bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

        if (listen(listenfd, 4) < 0)
        {
                perror("Error in Listening\n");
                exit(2);
        }

        printf("Server is Running | Waiting on a Connection\n");
        printf("Press Ctrl+C to stop server\n");

        for (; ;)
        {
                clilen = sizeof(cliaddr);

                connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

                if (connfd < 0)
                {
                        printf("Error in Accepting the Client's Message\n");
                        exit(2);
                }

                printf("Connection Recieved\n");

                while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
                {
                        strncpy(&op, buf, 1);     //operator
                        memcpy(&op1, buf+1, 4);     //operand1
                        memcpy(&op2, buf+5, 4);     //operand2
                        printf("Request received: %u %c %u \n", op1, op, op2);
                        switch(op)
{
                        case '+':
                                result= op1 + op2;
                                valid = 1;
                                break;
                        case '-':
                                result= abs(op1 - op2);
                                valid = 1;
                                break;
                        case 'x':
                                result= op1 * op2;
                                valid = 1;
                                break;
                        case '/':
                                if(op2 != 0){
                                        result = op1 / op2;
                                        valid = 1;
                                } else {
                                result = 0;
                                valid = 2;
                                }
                                break;
                        default:
                                printf("Error in Calculation\n");
                                result = 0;
                                valid = 2;
    }

                        memset(buf, 0, sizeof(buf));
                        strncpy(buf, &op, 1);
                        memcpy(buf + 1, &op1, 4);
                        memcpy(buf + 5, &op2, 4);
                        memcpy(buf + 9, &result, 4);
                        memcpy(buf + 13, &valid, 1);
                        send(connfd, buf, sizeof(buf), 0);
                        printf("Result sent: %u %c %u = %u \n", op1, op, op2, result);

                }

                if(n < 0)
                {
                        perror("Read Error\n");
                        exit(1);
                }

                close(connfd);
        }

        close(listenfd);
        return 1;
}