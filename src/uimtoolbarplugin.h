/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
#pragma once
#include <QObject>
#include <QWidget>
#include <lxqt/ilxqtpanelplugin.h>
#include <qtmetamacros.h>

class UimToolbarWidget;

// どんなウィジェットを作るかを登録
class UimToolbarPlugin : public QObject, public ILXQtPanelPlugin
{
    // Qt のメタオブジェクト機構（Meta-Object System）を有効にする
    Q_OBJECT

public:
    explicit UimToolbarPlugin(const ILXQtPanelPluginStartupInfo &startupInfo);
    ~UimToolbarPlugin() override;

    QWidget *widget() override;
    QString themeId() const override { return QStringLiteral("uim-toolbar"); }

private:
    UimToolbarWidget *m_widget = nullptr;
};

// アプレットの実態
// プラグインロード時に LXQt パネルが呼び出す関数群を提供
class UimToolbarApplet : public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT

    // 外部（LXQtなど）に対して“どんなプラグイン”か
    Q_PLUGIN_METADATA(IID "org.lxqt.ILXQtPanelPluginLibrary")

    // 内部的にどんなインターフェースを実装しているか
    Q_INTERFACES(ILXQtPanelPluginLibrary)

public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const override;
};

