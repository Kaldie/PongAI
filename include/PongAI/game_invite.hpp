#ifndef __GAME_INVITE_H__
#define __GAME_INVITE_H__

#include <PongAI/default_include.hpp>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <string>
#include <vector>

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

typedef std::pair<EntityType, std::string> Participent;
typedef std::vector<Participent> Participents;

class GameInvite
{
private:
    static ptree_ptr create_property_tree_from_participents(
        const Participents &participents);

    static Participents create_participents_from_property_tree(
        const boost::property_tree::ptree &property_tree);

    std::string game_id;

    virtual int get_number_of_participating_entity(const EntityType entity_type) const;

public:
    int current_turn = 0;
    int number_of_players;
    int number_of_referees;
    Participents participents;
    enum GameInviteIntend intend;
    enum FieldSize field_size;

    GameInvite();

    explicit GameInvite(const int &number_of_players);

    GameInvite(const int &number_of_players,
              const FieldSize &field_size);

    GameInvite(const int &number_of_players,
              const int &number_of_referees,
              const FieldSize &field_size,
              const Participents &participents,
              const GameInviteIntend &intend);

    static std::string to_json(const GameInvite &);
    static GameInvite from_json(const std::string &);

    std::string get_game_id() const { return game_id; };

    virtual bool requires_entity(const EntityType &entity_type) const;
    virtual bool has_enough_participents() const;
};

#endif // __GAME_INVITE_H__