#ifndef LAMPIMAGES_H
#define LAMPIMAGES_H

#include <vector>
#include <QString>

class LampImages
{
    static const std::vector<QString> imageNames;
public:
    LampImages() = default;
    static int getQuantity() {return static_cast<int>(imageNames.size());}
    static QString getImage(int num);
};

#endif // LAMPIMAGES_H
