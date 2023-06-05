#ifndef SCALIMAGES_H
#define SCALIMAGES_H

#include <vector>
#include <QString>

class ScalImages
{
    static const std::vector<QString> imageNames;
public:
    ScalImages() = default;
    static int getQuantity() {return static_cast<int>(imageNames.size());}
    static QString getImage(int num);
};

#endif // SCALIMAGES_H
