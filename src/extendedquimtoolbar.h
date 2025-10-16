/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
#pragma once
#include "common-quimhelpertoolbar.h"
#include <QResizeEvent>

class ExtendedQUimHelperToolbar : public QUimHelperToolbar
{
    Q_OBJECT
public:
    explicit ExtendedQUimHelperToolbar(QWidget *parent = nullptr, bool isApplet = false);

signals:
    void resized(const QSize &newSize);

protected:
    void resizeEvent(QResizeEvent *event) override;
};

