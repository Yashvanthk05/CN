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
    while(1){
        printf("Sending Frame %d\n",frame_number);
        bzero(buffer,256);
        sprintf(buffer,"%d",frame_number);
        n=write(sockfd,buffer,strlen(buffer));
        if(n<0){
            perror("Error on Writing");
            exit(1);
        }
        if(frame_loss_counter%3==2){
            printf("Simulating Loss of Frame: %d\n",frame_number);
            bzero(buffer,256);
        }else{
            sleep(1);
            bzero(buffer,256);
            n=read(sockfd,buffer,255);
            if(n<0){
                perror("Error on reading");
                exit(1);
            }
            int ack=buffer[0]-'0';
            if(ack!=frame_number){
                printf("Frame %d is Acknowledged. Sending Next Frame...\n",frame_number);
                frame_number=!frame_number;
            }
        }
        frame_loss_counter++;
    }
    return 0;
}