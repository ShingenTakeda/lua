cmake_minimum_required(VERSION 3.5)
project(lua C)

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

# Set properties for the static library
set_target_properties(lua_static PROPERTIES OUTPUT_NAME lua)

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
install(TARGETS lua_static DESTINATION lib)
install(FILES lua.h luaconf.h lualib.h lauxlib.h DESTINATION include)
