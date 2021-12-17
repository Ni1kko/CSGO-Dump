#include "CLuaHook.h"

void c_lua_hookManager::registerHook(std::string eventName, int scriptId, sol::protected_function func) {
	c_lua_hook hk = { scriptId, func };

	this->hooks[eventName].push_back(hk);
}

void c_lua_hookManager::unregisterHooks(int scriptId) {
	for (auto& ev : this->hooks) {
		int pos = 0;

		for (auto& hk : ev.second) {
			if (hk.scriptId == scriptId)
				ev.second.erase(ev.second.begin() + pos);

			pos++;
		}
	}
}

std::vector<c_lua_hook> c_lua_hookManager::getHooks(std::string eventName) {
	return this->hooks[eventName];
}
