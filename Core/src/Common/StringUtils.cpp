#include <ChibiEngine/Common/StringUtils.h>
#include <sstream>
#include <iomanip>

using namespace std;

std::vector<float> game::extractFloats(const std::string& _source, const std::string& delimiter){
    string source = _source;
    std::vector<float> result;
    if(_source.size()==0)
        return result;

    size_t pos = 0;
    std::string token;
    while ((pos = source.find(delimiter)) != std::string::npos) {
        token = source.substr(0, pos);
        result.push_back(toFloat(token));
        source.erase(0, pos + delimiter.length());
    }
    token = source.substr(0, pos);
    result.push_back(toFloat(token));
    return result;
}

std::string game::toString(const bool value){
    return value?"true":"false";
}

std::string game::toString(const int value){
    return to_string(value);
}

std::string game::toString(const size_t value){
    return to_string(value);
}

std::string game::toString(const float value, int percision){
    std::ostringstream out;
    if(percision>0){
        out << std::setprecision(percision);
    }
    out << value;
    return out.str();
}

float game::toFloat(const std::string &value){
    return static_cast<float>(atof(value.c_str()));
}

int game::toInt(const std::string &value){
    return atoi(value.c_str());
}

bool game::toBool(const std::string &value){
    return value=="true";
}