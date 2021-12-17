#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "..\Utils\GlobalVars.h"
#include "..\SDK\ClientClass.h"
#include "..\SDK\UtlVector.h"

class Antiaim
{
public:
	void Do(CUserCmd* cmd);
	void Desync(CUserCmd * pCmd);
private:
	float DoPitch();
	float DoYaw();
	
	bool DoEdging(float& yaw);
};

extern Antiaim* antiaim;