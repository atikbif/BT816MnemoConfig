#include "lampimages.h"

const std::vector<QString> LampImages::imageNames = {
    ":/images/lamps/lamp1.png",
    ":/images/lamps/lamp2.png",
    ":/images/lamps/lamp3.png",
    ":/images/lamps/lamp4.png",
    ":/images/lamps/lamp5.png",
};


QString LampImages::getImage(int num)
{
    if(num>=0 && num<getQuantity()) {
        return imageNames.at(num);
    }
    return imageNames.at(0);
}
