#include "toolbarapplet.h"
#include <QApplication>
#include <QPainter>
#include <QFont>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QPalette>

ToolbarApplet::ToolbarApplet(QObject *parent)
    : QObject(parent)
{
    sni = new KStatusNotifierItem(this);
    sni->setTitle(QStringLiteral("uim-toolbar-qt6-sni"));
    sni->setCategory(KStatusNotifierItem::ApplicationStatus);
    sni->setStatus(KStatusNotifierItem::Active);

    updateIcon();

    // ツールチップ
    sni->setToolTipTitle("UIM Toolbar");

    // クリック時の動作
    connect(sni, &KStatusNotifierItem::activateRequested,
            this, [this](bool, const QPoint &) {
        QMenu menu;
        menu.addAction("入力メニューを開く");
        menu.addAction("設定");
        menu.addAction("終了", []() {
            qApp->quit();
        });
        menu.exec(QCursor::pos());
    });
}

void ToolbarApplet::updateIcon()
{
    const int iconHeight = 32;  // トレイ高さに合わせる
    const int iconWidth = iconHeight * 2;  // 幅を控えめに

    QPixmap pixmap(iconWidth, iconHeight);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // テーマに基づいた文字色
    QColor fg = QApplication::palette().color(QPalette::WindowText);

    QFont font;
    font.setPixelSize(18);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(fg);

    // ▼ と ⚙ のみ描画（A/Rは削除）
    painter.drawText(8, 22, "▼");
    painter.drawText(iconWidth - 24, 22, "⚙");

    painter.end();

    // アイコン設定（Qt6 / KF6 対応）
    sni->setIconByPixmap(pixmap);
}

// ============================================================
//  UimToolbarApplet (LXQtパネルプラグイン側)
// ============================================================

UimToolbarApplet::UimToolbarApplet(const ILXQtPanelPluginStartupInfo &startupInfo)
    : QObject(startupInfo.parent)
{
#ifdef DEBUG_BUILD
    fprintf(stderr, "[uim-toolbar] UimToolbarApplet constructed\n");
    fflush(stderr);
#endif

    m_client = new UimHelperClient(this);
    connect(m_client, &UimHelperClient::imStateChanged,
            this, &UimToolbarApplet::updateState);

    m_client->connectToHelper();

    m_label = new QLabel("...", this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setMinimumWidth(40);
    m_label->setMaximumWidth(80);

    // テーマに合わせたスタイル
    QPalette pal = m_label->palette();
    pal.setColor(QPalette::WindowText,
                 QApplication::palette().color(QPalette::WindowText));
    m_label->setPalette(pal);
    m_label->setFont(QFont("Sans Serif", 14, QFont::Bold));

    setWidget(m_label);

#ifdef DEBUG_BUILD
    fprintf(stderr, "[uim-toolbar] connectToHelper() invoked\n");
    fflush(stderr);
#endif
}

