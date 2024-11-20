#include<stdio.h>

void printBinary(int num){
    for(int i=7;i>=0;i--){
        printf("%d",(num>>i)&1);
    }
}

void findClassAndSubnet(char *ip,unsigned char subnet[]){
    unsigned char firstoctet;
    sscanf(ip,"%hhu",&firstoctet);
    if(firstoctet>=0 &&  firstoctet<=127){
        printf("Class: A\nSubnet Mask: 255.0.0.0\n");
        subnet[0]=255;
    }else if(firstoctet>=128 && firstoctet<=191){
        printf("Class: B\nSubnet Mask: 255.255.0.0\n");
        subnet[0]=subnet[1]=255;
    }else if(firstoctet>=192 && firstoctet<223){
        printf("Class: C\nSubnet Mask: 255.255.255.0\n");
        subnet[0]=subnet[1]=subnet[2]=255;
    }else if(firstoctet>=224 && firstoctet<=239){
        printf("Class: D(Multi Casting)\nNo Subnet Mask\n");
    }else if(firstoctet>=240 && firstoctet<=255){
        printf("Class: E(Future Purpose)\nNo Subnet Mask\n");
    }
}

int main(){
    char ip[16];
    unsigned char bytes[4],subnet[4]={0};
    unsigned char networkid[4],hostid[4];
    scanf("%15s",ip);
    sscanf(ip,"%hhu.%hhu.%hhu.%hhu",&bytes[0],&bytes[1],&bytes[2],&bytes[3]);
    findClassAndSubnet(ip,subnet);
    printf("IPV4 address: %d.%d.%d.%d\n",bytes[0],bytes[1],bytes[2],bytes[3]);
    for(int i=0;i<4;i++){
        networkid[i]=bytes[i]&subnet[i];
        hostid[i]=bytes[i]&~subnet[i];
    }
    printf("IPV4 in binary: ");
    for(int i=0;i<4;i++){
        printBinary(bytes[i]);
        if(i<3) printf(".");
        else printf("\n");
    }
    printf("Network ID: %d.%d.%d.%d\n",networkid[0],networkid[1],networkid[2],networkid[3]);
    printf("Host ID: %d.%d.%d.%d\n",hostid[0],hostid[1],hostid[2],hostid[3]);

}