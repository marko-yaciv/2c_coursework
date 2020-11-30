#ifndef EXEPT_H
#define EXEPT_H
#include <exception>
#include <QString>
class Except: public std::exception
{
    std::string message;
public:
    Except(const QString& msg);
    const char* what() const noexcept override;
};

#endif // EXEPT_H
