#include "NetSession.h"

void WrapAddNetSession(WrapServer::PTR server, int fd, BaseNetSession::PTR connection, int pingCheckTime)
{
    server->addSession(fd, [connection, server, pingCheckTime](TCPSession::PTR session){
        connection->setSession(server, session->getSocketID(), session->getIP());
        connection->onEnter();

        server->getService()->setPingCheckTime(session->getSocketID(), pingCheckTime);

        session->setCloseCallback([connection](TCPSession::PTR session){
            connection->onClose();
        });

        session->setDataCallback([connection](TCPSession::PTR session, const char* buffer, int len){
            return connection->onMsg(buffer, len);
        });
    }, false);
}