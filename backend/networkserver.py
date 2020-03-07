from typing import List
from tornado import websocket

class NetworkServer:
    def __init__(self) -> None:
        self.sockets: List[websocket.WebSocketHandler] = []

    def add_socket(self, socket: websocket.WebSocketHandler) -> None:
        self.sockets += [socket]
    
    def remove_socket(self, socket: websocket.WebSocketHandler) -> None:
        self.sockets.remove(socket)
