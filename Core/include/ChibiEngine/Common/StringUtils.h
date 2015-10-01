#ifndef __STRING_UTIL_H_
#define __STRING_UTIL_H_

#include <vector>
#include <string>
namespace game{
    std::vector<float> extractFloats(const std::string& source, const std::string& delimiter);
    std::string toString(const float value, int percision=-1);
    std::string toString(const int value);
    std::string toString(const size_t value);
    std::string toString(const bool value);

    float toFloat(const std::string &value);
    int toInt(const std::string &value);
    bool toBool(const std::string &value);
}

#endif
