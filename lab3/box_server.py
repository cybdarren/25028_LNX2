###############################################################################
# File: box_server.py
# This is an example of a simple TCPIP server writte in Python
# A client (or terminal) is designed to connect to it and pass
# requests to lock or unlock a box. 
# It can simultaneously generate a message indicating that it
# would like the box unlocked but that the terminal has to authorise
# the action
#

import asyncio
import argparse
import socket

###############################################################################
# Asynchronous server application that listens to connections from a client
class AsyncTCPIPServer:
    def __init__(self, port: int):
        hostname = socket.gethostname()
        self.server_ip = socket.gethostbyname(hostname)
        self.port = port
        self.clients = []

    async def handle_client(self, reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
        # handle communications with a single client
        self.clients.append(writer)
        addr = writer.get_extra_info('peername')
        print(f"New connection from {addr}")

        try:
            while True:
                data = await reader.read(1024)
                if not data:
                    print(f"Client {addr} disconnected.")
                    break
                client_message = data.decode("utf-8").strip()
                print(f"Received from {addr}: {client_message}")
                
                match client_message.lower():
                    case 'lock':
                        print(f"Request to lock from {addr}.")

                    case 'unlock':
                        print(f"Request to unlock from {addr}")
                        
                    case 'close':
                        print(f"Closing connection with {addr}.")
                        break

                    case _:
                        # default echo back the message
                        writer.write(data)
                        await writer.drain()

        except asyncio.CancelledError:
            print(f"Connection with {addr} cancelled.")
        finally:
            self.clients.remove(writer)
            writer.close()
            await writer.wait_closed()


    async def send_message(self):
        # asynchronously send messages to all connected clients
        try:
            while True:
                message = await asyncio.to_thread(input, "Enter message to send to clients: ")
                if message:
                    for client in self.clients:
                        client.write(message.encode("utf-8"))
                        await client.drain()
                    print("Message sent to all clients.")
        except asyncio.CancelledError:
            print("Server message sender task cancelled.")


    async def run(self):
        # start the server
        server = await asyncio.start_server(self.handle_client, self.server_ip, self.port)
        addr = server.sockets[0].getsockname()
        print(f"Server running on {addr}")

        sender_task = asyncio.create_task(self.send_message())

        async with server:
            await server.serve_forever()

        await sender_task


###############################################################################
# Main entry point for box_server

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Box server application")
    parser.add_argument('port', type=int, nargs='?', default=8000,
                        help='Port to receive connections on')
    args = parser.parse_args()

    server = AsyncTCPIPServer(args.port)
    asyncio.run(server.run())
