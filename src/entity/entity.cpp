#include <PongAI/entity.hpp>

#include <cstdlib>
#include <iostream>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

void Entity::connect()
{
  if (const char *hostname = std::getenv("RABBIT_MQ_HOSTNAME"))
  {
    connect(hostname);
  }
  else
  {
    connect("localhost");
  }
}
void Entity::connect(const char *hostname)
{
  try
  {
    channel = AmqpClient::Channel::Create(hostname);
    connected = true;
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
  }
}

Entity::Entity()
{
  connect();
}
