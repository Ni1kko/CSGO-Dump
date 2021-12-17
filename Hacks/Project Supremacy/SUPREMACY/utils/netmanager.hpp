#pragma once

#include "..\sdk\misc\Recv.hpp"

#include <unordered_map>

class netvars {
public:

	void init();
	int get_offset(const char *tableName, const char *propName);
	bool hook_prop(const char *tableName, const char *propName, RecvVarProxyFn func);
	bool hook_prop(const char *tableName, const char *propName, RecvVarProxyFn func, RecvVarProxyFn &orig);
	void dump();

	static netvars& get() {
		static netvars instance;
		return instance;
	}

private:

	std::unordered_map<std::string, RecvTable*> tables;

	int get_prop(const char *tableName, const char *propName, RecvProp **prop = 0);
	int get_prop(RecvTable *recvTable, const char *propName, RecvProp **prop = 0);
	RecvTable *get_table(const char *tableName);
	void dump_table(RecvTable *table, std::string tabs);
	std::string type2str(Type t);
};

//(Zurückziehunterstüzung): Okay what now, i dont think we are safe here, what if 402 finds us?
//(Backtrackassistant): I dont think he visits that place here too
//(Zurückziehunterstüzung): But what if he does?
//(Backtrackassistant): Okay we have to make a Code Red Plan and ideas?
//(Zurückziehunterstüzung): i think we jump somewhere in legit hows that?
//(Backtrackassistant): Thats a great idea! Okay and what if..
//(Zurückziehunterstüzung): You really think they care about legit features?
//(Backtrackassistant): Haha ure right