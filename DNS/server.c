#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    int sockfd,newsockfd,port,clilen,n;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error on Opening the Socket");
        exit(1);
    }
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Error on Setting Socket Options");
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
    listen(sockfd,5);
    clilen=sizeof(cli_addr);
    newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);
    if(newsockfd<0){
        perror("Error on Accepting");
        exit(1);
    }
    n=read(newsockfd,buffer,255);
    if(n<0){
        perror("Error on reading");
        exit(1);
    }
    struct hostent *host;
    struct in_addr ip_addr;
    if(inet_aton(buffer,&ip_addr)){
        host=gethostbyaddr((const void*)&ip_addr,sizeof(ip_addr),AF_INET);
        if(host==NULL) strcpy(buffer,"No such Host");
        else sprintf(buffer,"Hostname: %s",host->h_name);
    }else{
        host=gethostbyname(buffer);
        if(host==NULL) strcpy(buffer,"No Such IP Address");
        else sprintf(buffer,"IPV4 Address: %s",inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    }
    n=write(newsockfd,buffer,strlen(buffer));
    if(n<0){
        perror("Error on Writing");
        exit(1);
    }
    return 0;
}