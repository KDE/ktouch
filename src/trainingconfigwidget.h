#ifndef TRAININGCONFIGWIDGET_H
#define TRAININGCONFIGWIDGET_H

#include <QWidget>
#include "ui_trainingconfigwidget.h"

class TrainingConfigWidget : public QWidget, private Ui::TrainingConfigWidget
{
    Q_OBJECT
public:
    explicit TrainingConfigWidget(QWidget *parent = 0);

signals:

public slots:

};

#endif // TRAININGCONFIGWIDGET_H
