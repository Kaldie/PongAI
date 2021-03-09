#define BOOST_LOG_DYN_LINK 1

#include <PongAI/entity.hpp>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <boost/log/trivial.hpp>

void Entity::connect()
{

  // If the channel exists, we return
  if (channel)
  {
    return;
  }

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
    connect(hostname, port, username, password);
  }
  else
  {
    connect("localhost", 5672, "guest", "guest");
  }
}
void Entity::connect(const char *hostname, int port, const char *username, const char *password)
{
  try
  {
    channel = AmqpClient::Channel::Create(hostname, port, username, password);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }
}

void Entity::initialise_heartbeat()
{

  auto adminstator_exchange_name = getenv("ADMINISTRATION_EXECHANGE_NAME");
  create_exchange(AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, adminstator_exchange_name);
  auto heartbeat_queue_name = create_queue("");

  std::ostringstream queue_stream;
  queue_stream << "heartbeat." << entity_type;
  channel->BindQueue(heartbeat_queue_name, adminstator_exchange_name, "heartbeat.#");
}

void Entity::create_exchange(const std::string &type, const std::string &name) const
{
  if (channel)
  {
    channel->DeclareExchange(name, type, false, true);
  }
}

std::string Entity::create_queue(const std::string &name) const
{
  if (channel)
  {
    return channel->DeclareQueue(name);
  }
  return std::string();
}

Entity::Entity()
{
  connect();
}

const std::string Entity::entity_type = "Entity";