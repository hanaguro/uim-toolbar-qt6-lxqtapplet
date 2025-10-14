#pragma once

#include <QWidget>
#include <QLabel>

class UimHelperClient;

class UimToolbarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UimToolbarWidget(QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *m_label = nullptr;
    UimHelperClient *m_client = nullptr;
};

