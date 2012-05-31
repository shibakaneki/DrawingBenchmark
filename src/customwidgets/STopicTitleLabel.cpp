#include "STopicTitleLabel.h"

STopicTitleLabel::STopicTitleLabel(const QString& caption, QWidget *parent, const char *name):QLabel(caption, parent){
    SETUP_STYLESHEET
    setObjectName(name);
}

STopicTitleLabel::~STopicTitleLabel()
{

}
