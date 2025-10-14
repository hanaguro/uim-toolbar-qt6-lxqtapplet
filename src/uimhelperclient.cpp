#include "uimhelperclient.h"
#include <QDebug>
#include <QFileInfo>
#include <unistd.h>

UimHelperClient::UimHelperClient(QObject *parent)
    : QObject(parent)
{
    fprintf(stderr, "=== uim-toolbar-qt6-lxqtapplet loaded ===\n");
    fflush(stderr);

    m_socket = new QLocalSocket(this);
    connect(m_socket, &QLocalSocket::readyRead, this, &UimHelperClient::onReadyRead);
    connect(m_socket, &QLocalSocket::disconnected, this, [this]() {
        qWarning() << "Disconnected from uim-helper, will retry...";
        m_reconnectTimer->start(3000);
    });

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &UimHelperClient::onReconnectTimeout);
}

void UimHelperClient::connectToHelper()
{
    const QString path = QStringLiteral("/run/user/%1/uim/socket/uim-helper").arg(getuid());

    if (!QFileInfo::exists(path)) {
        qWarning() << "uim-helper socket not found:" << path;
        m_reconnectTimer->start(3000);
        return;
    }

    qDebug() << "Connecting to uim-helper socket:" << path;
    m_socket->connectToServer(path);

    if (!m_socket->waitForConnected(1000)) {
        qWarning() << "Failed to connect to uim-helper.";
        m_reconnectTimer->start(3000);
        return;
    }

    qDebug() << "Connected to uim-helper.";

    // 状態初期取得
    requestToggle();

    QTimer::singleShot(1000, this, &UimHelperClient::requestToggle);
}

void UimHelperClient::onReconnectTimeout()
{
    connectToHelper();
}

void UimHelperClient::onReadyRead()
{
    static QByteArray buffer;
    buffer += m_socket->readAll();

    int newline;
    while ((newline = buffer.indexOf('\n')) != -1) {
        QByteArray line = buffer.left(newline);
        buffer.remove(0, newline + 1);
        QString s = QString::fromUtf8(line).trimmed();

        if (s.isEmpty()) continue;

        qDebug() << "[uim-helper raw]" << s;

        if (s.startsWith("prop_list_update")) {
            m_currentBlock.clear();
            m_inPropList = true;
        }

        if (m_inPropList) {
            m_currentBlock << s;
        }

        // 次のprop_list_updateまたはfocus_in/outでブロック終了
        if (((s.startsWith("prop_list_update") && m_currentBlock.size() > 1) ||
             s.startsWith("focus_in") || s.startsWith("focus_out")) &&
            m_inPropList) {

            m_inPropList = false;
            processPropList(m_currentBlock);
            m_currentBlock.clear();
        }
    }
}


void UimHelperClient::parseBuffer()
{
    while (true) {
        int newline = m_buffer.indexOf('\n');
        if (newline < 0)
            break;

        QByteArray line = m_buffer.left(newline);
        m_buffer.remove(0, newline + 1);

        QString s = QString::fromUtf8(line).trimmed();
        if (s.isEmpty())
            continue;

        // ブロックの開始
        if (s.startsWith("prop_list_update")) {
            m_currentBlock.clear();
            m_inPropList = true;
        }

        // focus_in/out もブロック終端扱い
        if (m_inPropList)
            m_currentBlock << s;

        if ((s == "focus_in" || s == "focus_out" || s.startsWith("END")) && m_inPropList) {
            m_inPropList = false;
            processPropList(m_currentBlock);
            m_currentBlock.clear();
        }
    }
}


void UimHelperClient::processPropList(const QStringList &block)
{
    QString activeLeaf;

    for (const QString &line : block) {
        if (line.startsWith("leaf") && line.contains('\t') && line.endsWith('*')) {
            QStringList parts = line.split('\t', Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                activeLeaf = parts[1].trimmed();
                qDebug() << "[uim-helper active leaf]" << activeLeaf;
                break;
            }
        }
    }

    if (activeLeaf.isEmpty())
        return;

    bool isJapanese = activeLeaf.contains("hiragana") || activeLeaf.contains("katakana");
    bool isLatin = activeLeaf.contains("romaji") || activeLeaf.contains("latin") || activeLeaf.contains("halfwidth");

    if (isJapanese)
        emit imStateChanged(true);  // あ
    else if (isLatin)
        emit imStateChanged(false); // A

    qDebug() << "[uim-helper state]" << (isJapanese ? "あ" : "A");
}


void UimHelperClient::requestToggle()
{
    if (m_socket->state() == QLocalSocket::ConnectedState) {
        QByteArray cmd("prop_list_get\n");
        m_socket->write(cmd);
        m_socket->flush();
        qDebug() << "Requested uim-helper status.";
    }
}

