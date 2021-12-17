#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class speclist : public singleton< speclist > {
public:

	void OnRenderSpectatorList();
	std::vector<int> GetObservervators(int playerId);
private:
};