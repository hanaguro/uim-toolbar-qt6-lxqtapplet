#include "uimtoolbarplugin.h"
#include "uimtoolbarwidget.h"
#include <QDebug>

UimToolbarPlugin::UimToolbarPlugin(const ILXQtPanelPluginStartupInfo &startupInfo)
    : ILXQtPanelPlugin(startupInfo)  // ← ここが重要
{
#ifdef DEBUG_BUILD
    qDebug() << "UimToolbarPlugin created";
#endif
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
#ifdef DEBUG_BUILD
    qDebug() << "UimToolbarApplet::instance() called";
#endif
    return new UimToolbarPlugin(startupInfo);
}

