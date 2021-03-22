#ifndef __GAME_INVITE_H__
#define __GAME_INVITE_H__

#include <boost/shared_ptr.hpp>
#include <string>

namespace messaging
{
    namespace messages
    {

        class Heartbeat
        {
        public:
            std::string name;
            std::string entity_type;
            bool is_request;

            Heartbeat() {}
            Heartbeat(const std::string &name, const std::string &entity_type, const bool is_request = false);

            static std::string to_json(const Heartbeat &);
            static boost::shared_ptr<Heartbeat> from_json(const std::string &);

            static boost::shared_ptr<Heartbeat> create_shared_pointer(const std::string &name,
                                                                      const std::string &entity_type,
                                                                      const bool is_request);

            static boost::shared_ptr<Heartbeat> create_shared_pointer();
            static boost::shared_ptr<Heartbeat> create_shared_pointer(const Heartbeat &);
        };
    };
};

#endif // __GAME_INVITE_H__