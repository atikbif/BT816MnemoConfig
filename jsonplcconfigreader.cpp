#include "jsonplcconfigreader.h"

JSONPLCConfigReader::JSONPLCConfigReader()
{

}

PLCConfig JSONPLCConfigReader::readFromFile(const QString &fName)
{
    Q_UNUSED(fName)
    PLCConfig result("");
    return result;
}
