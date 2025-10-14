#include "uimtoolbarwidget.h"
#include "uimhelperclient.h"
#include "common-quimhelpertoolbar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QPalette>
#include <QApplication>

UimToolbarWidget::UimToolbarWidget(QWidget *parent)
    : QWidget(parent)
{
#ifdef DEBUG_BUILD
    qDebug() << "UimToolbarWidget constructed";
#endif

    // --- UIセットアップ ---
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 0, 4, 0);
    layout->setSpacing(0);

    // 初期状態は空文字（"..."は不要）
    m_label = new QLabel(QString(), this);

    QFont font = m_label->font();
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() * 1.3);
    m_label->setFont(font);

    // LXQtテーマに合わせた文字色
    QPalette pal = m_label->palette();
    pal.setColor(QPalette::WindowText, QApplication::palette().color(QPalette::WindowText));
    m_label->setPalette(pal);

    m_label->setAlignment(Qt::AlignCenter);
    m_label->setMinimumWidth(60);
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_label->setTextFormat(Qt::PlainText);
    m_label->setTextInteractionFlags(Qt::NoTextInteraction);
    layout->addWidget(m_label);
    setLayout(layout);

    setMinimumSize(60, 24);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // --- uim-helper クライアント初期化 ---
#ifdef DEBUG_BUILD
    qDebug() << "Creating UimHelperClient...";
#endif
    m_client = new UimHelperClient(this);

    // --- uim-helper toolbar 機能（未使用でもインスタンス必要） ---
#ifdef DEBUG_BUILD
    qDebug() << "Creating QUimHelperToolbar...";
#endif
    m_helperToolbar = new QUimHelperToolbar(this, true);

    // --- 状態更新シグナル接続 ---
    connect(m_client, &UimHelperClient::stateChanged, this,
            [this](const QString &state) {
#ifdef DEBUG_BUILD
                qDebug() << "uim stateChanged received:" << state;
#endif
                if (state == "あ") {
                    m_label->setText(QStringLiteral("あ"));
                } else if (state == "A") {
                    m_label->setText(QStringLiteral("A"));
                } else {
                    m_label->clear();  // 空に戻す
                }
            });

    // --- uim-helper 接続 ---
#ifdef DEBUG_BUILD
    qDebug() << "Connecting to uim-helper...";
#endif
    m_client->connectToHelper();
#ifdef DEBUG_BUILD
    qDebug() << "connectToHelper() called.";
#endif
}

QSize UimToolbarWidget::sizeHint() const
{
    return QSize(80, 24);
}

void UimToolbarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_client) {
        m_client->requestToggle();
    }
    QWidget::mousePressEvent(event);
}
