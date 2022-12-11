import socket
import time
from _thread import *

host = ''
port = 30001
ThreadCount = 0
# busy = False;
currentConnection = None
lastConnection = None

def client_handler(connection):
    # connection.send(str.encode('You are now connected to the replay server... Type BYE to stop'))
    # global busy
    global currentConnection
    global lastConnection
    timer = time.perf_counter()
    while True:
        if currentConnection == None and connection != lastConnection:
            currentConnection = connection
            lastConnection = connection
            
        # if (time.perf_counter() - timer > 5): 
        #     # busy = False
        #     currentConnection = None
        #     lastConnection = None
        #     break
        
        if currentConnection == connection:
            # data = connection.recv(1024)
            # message = data.decode('utf-8')
            # # busy = True
            # # currentConnection = connection
            # if message[1] == "3":
            #     # print(connection)
            #     reply = "5"
            #     # print("accepted")
            #     connection.sendall(str.encode(reply))
            #     # data = connection.recv(1024)
            #     # message = data.decode('utf-8')
                
            # else:
            #     # print(connection)
            #     print(message)
            #     # print("received")
            #     timer = timer = time.perf_counter()
            #     # busy = False
            #     currentConnection = None
            print("send request")
            connection.sendall(str.encode("1"))
            data = ""
            print("before while")
            while not data or (time.perf_counter() - timer > 5):
                data = connection.recv(1024).decode('utf-8')
            print("after while")
            if data:
                print(data)
                timer = timer = time.perf_counter()
                currentConnection = None
                lastConnection = connection
            else:
                currentConnection = None
                lastConnection = None
                break

    connection.close()

def accept_connections(ServerSocket):
    Client, address = ServerSocket.accept()
    print('Connected to: ' + address[0] + ':' + str(address[1]))
    start_new_thread(client_handler, (Client, ))

def start_server(host, port):
    ServerSocket = socket.socket()
    ServerSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        ServerSocket.bind((host, port))
    except socket.error as e:
        print(str(e))
    print(f'Server is listing on the port {port}...')
    ServerSocket.listen()

    while True:
        accept_connections(ServerSocket)
start_server(host, port)