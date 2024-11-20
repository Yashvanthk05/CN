#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

uint8_t calculateChecsum(const char *data){
    uint32_t sum=0;
    for(int i=0;i<strlen(data);i++){
        sum+=(uint8_t)data[i];
        if(sum>0xFF){
            sum=(sum&0xFF)+1;
        }
    }
    return (uint8_t)~sum;
}

int main(int argc,char *argv[]){
    int port,clilen,n,sockfd,newsockfd;
    char buffer[256];
    struct sockaddr_in serv_addr,cli_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error on Opening the Socket");
        exit(1);
    }
    int opt=1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
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
        perror("Error on Acceptiong");
        exit(1);
    }
    uint8_t cal_checksum;
    uint8_t cli_checksum;
    n=read(newsockfd,buffer,255);
    if(n<0){
        perror("Error on Reading");
        exit(1);
    }
    cal_checksum=calculateChecsum(buffer);
    printf("Received Message: %s\n",buffer);
    n=read(newsockfd,&cli_checksum,sizeof(cli_checksum));
    if(n<0){
        perror("Error on Reading");
        exit(1);
    }
    printf("Received Checksum: %02x\nCalculated Checksum: %02x\n",cli_checksum,cal_checksum);
    if(cal_checksum==cli_checksum){
        n=write(newsockfd,"Received Message is Correct",28);
    }else{
        n=write(newsockfd,"Received Message is Errorneous",31);
    }
    if(n<0){
        perror("Error on Writing");
        exit(1);
    }
}