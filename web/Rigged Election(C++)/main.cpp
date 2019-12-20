    #include<stdio.h>
    #include<fstream>
    #include<stdlib.h>
    #include<string.h>
    #include<sys/socket.h>
    #include<arpa/inet.h>
    #include<unistd.h>
    #include<iostream>
    #include<fstream>
    #include <thread>
    #include <chrono>
    #include "md5.h"
    #include <string>
    #include <string.h>
    
    using namespace std;
    using std::cout; using std::endl;
    
    int attempt=0;
    
    struct elem
    {
      char string[8];
      char hash[7];
    };
    
    bool has[16777216+1000];
    int pos[16777216+1000];
    elem elems[16777216/2];
    
    char letters[63]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int c[8];
    char c2[8];
    
    char normalHeader[1000];
    
    ofstream outR("resp.gz");
    ofstream fout("Much DB such wow.txt");
    ifstream fin("header.in");
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
        int ServerPort = 11001;
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
      //  printf("%s",Rsp);
        return shortRetval;
    }
    
    long long hexToDec(char v[7])
    {
      long long num=0;
      for(int i=0;v[i]!='\0';i++)
      {
        num=num*16;
        if(v[i]>='0'&&v[i]<='9')
          num+=v[i]-'0';
        else
          num+=(v[i]-'a')+10;
      }
      return num;
    }
    //main driver program
    
    int newSock(int &hSocket)
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
    
    void closeSock(int &hSocket)
    {
        
      close(hSocket);
      shutdown(hSocket,0);
      shutdown(hSocket,1);
      shutdown(hSocket,2);
    }
    int main(int argc, char *argv[])
    {
     
      int len=1;
      long long na=0;
      for(int i=0;i<(16777216/2);i++)
      {
        bool ok=false;
        for(int j=len-1;j>=0;j--)
          if(c[j]<61)
          {
            c[j]++;
            for(int k=j+1;k<len;k++)
              c[k]=0;
            ok=true;
            break;
          }
        if(!ok)
        {
          len++;
          for(int j=0;j<len;j++)
            c[j]=0;
      //    printf("%d\n",len);
        }
        for(int j=0;j<len;j++)
          c2[j]=letters[c[j]];
        c2[len]='\0';
        char actual[30];
        strcpy(actual,"watch__bisqwit__1111111");
        strcat(actual,c2);
        string hash=md5(actual);
        char semi[7];
        for(int j=0;j<6;j++)
          semi[j]=hash[j];
        semi[6]='\0';
        na= hexToDec(semi);
        if(na>16777216+1||na<0)
        {
          printf("%s\n",semi);
          break;
        }
        if(has[na])
        {
          i--;
          continue;
        }
        has[na]=1;
        pos[na]=i;
        strcpy(elems[i].string,c2);
        strcpy(elems[i].hash,semi);
        if(i%100000==0) printf("%d\n",i/100000+1);
      }
      while(!fin.eof())
      {
        char c3[300];
        fin.getline(c3,300);
        strcat(normalHeader,c3);
        strcat(normalHeader,"\r\n");
      }
      strcat(normalHeader,"\r\n");
      int hSocket;
      if(newSock(hSocket)) return 1;
      for(int i=0;i<250;i++)
      {
        char p_request[20000]={0};
        char p_response[50000]={0};
        char target[7];
        char p_resourcepath1[]="/vote.php?g=1";
        char p_resourcepath2[]="/vote.php?id=837&u=1&h=";
        sprintf(p_request, "GET %s HTTP/1.1\r\n", p_resourcepath1);
        strcat(p_request,normalHeader);
        SocketSend(hSocket, p_request, strlen(p_request));
        SocketReceive(hSocket, p_response, 50000);
        printf("size:%d\n",strlen(p_response));
        if(strlen(p_response)==0)
        {
          i--;
          closeSock(hSocket);
          if(newSock(hSocket)) return 1;
          std::this_thread::sleep_for(250ms);
          continue;
        }
        char untarget[20];
        strcpy(untarget,strstr(p_response,"\r\n\r\n6\r\n"));
        if(untarget==NULL)
        {
          i--;
          continue;
        }
        strncpy(target,untarget+7,6);
        printf("%s",target);
        int nTarget=hexToDec(target);
        if(nTarget>=0&&nTarget<=16777216 &&has[nTarget])
        {
          sprintf(p_request, "GET %s1111111%s HTTP/1.1\r\n",p_resourcepath2,elems[pos[nTarget]].string);
          strcat(p_request,normalHeader);
          SocketSend(hSocket, p_request, strlen(p_request));
          SocketReceive(hSocket, p_response, 50000);
        }
        else
        {
          i--;
          sprintf(p_request, "GET %s11111111 HTTP/1.1\r\n",p_resourcepath2);
          strcat(p_request,normalHeader);
          SocketSend(hSocket, p_request, strlen(p_request));
          SocketReceive(hSocket, p_response, 50000);
        }
        printf("i:%d\n",i);
        std::this_thread::sleep_for(250ms);
      }
 
      return 0;
    }
