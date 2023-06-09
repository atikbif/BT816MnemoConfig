#ifndef PLCCONFIGREADER_H
#define PLCCONFIGREADER_H

#include <QString>
#include "plcconfig.h"
#include <optional>


class PLCConfigReader
{
public:
    PLCConfigReader() = default;
    virtual std::optional<PLCConfig>  readFromFile(const QString &fName) = 0;
    virtual ~PLCConfigReader() = default;
};

#endif // PLCCONFIGREADER_H
