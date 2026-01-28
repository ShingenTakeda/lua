cmake_minimum_required(VERSION 3.5)
project(lua C)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Extract Lua version from lua.h
file(READ lua.h LUA_H_CONTENTS)
string(REGEX MATCH "LUA_VERSION_MAJOR\t\"([0-9]+)\"" _ "${LUA_H_CONTENTS}")
set(LUA_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "LUA_VERSION_MINOR\t\"([0-9]+)\"" _ "${LUA_H_CONTENTS}")
set(LUA_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "LUA_VERSION_RELEASE\t\"([0-9]+)\"" _ "${LUA_H_CONTENTS}")
set(LUA_VERSION_RELEASE ${CMAKE_MATCH_1})

# Set C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define source files
set(CORE_SOURCES
    lapi.c
    lcode.c
    lctype.c
    ldebug.c
    ldo.c
    ldump.c
    lfunc.c
    lgc.c
    llex.c
    lmem.c
    lobject.c
    lopcodes.c
    lparser.c
    lstate.c
    lstring.c
    ltable.c
    ltm.c
    lundump.c
    lvm.c
    lzio.c
    ltests.c
)

set(AUX_SOURCES
    lauxlib.c
)

set(LIB_SOURCES
    lbaselib.c
    ldblib.c
    liolib.c
    lmathlib.c
    loslib.c
    ltablib.c
    lstrlib.c
    lutf8lib.c
    loadlib.c
    lcorolib.c
    linit.c
)

set(LUA_SOURCES
    lua.c
)

# Add compile definitions
add_definitions(-DLUA_USE_LINUX)

# Add compiler warnings
if(CMAKE_COMPILER_IS_GNUCC)
    add_compile_options(
        -Wfatal-errors
        -Wextra
        -Wshadow
        -Wundef
        -Wwrite-strings
        -Wredundant-decls
        -Wdisabled-optimization
        -Wdouble-promotion
        -Wmissing-declarations
        -Wdeclaration-after-statement
        -Wmissing-prototypes
        -Wnested-externs
        -Wstrict-prototypes
        -Wc++-compat
        -Wold-style-definition
        -Wlogical-op
        -Wno-aggressive-loop-optimizations
    )
endif()

# Build the static library 'liblua.a'
add_library(lua_static STATIC
    ${CORE_SOURCES}
    ${AUX_SOURCES}
    ${LIB_SOURCES}
)
set_target_properties(lua_static PROPERTIES OUTPUT_NAME lua)
target_include_directories(lua_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_SHARED_LIBS)
    # Build the shared library 'liblua.so'
    add_library(lua_shared SHARED
        ${CORE_SOURCES}
        ${AUX_SOURCES}
        ${LIB_SOURCES}
    )
    set_target_properties(lua_shared PROPERTIES
        OUTPUT_NAME lua
        SOVERSION "${LUA_VERSION_MAJOR}.${LUA_VERSION_MINOR}"
        VERSION "${LUA_VERSION_MAJOR}.${LUA_VERSION_MINOR}.${LUA_VERSION_RELEASE}"
    )
    target_include_directories(lua_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Add the executable 'lua'
add_executable(lua ${LUA_SOURCES})

# Find readline library
find_package(Readline)
if(Readline_FOUND)
    add_definitions(-DLUA_USE_READLINE)
    target_include_directories(lua PRIVATE ${Readline_INCLUDE_DIR})
    target_link_libraries(lua PRIVATE ${Readline_LIBRARY})
endif()

# Link lua executable against the static library and other libraries
target_link_libraries(lua PRIVATE lua_static m dl)

# Installation
install(TARGETS lua DESTINATION bin)
install(FILES lua.h luaconf.h lualib.h lauxlib.h DESTINATION include)

include(GNUInstallDirs)

set(LUA_INSTALL_LIBS lua_static)
if(BUILD_SHARED_LIBS)
    list(APPEND LUA_INSTALL_LIBS lua_shared)
endif()

install(TARGETS ${LUA_INSTALL_LIBS}
    EXPORT lua-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT lua-targets
    FILE LuaTargets.cmake
    NAMESPACE Lua::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lua
)
