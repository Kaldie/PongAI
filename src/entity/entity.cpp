#define BOOST_LOG_DYN_LINK 1

#include <PongAI/entity.hpp>
#include <PongAI/default_include.hpp>
#include <PongAI/heartbeat.hpp>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>

AmqpClient::Channel::ptr_t Entity::create_channel() const
{

  const char *hostname = std::getenv("RABBIT_MQ_HOSTNAME");
  const char *username = std::getenv("RABBITMQ_DEFAULT_USER");
  const char *password = std::getenv("RABBITMQ_DEFAULT_PASS");
  const int port = atoi(std::getenv("RABBITMQ_DEFAULT_PORT"));

  BOOST_LOG_TRIVIAL(info) << "Hostname: " << hostname << " username: "
                          << username << " password: "
                          << "****"
                          << " port: " << port;

  if (hostname && username && password)
  {
    return connect(hostname, port, username, password);
  }
  else
  {
    return connect("localhost", 5672, "guest", "guest");
  }
}

AmqpClient::Channel::ptr_t Entity::connect(const char *hostname, int port, const char *username, const char *password) const
{
  try
  {
    return AmqpClient::Channel::Create(hostname, port, username, password);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }
  return nullptr;
}

std::string Entity::create_heartbeat_topic() const
{
  BOOST_LOG_TRIVIAL(info) << "Creating topic name with entity_type: " << get_entity_type();
  return create_heartbeat_topic(get_entity_type());
}

std::string Entity::create_heartbeat_topic(const std::string &entity_type) const
{
  std::ostringstream queue_stream;
  queue_stream << "heartbeat." << entity_type;
  return queue_stream.str();
}

std::string Entity::declare_administration_exchange(const AmqpClient::Channel::ptr_t &channel) const
{
  auto adminstator_exchange_name = getenv("ADMINISTRATION_EXECHANGE_NAME");
  channel->DeclareExchange(adminstator_exchange_name, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true);
  return adminstator_exchange_name;
}

std::string Entity::create_heartbeat_queue(const AmqpClient::Channel::ptr_t &channel, const std::string &heartbeat_topic) const
{

  auto adminstator_exchange_name = declare_administration_exchange(channel);
  // create queue and bind queue
  //
  auto heartbeat_queue_name = heartbeat_topic + name + "_" + create_random_string(5);
  BOOST_LOG_TRIVIAL(info) << "Creating a queue with the name: " << heartbeat_queue_name;
  heartbeat_queue_name = channel->DeclareQueue(heartbeat_queue_name);
  BOOST_LOG_TRIVIAL(info) << "Binding a queue with the name: " << heartbeat_queue_name << " " << adminstator_exchange_name << " " << heartbeat_topic;
  channel->BindQueue(heartbeat_queue_name, adminstator_exchange_name, heartbeat_topic);
  BOOST_LOG_TRIVIAL(info) << "Done binding a queue with the name: " << heartbeat_queue_name;

  return heartbeat_queue_name;
}

Entity::Entity()
{
  name = create_random_string(19, get_entity_type() + "_");
}

void Entity::manage_heartbeats()
{
  auto channel = create_channel();
  auto heartbeat_topic = create_heartbeat_topic();
  std::string queue_name = create_heartbeat_queue(channel, heartbeat_topic);
  listen_and_respond_heartbeat_requests(queue_name, channel);
}

boost::shared_ptr<Heartbeat> Entity::listen_heartbeat_request(const std::string &consumer_name,
                                                              const AmqpClient::Channel::ptr_t &channel) const
{
  AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_name);
  return Heartbeat::from_json(envelope->Message()->Body());
}

void Entity::listen_and_respond_heartbeat_requests(const std::string &queue_name,
                                                   const AmqpClient::Channel::ptr_t &channel) const
{
  std::string consumer_name = channel->BasicConsume(queue_name);

  boost::shared_ptr<Heartbeat> heartbeat;

  while (true)
  {

    heartbeat = listen_heartbeat_request(consumer_name, channel);
    BOOST_LOG_TRIVIAL(info) << "Heared hearbeat!" << std::endl;

    if (heartbeat->is_request)
    {
      BOOST_LOG_TRIVIAL(info) << "Responding to hearbeat request!" << std::endl;
      auto adminstator_exchange_name = getenv("ADMINISTRATION_EXECHANGE_NAME");
      heartbeat->is_request = false;
      heartbeat->name = name;
      channel->BasicPublish(adminstator_exchange_name,
                            create_heartbeat_topic(),
                            AmqpClient::BasicMessage::Create(Heartbeat::to_json(*heartbeat)));
      break;
    }
  }
}

void Entity::request_heartbeat(const std::string &entity_type, const AmqpClient::Channel::ptr_t &channel) const
{
  auto exchange_name = declare_administration_exchange(channel);

  BOOST_LOG_TRIVIAL(info) << "Requesting heartbeat for: " << entity_type;

  channel->BasicPublish(
      exchange_name,
      create_heartbeat_topic(entity_type),
      AmqpClient::BasicMessage::Create(
          Heartbeat::to_json(Heartbeat(name, entity_type, true))));
}

std::vector<std::string> Entity::find_entities(const std::string &entity_type) const
{
  std::vector<std::string> entity_names;

  auto channel = create_channel();

  auto heartbeat_topic = create_heartbeat_topic(entity_type);
  std::string queue_name = create_heartbeat_queue(channel, heartbeat_topic);

  request_heartbeat(entity_type, channel);

  std::string consumer_name = channel->BasicConsume(queue_name);
  AmqpClient::Envelope::ptr_t envelope;
  bool found_envelope;
  do
  {
    found_envelope = channel->BasicConsumeMessage(consumer_name, envelope, 50);

    if (found_envelope)
    {
      auto heartbeat = Heartbeat::from_json(envelope->Message()->Body());
      BOOST_LOG_TRIVIAL(info) << "Heared a hearbeat!" << envelope->Message()->Body() << heartbeat << std::endl;
      BOOST_LOG_TRIVIAL(info) << heartbeat->is_request << std::endl;
      BOOST_LOG_TRIVIAL(info) << heartbeat->name << std::endl;
      if (! (heartbeat->is_request) )
      {
        entity_names.push_back(heartbeat->name);
        BOOST_LOG_TRIVIAL(info) << "Heared hearbeat of: " << heartbeat->name << " !" << std::endl;
      }
    }
  } while (found_envelope);
  return entity_names;
}
