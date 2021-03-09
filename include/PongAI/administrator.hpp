#include <PongAI/default_include.hpp>
#include <PongAI/entity.hpp>

class Administrator : Entity
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
    Administrator();
    void create_new_game(int number_of_players, FieldSize field_size);
};
