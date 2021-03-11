#include <iostream>
#include <PongAI/administrator.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <PongAI/game_invite.hpp>
#include <boost/log/core.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

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
                                     logging::trivial::info);
}

void listen_for_heartbeat(boost::shared_ptr<Administrator>& administrator) {
    while (true) {
        administrator->manage_heartbeats();
    }
}

void wait(int seconds)
{
  boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void request_heart_beat(boost::shared_ptr<Administrator>& administrator) {
    while (true) {
        administrator->request_heartbeat(Administrator::entity_type);
        wait(1);        
    }
}


int main(int, char **)
{

    log_init();
    std::cout << "Hello, world!" << std::endl
              << "This is Referee!\n";
              
    boost::shared_ptr<Administrator> referee = boost::make_shared<Administrator>();
    auto thread_1 = boost::thread(listen_for_heartbeat, referee);
    
    std::cout << "After thread 1" <<  std::endl <<Administrator::entity_type << std::endl;

    auto thread_2 = boost::thread(request_heart_beat, referee);
    std::cout << "After thread 2"<< std::endl;

    // thread_1.join();
    thread_2.join();

    // referee->create_new_game(2, Medium);

    // AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
    // std::string consumer_tag = channel->BasicConsume("my_queue", "");
    // AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_tag);
}