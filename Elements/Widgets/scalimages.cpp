#include "scalimages.h"

const std::vector<QString> ScalImages::imageNames = {
    ":/images/pictures/picture1.png",
    ":/images/pictures/picture2.png",
    ":/images/pictures/picture3.png",
    ":/images/pictures/picture4.png",
    ":/images/pictures/picture5.png",
    ":/images/pictures/picture6.png",
    ":/images/pictures/background.png"
};

QString ScalImages::getImage(int num)
{
    if(num>=0 && num<getQuantity()) {
        return imageNames.at(num);
    }
    return imageNames.at(0);
}
