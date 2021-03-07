add_executable(Player player.cpp)
add_executable(Referee referee.cpp)

target_include_directories(Player PRIVATE ../src)
target_include_directories(Referee PRIVATE entity ../src)

target_link_libraries(Player SimpleAmqpClient PongAI)
target_link_libraries(Referee SimpleAmqpClient PongAI)