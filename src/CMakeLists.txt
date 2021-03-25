# Optionally glob, but only for CMake 3.12 or later:
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PongAI_SOURCE_DIR}/include/PongAI/*.hpp")

file(GLOB_RECURSE ENTITY_SRC CONFIGURE_DEPENDS "${PongAI_SOURCE_DIR}/src/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(PongAI SHARED ${ENTITY_SRC} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(PongAI PUBLIC ../include)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Boost COMPONENTS log random thread REQUIRED)

# This depends on (header only) boost
target_link_libraries(PongAI PRIVATE Boost::boost Boost::log Boost::random Boost::thread SimpleAmqpClient)

# All users of this library will need at least C++11
target_compile_features(PongAI PUBLIC cxx_std_14)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
