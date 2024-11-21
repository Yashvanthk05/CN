#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    int sockfd,port,n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if(argc<3){
        fprintf(stderr,"Usage %s Hostname port\n",argv[0]);
        exit(0);
    }
    port=atoi(argv[2]);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error on Opening Socket");
        exit(1);
    }
    server=gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr,"No such Host");
        exit(0);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char*)server->h_addr_list[0],(struct sockaddr*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port=htons(port);
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Error on Connecting");
        exit(1);
    }
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    buffer[strcspn(buffer,"\n")]='\0';
    n=write(sockfd,buffer,strlen(buffer));
    if(n<0){
        perror("Error on Writing");
        exit(1);
    }
    while(read(sockfd,buffer,255)>0){
        printf("%s\n",buffer);
    }
    return 0;
}