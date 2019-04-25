import socket
import ssl


def deal_with_client(connection):
    data = connection.read()
    while data:
        print("Received: %s" % data)
        connection.send(data)
        data = connection.read()


if __name__ == '__main__':
    print("Starting SSL server...")
    bindsocket = socket.socket()
    bindsocket.bind(('', 5300))
    bindsocket.listen(5)

    while True:
        newsocket, fromaddr = bindsocket.accept()
        connstream = ssl.wrap_socket(newsocket,
                                     server_side=True,
                                     certfile="cert.pem",
                                     keyfile="key.pem")
        print("Client connected!")
        try:
            deal_with_client(connstream)
        finally:
            connstream.shutdown(socket.SHUT_RDWR)
            connstream.close()
