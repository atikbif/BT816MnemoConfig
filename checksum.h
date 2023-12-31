#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QByteArray>

class CheckSum
{
private:
    static const quint16 wCRCTable[];
    static const quint8 crc8Table[];
    CheckSum();
    ~CheckSum();
public:
    static int getCRC16(const QByteArray &inpData);
    static int getCRC16(const char *inpData, int length);
    static unsigned char getCRC8(const QByteArray &inpData);
    static unsigned char getCRC8(const char *inpData, int length);
};

#endif // CHECKSUM_H
