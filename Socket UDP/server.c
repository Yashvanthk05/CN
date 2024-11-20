#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    int sockfd,port,n,clilen;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        perror("Error on Opening the Socket");
        exit(1);
    }
    port=5000;
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port=htons(port);
    if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Error on Binding");
        exit(1);
    }
    clilen=sizeof(cli_addr);
    while(1){
        n=recvfrom(sockfd,buffer,255,0,(struct sockaddr*)&cli_addr,&clilen);
        if(n<0){
            perror("Error on Reading");
            exit(1);
        }
        printf("Client Message: %s\n",buffer);
        n=sendto(sockfd,"Message Received Successfully",29,0,(struct sockaddr*)&cli_addr,clilen);
        if(n<0){
            perror("Error on Writing");
            exit(1);
        }
    }
    return 0;
}