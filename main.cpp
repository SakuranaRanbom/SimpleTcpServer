#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


void error_handling(char *message){
    std::cout << message << std::endl;
}

int main() {
    int serv_sock;
    int clnt_sock;//创建套接字

    struct sockaddr_in serv_add;
    struct sockaddr_in clnt_add;

    socklen_t cliaddrlen;
    char message[256] = "";

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);//TCP
    if(serv_sock < 0) {
        std::cout << "Create Socket failed!" << std::endl;
        return 0;
    }
    memset(&serv_add, 0, sizeof(serv_add));
    serv_add.sin_family = AF_INET;//地址zu
    serv_add.sin_addr.s_addr = inet_addr("0.0.0.0");
    serv_add.sin_port = htons(9527);

    int ret = bind(serv_sock,(struct sockaddr*) &serv_add, sizeof(serv_add));
    if(ret == -1) {
        error_handling("bind failed!");
        close(serv_sock);
        return 0;
    }

    ret = listen(serv_sock, 3);
    if(ret == -1) {
        error_handling("listen failed!");
        close(serv_sock);
        return 0;
    }



    ssize_t len = 0;
    while(1){
        memset(message, 0, sizeof(message));
        clnt_sock = accept(serv_sock, (sockaddr *) &clnt_add, &cliaddrlen);
        if (clnt_sock == -1) {
            error_handling("accept failed!");
            //close(serv_sock);
            //return 0;
        }
        {
            ret = read(clnt_sock, message, sizeof(message));

            fputs(message, stdout);
            std::string str = message;
            str = "From server:" + str;

            ret = write(clnt_sock, message, strlen(message));
            if (ret != strlen(message)) {
                error_handling("write failed!");
                //close(serv_sock);
                //return 0;
            }
        }
        close(clnt_sock);
    }


    close(serv_sock);
    return 0;
}
