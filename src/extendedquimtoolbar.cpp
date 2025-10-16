/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
#include "extendedquimtoolbar.h"
#include <QDebug>

ExtendedQUimHelperToolbar::ExtendedQUimHelperToolbar(QWidget *parent, bool isApplet)
    : QUimHelperToolbar(parent, isApplet)
{
}

void ExtendedQUimHelperToolbar::resizeEvent(QResizeEvent *event)
{
    QUimHelperToolbar::resizeEvent(event);  // 元の動作も維持
    emit resized(event->size());            // 新しいシグナルを発行
    qDebug() << "[ExtendedQUimHelperToolbar] resized:" << event->size();
}

