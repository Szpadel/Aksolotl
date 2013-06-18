#include "chunk.h"

Chunk::Chunk(int possition, quint16 checksum, QObject *parent) :
    QObject(parent)
{
    this->pos = possition;
    this->cs = checksum;
}

Chunk::Status Chunk::getStatus()
{
    return status;
}

void Chunk::setStatus(Status status)
{
    this->status = status;
}

quint16 Chunk::checksum()
{
    return cs;
}

int Chunk::possition()
{
    return pos;
}
