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

    void table(const char *name) {
        lua_getglobal(m_L, name);

        if (!lua_istable(m_L, -1)) {
            throw std::string {"Falha ao ler tabela."};
        }
    }

    template<typename T>
    T load(const char *key) {
        lua_pushstring(m_L, key);
        lua_gettable(m_L, -2);

        if (!lua_isnumber(m_L, -1)) {
            throw std::string {"Falha ao ler dado da tabela."};
        }

        T result {lua_tonumber(m_L, -1)};

        lua_pop(m_L, 1);

        return result;
    }

private:
    lua_State *m_L {nullptr};
};

}

#endif
