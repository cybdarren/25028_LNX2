###############################################################################
# File: box_client.py
# This is an example of a simple TCPIP client writte in Python
# A client (or terminal) is designed to connect to a server and pass
# requests to lock or unlock a box. The server controls the box
# and this terminal is a remote controller for it.
#

import argparse
import socket


def run_client(server_ip, server_port):
    # create a socket object
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # establish connection with server
    client.connect((server_ip, server_port))

    try:
        while True:
            # input message and send it to the server
            msg = input("Enter message: ")
            client.send(msg.encode("utf-8")[:1024])

            # receive message from the server
            response = client.recv(1024)
            response = response.decode("utf-8")

            # if server sent us "closed" in the payload, we break out of the loop and close our socket
            if response.lower() == "closed":
                break

            print(f"Received: {response}")

    except Exception as e:
        print(f"Error: {e}")
    finally:
        # close client socket (connection to the server)
        client.close()
        print("Connection to server closed")

###############################################################################
# main entry point for the box client
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Box client application")
    parser.add_argument('server', type=str, nargs=1, help='Box server IP address')
    parser.add_argument('port', type=int, nargs='?', default=8000,
                        help='Port to receive connections on')
    args = parser.parse_args()

    run_client('.'.join(args.server), args.port)
