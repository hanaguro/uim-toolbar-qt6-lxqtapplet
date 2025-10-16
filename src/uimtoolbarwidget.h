/* vim: set tabstop=4 shiftwidth=4 softtabstop=4 expandtab: */
#include <QWidget>
#include <QLabel>

// 前方宣言
// クラスの存在だけをコンパイルに伝える。
// ヘッダファイルをインクルードして、そのファイルにある全てのクラスを読み込まなくても良い。
// ポインタや参照だけを使う場合は、中身を知らなくても良いので使える。
class ExtendedQUimHelperToolbar;

class UimToolbarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UimToolbarWidget(QWidget *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    ExtendedQUimHelperToolbar *m_helperToolbar;  // ← 追加
};

