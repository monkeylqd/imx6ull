#include "socketclient.h"
SocketClient::SocketClient(QObject *parent) : QObject(parent) {
    qDebug()<<"SocketClient create";
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &SocketClient::onConnected_slot);
    connect(m_socket, &QTcpSocket::disconnected, this, &SocketClient::onDisconnected_slot);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead_slot);
}

SocketClient::~SocketClient() {
    m_socket->disconnectFromHost();
    m_socket->deleteLater();
}


void SocketClient::connect_server(QString ip, int port)
{
    qDebug()<<"ip:"<<ip<<" port"<<port;
    m_socket->connectToHost(ip, port);
}

void SocketClient::send_to_server(const QByteArray &data)
{
    m_socket->write(data);
    m_socket->waitForBytesWritten();
}

void SocketClient::disconnect_server()
{
    m_socket->close();
}

void SocketClient::onConnected_slot()
{
    if(m_socket->state() == QAbstractSocket::ConnectedState)
    {
        emit report_socket_status(SOCKET_STATUS_CONNECT);
    }
}

void SocketClient::onDisconnected_slot()
{
    if(m_socket->state() == QAbstractSocket::UnconnectedState)
    {
        emit report_socket_status(SOCKET_STATUS_DISCONNECT);
    }
}

void SocketClient::onReadyRead_slot()
{
    QByteArray data = m_socket->readAll();
    emit rev_form_server(data);
}

