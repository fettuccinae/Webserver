#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <string.h>

int main(){

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (result != 0){
        printf("WSAStartup crashing");
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET){
        printf("SOCKET UNABLE TO INITIALISZE");
        return 1;
    }

    //client metadata
    struct sockaddr_in clientAddr;
    int varsize = sizeof(clientAddr);
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.1.2.7");
    serverAddress.sin_port = htons(9999);

    int bindResult = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bindResult == SOCKET_ERROR){
        printf("SOCKET NOT BINDING");
        return 1;
    }

    int listenResult=listen(serverSocket,SOMAXCONN);
    if (listenResult == SOCKET_ERROR){
        printf("NOT LISTENING THO");
        return 1;
    }
    while(1){
        SOCKET clientSocket = accept(serverSocket,(struct sockaddr *)&clientAddr, &varsize);
        printf("%d\n",clientSocket);
        if (clientSocket == INVALID_SOCKET){
            printf("WHAT");
            return 1;
        }
        printf("%d\t%d\t%s\n",clientAddr.sin_family,ntohs(clientAddr.sin_port),inet_ntoa(clientAddr.sin_addr));
        char buffer[1024];
        recv(clientSocket,buffer,1023,0);
        printf("%s",buffer);
        
        char method[5];
        int get_it = 0;
        strncpy(method,buffer,4);
        if (strcmp(method,"GET ") == 0){
            get_it = 1;
        }
        char route[256];
        int i = get_it? 4: 5;
        int j;
        int count = 0;
        for (j = i; buffer[j] != ' ' && buffer[j] != EOF; j++){   
            route[j-i] = buffer[j];
            count+=1;
        }
        route[j-i] = '\0';
        printf("%s %d \n",route,count);
        
        char fuckinghtmlresponse [] = "HTTP/1.1 200 OK\nContent-Type: text/html\ncharset=UTF-8\nConnection: keep-alive\nKeep-Alive: timeout=5, max=100\n\n";
        int hsize = sizeof(fuckinghtmlresponse)/sizeof(char);
        char fuckingcssresponse [] = "HTTP/1.1 200 OK\nContent-Type: text/css\nConnection: keep-alive\nKeep-Alive: timeout=5, max=100\n\n";
        int csize = sizeof(fuckingcssresponse)/sizeof(char);

        if (strcmp(route,"/") == 0 || strcmp(route,"/index") == 0){
            FILE *file = fopen("index.html","rb");
            fseek(file,0,SEEK_END);
            int fSize = ftell(file);
            rewind(file);
            char data[fSize+1];
            fread(data,1,fSize,file);
            data[fSize] = '\0';
            send(clientSocket,fuckinghtmlresponse,hsize,0);
            send(clientSocket,data,fSize,0);
            fclose(file);

            printf("WEHER");
        }

        else if (strcmp(route,"/styles.css") == 0){
            printf("HERE");
            FILE* file = fopen("styles.css", "rb");
            fseek(file, 0, SEEK_END);
            long fsize = ftell(file);  // Get the size of the file
            fseek(file, 0, SEEK_SET);  // Move the file pointer back to the beginning
            char* data = malloc(fsize + 1);
            if (data == NULL) {
                printf("Memory allocation failed\n");
                fclose(file);
                return 0;
            }
            char ch;
            int i = 0;
            while ((ch = fgetc(file)) != EOF) {
                data[i] = ch;
                i++;
                }

                data[i] = '\0';  // Null-terminate the string
            //printf("%s",data);
                fclose(file);

            send(clientSocket,fuckingcssresponse,csize,0);
            send(clientSocket,data,fsize,0);
        }
        else if (strcmp(route,"/dot") == 0){
            FILE *file = fopen("dot.html","r");
            fseek(file,0,SEEK_END);
            long fSize = ftell(file);
            rewind(file);
            char *data = malloc(fSize + 1);
            fread(data, 1, fSize, file);
            data[fSize] = '\0';
            send(clientSocket,fuckinghtmlresponse,hsize,0);
            send(clientSocket,data,fSize,0);
            fclose(file);
            printf("%d DOT",fSize);
        
        }
        else if (strcmp(route,"/head") == 0){
            FILE *file = fopen("head.html","rb");
            fseek(file,0,SEEK_END);
            int fSize = ftell(file);
            rewind(file);
            char data[fSize+1];
            fread(data,1,fSize,file);
            data[fSize] = '\0';
            send(clientSocket,fuckinghtmlresponse,hsize,0);
            send(clientSocket,data,fSize-1,0);
            fclose(file);

            printf("Head");
        
        }
        else if (strcmp(route,"/cont") == 0){
            FILE *file = fopen("cont.html","rb");
            fseek(file,0,SEEK_END);
            int fSize = ftell(file);
            rewind(file);
            char data[fSize+1];
            fread(data,1,fSize,file);
            data[fSize] = '\0';
            send(clientSocket,fuckinghtmlresponse,hsize,0);
            send(clientSocket,data,fSize-1,0);
            fclose(file);

            printf("CUNT US");
        
        }
        

        
        
        
        else{
            send(clientSocket, "HTTP/1.1 404 Not Found\n", 22, 0);

            FILE *file = fopen("index.html","rb");
            fseek(file,0,SEEK_END);
            int fSize = ftell(file);
            rewind(file);
            char data[fSize+1];
            fread(data,1,fSize,file);
            data[fSize] = '\0';
            send(clientSocket,fuckinghtmlresponse,hsize,0);
            send(clientSocket,data,fSize,0);
            fclose(file);
            printf("WEHER");
            
        }
        closesocket(clientSocket);
            
    }

    WSACleanup();
    return 0;
    

}
