#include <PongAI/game_invite.hpp>
#include <PongAI/default_include.hpp>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // can be removed once this is fixed: https://github.com/boostorg/property_tree/pull/50
#include <boost/property_tree/json_parser.hpp>
#undef BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/ptree.hpp>
#include <boost/make_shared.hpp>
#include <iostream>
#include <sstream>
#include <string>

GameState::GameState()
    : GameState(2,
                FieldSize::Medium,
                Participents(),
                GameStateIntend::Requesting)
{
}
GameState::GameState(const int &number_of_players)
    : GameState(number_of_players,
                FieldSize::Medium,
                Participents(),
                GameStateIntend::Requesting)
{
}

GameState::GameState(const int &number_of_players, const FieldSize &field_size)
    : GameState(number_of_players,
                field_size,
                Participents(),
                GameStateIntend::Requesting)
{
}

GameState::GameState(const int &number_of_players,
                     const FieldSize &field_size,
                     const Participents &participents,
                     const GameStateIntend &intend)
{
    this->game_id = create_random_string(16, "game.");
    this->number_of_players = number_of_players;
    this->field_size = field_size;
    this->participents = participents;
    this->intend = intend;
}

boost::shared_ptr<boost::property_tree::ptree> GameState::create_property_tree_from_participents(
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

std::vector<std::pair<EntityType, std::string>> GameState::create_participents_from_property_tree(
    const boost::shared_ptr<boost::property_tree::ptree> property_tree)
{
    std::vector<std::pair<EntityType, std::string>> participents;
    // auto optional_child2 = property_tree->get_child("participents");
    auto default_ptree = boost::property_tree::ptree();
    auto optional_child = property_tree->get_child_optional("participents");

    for (auto const &item : optional_child.value_or(default_ptree))
    {
        auto participent = std::pair<EntityType, std::string>();
        participent.first = (EntityType)item.second.get<int>("type");
        participent.second = item.second.get<std::string>("name");
        participents.push_back(participent);
    }
    return participents;
}

std::string GameState::to_json(const GameState &invite)
{
    boost::property_tree::ptree out;
    out.put("invite.game_id", invite.game_id);
    out.put("invite.number_of_players", invite.number_of_players);
    out.add_child("invite.players", *create_property_tree_from_participents(invite.participents));
    out.put("invite.current_turn", invite.current_turn);
    out.put("invite.intend", invite.intend);

    std::ostringstream oss;
    boost::property_tree::write_json(oss, out);
    return oss.str();
}

GameState GameState::from_json(const std::string &input)
{
    std::stringstream ss;
    boost::property_tree::ptree out;
    ss << input;
    boost::property_tree::read_json(ss, out);
    GameState game_invite;

    game_invite.game_id = out.get<std::string>("invite.game_id");
    game_invite.number_of_players = out.get<int>("invite.number_of_players");
    game_invite.participents = create_participents_from_property_tree(boost::make_shared<boost::property_tree::ptree>(out));
    game_invite.current_turn = out.get<int>("invite.current_turn");
    game_invite.intend = (GameStateIntend)out.get<int>("invite.intend");

    return game_invite;
}