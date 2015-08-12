#include "lua_config.h"

#include <stdlib.h>
#include <assert.h>

LuaEnv:: LuaEnv()
{
		
}

LuaEnv::~ LuaEnv()
{
	lua_close(L);
}

bool LuaEnv::load_config(const char *config_file)
{
	lua_env_init();
	luaL_openlibs(L);

	int err = luaL_dofile(L, config_file);
	if (err)
	{
		fprintf(stderr,"%s\n",lua_tostring(L,-1));
		lua_close(L);
		return false;
	} 

	return true;
}


const char *LuaEnv::lua_getenv(const char *key)
{
	lua_getglobal(L, key);
	const char * result = lua_tostring(L, -1);
	lua_pop(L, 1);

	return result;
}

void LuaEnv::lua_setenv(const char *key, const char *value)
{
	lua_getglobal(L, key);
	assert(lua_isnil(L, -1));
	lua_pop(L,1);
	lua_pushstring(L,value);
	lua_setglobal(L,key);
}

void LuaEnv::lua_env_init()
{
	L = luaL_newstate();
}


int LuaEnv::optint(const char *key, int opt)
{
	const char * str = lua_getenv(key);
	if (str == NULL)
	{
		char tmp[20];
		sprintf(tmp,"%d",opt);
		lua_setenv(key, tmp);
		return opt;
	}

	return strtol(str, NULL, 10);
}

const char * LuaEnv::optstring(const char *key,const char * opt)
{
	const char * str = lua_getenv(key);
	if (str == NULL)
	{
		if (opt)
		{
			lua_setenv(key, opt);
			opt = lua_getenv(key);
		}
		return opt;
	}
	return str;
}

void ReadTableFromItem(lua_State *L, const char* lpszTableName, const char* lpszTableItem)
{
	lua_getglobal(L, lpszTableName);

	lua_pushnumber(L, 1);
	lua_gettable(L, -2);
	printf("%s.%s=%d\n", lpszTableName, lpszTableItem, (int)lua_tonumber(L, -1));
	lua_pop(L, 2);
}

const char* LuaEnv::opttablestring(const char *tableName, const char *talbeItem, const char *key, const char *opt)
{
	//lua_getglobal(L, tableName);  
	//lua_pushstring(L, talbeItem);  
	//lua_gettable(L, -2);

	////lua_getglobal(L,"1");
	//lua_getfield(L,-1,"ip");
	////lua_getfield(lua,-2,"author");

	////lua_rawgeti(L, -1, 1);
	////lua_pushstring(L, "1");  
	////lua_gettable(L, -2);

	//printf("isTable:%d\n",lua_isstring(L,-1));

	////lua_getglobal(L, talbeItem);
	////lua_getfield(L, -1, key);
	//const char* result = lua_tostring(L,-1);
	//printf("%s\n", result);
	//lua_pop(L, 3);  

	//return result;

	return nullptr;
}


int LuaEnv::opttableint(const char *tableName, const char *talbeItem, const char *key, int opt)
{
	lua_getglobal(L, tableName);
	lua_pushstring(L, talbeItem);
	lua_gettable(L, -2);

	lua_getfield(L, -1, key);
	int rst = (int)lua_tonumber(L, -1);
	lua_pop(L, 3); 

	return rst;
}


int LuaEnv::get_table_len(const char *table)
{
	//lua_getglobal(L, table);
	//int it = lua_gettop(L);
	//lua_pushnil(L);
	////printf("Enum %s:", lpszTableName);
	//while(lua_next(L, it))
	//{
	//	printf(get_table_filed("ip"));
	//	printf(get_table_filed("port"));
	//	//lua_pushstring(L, "ip");
	//	////lua_pushstring(L, "port");
	//	//int it = lua_gettop(L);
	//	////lua_gettable(L, -2);
	//	////printf("%s, \n", lua_tostring(L, -1));
	//	//
	//	//lua_gettable(L, -2);
	//	//printf("%s\n", lua_tostring(L, -1));
	//	//lua_pop(L, 1);

	//	//lua_pushstring(L, "port");
	//	//lua_gettable(L, -2);
	//	//printf("%s\n", lua_tostring(L, -1));
	//	//lua_pop(L, 1);

	//	lua_pop(L, 1);
	//}
	//printf("\n");
	//lua_pop(L, 1);

	lua_getglobal(L, table);
	return lua_objlen(L, -1); 
}
void LuaEnv::get_table(const char *tablename, const char *key, std::vector<const char*>& values)
{
	lua_getglobal(L, tablename);
	int it = lua_gettop(L);
	lua_pushnil(L);
	while(lua_next(L, it))
	{
		values.push_back(get_table_filed(key));

		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	//return values;
}

const char* LuaEnv::get_table_filed(const char *key)
{
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	const char* rst = lua_tostring(L, -1);
	lua_pop(L, 1);

	return rst;
}