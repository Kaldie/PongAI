#define BOOST_LOG_DYN_LINK 1

#include <PongAI/entity.hpp>
#include <PongAI/default_include.hpp>
#include <PongAI/heartbeat.hpp>
#include <PongAI/util.hpp>
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

AmqpClient::Channel::ptr_t Entity::connect(const char *hostname,
                                           int port,
                                           const char *username,
                                           const char *password) const
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

std::string Entity::prepare_listen_exchange_topic(const AmqpClient::Channel::ptr_t &channel,
                                                  const std::string &exchange_name,
                                                  const std::string &topic_name) const
{
  channel->DeclareExchange(exchange_name, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, false, true);
  std::string queue_name = channel->DeclareQueue(create_random_string(32));
  channel->BindQueue(queue_name, exchange_name, topic_name);
  return channel->BasicConsume(queue_name);
}

std::string Entity::prepare_listen_admin_exchange_topic(const AmqpClient::Channel::ptr_t &channel,
                                                        const std::string &topic_name) const
{
  auto adminstator_exchange_name = get_administration_exchange_name();
  return prepare_listen_exchange_topic(channel, adminstator_exchange_name, topic_name);
}

std::string Entity::prepare_listen_heartbeat_topic(const AmqpClient::Channel::ptr_t &channel,
                                                   const std::string &entity_type) const
{
  auto topic_name = create_heartbeat_topic_name(entity_type);
  return prepare_listen_admin_exchange_topic(channel, topic_name);
}

std::string Entity::prepare_listen_heartbeat_topic(const AmqpClient::Channel::ptr_t &channel) const
{
  auto topic_name = create_heartbeat_topic_name();
  return prepare_listen_admin_exchange_topic(channel, topic_name);
}

std::string Entity::create_heartbeat_topic_name() const
{
  BOOST_LOG_TRIVIAL(info) << "Creating topic name with entity_type: " << entity_type();
  return create_heartbeat_topic_name(entity_type());
}

std::string Entity::create_heartbeat_topic_name(const std::string &entity_type) const
{
  std::ostringstream queue_stream;
  queue_stream << "heartbeat." << entity_type;
  return queue_stream.str();
}

Entity::Entity() : id(create_random_string(19))
{}

void Entity::manage_heartbeats() const
{
  auto channel = create_channel();
  auto consumer_name = prepare_listen_heartbeat_topic(channel);
  listen_and_respond_heartbeat_requests(consumer_name, channel);
}

void Entity::listen_and_respond_heartbeat_requests(const std::string &consumer_name,
                                                   const AmqpClient::Channel::ptr_t &channel) const
{
  boost::shared_ptr<Heartbeat> heartbeat;

  while (true)
  {
    AmqpClient::Envelope::ptr_t envelope = channel->BasicConsumeMessage(consumer_name);
    heartbeat = Heartbeat::from_json(envelope->Message()->Body());
    BOOST_LOG_TRIVIAL(info) << "Heared hearbeat!" << std::endl;

    if (heartbeat->is_request)
    {
      BOOST_LOG_TRIVIAL(info) << "Responding to hearbeat request!" << std::endl;
      auto adminstator_exchange_name = get_administration_exchange_name();
      heartbeat->is_request = false;
      heartbeat->name = get_name();
      channel->BasicPublish(adminstator_exchange_name,
                            create_heartbeat_topic_name(),
                            AmqpClient::BasicMessage::Create(Heartbeat::to_json(*heartbeat)));
      break;
    }
  }
}

void Entity::request_heartbeat(const std::string &entity_type, const AmqpClient::Channel::ptr_t &channel) const
{
  BOOST_LOG_TRIVIAL(info) << "Requesting heartbeat for: " << entity_type;

  prepare_listen_heartbeat_topic(channel, entity_type);
  auto adminstator_exchange_name = get_administration_exchange_name();
  channel->BasicPublish(
      adminstator_exchange_name,
      create_heartbeat_topic_name(entity_type),
      AmqpClient::BasicMessage::Create(
          Heartbeat::to_json(Heartbeat(get_name(), entity_type, true))));
}

std::vector<std::string> Entity::find_entities(const std::string &entity_type) const
{
  std::vector<std::string> entity_names;
  auto channel = create_channel();
  auto consumer_name = prepare_listen_heartbeat_topic(channel, entity_type);
  request_heartbeat(entity_type, channel);

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
      if (!(heartbeat->is_request))
      {
        entity_names.push_back(heartbeat->name);
        BOOST_LOG_TRIVIAL(info) << "Heared hearbeat of: " << heartbeat->name << " !" << std::endl;
      }
    }
  } while (found_envelope);
  return entity_names;
}
