#ifndef LUA_SCRIPT_HPP
#define LUA_SCRIPT_HPP

namespace LuaScript {

class LuaScript {
public:
    LuaScript(const char *scriptPath) : m_L {luaL_newstate()} {
        luaL_openlibs(m_L);

        if (luaL_dofile(m_L, scriptPath) != LUA_OK) {
            throw std::string {"Falha ao abrir arquivo lua."};
        }
    }

    ~LuaScript() {
        lua_close(m_L);
    }

private:
    lua_State *m_L {nullptr};
};

}

#endif
