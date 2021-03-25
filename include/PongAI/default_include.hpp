#ifndef __DEFAULT_INCLUDE_H__
#define __DEFAULT_INCLUDE_H__

#include <boost/shared_ptr.hpp>

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

#endif // __DEFAULT_INCLUDE_H__