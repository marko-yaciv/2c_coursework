#include "exept.h"

Except::Except(const QString& msg)
{
    message = msg.toStdString();
}

const char *Except::what() const noexcept
{
    return message.data();
}
