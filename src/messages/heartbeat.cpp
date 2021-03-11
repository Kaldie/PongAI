#include <PongAI/heartbeat.hpp>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // can be removed once this is fixed: https://github.com/boostorg/property_tree/pull/50
#include <boost/property_tree/json_parser.hpp>
#undef BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <sstream>
#include <string>

Heartbeat::Heartbeat(const std::string& name, const std::string& entity_type, const bool is_request) {
    this->name = name;
    this->entity_type = entity_type;
    this->is_request = is_request;
}

std::string Heartbeat::to_json(const Heartbeat& heartbeat) {
   boost::property_tree::ptree out;
   out.put("heartbeat.name", heartbeat.name);
   out.put("heartbeat.entity_type", heartbeat.entity_type);
   out.put("heartbeat.is_request", heartbeat.is_request);
   std::ostringstream oss;
   boost::property_tree::write_json(oss, out);
   return oss.str();
}

Heartbeat Heartbeat::from_json(const std::string& input) {
    std::stringstream ss;
    boost::property_tree::ptree out;
    ss << input;
    boost::property_tree::read_json(ss, out);
    Heartbeat heartbeat;
    heartbeat.name = out.get<std::string>("heartbeat.name","unknown");
    heartbeat.entity_type = out.get<std::string>("heartbeat.entity_type");
    heartbeat.is_request = out.get<bool>("heartbeat.is_request");
    return heartbeat;
}