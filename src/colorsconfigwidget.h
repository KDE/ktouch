#ifndef COLORSCONFIGWIDGET_H
#define COLORSCONFIGWIDGET_H

#include <QWidget>
#include "ui_colorsconfigwidget.h"

class ColorsConfigWidget : public QWidget, private Ui::ColorsConfigWidget
{
    Q_OBJECT
public:
    explicit ColorsConfigWidget(QWidget *parent = 0);

signals:

public slots:

};

#endif // COLORSCONFIGWIDGET_H
