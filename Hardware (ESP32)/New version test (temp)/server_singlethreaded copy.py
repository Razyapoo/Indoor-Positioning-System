import select, socket
from datetime import datetime
import time


server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.setblocking(0)
server.bind(('', 30001))
server.listen()
# List of sockets with expected responses
inputs = [server]
# List of sockets used for requests
outputs = []
isServerBusy = False
requestSent = None

while inputs:
    # try:
    readable, writable, exceptional = select.select(inputs, outputs, inputs)
    # except:
    
    if readable:
        for r in readable:
            # print("Readable: ", r)
            if r is server:
                connection, client_address = r.accept()
                print('Connected to: ' + client_address[0] + ':' + str(client_address[1]))
                connection.setblocking(1)
                outputs.append(connection)
            else:
                data = r.recv(1024)
                if data:
                    print("received from: ", r)
                    print(datetime.now().time(), " data: ", data.decode('utf-8'))
                    ack = "7" # ack received
                    r.send(str.encode(ack))
                    if r in inputs:
                        inputs.remove(r)
                    if r not in outputs:
                        outputs.append(r)
                    isServerBusy = False
    elif (len(inputs) > 1) and (time.perf_counter() - requestSent) > 0.3:
        for i in inputs:
            if i != server:
                print("close connection")
                if i in inputs:
                    inputs.remove(i)
                isServerBusy = False
                i.close()

    if writable and not isServerBusy:
        isServerBusy = True
        w = writable.pop(0)
        print("writable: ", w)
        w.send(str.encode("1"))
        outputs.remove(w)
        inputs.append(w)
        print("send request to: ", w)
        requestSent = time.perf_counter()
        
    if exceptional:
        for e in exceptional:
            outputs.remove(e)
            print("removed exceptional")
            if e in inputs:
                inputs.remove(e)
            e.close()