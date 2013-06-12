#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include <QRegExp>

class Helpers
{
public:
    static QString humanReadableSize(quint64 size, int prec = 2);
    static const QRegExp digitalOnlyRegExp();

private:
    static QRegExp digitOnlyRegExp;
};

#endif // HELPERS_H
