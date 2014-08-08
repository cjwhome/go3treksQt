#ifndef CARBONDATAWIDGET_H
#define CARBONDATAWIDGET_H

#include <QWidget>

namespace Ui {
class CarbonDataWidget;
}

class CarbonDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CarbonDataWidget(QWidget *parent = 0);
    ~CarbonDataWidget();

private:
    Ui::CarbonDataWidget *ui;
};

#endif // CARBONDATAWIDGET_H
