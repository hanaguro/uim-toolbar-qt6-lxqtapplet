#pragma once
#include <QObject>
#include <lxqt/ilxqtpanelplugin.h>
#include <lxqt/LXQt/lxqtplugininfo.h>
#include "uimtoolbarwidget.h"

// 実際のパネルウィジェット（ILXQtPanelPluginを継承）
class UimToolbar : public ILXQtPanelPlugin
{
public:
    explicit UimToolbar(const ILXQtPanelPluginStartupInfo &startupInfo)
        : ILXQtPanelPlugin(startupInfo)
    {
        mWidget = new UimToolbarWidget();
    }

    QWidget *widget() override { return mWidget; }
    QString themeId() const override { return QStringLiteral("default"); }
    void settingsChanged() override {}

private:
    QWidget *mWidget = nullptr;
};

// プラグインエントリ（QObject + ILXQtPanelPluginLibrary）
class UimToolbarPlugin : public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0"
                      FILE "uim-toolbar-qt6-lxqtapplet.json")
    Q_INTERFACES(ILXQtPanelPluginLibrary)

public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const override
    {
        return new UimToolbar(startupInfo);
    }
};

