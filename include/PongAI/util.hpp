#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

std::string create_random_string(const int &length,
                                 const std::string &prefix = "",
                                 const std::string &postfix = "");

#endif // __UTIL_H__