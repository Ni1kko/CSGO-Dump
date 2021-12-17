#pragma once

class KnifeAnimFix {
private:
	int RandomSeq(const int low, const int high);
public:
	int Fix(const char* model, const int sequence);
};

extern KnifeAnimFix* g_KnifeAnimFix;