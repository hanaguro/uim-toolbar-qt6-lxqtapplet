#include "uimtoolbarwidget.h"
#include "uimhelperclient.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

UimToolbarWidget::UimToolbarWidget(QWidget *parent)
    : QWidget(parent)
{
    // レイアウト設定
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 0, 4, 0);
    layout->setSpacing(0);

    // ラベル作成（初期値 "A"）
    m_label = new QLabel(QStringLiteral("A"), this);
    QFont font = m_label->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.3); // 少し大きく
    m_label->setFont(font);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setMinimumWidth(60);
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_label->setTextFormat(Qt::PlainText);
    m_label->setTextInteractionFlags(Qt::NoTextInteraction); // 編集や選択を無効化

    layout->addWidget(m_label);
    setLayout(layout);

    // サイズヒント調整
    setMinimumSize(60, 24);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // uim-helper クライアント接続
    m_client = new UimHelperClient(this);
    connect(m_client, &UimHelperClient::imStateChanged, this, [this](bool active) {
        m_label->setText(active ? QStringLiteral("あ") : QStringLiteral("A"));
    });
}

QSize UimToolbarWidget::sizeHint() const
{
    // 横長のヒントを返す
    return QSize(80, 24);
}

void UimToolbarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_client) {
        m_client->requestToggle();
    }
    QWidget::mousePressEvent(event);
}

