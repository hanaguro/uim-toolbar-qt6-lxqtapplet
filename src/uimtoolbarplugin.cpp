#include "uimtoolbarplugin.h"
#include "uimtoolbarwidget.h"
#include <QDebug>

UimToolbarPlugin::UimToolbarPlugin(const ILXQtPanelPluginStartupInfo &startupInfo)
    : ILXQtPanelPlugin(startupInfo)  // ← ここが重要
{
    qDebug() << "UimToolbarPlugin created";
    m_widget = new UimToolbarWidget();
}

UimToolbarPlugin::~UimToolbarPlugin()
{
    delete m_widget;
}

QWidget *UimToolbarPlugin::widget()
{
    return m_widget;
}

ILXQtPanelPlugin *UimToolbarApplet::instance(const ILXQtPanelPluginStartupInfo &startupInfo) const
{
    qDebug() << "UimToolbarApplet::instance() called";
    return new UimToolbarPlugin(startupInfo);
}

