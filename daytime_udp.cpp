#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

void errHandler (const char * why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET;      
    selfAddr->sin_port = 0;              
    selfAddr->sin_addr.s_addr = 0;       

    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET; 
    remoteAddr->sin_port = htons(13); 
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12"); 

    char *buf = new char [256];
    strcpy(buf, "Который час?\n"); 
    int msgLen = strlen(buf);             

    int mySocket = socket(AF_INET, SOCK_DGRAM, 0); 
    if (mySocket == -1) {
        errHandler("Ошибка открытия сокета", 11);
    }

    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in));
    if (rc == -1) { 
        close(mySocket);
        errHandler("Ошибка привязки сокета к адресу", 12);
    }

    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка связи с уалённым сервером", 13);
    }

    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) { 
        close(mySocket);
        errHandler("Ошибка отправки сообщения", 14);
    }
    cout << "Отправлено: " << buf << endl;

    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) { 
        close(mySocket);
        errHandler("Ошибка получения сообщения", 15);
    }
    buf[rc]= '\0';
    cout<<"Получено: "<< buf << endl;


    close(mySocket);
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    
    return 0;
}