#ifndef CHUNK_H
#define CHUNK_H

#include <QObject>

class Chunk : public QObject
{
    Q_OBJECT
public:
    Chunk(int possition, quint16 checksum, QObject *parent = 0);

    enum Status {
        UNKNOWN,
        OK,
        CORRUPTED,
        MISSING
    };

    Status getStatus();
    void setStatus(Status status);
    quint16 checksum();
    int possition();

protected:
    quint16 cs;
    int pos;
    Status status = UNKNOWN;

    
signals:
    
public slots:
    
};

#endif // CHUNK_H
