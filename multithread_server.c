#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


void* connection_handler(void *);

int main(int argc, char* argv[]) {

    //Socket 
    int socket_desc;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
        return 1;
    }
    

    //Bind
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
   
    if ( bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
    	printf("bind failed");
        return 1;
    }
    
    //Listen
    listen(socket_desc, 10);        
    printf("waiting for incoming connection\n");
    
    //Create 2 worker threads
    pthread_t tid[2];
    int err;
    int i = 0;
    while (i < 2) {
   	err = pthread_create(&tid[i], NULL, &connection_handler, (void*) &socket_desc);
    	i++;
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}

void* connection_handler(void* socket_desc) {
    //Accept
    int client_sock, c;
    struct sockaddr_in client;
    char* message = "Greetings!";
    while ( (client_sock = accept( *(int*) socket_desc, (struct sockaddr *) &client, (socklen_t*) &c))) {
    	write(client_sock, message, strlen(message));
        getchar();
        close(client_sock); 
    }

}
