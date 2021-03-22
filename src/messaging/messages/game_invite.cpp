#include <PongAI/default_include.hpp>
#include <PongAI/messaging/messages/game_invite.hpp>
#include <PongAI/util.hpp>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // can be removed once this is fixed: https://github.com/boostorg/property_tree/pull/50
#include <boost/property_tree/json_parser.hpp>
#undef BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>
#include <string>

namespace messaging
{
    namespace messages
    {

        GameInvite::GameInvite()
            : GameInvite(2,
                         1,
                         FieldSize::Medium,
                         Participents(),
                         GameInviteIntend::Requesting)
        {
        }

        GameInvite::GameInvite(const int &number_of_players)
            : GameInvite(number_of_players,
                         1,
                         FieldSize::Medium,
                         Participents(),
                         GameInviteIntend::Requesting)
        {
        }

        GameInvite::GameInvite(const int &number_of_players, const FieldSize &field_size)
            : GameInvite(number_of_players,
                         1,
                         field_size,
                         Participents(),
                         GameInviteIntend::Requesting)
        {
        }

        GameInvite::GameInvite(const int &number_of_players,
                               const int &number_of_referees,
                               const FieldSize &field_size,
                               const Participents &participents,
                               const GameInviteIntend &intend)
        {
            this->game_id = create_random_string(16, "game.");
            this->number_of_players = number_of_players;
            this->number_of_referees = number_of_referees;
            this->participents = participents;
            this->intend = intend;
        }

        boost::shared_ptr<boost::property_tree::ptree> GameInvite::create_property_tree_from_participents(
            const std::vector<std::pair<EntityType, std::string>> &participents)
        {
            auto player_tree = boost::make_shared<boost::property_tree::ptree>();

            for (auto participent : participents)
            {
                boost::property_tree::ptree participent_ptree;
                participent_ptree.put("type", (int)participent.first);
                participent_ptree.put("name", participent.second);
                player_tree->push_back(std::make_pair("", participent_ptree));
            }
            return player_tree;
        }

        std::vector<std::pair<EntityType, std::string>> GameInvite::create_participents_from_property_tree(
            const boost::property_tree::ptree &property_tree)
        {
            std::vector<std::pair<EntityType, std::string>> participents;

            // the declaration if necessary otherwise an rvalue would 
            // be used as reference of the default return
            auto default_ptree = boost::property_tree::ptree();
            auto optional_child = property_tree.get_child_optional("invite.participents");

            for (auto const &item : optional_child.value_or(default_ptree))
            {
                auto participent = std::pair<EntityType, std::string>();
                participent.first = (EntityType)item.second.get<int>("type");
                participent.second = item.second.get<std::string>("name");
                participents.push_back(participent);
            }
            return participents;
        }

        std::string GameInvite::to_json(const GameInvite &invite)
        {
            boost::property_tree::ptree out;
            out.put("invite.game_id", invite.game_id);
            out.put("invite.number_of_players", invite.number_of_players);
            out.add_child("invite.participents", *create_property_tree_from_participents(invite.participents));
            out.put("invite.intend", invite.intend);

            std::ostringstream oss;
            boost::property_tree::write_json(oss, out);
            return oss.str();
        }

        GameInvite GameInvite::from_json(const std::string &input)
        {
            std::stringstream ss;
            boost::property_tree::ptree out;
            ss << input;
            boost::property_tree::read_json(ss, out);
            GameInvite game_invite;

            game_invite.game_id = out.get<std::string>("invite.game_id");
            game_invite.number_of_players = out.get<int>("invite.number_of_players");
            game_invite.participents = create_participents_from_property_tree(out);
            game_invite.intend = (GameInviteIntend)out.get<int>("invite.intend");

            return game_invite;
        }

        int GameInvite::get_number_of_participating_entity(const EntityType entity_type) const
        {

            return std::count_if(participents.begin(),
                                 participents.end(),
                                 [entity_type](Participent participent) { return participent.first == entity_type; });
        }

        bool GameInvite::requires_entity(const EntityType &entity_type) const
        {
            bool requires_entity;
            switch (entity_type)
            {
            case EntityType::RefereeType:
            {
                requires_entity = get_number_of_participating_entity(EntityType::RefereeType) < number_of_referees;
                break;
            };
            case EntityType::PlayerType:
            {
                requires_entity = get_number_of_participating_entity(EntityType::PlayerType) < number_of_players;
                break;
            };
            default:
                throw "Unknown entity type";
            }
            return requires_entity;
        }

        bool GameInvite::has_enough_participents() const
        {
            // if it does not require referees or players then it has enough
            return !(requires_entity(EntityType::RefereeType) || requires_entity(EntityType::PlayerType));
        }

    }
}