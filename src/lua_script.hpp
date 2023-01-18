#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP

namespace lua_script {

struct lua_script {
    lua_script(const char *script_path) : m_L {luaL_newstate()} {
        luaL_openlibs(m_L);

        try {
            if (luaL_dofile(m_L, script_path) != LUA_OK) throw util::program_exception {"Falha ao ler script Lua."};
        } catch (util::program_exception &e) {
            printf("%s", e.get_description());
        }
    }

    ~lua_script() {
        lua_close(m_L);
    }

private:
    lua_State *m_L {nullptr};
};

}

#endif
