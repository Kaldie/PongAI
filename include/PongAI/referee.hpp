#include <PongAI/default_include.hpp>
#include <PongAI/entity.hpp>

class Referee : Entity
{
private:
    /* data */

    std::vector<Field_ptr> fields;

    void notify_new_game();
    void notify_end_game();
    


public:

    void create_new_game(int number_of_players, FieldSize field_size);
};

