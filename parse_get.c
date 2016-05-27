#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    char HTTP_REQUEST[] = "GET /filename HTTP/1.1\nHost: www.example.com:8080\n";
    
    printf("\n%s\n", HTTP_REQUEST);
   
    char request_type[4];
    char filename[1024];
    char host[256];
    char port[6];

    char* strptr;
    strptr = strtok(HTTP_REQUEST, " ");
    strcpy(request_type, strptr);
    printf("request type: %s\n", request_type);

    strptr = strtok(NULL, " ");
    strcpy(filename, strptr+1);
    printf("filename: %s\n", filename);
    
 
    strptr = strtok(NULL, ":");
    strptr = strtok(NULL, ":");
    strcpy(host, strptr+1);
    printf("host: %s\n", host);
    
    strptr = strtok(NULL, ":");
    printf("port: %s\n", strptr);
 
}
