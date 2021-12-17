#pragma once
#include "..\Utils\GlobalVars.h"
#include "..\SDK\Materials.h"
#include "..\SDK\ClientClass.h"
#include "..\Settings.h"
#include <unordered_map>

class SkinChanger
{
	public:
		void KnifeChanger();
		void ChangeSkins();
};

extern SkinChanger* skin_changer;