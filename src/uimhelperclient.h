#pragma once
#include <QObject>
#include <QLocalSocket>
#include <QTimer>

class UimHelperClient : public QObject
{
    Q_OBJECT
public:
    explicit UimHelperClient(QObject *parent = nullptr);
    void connectToHelper();
    void requestStatus();
    void requestToggle();

signals:
    void imStateChanged(bool active);

private slots:
    void onReadyRead();
    void onReconnectTimeout();

private:
    void parseLine(const QString &line);

    QLocalSocket *m_socket = nullptr;
    QTimer *m_reconnectTimer = nullptr;
    QByteArray m_buffer;

    void parseBuffer();
    void processPropList(const QStringList &block);

    QStringList m_currentBlock;
    bool m_inPropList = false;
};

