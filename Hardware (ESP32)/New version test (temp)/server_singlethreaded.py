import select, socket
from datetime import datetime
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.setblocking(0)
server.bind(('', 30001))
server.listen()
inputs = [server]
outputs = []
isServerBusy = False

while inputs:
    readable, writable, exceptional = select.select(inputs, outputs, inputs)
    for s in readable:
        # print("Readable: ", s)
        if s is server:
            connection, client_address = s.accept()
            print('Connected to: ' + client_address[0] + ':' + str(client_address[1]))
            connection.setblocking(1)
            outputs.append(connection)
        else:
            data = s.recv(1024)
            if data:
                # print("received from: ", s)
                print(datetime.now().time(), " data: ", data.decode('utf-8'))
                ack = "7" # ack received
                s.send(str.encode(ack))
                if s in inputs:
                    inputs.remove(s)
                if s not in outputs:
                    outputs.append(s)
                isServerBusy = False
            else:
                print("close connection")
                if s in outputs:
                    outputs.remove(s)
                inputs.remove(s)
                s.close()

    if writable and not isServerBusy:
        isServerBusy = True
        s = writable.pop(0)
        # print("writable: ", s)
        # print("send request to connection: ", connection)
        s.send(str.encode("1"))
        outputs.remove(s)
        inputs.append(s)
        

    for s in exceptional:
        outputs.remove(s)
        print("removed exceptional")
        if s in inputs:
            inputs.remove(s)
        s.close()