#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

class Enums : public QObject
{
    Q_OBJECT
public:
    enum Commands {
        ComFillScreen = 1,
        ComSendText
    };

    Q_ENUM(Commands)
};

#endif // ENUMS_H
