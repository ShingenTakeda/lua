#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main(void) {
    // Create a new Lua state
    lua_State *L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "Error: Failed to create Lua state.\n");
        return 1;
    }

    // Load the standard Lua libraries (io, string, etc.)
    luaL_openlibs(L);

    // The Lua script to execute
    const char *lua_script = "print('Hello from Lua! The answer is ' .. 2 + 2)";

    // Load and run the script
    int status = luaL_dostring(L, lua_script);
    if (status != LUA_OK) {
        // If there was an error, print it to the console
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from the stack
    }

    // Close the Lua state
    lua_close(L);

    printf("Successfully executed Lua script.\n");

    return 0;
}
