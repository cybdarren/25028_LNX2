###############################################################################
# File: box_client.py
# This is an example of a simple TCPIP client writte in Python
# A client (or terminal) is designed to connect to a server and pass
# requests to lock or unlock a box. The server controls the box
# and this terminal is a remote controller for it.
#

import asyncio
import argparse

###############################################################################
# Class to handle Asynchronous TCPIP Client operations
class AsyncTCPIPClient:
    def __init__(self, host: str, port: int):
        self.host = host
        self.port = port
        self.writer = None
        self.sender_task = None
        self.connection_closed = asyncio.Event()

    async def handle_server_messages(self, reader: asyncio.StreamReader):
        # continuously read messages from the server.
        try:
            while True:
                server_message = await reader.read(1024)
                if not server_message:
                    print("Server closed the connection.")
                    self.connection_closed.set()
                    break
                server_message = server_message.decode("utf-8").strip()
                print(f"Received: {server_message}")

                match server_message.lower():
                    case 'close':
                        print("Server requested to close the connection.")
                        self.connection_closed.set()
                        if self.sender_task:
                            self.sender_task.cancel()
                        if self.writer and not self.writer.is_closing():
                            self.writer.close()
                            await self.writer.wait_closed()
                        break

        except asyncio.CancelledError:
            print("Listener task cancelled.")

    async def send_messages(self, writer: asyncio.StreamWriter):
        # send messages to the server
        try:
            while not self.connection_closed.is_set():
                message = await asyncio.to_thread(input, "Enter message: ")
                if self.connection_closed.is_set():
                    break
                writer.write(message.encode("utf-8"))
                await writer.drain()
                if message.lower() == "close":
                    print("Closing connection...")
                    writer.close()
                    await writer.wait_closed()
                    break
        except asyncio.CancelledError:
            print("Sender task cancelled.")
        except (BrokenPipeError, ConnectionResetError):
            print("Connection lost. Stopping message sending.")

    async def run(self):
        # handles the connection, sending and receiving
        print(f"Connecting to {self.host}:{self.port}...")
        reader, writer = await asyncio.open_connection(self.host, self.port)
        self.writer = writer

        listener_task = asyncio.create_task(self.handle_server_messages(reader))
        self.sender_task = asyncio.create_task(self.send_messages(writer))

        await asyncio.gather(listener_task, self.sender_task, return_exceptions=True)
        print("Connection closed.")

###############################################################################
# main entry point for the box client
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Box client application")
    parser.add_argument('server', type=str, nargs=1, help='Box server IP address')
    parser.add_argument('port', type=int, nargs='?', default=8000,
                        help='Port to receive connections on')
    args = parser.parse_args()

    client = AsyncTCPIPClient('.'.join(args.server), args.port)
#    client = AsyncTCPIPClient("192.168.2.200", 8000)
    asyncio.run(client.run())
