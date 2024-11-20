#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

uint8_t calculateChecksum(const char *data){
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
    int sockfd,port,n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if(argc<3){
        fprintf(stderr,"Usage %s Hostname Port\n",argv[0]);
        exit(0);
    }
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    port=atoi(argv[2]);
    if(sockfd<0){
        perror("Error on Opening the Socket");
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
    uint8_t cal_checksum=calculateChecksum(buffer);
    n=write(sockfd,buffer,strlen(buffer));
    if(n<0){
        perror("Error on Writing");
        exit(1);
    }
    n=write(sockfd,&cal_checksum,sizeof(cal_checksum));
    if(n<0){
        perror("Error on Writing");
        exit(1);
    }
    n=read(sockfd,buffer,255);
    if(n<0){
        perror("Error on Reading");
        exit(1);
    }
    printf("%s\n",buffer);
    return 0;
}