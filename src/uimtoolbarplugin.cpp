/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
#include "uimtoolbarplugin.h"
#include "uimtoolbarwidget.h"
#include <QDebug>

UimToolbarPlugin::UimToolbarPlugin(const ILXQtPanelPluginStartupInfo &startupInfo)
    : ILXQtPanelPlugin(startupInfo)	// 基底クラスのコンストラクタを呼び出す。
					// これがないと基底クラスのデフォルトコンストラクタが呼び出される
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

