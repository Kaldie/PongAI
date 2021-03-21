#ifndef __GAME_INVITE_H__
#define __GAME_INVITE_H__

#include <string>
#include <vector>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <PongAI/default_include.hpp>

namespace boost
{
    namespace property_tree
    {
        template <class Key, class Data, class KeyCompare>
        class basic_ptree;
        typedef basic_ptree<std::string, std::string, std::less<std::string>> ptree;

    }
}

typedef boost::shared_ptr<boost::property_tree::ptree> ptree_ptr;

typedef std::vector<std::pair<EntityType, std::string>> Participents;

class GameState
{
private:
    static ptree_ptr create_property_tree_from_participents(
        const Participents &participents);

    static Participents create_participents_from_property_tree(
        const ptree_ptr property_tree);

    std::string game_id;

public:
    int current_turn = 0;
    int number_of_players;
    Participents participents;
    enum GameStateIntend intend;
    enum FieldSize field_size;

    GameState();
    
    explicit GameState(const int &number_of_players);

    GameState(const int &number_of_players,
              const FieldSize &field_size);

    GameState(const int &number_of_players,
              const FieldSize &field_size,
              const Participents &participents,
              const GameStateIntend &intend);

    static std::string to_json(const GameState &);
    static GameState from_json(const std::string &);

    std::string get_game_id() const {return game_id;};
};

#endif // __GAME_INVITE_H__