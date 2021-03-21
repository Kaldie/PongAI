#include <PongAI/default_include.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <string> 

std::string create_random_string(const int& length, const std::string& prefix, const std::string& postfix)  {
    std::string random_string = prefix;
    std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890");
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    for (int i = 0; i < 16; ++i)
    {
        random_string += chars[index_dist(rng)];
    }

    random_string += postfix;
    return random_string;
}

char *get_administration_exchange_name()
{
    return getenv("ADMINISTRATION_EXECHANGE_NAME");
}

char *get_game_exchange_name()
{
    return getenv("GAME_EXECHANGE_NAME");
}