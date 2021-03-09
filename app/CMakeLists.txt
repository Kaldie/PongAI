add_executable(Referee referee.cpp)

find_package(Boost COMPONENTS log log_setup random REQUIRED)

# We need this directory, and users of our library will need it too
target_include_directories(Referee PRIVATE ../include)

target_link_libraries(Referee SimpleAmqpClient PongAI Boost::boost Boost::log_setup Boost::log )