#ifndef __GAME_INVITE_H__
#define __GAME_INVITE_H__

#include <string>
#include <vector>
#include <functional>
#include <boost/shared_ptr.hpp>

namespace boost
{
    namespace property_tree
    {
        template < class Key, class Data, class KeyCompare >
        class basic_ptree;
        typedef basic_ptree<std::string, std::string, std::less<std::string>> ptree;
 
        }
}

typedef boost::shared_ptr<boost::property_tree::ptree> ptree_ptr;

class GameInvite
{
private:
    static ptree_ptr create_property_tree_from_players(
        const std::vector<std::string> &player_names);

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