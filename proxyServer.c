#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
 #include <errno.h>
 
 	struct Handler {
		char **blacklistH;
		int  sockfdH;
		int   blacklistlengthH;
	}; 
	
	struct Cache {
		//File
		int empty;
		char name[1024];
		FILE *cacheFile; 
	}; 
	
	struct Cache cache1;
	struct Cache cache2;
	int replace=1;
	char* CfileName1 ="/tmp/cachedDataOne.txt";
	char* CfileName2 ="/tmp/cachedDataTwo.txt";
	
	

/*
    For this assignment, we used references from Beej's Guide to Network
    Programming.
    We also incorporated code from multithread_server.c and parse_get.c
    into this program.
*/

/*
Once you type './proxyFilter [port] [black list txt]' to start the proxy, the program should run on its own like a server.
Program is not interactive!
*/


void* connection_handler(void* handle) {
	
	struct Handler handler = *(struct Handler*) handle;
	int sockfd = handler.sockfdH;
	char** blacklist = handler.blacklistH;
	int blacklistlength = handler.blacklistlengthH;
	 struct sockaddr_in client;
    bzero((char*) &client, sizeof(client));
	
	 int acceptsockfd;
   socklen_t clientlength = sizeof(client); // recall client of type sockaddr_in
   acceptsockfd = accept(sockfd, (struct sockaddr*)&client, &clientlength);
   printf("Accepted incoming connection\n");

// Set up for client's GET request

   struct sockaddr_in hostaddr;
   int remsock; // remote socket
   int defPort = 80; // default port

   // GET http://www.cs.ubc.ca/~acton/lawler.txt HTTP/1.1 <<-- using this as an example...
   char type[256]; // type of request (GET)
   char url[4096]; // url in request
   char protocol[256]; // protocol: HTTP/1.1

  char host[256]; // obtain url host from parsing
  char path[256]; // obtain url path from parsing

   char srbuffer[4096]; // buffer used for send and receive
   bzero((char*) srbuffer, 4096);

   // receive http request from client
   recv(acceptsockfd, srbuffer, 4096, 0); // NOT READ();
   sscanf(srbuffer, "%s %s %s", type, url, protocol); // we just want the first line

   if (url[0] == '/'){
    strcpy(srbuffer, &url[1]);
    strcpy(url, srbuffer);
   }

   // accept only get requests
   if ((strncmp (type, "GET", 3) != 0) || ((strncmp(protocol, "HTTP/1.1", 8) != 0))){
    // this is an invalid request, we only want GETs
    printf("405: Bad request. Only GET requests allowed");
    return;
   }

/////////////////////////////////////////////////////////////////////////
   // PARSING URL !!!!!!!!!! //
   printf("Parsing client's GET request\n");
   
   char copyurl[1024];
   char* finder = NULL;
   int offsetParse = 1;
   strcpy(copyurl, url); // used copyurl to parse
   char *npath = &(copyurl[0]);
   ///start////////////////////////////
      char message[1024];
	 int bytes;
	
	
	int get = 0;
	char G = 'G';
	char E = 'E';
	char T = 'T';
	char H = 'H';
	char P = 'P';
	int i = 0;
	int j =0;
	int start = 0;
	char port[6];
	char request_type[4];
    char URL[1024];
	char relativeURL[1024];
	char fileName[1024];
	
	
	while(i< sizeof(srbuffer)){
	if (srbuffer[i] == G && get ==0){
		get = 1;
		 
	}else if(srbuffer[i] == E && get ==1){
		get = 2;
		
	}else if(srbuffer[i] == T && get ==2){
		get = 3;
		
		i++;
		break;
		//srbuffer[i] i now at the T
	}else{
		get = 0;
	}
	i++;
	}
	
	if (get !=3){
		char error[] = "405 : only GET is allowed";
		 printf("sending back 405 : only GET is allowed\n");
		  bytes = write(acceptsockfd,error,sizeof(error));
	 if(bytes < 0){
		  printf("writeback 405 failed\n");
	 }else{
		 close(sockfd);
    close(acceptsockfd);
    return;
	 }
	
	}


	//srbuffer[i] i now at the space 
	i++;
	//srbuffer[i] i now at the h of http 
	
	start = i; //start is handle
	
	int HTTP = 0;
	int end = 0;
	
	while(i<sizeof(srbuffer)){
	if (srbuffer[i] == H && HTTP ==0){
		HTTP = 1;
	}else if(srbuffer[i] == T && (HTTP ==1 || HTTP == 2)){
		HTTP++;
	}else if(srbuffer[i] == P && HTTP ==3){
		HTTP = 4;
		i++;
		break;
		//srbuffer[i] i now at the P
	}else{
		HTTP = 0;
	}
	i++;
	}
	
	i -= 4;
	//srbuffer[i] i now at the H
	end = i;

	 printf("srbuffer is: %s\n", srbuffer);  
	 int k = start;
	 int colon =0;
	 j=0;
	 while(k<end){
		 if(srbuffer[k] ==':'){
			 colon++;
		 }
		 if(colon == 2){
			 break;
		 }
		 URL[j] = srbuffer[k];
		 k++;
		 j++;
		 
	 }
	 printf("Absolute URL is: %s\n", URL);
	 char absoluteURL[1024];
	 strcpy(absoluteURL,URL);
	 
	 
	 if(strcmp(cache1.name,absoluteURL) == 0){
		  printf("Cache1 used\n");
		  printf("Cached URL is: %s\n", cache1.name);
		   
   // Case when we cannot open the file 
   FILE *CachedFile;
    char* CfileName = CfileName1;
   CachedFile = fopen(CfileName, "r"); // open cachedDataOne.txt  
   
   // Case when we cannot open the file 
   if (CachedFile == NULL){
   	printf("Error. Failed to load cachedDataOne.txt\n"); 
        return; 
  } 
  char fileBuff[9000]; // use this when using fgets() which reads the stuff inside request.txt
  while(fgets(fileBuff, 9000, CachedFile) != NULL){
     strtok(fileBuff, "\n"); // take out the extra line
    // strcpy(blacklist[blacklistlength], fileBuff); // use strcpy to copy from fileBuff into blacklist
     printf("%s\n", fileBuff);
	 send(acceptsockfd, fileBuff, strlen(fileBuff), 0);
	// bzero((char*) fileBuff, 9000);

  }
  
  fclose(CachedFile); 
   close(acceptsockfd);
    close(sockfd);

    return;
	  
		  
	 }else if(strcmp(cache2.name,absoluteURL) == 0){
		 printf("Cache2 used\n");
		  printf("Cached URL is: %s\n", cache2.name);
		  
		  FILE *CachedFile;
    char* CfileName = CfileName2;
   CachedFile = fopen(CfileName, "r"); // open cachedDataOne.txt  
   
   // Case when we cannot open the file 
   if (CachedFile == NULL){
   	printf("Error. Failed to load cachedDataOne.txt\n"); 
        return; 
  } 
  char fileBuff[9000]; // use this when using fgets() which reads the stuff inside request.txt
  while(fgets(fileBuff, 9000, CachedFile) != NULL){
	  send(acceptsockfd, fileBuff, 9000, 0);
     strtok(fileBuff, "\n"); // take out the extra line
    // strcpy(blacklist[blacklistlength], fileBuff); // use strcpy to copy from fileBuff into blacklist
     printf("%s\n", fileBuff);
	 
	 bzero((char*) fileBuff, 9000);

  }
  fclose(CachedFile); 
   close(acceptsockfd);
    close(sockfd);

    return;
		 
	 }
	 //srbuffer[k] is now at either ':' or ' '
	 
	 	if(srbuffer[k] == ':'){
		k++;  //srbuffer[k] is now at the First digist of port
		j=0;
		while(j<6){
			if(srbuffer[k] == ' '){
				break;
			}
			port[j] = srbuffer[k];
			k++;
			j++;
		}
		
	}else{
		port[0] = '8';
		port[1] = '0';
	}
	printf("port is: %s\n", port);
		 
	 j=0;	
	for(k=0; k<8; k++){
		request_type[j] = srbuffer[end];
		 end++;
		 j++;
	 }
	 printf("Request type is: %s\n", request_type);
	 j=7;
	 i=0;
	 int ca = 0;
	 while(j<sizeof(URL)){
		 if(URL[j] == '.' && ca == 0){
			 ca=1;
			 relativeURL[i] = URL[j];
		 }else if(URL[j] == 'c' && ca == 1){
			 relativeURL[i] = URL[j];
			 ca=2;
		 }else if(URL[j] == 'a' && ca == 2){
			 relativeURL[i] = URL[j];
			 ca=0;
			 if(URL[j+1] == '/'){
				 break;
			 }
		 }else if(URL[j] == 'o' && ca == 2){
			 ca=0;
			relativeURL[i] = URL[j];
			 if(URL[j+2] == '/' && URL[j+1] == 'm' ){
				 relativeURL[i+1] = URL[j+1];
				 break;
			 }
		 }else if(URL[j] == 'o' && ca == 1){
			 ca=0;
			 relativeURL[i] = URL[j];
			 if(URL[j+1] == 'r' && URL[j+2] == 'g' && URL[j+3] == '/'){
				 relativeURL[i+1] = URL[j+1];
				 relativeURL[i+2] = URL[j+2];
				 break;			 
			 }
		 }else{
			 ca=0;
			 relativeURL[i] = URL[j];
			 
		 }
		 j++;
		 i++;
	 }
	 j++;
	 printf("RelativeURL is: %s\n", relativeURL);
	 i=0;
	 while(URL[j]!= ' '){
		 fileName[i] = URL[j];
		 j++;
		 i++;
	 
	 }
		
	  printf("filename is: %s\n", fileName);
	  
	  
	  ////////////end/////////////////////////////// 

// GET http://www.cs.ubc.ca/~acton/lawler.txt HTTP/1.1
// get rid of http://
   if (strstr(npath, "http://") != NULL){  // start with "http://"
        npath = &(copyurl[6]); // npath = /www.cs.ubc.ca/~acton/lawler.txt
                               // npath = /www.cs.ubc.ca/~acton/lawler.txt:8000
        printf("npath so far: %s\n", npath);
        offsetParse += 6;
   }

   // determine hostname
   finder = strtok(npath, "/"); // www.cs.ubc.ca
   strcpy(host, finder); // host = www.cs.ubc.ca
   printf("Host: %s\n", host);


  // check the blacklist
  int blacklisti;
  for (blacklisti = 0; blacklisti < blacklistlength ; blacklisti++){
    if (strstr(url, blacklist[blacklisti]) != NULL){
      // response contains a blacklisted term
      printf("403 blacklist element in get response\n");
      close(acceptsockfd); 
      return;
    }
  }
  printf("GET request does not contain a blacklist element\n");


   struct hostent * hostent;
   if ((hostent = gethostbyname(host)) == NULL){
      printf("Failed to connect to client's get request host");
      return;
   }

   bzero((char*)& hostaddr , sizeof(hostaddr));
   defPort = htons(atoi(port));
   hostaddr.sin_port = defPort;
   hostaddr.sin_family = AF_INET;
   bcopy((char*) hostent->h_addr, (char*)&hostaddr.sin_addr.s_addr, hostent->h_length);

   remsock = socket(AF_INET, SOCK_STREAM, 0);

   if (remsock < 0){
      printf("Failed to create socket to connect to host\n");
      return;

   }printf("Created socket to connect to host\n");


   if (connect(remsock, (struct sockaddr*)&hostaddr, sizeof(struct sockaddr)) < 0){
      printf("Failed to connect to host\n");
      return;
   }printf("Connected to host\n");

   // SEND //
   char rts[4096];
   strcpy(rts, "GET ");
   strcat(rts, fileName);
   strcat(rts, " ");
   strcat(protocol, "\n");
   strcat(rts, protocol);
   strcat(rts, "Host: ");
   strcat(rts, host);
   strcat(rts, "\r\n\r\n");
   printf("rts: %s\n", rts);

   int tosend; // send to client's get request server
   tosend = send(remsock, rts, strlen(rts), 0);

   if (tosend < 0){
      printf("Failed to send to host\n");
      return;
   }printf("Request sent to host\n");

   bzero((char*) srbuffer, 4096);

   int toreceive; // receive from client's get request server
   toreceive = recv(remsock, srbuffer, 4096, 0);

   while (toreceive > 0){
    // receive successful!!!
	 if(cache1.empty == 0){
		 
		 strcpy(cache1.name,absoluteURL);
		 cache1.empty= 1;
		 cache1.cacheFile = fopen("/tmp/cachedDataOne.txt", "a+");
		 int fileMade;
		 fileMade = fputs(srbuffer, cache1.cacheFile );
		
		 int fileClosed = fclose(cache1.cacheFile);
		  if(fileClosed!=0){
			 printf("File failed to closeeeeeeeeeeeeeeeeeeeee\n");
		 }
		 
	 }else if(cache2.empty == 0){
		 strcpy(cache2.name,absoluteURL);
		 cache2.empty=1;
		 cache2.cacheFile = fopen("/tmp/cachedDataTwo.txt", "a+");
		 int fileMade;
		 fileMade = fputs(srbuffer, cache2.cacheFile );
		 
		 int fileClosed = fclose(cache2.cacheFile);
		  if(fileClosed!=0){
			 printf("File failed to closeeeeeeeeeeeeeeeeeeeee\n");
		 }
	 }else if(replace ==1){
		 strcpy(cache1.name,absoluteURL);
		 cache1.empty = 1;
		 replace = 2;
		  cache1.cacheFile = fopen("/tmp/cachedDataOne.txt", "a+");
		 int fileMade;
		 fileMade = fputs(srbuffer, cache1.cacheFile );
		 
		 int fileClosed = fclose(cache1.cacheFile);
		  if(fileClosed!=0){
			 printf("File failed to closeeeeeeeeeeeeeeeeeeeee\n");
		 }
		 
	 }else if(replace ==2){
		 strcpy(cache2.name,absoluteURL);
		 cache2.empty = 1;
		 replace = 1;
		  cache2.cacheFile = fopen("/tmp/cachedDataTwo.txt", "a+");
		 int fileMade;
		 fileMade = fputs(srbuffer, cache2.cacheFile );
		
		 int fileClosed = fclose(cache2.cacheFile);
		  if(fileClosed!=0){
			 printf("File failed to closeeeeeeeeeeeeeeeeeeeee\n");
		 }
		 
	 }
	 ////////////PArt 4////////////
	 
    //printf("%s\n", srbuffer);
    send(acceptsockfd, srbuffer, strlen(srbuffer), 0);
	 bzero((char*) srbuffer, 4096);
	toreceive = recv(remsock, srbuffer, 4096, 0);
   }
			
    // close sockets
    close(remsock);
    close(acceptsockfd);
    close(sockfd);

    return;
	
	
}

int main(int argc, char **argv) {
	

    struct sockaddr_in server; // use this to bind
	bzero((char*) &server, sizeof(server));//need this line else bind will fail
   

    // a file contains no more than 100 entries
    char *blacklist[100];
    int blacklistlength = 0;
    FILE *fileToFilter;
	
    int sockfd; // use this to make a socket

   if (argc != 3){
    printf("Error. Invalid number of arguments.\n");  
    return 1;
   }
   
   // Here, we shall use argv[2] and check blacklist.txt
   char* fileName = argv[2];
   fileToFilter = fopen(fileName, "r"); // open blacklist.txt  
   
   // Case when we cannot open the file 
   if (fileToFilter == NULL){
   	printf("Error. Failed to load blacklist.txt\n"); 
        return 1; 
  } 
  char fileBuff[100]; // use this when using fgets() which reads the stuff inside request.txt
  while(fgets(fileBuff, 100, fileToFilter) != NULL){
     strtok(fileBuff, "\n"); // take out the extra line
     blacklist[blacklistlength] = (char*) malloc( 100); // since we assume an entry is no more than 100 chars long
     strcpy(blacklist[blacklistlength], fileBuff); // use strcpy to copy from fileBuff into blacklist
     blacklistlength++;

  }
  fclose(fileToFilter);  // close blacklist.txt

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0){
   // recall that sockfd is essentially type int.
    printf("Error. Socket cannot connect. \n");
    return 1;
   }

   // use bind(); // because we are setting up a server
   server.sin_family = AF_INET; // we will only be using IPv4
   server.sin_port = htons(atoi(argv[1])); // convert string type to int and convert to to network byte order
   server.sin_addr.s_addr = INADDR_ANY; // any internet address

   if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) <0){ //bind fails if first port number is too small
        printf("bind failed\n");
        return 1;
   }

   // then listen();
    listen(sockfd, 10);
    printf("Waiting for incoming connection\n");
	
	struct Handler handler;
	handler.sockfdH = sockfd;
	handler.blacklistlengthH = blacklistlength;
	handler.blacklistH = blacklist;
	


	 pthread_t tid[4];
    int err;
    int i = 0;
    while (i < 4) {
   	err = pthread_create(&tid[i], NULL, &connection_handler, (void*) &handler);
    	i++;
    }
	
	//void* connection_handler(void* socket_desc)
	//void connection_handler(int sockfd, char** blacklist, int blacklistlength)
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
	
	


}
