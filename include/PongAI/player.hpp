#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.hpp"

class Player : Entity
{
private:
    virtual void evaluate_turn();
    virtual std::string entity_type() const override { return "Player"; };
    void listen_and_accept_game_invite(const channel_ptr &channel,
                                        const std::string &consumer) const;

    bool has_acknowledged_invite(const channel_ptr &channel,
                                        const std::string &consumer) const;

public:
    void find_and_participate();
};

#endif // __PLAYER_H__