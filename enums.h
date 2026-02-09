#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

class Enums : public QObject
{
    Q_OBJECT
public:
    enum Commands {
        ComFillScreen = 1,
        ComSendText,
        ComAnswer = 16
    };

    Q_ENUM(Commands)

    enum Messages {
        Info = 1,
        Error,
        Send,
        Recieve
    };

    Q_ENUM(Messages)
};

#endif // ENUMS_H
