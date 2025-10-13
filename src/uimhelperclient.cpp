#include <unistd.h>  // getuid()
#include <sys/types.h>
#include "uimhelperclient.h"
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QProcessEnvironment>

UimHelperClient::UimHelperClient(QObject *parent) : QObject(parent) {
    connect(&m_sock, &QLocalSocket::connected, this, &UimHelperClient::onConnected);
    connect(&m_sock, &QLocalSocket::readyRead, this, &UimHelperClient::onReadyRead);
    connect(&m_sock, &QLocalSocket::errorOccurred, this, &UimHelperClient::onErrorOccurred);

    m_reconnectTimer.setInterval(1500);
    m_reconnectTimer.setSingleShot(false);
    connect(&m_reconnectTimer, &QTimer::timeout, this, &UimHelperClient::tryConnect);

    tryConnect();
}

QString UimHelperClient::detectSocketPath() const
{
    // 1. 環境変数 UIM_HELPER_SOCKET を最優先
    const auto env = QProcessEnvironment::systemEnvironment();
    const QString envPath = env.value("UIM_HELPER_SOCKET");
    if (!envPath.isEmpty() && QFileInfo::exists(envPath))
        return envPath;

    // 2. XDG_RUNTIME_DIR (systemd/logind が使う標準位置)
    QString runtimeDir = env.value("XDG_RUNTIME_DIR");
    if (runtimeDir.isEmpty()) {
        // fallback: /run/user/<uid>
        runtimeDir = QStringLiteral("/run/user/%1").arg(getuid());
    }

    const QString candidate1 = runtimeDir + QStringLiteral("/uim/socket/uim-helper");
    if (QFileInfo::exists(candidate1))
        return candidate1;

    // 3. fallback: ~/.uim.d/helper （旧実装系）
    const QString candidate2 = QDir::homePath() + QStringLiteral("/.uim.d/helper");
    if (QFileInfo::exists(candidate2))
        return candidate2;

    // 4. fallback: /tmp/uim-helper-<uid> 形式（古い uim-helper-server）
    const QString candidate3 = QStringLiteral("/tmp/uim-helper-%1").arg(getuid());
    if (QFileInfo::exists(candidate3))
        return candidate3;

    return QString(); // 見つからなかった
}

void UimHelperClient::tryConnect() {
    if (m_sock.state() == QLocalSocket::ConnectedState ||
        m_sock.state() == QLocalSocket::ConnectingState)
        return;

    const QString path = detectSocketPath();
    if (path.isEmpty()) {
        emit errorText(QStringLiteral("uim helper socket not found"));
        m_reconnectTimer.start();
        return;
    }

    m_sock.connectToServer(path);
    if (!m_reconnectTimer.isActive())
        m_reconnectTimer.start();
}

void UimHelperClient::onConnected() {
    // 接続できたら一度状態取得
    requestState();
}

void UimHelperClient::requestState() {
    sendRequest(buildStateQuery());
}

void UimHelperClient::requestToggle() {
    sendRequest(buildToggleCommand());
    // 返答待ちで onReadyRead→parseIncoming から stateChanged が来る想定
}

void UimHelperClient::sendRequest(const QByteArray &line) {
    if (m_sock.state() == QLocalSocket::ConnectedState) {
        m_sock.write(line);
        m_sock.flush();
    }
}

QByteArray UimHelperClient::buildStateQuery() const {
    // ★ここを実際の uim helper プロトコルに合わせて入れ替える
    // 例: "im_state_get\n" や S式など
    return QByteArray("state?\n");
}

QByteArray UimHelperClient::buildToggleCommand() const {
    // ★ここを実際の uim helper プロトコルに合わせて入れ替える
    return QByteArray("toggle\n");
}

void UimHelperClient::onReadyRead() {
    m_rxBuffer += m_sock.readAll();
    // 改行区切りで処理
    int p = -1;
    while ((p = m_rxBuffer.indexOf('\n')) >= 0) {
        const QByteArray line = m_rxBuffer.left(p);
        m_rxBuffer.remove(0, p + 1);
        parseIncoming(line);
    }
}

void UimHelperClient::parseIncoming(const QByteArray &line) {
    // ★仮の超シンプルパーサ
    // 例: "state:ON:あ" / "state:OFF:A"
    if (line.startsWith("state:")) {
        // state:ON:あ
        QList<QByteArray> parts = line.split(':');
        if (parts.size() >= 3) {
            const bool on = (parts.at(1) == "ON");
            const QString mode = QString::fromUtf8(parts.at(2));
            emit stateChanged(on, mode);
        }
        return;
    }

    // 必要に応じて他のメッセージを追加
}

void UimHelperClient::onErrorOccurred(QLocalSocket::LocalSocketError) {
    emit errorText(QStringLiteral("uim helper disconnected; retry…"));
    m_sock.abort();
    m_reconnectTimer.start();
}
