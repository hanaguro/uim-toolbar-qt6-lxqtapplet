#ifndef UIMTOOLBARWIDGET_H
#define UIMTOOLBARWIDGET_H

#pragma once
#include <QWidget>
class QLabel;
class UimHelperClient;

class UimToolbarWidget : public QWidget {
    Q_OBJECT
public:
    explicit UimToolbarWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev) override;

private slots:
    void onStateChanged(bool on, const QString &text);
    void onError(const QString &msg);

private:
    QLabel *m_label;
    UimHelperClient *m_client;
    bool m_on = false;
};

#endif
