###############################################################################
# File: box_server.py
# This is an example of a simple TCPIP server writte in Python
# A client (or terminal) is designed to connect to it and pass
# requests to lock or unlock a box. 
# It can simultaneously generate a message indicating that it
# would like the box unlocked but that the terminal has to authorise
# the action
#

import threading
import argparse
import socket

###############################################################################
# This is the main server loop that listens for connections from clients
def run_server(port):
    hostname = socket.gethostname()
    server_ip = socket.gethostbyname(hostname)

    # create the socket object
    try:
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # bind the socket to a specific address and port
        server.bind((server_ip, port))

        # listen for incoming connections
        server.listen(0)
        print(f"Listening on {server_ip}:{port}")

        while True:
            # accept a client connection
            client_socket, addr = server.accept()
            print(f"Accepted connection from {addr[0]}:{addr[1]}")
            # start a new thread to handle the client
            thread = threading.Thread(target=handle_client, args=(client_socket, addr,))
            thread.start()

    except Exception as e:
        print(f"Error: {e}")
    finally:
        server.close()

###############################################################################
# This is a basic client, multiple threads of this same code are run
# one for each connected client
def handle_client(client_socket, addr):
    try:
        while True:
            # receive and print client messages
            request = client_socket.recv(1024)
            request = request.decode("utf-8")

            match request.lower():
                case "close":
                    client_socket.send("closed".encode("utf-8"))
                    break
                case "lock":
                    print("Lock")
                case "unlock":
                    print("Unlock")
                case _:
                    print(f"Received: {request}")

            response = "accepted".encode("utf-8")
            client_socket.send(response)

    except Exception as e:
        print(f"Error when handling clinet: {e}")
    finally:
        client_socket.close()
        print(f"Connection client ({addr[0]}:{addr[1]}) closed")



###############################################################################
# Main entry point for box_server

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Box server application")
    parser.add_argument('port', type=int, nargs='?', default=8000,
                        help='Port to receive connections on')
    args = parser.parse_args()

    run_server(args.port)
