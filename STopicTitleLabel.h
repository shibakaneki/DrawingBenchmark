#ifndef STOPICTITLELABEL_H
#define STOPICTITLELABEL_H

#include <QLabel>

#include "SGlobals.h"

class STopicTitleLabel : public QLabel
{
    Q_OBJECT
public:
    STopicTitleLabel(const QString& caption, QWidget* parent=0, const char* name="STopicTitleLabel");
    ~STopicTitleLabel();
};

#endif // STOPICTITLELABEL_H
