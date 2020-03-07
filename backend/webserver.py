import os
import logging
import asyncio
from typing import Optional, Callable, Awaitable
import tornado
from tornado import httpserver, ioloop, websocket  # pylint: disable=W0611

class WebServer(tornado.web.Application):
    """
    A simple web server that runs in a different thread. You can connect gets/puts/ posts or websockets to it
    """

    def __init__(self, port: int, handlers: tornado.routing._RuleList) -> None:
        self.port = port
        self.listen_connection: Optional[httpserver.HTTPServer] = None
        settings = dict(
            cookie_secret="__TODO:_GENERATE_YOUR_OWN_RANDOM_VALUE_HERE__",
            template_path=os.path.join(os.path.dirname(__file__), ""),
            static_path=os.path.join(os.path.dirname(__file__), "html"),
            xsrf_cookies=False
        )
        super(WebServer, self).__init__(handlers=handlers, settings=settings)

    def run(self, callback: Optional[Callable[[], Awaitable[None]]] = None) -> None:
        """
        This function runs in a different thread. Warning, the handlers you connect
        To this webserver are also running in this thread
        """
        asyncio.set_event_loop(asyncio.new_event_loop())
        logging.info('Listening to port %d', self.port)
        self.listen_connection = self.listen(self.port)
        if callback:   # pragma: no cover
            loop = asyncio.get_event_loop()
            loop.create_task(callback())
        ioloop.IOLoop.instance().start()
        logging.info('FINISHED')

    def shutdown(self) -> None:
        if self.listen_connection is not None:   # pragma: no cover
            self.listen_connection.stop()
        ioloop.IOLoop.instance().stop()
