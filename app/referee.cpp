#include <PongAI/messaging/administrator.hpp>
#include <PongAI/messaging/messages/game_invite.hpp>
#include <PongAI/messaging/participant.hpp>
#include <boost/chrono.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace messaging;

void log_init()
{
    std::string format = "[%TimeStamp%][%Severity%]: %Message%";

    logging::register_simple_formatter_factory<
        boost::log::trivial::severity_level, char>("Severity");

    logging::add_common_attributes();

    logging::add_file_log(
        keywords::target = "logs/", keywords::file_name = "%y%m%d_%3N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::scan_method = sinks::file::scan_matching,
        keywords::format = format);

    logging::add_console_log(std::cout,
                             boost::log::keywords::format = format);

    logging::core::get()->set_filter(logging::trivial::severity >=
                                     logging::trivial::debug);

}

void lala(boost::shared_ptr<Administrator> &administrator)
{
    while (true)
    {
        administrator->manage_heartbeats();
    }
}

void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void start_new_game(boost::shared_ptr<Administrator> &administrator)
{
    wait(1);
    administrator->create_new_game(2, FieldSize::Medium);
}

void listner_for_game(boost::shared_ptr<messaging::Participant> &player)
{
    player->find_and_participate();
}

void request_heart_beat(boost::shared_ptr<Administrator> &administrator)
{

    while (true)
    {
        for (auto name : administrator->find_entities(administrator->get_entity_type()))
        {
            std::cout << "Found an: " << name << std::endl;
        }
        wait(1);
    }
}

int main(int, char **)
{
    log_init();

    boost::shared_ptr<Administrator> administrator = boost::make_shared<Administrator>();
    std::vector<boost::thread> threads;
    for (auto i = 0; i < 4; ++i)
    {
        boost::shared_ptr<messaging::Participant> player = boost::make_shared<messaging::Participant>();
        threads.push_back(boost::thread(listner_for_game, player));
    }
    auto thread_1 = boost::thread(start_new_game, administrator);
    // wait(5);
    // auto thread_2 = boost::thread(start_new_game, administrator);
    // wait(5);
    // auto thread_3 = boost::thread(start_new_game, administrator);

    thread_1.join();
    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i].join();
    }
}