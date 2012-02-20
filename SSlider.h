#ifndef SSLIDER_H
#define SSLIDER_H

#include <QSlider>
#include "SGlobals.h"

class SSlider : public QSlider
{
    Q_OBJECT
public:
    SSlider(Qt::Orientation orientation, QWidget* parent=0, const char* name="SSlider");
    ~SSlider();
};

#endif // SSLIDER_H
