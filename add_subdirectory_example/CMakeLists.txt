cmake_minimum_required(VERSION 3.5)
project(MyProject C)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add the 'lua' subdirectory. CMake will find and configure the Lua project,
# making its targets (e.g., lua_static, lua_shared) available to us.
add_subdirectory(..)

# Define our executable and its source file.
add_executable(my_app main.c)

# Link our executable against the static Lua library.
# Because we updated the lua/CMakeLists.txt to export its include directories,
# CMake will automatically know where to find "lua.h", "lualib.h", etc.
target_link_libraries(my_app PRIVATE lua_static)
