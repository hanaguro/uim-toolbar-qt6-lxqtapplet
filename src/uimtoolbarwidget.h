#include <QWidget>
#include <QLabel>

class UimHelperClient;
class QUimHelperToolbar;

class UimToolbarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UimToolbarWidget(QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *m_label;
    UimHelperClient *m_client;
    QUimHelperToolbar *m_helperToolbar;  // ← 追加
};

