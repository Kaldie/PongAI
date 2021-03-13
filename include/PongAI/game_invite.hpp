#ifndef __GAME_INVITE_H__
#define __GAME_INVITE_H__

#include <string>
#include <vector>

class GameInvite
{
public:
    std::string game_id;
    int number_of_players;
    std::vector<std::string> player_names;

    GameInvite() {}
    GameInvite(const std::string &game_id, const int &number_of_players);

    static std::string to_json(const GameInvite &);
    static GameInvite from_json(const std::string &);
};

#endif // __GAME_INVITE_H__