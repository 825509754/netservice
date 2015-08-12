#ifndef _LUA_CONFIG_H_
#define _LUA_CONFIG_H_

extern "C"
{
#include <lua.h>  
#include <lualib.h>  
#include <lauxlib.h> 
};

#include <map>
#include <vector>

class LuaEnv
{
	struct lua_table
	{
		const char *key;
		const char *value;
	};

public:
	 LuaEnv();
	~ LuaEnv();

public:
	bool load_config(const char *config_file);
	int optint(const char *key, int opt);
	const char * optstring(const char *key,const char * opt);

	const char* opttablestring(const char *tableName, const char *talbeItem, const char *key, const char *opt);
	int opttableint(const char *tableName, const char *talbeItem, const char *key, int opt);

	int get_table_len(const char *table);
	void get_table(const char *tablename, const char *key, std::vector<const char*>& values);

private:
	void lua_env_init();
	const char * lua_getenv(const char *key);
	void lua_setenv(const char *key, const char *value);

	const char* get_table_filed(const char *key);

private:
	lua_State *L;

	std::vector<lua_table> mTables;
};

#endif