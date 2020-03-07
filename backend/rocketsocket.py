import logging
from typing import Optional
from tornado import websocket
from networkserver import NetworkServer

class RocketSocket(websocket.WebSocketHandler):
    def initialize(self, server: NetworkServer) -> None:  # pylint: disable=W0221
        """
        When the connection is made, give this socket a pointer to the room
        """
        self.server: NetworkServer = server  # pylint: disable=W0201

    def check_origin(self, origin: str) -> bool:
        """
        This is necessary for security reasons for CORS
        """
        return True

    def open(self, *args: str, **kwargs: str) -> None:
        """
        This function is called each time a new connection is opened
        """
        logging.info("A new connection has been created")
        self.server.add_socket(self)

    def close(self, code: Optional[int] = None, reason: Optional[str] = None) -> None:
        """
        Onclose function
        """
        logging.info("The connection is now closed")
        self.server.remove_socket(self)
