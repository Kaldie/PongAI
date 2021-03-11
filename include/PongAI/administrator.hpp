#ifndef __ADMINISTRATOR_H__
#define __ADMINISTRATOR_H__

#include <PongAI/default_include.hpp>
#include <PongAI/entity.hpp>

class Administrator : public Entity
{
private:
    /* data */
    
    void notify_new_game();
    void notify_start_game();
    void notify_end_game();
    void create_game_exchange_and_queue(const std::string &) const;
    void publish_game_invite(const std::string &) const;
    std::string create_administration_exchange() const;
    std::string create_new_game_id() const;

public:
    void create_new_game(int number_of_players, FieldSize field_size);
    std::string get_entity_type() const {return Administrator::entity_type;};
    const static std::string entity_type;
};

#endif // __ADMINISTRATOR_H__