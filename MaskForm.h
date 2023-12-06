#ifndef MASKFORM_H
#define MASKFORM_H

#include <QWidget>

//遮罩头文件
class MaskForm : public QWidget
{
    Q_OBJECT

public:
    MaskForm(QWidget *parent = 0);
    ~MaskForm();

    //注册要显示在遮罩中的窗口
    void installWidget(QWidget *widget);

    //设置遮罩颜色、透明度
    void setMaskColor(const QColor &color,float opacity);

    //设置顶层窗口
    void setTopWidget(QWidget *widget);

private:
    void init();
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget *m_topWidget;    //顶层窗口，设置遮罩颜色、透明度
    QWidget *m_widget;       //遮罩中显示的窗口
};


#endif // MASKFORM_H
