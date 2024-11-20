#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

int main(int argc,char *argv[]){
    int sockfd,port,n,frame_number=0;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if(argc<3){
        fprintf(stderr,"Usage %s Hostname Port\n",argv[0]);
    }
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    port=atoi(argv[2]);
    if(sockfd<0){
        perror("Error on Opening the Socket");
        exit(1);
    }
    server=gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr,"No Such Host");
        exit(1);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char*)server->h_addr_list[0],(struct sockaddr*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port=htons(port);
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Error on Connecting");
        exit(1);
    }
    int frame_loss_counter=0;
    frame_loss_counter++;
    int window=4;
    int base=0;
    int next_frame=0;
    while(1){
        if(next_frame<window+base){
            printf("Sending Frame: %d\n",frame_number);
            bzero(buffer,256);
            sprintf(buffer,"%d",frame_number);
            n=write(sockfd,buffer,strlen(buffer));
            if(n<0){
                perror("Error on Writing");
                exit(1);
            }
            next_frame++;
        }
        bzero(buffer,256);
        n=read(sockfd,buffer,255);
        if(n<0){
            perror("Error on Reading");
            exit(1);
        }
        int ack=buffer[0]-'0';
        if(ack>=base){
            printf("Frame %d is Acknowledged\n",ack);
            base=ack+1;
        }

    }
    return 0;
}