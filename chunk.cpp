#include "chunk.h"

Chunk::Chunk(int possition, quint16 checksum, QObject *parent) :
    QObject(parent)
{
    this->possition = possition;
    this->checksum = checksum;
}

Status Chunk::getStatus()
{
    return status;
}

void Chunk::setStatus(Status status)
{
    this->status = status;
}

quint16 Chunk::checksum()
{
    return checksum;
}

int Chunk::possition()
{
    return possition;
}
