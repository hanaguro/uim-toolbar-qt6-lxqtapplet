#include "toolbarapplet.h"
#include <QApplication>
#include <QPainter>
#include <QFont>
#include <QMenu>
#include <QAction>
#include <QDebug>

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
        menu.addAction("終了", [](){
            qApp->quit();
        });
        menu.exec(QCursor::pos());
    });
}

void ToolbarApplet::updateIcon()
{
    const int iconHeight = 32;  // トレイ高さに合わせる
    const int iconWidth = iconHeight * 4;  // 横長4倍

    QPixmap pixmap(iconWidth, iconHeight);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font;
    font.setPixelSize(18);
    painter.setFont(font);
    painter.setPen(Qt::black);

    // 簡易枠線（デバッグ用）
    painter.drawRect(0, 0, iconWidth - 1, iconHeight - 1);

    // ▼AＲ⚙配置
    painter.drawText(8, 22, "▼");
    painter.drawText(40, 22, "A");
    painter.drawText(72, 22, "R");
    painter.drawText(104, 22, "⚙");

    pixmap.fill(QColor(200, 200, 200));  // 灰色背景
    painter.setPen(Qt::black);           // 黒文字（または白文字）

    painter.end();

    // KF6 は setIconByPixmap()
    sni->setIconByPixmap(pixmap);

    // テスト用：一時的に別ウィンドウでも確認可能
    // QLabel *test = new QLabel;
    // test->setPixmap(pixmap);
    // test->show();
}

UimToolbarApplet::UimToolbarApplet(const ILXQtPanelPluginStartupInfo &startupInfo)
    : QObject(startupInfo.parent)
{
    fprintf(stderr, "[uim-toolbar] UimToolbarApplet constructed\n");
    fflush(stderr);

    m_client = new UimHelperClient(this);
    connect(m_client, &UimHelperClient::imStateChanged, this, &UimToolbarApplet::updateState);

    m_client->connectToHelper();  // 👈 確実に呼ぶ

    m_label = new QLabel("...", this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setMinimumWidth(40);
    m_label->setMaximumWidth(80);

    setWidget(m_label);

    fprintf(stderr, "[uim-toolbar] connectToHelper() invoked\n");
    fflush(stderr);
}
