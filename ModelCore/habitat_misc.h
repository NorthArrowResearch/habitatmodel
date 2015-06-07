#ifndef HABITAT_MISC_H
#define HABITAT_MISC_H

#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace Habitat {

/**
 * @brief
 *
 */
class BadConversion : public std::runtime_error {
public:
    /**
     * @brief
     *
     * @param s
     */
    BadConversion(const std::string& s)	: std::runtime_error(s)	{ }
};


/**
 * @brief
 *
 * @param s
 * @param i
 * @return bool
 */
inline bool string2int(const std::string& s, int& i) {
    std::istringstream stream(s);
    if (stream >> i)
        return true;
    else
        return false;
}

/**
 * @brief
 *
 * @param s
 * @return int
 */
inline int convertToInt(const std::string& s) {
    std::istringstream i(s);
    int x;
    if (!(i >> x))
        throw BadConversion("convertToInt(\"" + s + "\")");
    return x;
}

/**
 * @brief
 *
 * @param s
 * @param i
 * @return bool
 */
inline bool string2double(const std::string& s, double& i) {
    std::istringstream stream(s);
    if (stream >> i)
        return true;
    else
        return false;
}

/**
 * @brief
 *
 * @param s
 * @return double
 */
inline double convertToDouble(const std::string& s) {
    std::istringstream i(s);
    double x;
    if (!(i >> x))
        throw BadConversion("convertToDouble(\"" + s + "\")");
    return x;
}

/*
     * Delimits on any of the characters in delimiters.
     * From http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
     */
/**
 * @brief
 *
 * @param str
 * @param tokens
 * @param delimiters
 */
inline void tokenize(const std::string& str, std::vector<std::string>& tokens,
                     const std::string& delimiters=" ") {
    tokens.clear(); // I added this
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}



}


#endif // HABITAT_MISC_H
