#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

std::string create_random_string(const int &length,
                                 const std::string &prefix = "",
                                 const std::string &postfix = "");

char *get_administration_exchange_name();

char *get_game_exchange_name();

int get_random_integer(const int &min, const int &max);

#endif // __UTIL_H__
