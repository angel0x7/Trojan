import socket

import sys

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 50000

client_socket.bind(('', port))
client_socket.listen(5)
print(f"[*] Listening on port {port}...")

while True:
    (client, addr) = client_socket.accept()
    print(f"[*] Connection from {addr[0]}:{addr[1]}")
    while True:
        r = str(sys.stdin.readline())
        serveursocket.send(r.encode())
        r = serveursocket.recv(1024)
        client_answer = str(r.decode("utf-8", error = "ignore"))
        print(str(client_answer) + "\n")
        
        
        

