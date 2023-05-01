#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct t_clients{
    char msg[6500];
    int id;
}f_clients;

f_clients clients[1500];
int fdMax=0;
int act_client=0;


void zero_all(){
    for(int i=0;i<1500;i++){
        clients[i].id=-1;
        bzero(clients[i].msg,6500);
        
    }
}

void error_exit(char * msg){
    write(2,msg,strlen(msg));
    exit(1);
}

int main(int argc, char * argv[]) {
	int sockfd, connfd;
    socklen_t len;
	struct sockaddr_in servaddr; 
    fd_set writefds,readfds,active;

    if(argc < 2){FD_SET(sockfd,&readfds);
        error_exit("Wrong number of arguments\n");
    }

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM,0);
  FD_ZERO(&writefds);
    FD_ZERO(&readfds);

    zero_all();
    FD_SET(sockfd,&writefds);
    FD_SET(sockfd,&active);
    fdMax=sockfd;
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		 error_exit("Fatal error\n");
	} 

	if (listen(sockfd, 10) != 0) {
	    error_exit("Fatal error\n");
	}
	
  
    while(1){

      readfds = writefds = active;
    if(select(fdMax + 1,&readfds,&writefds,NULL,NULL) < 0){
        continue;
    }
    else{
    for(int i=0; i<=fdMax + 1;i++){
         if(FD_ISSET(i,&readfds)){
        if(i!= sockfd){
            char buff[400];
            int readead = recv(i,buff,400,0);
            if(readead <= 0)
                continue;            
            buff[readead]=0 ;
            printf("%s",buff);
            bzero(buff,400);
            continue;
        }
        if( i == sockfd){
        struct sockaddr_in cli;
        len = sizeof(cli);
	    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	    if (connfd < 0) {printf("entrrei\n"); break;} 
        else{
            printf("server acccept the client...\n");
            fdMax++;
            FD_SET(connfd,&active);
            send(connfd,"OLA\n",5,0);

            break;
           
            
        }
         }}}
        

    }
    }

}
    
    
