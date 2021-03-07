# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PongAI_SOURCE_DIR}/include/PongAI/*.hpp")

file(GLOB ENTITY_SRC CONFIGURE_DEPENDS "${PongAI_SOURCE_DIR}/src/**/*.cpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(PongAI ${ENTITY_SRC} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(PongAI PUBLIC ../include)

# This depends on (header only) boost
target_link_libraries(PongAI PRIVATE Boost::boost SimpleAmqpClient)

# All users of this library will need at least C++11
target_compile_features(PongAI PUBLIC cxx_std_11)

find_package(Boost REQUIRED)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
