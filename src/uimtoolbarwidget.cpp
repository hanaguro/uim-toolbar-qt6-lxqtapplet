/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
#include "uimtoolbarwidget.h"
#include "extendedquimtoolbar.h"

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
    layout->setContentsMargins(0, 0, 0, 0);  // レイアウト全体の内側の余白（マージン）を設定する(枠との間の余白)
    layout->setSpacing(0);  // ウィジェット同士の間隔を制御する

    setMinimumSize(60, 24);  // どんなに小さくリサイズされても最低限確保したいサイズ（最小サイズ） を設定する
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);  // どの方向にどのくらい伸縮できるかを設定する

    // --- uim-helper toolbar 機能（未使用でもインスタンス必要） ---
#ifdef DEBUG_BUILD
    qDebug() << "Creating QUimHelperToolbar...";
#endif
    m_helperToolbar = new ExtendedQUimHelperToolbar(this, true);

    // 左右に空白スペーサを追加
    layout->addStretch(1);
    layout->addWidget(m_helperToolbar);
    layout->addStretch(1);
}

QSize UimToolbarWidget::sizeHint() const
{
    return m_helperToolbar ? m_helperToolbar->sizeHint()
                           : QSize(60, 24);
}

void UimToolbarWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}
