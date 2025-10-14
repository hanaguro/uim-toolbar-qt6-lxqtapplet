#include "uimhelperclient.h"
#include <QFileInfo>
#include <unistd.h>
#include <QTimer>
#include <QDebug>

UimHelperClient::UimHelperClient(QObject *parent)
    : QObject(parent), m_socket(new QLocalSocket(this))
{
    connect(m_socket, &QLocalSocket::readyRead, this, &UimHelperClient::onReadyRead);
    connect(m_socket, &QLocalSocket::connected, this, &UimHelperClient::onConnected);
    connect(m_socket, &QLocalSocket::disconnected, this, &UimHelperClient::onDisconnected);
}

UimHelperClient::~UimHelperClient()
{
    if (m_socket->isOpen())
        m_socket->close();
}

void UimHelperClient::connectToHelper()
{
    if (m_socket->isOpen())
        return;

    QString socketPath = QStringLiteral("/run/user/%1/uim/socket/uim-helper").arg(getuid());
    if (!QFileInfo::exists(socketPath)) {
        qWarning() << "uim-helper socket not found:" << socketPath;
        return;
    }

#ifdef DEBUG_BUILD
    qDebug() << "Connecting to" << socketPath;
#endif
    m_socket->connectToServer(socketPath);
}

void UimHelperClient::onConnected()
{
#ifdef DEBUG_BUILD
    qDebug() << "Connected to uim-helper.";
#endif
    emit connected();

    // 現在の入力状態を要求
    requestState();
}

void UimHelperClient::onDisconnected()
{
#ifdef DEBUG_BUILD
    qWarning() << "Disconnected from uim-helper.";
#endif
    emit disconnected();

    // 自動再接続
    QTimer::singleShot(3000, this, &UimHelperClient::connectToHelper);
}

void UimHelperClient::sendCommand(const QString &cmd)
{
    if (!m_socket->isOpen()) {
        qWarning() << "Not connected; cannot send command.";
        return;
    }

#ifdef DEBUG_BUILD
    QString line = cmd + "\n";
    m_socket->write(line.toUtf8());
    m_socket->flush();
    qDebug() << "Sent command:" << cmd;
#endif
}

void UimHelperClient::requestState()
{
    sendCommand(QStringLiteral("-r getstate"));
}

void UimHelperClient::onReadyRead()
{
    m_buffer.append(QString::fromUtf8(m_socket->readAll()));

    // uim-helperは改行区切りで複数メッセージを送ってくる
    while (m_buffer.contains("\n")) {
        int idx = m_buffer.indexOf('\n');
        QString line = m_buffer.left(idx).trimmed();
        m_buffer.remove(0, idx + 1);

        if (!line.isEmpty()) {
            QString parsed = parseMessage(line);
            if (!parsed.isEmpty())
                emit stateChanged(parsed);
        }
    }
}

QString UimHelperClient::parseMessage(const QString &msg)
{
#ifdef DEBUG_BUILD
    // デバッグ用出力
    qDebug() << "UimHelperClient::parseMessage:" << msg;
#endif

    if (msg.startsWith("focus_in")) {
#ifdef DEBUG_BUILD
        qDebug() << "Focus in detected → 状態要求";
#endif
        requestState();  // 状態を要求するのみ
        return QString(); // ここで"A"は返さない
    }
    else if (msg.startsWith("focus_out")) {
#ifdef DEBUG_BUILD
        qDebug() << "Focus out detected";
#endif
        return QString();  // 状態クリア不要
    }
    else if (msg.startsWith("state") || msg.startsWith("im_change")) {
        // 例: "state ja_hiragana"
        QStringList parts = msg.split(' ');
        if (parts.size() >= 2) {
            QString mode = parts[1];
            if (mode.contains("hiragana") || mode.contains("skk"))
                return QStringLiteral("あ");
            else
                return QStringLiteral("A");
        }
    }

#ifdef DEBUG_BUILD
    qDebug() << "Unhandled uim message:" << msg;
#endif
    return QString();
}

void UimHelperClient::requestToggle()
{
    if (!m_socket || m_socket->state() != QLocalSocket::ConnectedState) {
        qWarning() << "uim-helper not connected, cannot toggle IM";
        return;
    }

    // 実際の切り替えコマンド送信（SKK前提、必要なら他IMに応じて変更）
    sendCommand(QStringLiteral("-r action_skk_toggle"));
}

void UimHelperClient::parseLine(const QString &line)
{
    // 旧コード互換 — 現状は未使用
    QString parsed = parseMessage(line);
    if (!parsed.isEmpty())
        emit stateChanged(parsed);
}

