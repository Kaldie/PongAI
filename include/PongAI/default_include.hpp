#ifndef __DEFAULT_INCLUDE_H__
#define __DEFAULT_INCLUDE_H__

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

enum FieldSize
{
    Small,
    Medium,
    Large,
};

enum GameInviteIntend
{
    Requesting,
    Accepting,
    Acknowledge,
    Reject,
    Starting,
    Running,
    Stale,
};

enum EntityType
{
    Entity,
    AdministratorType,
    RefereeType,
    PlayerType
};

typedef std::pair<EntityType, std::string> Participent;
typedef std::vector<Participent> Participents;

#endif // __DEFAULT_INCLUDE_H__