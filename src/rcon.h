#ifndef RCON_H
#define RCON_H

#include "common.h"

class Rcon : public QObject
{
    Q_OBJECT

public:
    struct PlayerInfo
    {
        QString name;
    };

    enum Error
    {
        Banned,
        InvalidVersion,
        InvalidPassword,
        TimedOut
    };

    enum State
    {
        Disconnected,
        Connecting,
        SendingPassword,
        LoggedIn,
        Canceled
    };

public:
    explicit Rcon(QObject *parent = 0);
    ~Rcon();

    QHostAddress getAddress() { return address; }
    quint16 getPort() { return port; }
    QString getPassword() { return password; }

    int getState() { return state; }

public slots:
    void connectToHost(QHostAddress address, quint16 port, QString password);
    void reconnect();
    void disconnect();
    void sendCommand(QString command);
    void ConnectTimer();
    bool checkCancelEvent();
    void forceloggedin();

signals:
    void error(int e);
    void stateChanged(int state);
    void message(QString message);
    void mapChanged(QString mapname);
    void adminCountChanged(int admincount);
    void playerCountChanged(int playercount);
    void playerListChanged(QList<PlayerInfo> players);
    void servernamereceived(QString name);
    void serverlog(QString message);

private:
    void abort();
    void setState(int state) { this->state = state; emit stateChanged(state); }
    void processPacket(QBuffer &packet);
    void processUpdate(QBuffer &packet);
    void sendPassword(QByteArray salt);

private slots:
    void readPendingPackets();
    void sendKeepAlive();

private:
    int state;

    QHostAddress address;
    quint16 port;
    QString password;

    QUdpSocket socket;
    QTimer     keepaliveTimer;
};


#endif // RCON_H
