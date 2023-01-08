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

    void readTable(const char *name) {
        lua_getglobal(m_L, name);

        if (!lua_istable(m_L, -1)) {
            throw std::string {"Falha ao ler tabela."};
        }
    }

    template<typename T>
    T readNumber(const char *key) {
        lua_pushstring(m_L, key);
        lua_gettable(m_L, -2);

        if (!lua_isnumber(m_L, -1)) {
            throw std::string {"Falha ao ler dado da tabela."};
        }

        auto result {lua_tonumber(m_L, -1)};

        lua_pop(m_L, 1);

        return static_cast<T>(result);
    }

    std::string readString(const char *key) {
        lua_pushstring(m_L, key);
        lua_gettable(m_L, -2);

        if (!lua_isstring(m_L, -1)) {
            throw std::string {"Falha ao ler dado da tabela."};
        }

        auto result {lua_tostring(m_L, -1)};

        lua_pop(m_L, 1);

        return static_cast<std::string>(result);
    }

private:
    lua_State *m_L {nullptr};
};

}

#endif
