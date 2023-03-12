#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP

namespace lua_script {

struct lua_script {
    lua_script(const char *script_path) : L {luaL_newstate()} {
        luaL_openlibs(L);

        if (luaL_dofile(L, script_path) != LUA_OK) error_log(__FILE__, __LINE__, "falha ao abrir/ler script lua");

        lua_getglobal (L, "main");
        lua_isfunction(L, -1);
        lua_pcall     (L, 0, 0, 0);
    }

    ~lua_script() { lua_close(L); }

protected:
    lua_State *L {nullptr};
};

}

#endif
