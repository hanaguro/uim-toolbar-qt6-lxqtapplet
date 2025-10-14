#pragma once
#include <QObject>
#include <QWidget>
#include <lxqt/ilxqtpanelplugin.h>

class UimToolbarWidget;

class UimToolbarPlugin : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT

public:
    explicit UimToolbarPlugin(const ILXQtPanelPluginStartupInfo &startupInfo);
    ~UimToolbarPlugin() override;

    QWidget *widget() override;
    QString themeId() const override { return QStringLiteral("uim-toolbar"); }

private:
    UimToolbarWidget *m_widget = nullptr;
};

class UimToolbarApplet : public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.lxqt.ILXQtPanelPluginLibrary")
    Q_INTERFACES(ILXQtPanelPluginLibrary)

public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const override;
};

