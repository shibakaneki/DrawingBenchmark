#include "SSlider.h"

SSlider::SSlider(Qt::Orientation orientation, QWidget *parent, const char *name):QSlider(orientation, parent)
{
    SETUP_STYLESHEET
    setObjectName(name);
}

SSlider::~SSlider()
{

}
