#ifndef __GAME_INVITE_H__
#define __GAME_INVITE_H__

#include <string>

class Heartbeat
{
public:
    std::string name;
    std::string entity_type;
    bool is_request;

    Heartbeat() {}
    Heartbeat(const std::string &name, const std::string& entity_type, const bool is_request = false);

    static std::string to_json(const Heartbeat &);
    static Heartbeat from_json(const std::string &);
};

#endif // __GAME_INVITE_H__