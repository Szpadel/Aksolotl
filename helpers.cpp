#include "helpers.h"

QString Helpers::humanReadableSize(quint64 size, int prec)
{
    float hsize = size;
    QString unit = "B";
    if(hsize >= 1024) {
        unit = "KiB";
        hsize /= 1024;

        if(hsize >= 1024) {
            unit = "MiB";
            hsize /= 1024;

            if(hsize >= 1024) {
                unit = "GiB";
                hsize /= 1024;

                if(hsize >= 1024) {
                    unit = "TiB";
                    hsize /= 1024;
                }
            }
        }
    }
    return QString::number(hsize, 'f', prec) + unit;
}

const QRegExp Helpers::digitalOnlyRegExp()
{
    return digitOnlyRegExp;
}

QRegExp Helpers::digitOnlyRegExp = QRegExp("[1-9][0-9]\\d{0,15}");
