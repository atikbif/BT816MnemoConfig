#ifndef JSONPLCCONFIGREADER_H
#define JSONPLCCONFIGREADER_H

#include "plcconfigreader.h"

class JSONPLCConfigReader : public PLCConfigReader
{
public:
    JSONPLCConfigReader();

    // PLCConfigReader interface
public:
    std::optional<PLCConfig> readFromFile(const QString &fName);
};

#endif // JSONPLCCONFIGREADER_H
