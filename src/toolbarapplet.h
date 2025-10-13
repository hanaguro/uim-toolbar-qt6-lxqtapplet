#ifndef TOOLBARAPPLET_H
#define TOOLBARAPPLET_H

#include <QObject>
#include <QPixmap>
#include <QIcon>
#include <KStatusNotifierItem>

class ToolbarApplet : public QObject
{
    Q_OBJECT
public:
    explicit ToolbarApplet(QObject *parent = nullptr);

private:
    void updateIcon();
    KStatusNotifierItem *sni;
};

#endif // TOOLBARAPPLET_H

