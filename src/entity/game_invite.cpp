#include <PongAI/game_invite.hpp>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // can be removed once this is fixed: https://github.com/boostorg/property_tree/pull/50
#include <boost/property_tree/json_parser.hpp>
#undef BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>
#include <string>

GameInvite::GameInvite(const std::string& game_id, const int& number_of_players) {
    this->game_id = game_id;
    this->number_of_players = number_of_players;
}

std::string GameInvite::to_json(const GameInvite& invite) {
   boost::property_tree::ptree out;
   out.put("invite.game_id", invite.game_id);
   out.put("invite.number_of_players", invite.number_of_players);
   std::ostringstream oss;
   boost::property_tree::write_json(oss, out);
   return oss.str();
}

GameInvite GameInvite::from_json(const std::string& input) {
    std::stringstream ss;
    boost::property_tree::ptree out;
    ss << input;
    boost::property_tree::read_json(ss, out);
    GameInvite game_invite;
    game_invite.game_id = out.get<std::string>("invite.game_id");
    game_invite.number_of_players = out.get<int>("invite.number_of_players");
    return game_invite;
}