/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
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
    layout->setContentsMargins(4, 0, 4, 0);  // レイアウト全体の内側の余白（マージン）を設定する(枠との間の余白)
    layout->setSpacing(0);  // ウィジェット同士の間隔を制御する

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
