#pragma once
#include <QObject>
#include <QLocalSocket>
#include <QTimer>
#include <QDebug>
#include <QRegularExpression>

class UimHelperClient : public QObject
{
    Q_OBJECT
public:
    explicit UimHelperClient(QObject *parent = nullptr)
        : QObject(parent)
    {
        socket = new QLocalSocket(this);
        connect(socket, &QLocalSocket::readyRead, this, &UimHelperClient::onReadyRead);
    }

    void connectToHelper() {
        const QString socketPath = detectSocketPath();
        if (socketPath.isEmpty()) {
            qWarning() << "uim helper socket not found";
            return;
        }
        socket->connectToServer(socketPath);
        if (!socket->waitForConnected(1000)) {
            qWarning() << "Failed to connect to" << socketPath;
            return;
        }
        qDebug() << "Connected to uim helper at" << socketPath;

        // 状態問い合わせを送る
        sendCommand("(im-state-get)\n");
    }

    void sendCommand(const QString &cmd) {
        if (socket->state() == QLocalSocket::ConnectedState) {
            socket->write(cmd.toUtf8());
            socket->flush();
        }
    }

private slots:
    void onReadyRead() {
        QByteArray data = socket->readAll();
        buffer += data;

        // 改行で分割して解析
        while (buffer.contains('\n')) {
            const int idx = buffer.indexOf('\n');
            const QByteArray line = buffer.left(idx).trimmed();
            buffer.remove(0, idx + 1);

            if (line.startsWith("leaf")) {
                parseImListLine(line);
            } else if (line.contains("im-state")) {
                parseStateLine(line);
            } else {
                qDebug() << "helper:" << line;
            }
        }
    }

private:
    QLocalSocket *socket;
    QByteArray buffer;

    void parseImListLine(const QByteArray &line) {
        QList<QByteArray> fields = line.split('\t');
        if (fields.size() >= 3)
            qDebug() << "IM:" << fields[1] << "-" << fields[2];
    }

    void parseStateLine(const QByteArray &line) {
        if (line.contains("#t"))
            emit imStateChanged(true);
        else if (line.contains("#f"))
            emit imStateChanged(false);
    }

signals:
    void imStateChanged(bool active);

    QString detectSocketPath() const {
        const auto env = QProcessEnvironment::systemEnvironment();
        QString runtimeDir = env.value("XDG_RUNTIME_DIR");
        if (runtimeDir.isEmpty())
            runtimeDir = QStringLiteral("/run/user/%1").arg(getuid());
        const QString path = runtimeDir + QStringLiteral("/uim/socket/uim-helper");
        return QFileInfo::exists(path) ? path : QString();
    }
};
