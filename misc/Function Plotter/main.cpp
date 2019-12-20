    #include<stdio.h>
    #include<fstream>
    #include<stdlib.h>
    #include<string.h>
    #include<sys/socket.h>
    #include<arpa/inet.h>
    #include<unistd.h>
    #include<iostream>
    #include <string.h>
    #include <thread>
    #include <chrono>
    
    using namespace std;
    
    ofstream fout("your d.txt");
    int x,y;
    int pos;
    char v[200];
    bool val[32][32];
    int attempt;
    
    
    int hSocket, read_size;
    
    
    //Create a Socket for server communication
    short SocketCreate(void)
    {
        short hSocket;
        printf("Create the socket\n");
        hSocket = socket(AF_INET, SOCK_STREAM, 0);
        return hSocket;
    }
    //try to connect with server
    int SocketConnect(int hSocket)
    {
        int iRetval=-1;
        int ServerPort = 13005;
        struct sockaddr_in remote= {0};
        remote.sin_addr.s_addr = inet_addr("45.76.93.10"); 
        remote.sin_family = AF_INET;
        remote.sin_port = htons(ServerPort);
        iRetval = connect(hSocket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
        return iRetval;
    }
    // Send the data to the server and set the timeout of 20 seconds
    int SocketSend(int hSocket,char* Rqst,short lenRqst)
    {
        int shortRetval = -1;
        struct timeval tv;
        tv.tv_sec = 20;  /* 20 Secs Timeout */
        tv.tv_usec = 0;
        if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0)
        {
            printf("Time Out\n");
            return -1;
        }
        shortRetval = send(hSocket, Rqst, lenRqst, 0);
        printf("%s",Rqst);
        return shortRetval;
    }
    //receive the data from the server
    int SocketReceive(int hSocket,char* Rsp,short RvcSize)
    {
        int shortRetval = -1;
        struct timeval tv;
        tv.tv_sec = 20;  /* 20 Secs Timeout */
        tv.tv_usec = 0;
        if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv)) < 0)
        {
            printf("Time Out\n");
            return -1;
        }
        shortRetval = recv(hSocket, Rsp, RvcSize, 0);
        if(shortRetval!=-1)
          Rsp[shortRetval]='\0';
        printf("%s\n",Rsp);
        return shortRetval;
    }
    
    int sockCon()
    {
        //Create socket
        hSocket = SocketCreate();
        if(hSocket == -1)
        {
            printf("Could not create socket\n");
            return 1;
        }
        printf("Socket is created\n");
        //Connect to remote server
        if (SocketConnect(hSocket) < 0)
        {
            perror("connect failed.\n");
            return 1;
        }
        printf("Sucessfully conected with server\n");
        printf("Enter the Message: ");
        return 0;
    }
    
    void sockClose()
    {
        close(hSocket);
        shutdown(hSocket,0);
        shutdown(hSocket,1);
        shutdown(hSocket,2);
    }
    
    //main driver program
    int main(int argc, char *argv[])
    {
        struct sockaddr_in server;
        if(sockCon()) return 1;
        char c[500] = {0};
        char SendToServer[500] = {0};
        char server_reply[3000] = {0};
        SocketSend(hSocket, " ",1 );
    //    SocketSend(hSocket, SendToServer, strlen(SendToServer));
        read_size = SocketReceive(hSocket, server_reply, 3000);
        char valc[20];
        strcpy(valc,strstr(server_reply,"f(")+2);
        char * p = strtok(valc,", ");
        x= atoi(p);
        p=strtok(NULL,", ");
        y=atoi(p);
        for(;attempt<961;attempt++)
        {
          strcpy(SendToServer,"0\n");
          
          //Send data to the server
          SocketSend(hSocket, SendToServer, strlen(SendToServer));
          //receive data
          read_size = SocketReceive(hSocket, server_reply, 3000);
          if(server_reply[0]=='G')
            val[x][y]=0;
          else
            val[x][y]=1;
          if(strstr(server_reply,"f(")==NULL) break;
          strcpy(valc,strstr(server_reply,"f(")+2);
          p = strtok(valc,", ");
          x= atoi(p);
          p=strtok(NULL,", ");
          y=atoi(p);
          printf("%d 0\n",attempt);
         // std::this_thread::sleep_for(50ms);
        }
        sockClose();
        //Code that leads to the false "ending" and would be how the "solve all 961 values" task would have been done
     /*   sockCon();
        for(attempt=0;attempt<961;attempt++)
        {
          read_size = SocketReceive(hSocket, server_reply, 3000);
          if(read_size==-1)
          {
            attempt--;
            continue;
          }
          strcpy(valc,strstr(server_reply,"f(")+2);
          p = strtok(valc,", ");
          x= atoi(p);
          p=strtok(NULL,", ");
          y=atoi(p);
          char arr[40]={0};
          SendToServer[0]='0'+val[x][y];
          SendToServer[1]='\0';
          strcat(SendToServer,"\n");
          
          //Send data to the server
          SocketSend(hSocket, SendToServer, strlen(SendToServer));
          printf("%d 1\n",attempt);
     //     std::this_thread::sleep_for(50ms);
        }
        read_size = SocketReceive(hSocket, server_reply, 3000);
        strcpy(SendToServer,"0\n");
          
        //Send data to the server
        SocketSend(hSocket, SendToServer, strlen(SendToServer));
        //receive data
        read_size = SocketReceive(hSocket, server_reply, 3000);
        sockClose();*/
        //print the matrix so as to "plot" the function
        for(int i=0;i<32;i++)
        {
          for(int j=0;j<32;j++)
          {
            //# instead of 1 and _ instead of 0 so they are more distinct
            if(val[j][i])
              printf("#");
            else
              printf("_");
          }
          printf("\n");
        }
        return 0;
    }
