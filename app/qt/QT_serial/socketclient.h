#include <QTcpSocket>
#include <QThread>

enum SOCKET_STATUS
{
    SOCKET_STATUS_CONNECT = 1,
    SOCKET_STATUS_DISCONNECT = 2,
};

class SocketClient : public QObject {
    Q_OBJECT
public:
    SocketClient(QObject *parent = nullptr);
    ~SocketClient();


signals:
    void rev_form_server(const QByteArray &data);
    void report_socket_status(int status);



public slots:
    void connect_server(QString ip, int port);
    void send_to_server(const QByteArray &data);
    void disconnect_server();
private slots:
    void onConnected_slot();
    void onDisconnected_slot();
    void onReadyRead_slot();

private:
    QTcpSocket *m_socket;
};
