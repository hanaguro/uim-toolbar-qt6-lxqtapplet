#pragma once
#include <QObject>
#include <QLocalSocket>
#include <QTimer>
#include <QDebug>

class UimHelperClient : public QObject
{
    Q_OBJECT

public:
    explicit UimHelperClient(QObject *parent = nullptr);
    ~UimHelperClient();

    void connectToHelper();
    void sendCommand(const QString &cmd);
    void requestState();
    void requestToggle();
    void parseLine(const QString &line);

signals:
    void stateChanged(const QString &state);  // e.g. "A", "あ"
    void connected();
    void disconnected();

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    QLocalSocket *m_socket;
    QString m_buffer;

    QString parseMessage(const QString &msg);
};

