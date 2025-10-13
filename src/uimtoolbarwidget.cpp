#include "uimtoolbarwidget.h"
#include "uimhelperclient.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

UimToolbarWidget::UimToolbarWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_client(new UimHelperClient(this))
{
    auto *lay = new QHBoxLayout(this);
    lay->setContentsMargins(6, 0, 6, 0);
    m_label->setText(QStringLiteral("…"));
    lay->addWidget(m_label);

    connect(m_client, &UimHelperClient::stateChanged,
            this, &UimToolbarWidget::onStateChanged);
    connect(m_client, &UimHelperClient::errorText,
            this, &UimToolbarWidget::onError);

    // 起動時に一度問い合わせ
    m_client->requestState();
}

void UimToolbarWidget::mousePressEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::LeftButton) {
        m_client->requestToggle();
    }
    QWidget::mousePressEvent(ev);
}

void UimToolbarWidget::onStateChanged(bool on, const QString &text) {
    m_on = on;
    // “あ/A” は helper 側から来た表示文字列を優先
    m_label->setText(text.isEmpty() ? (on ? QStringLiteral("あ") : QStringLiteral("A")) : text);
    setToolTip(on ? QStringLiteral("uim: ON") : QStringLiteral("uim: OFF"));
}

void UimToolbarWidget::onError(const QString &msg) {
    setToolTip(msg);
}

