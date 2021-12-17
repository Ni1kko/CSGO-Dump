#pragma once
#include "../DLL/Client/ActiveSocket.h"
#include "../Include/GlobalIncludes.h"
#include "../Hashing/xorstr.h"
namespace Cheat
{
	class CSettings
	{
		public:
			bool PanoramaMode = false;
			bool ShowConsole  = false;
			bool UnloadCheat  = false;
			struct
			{
				int CreateMove                = 0;
				int StandardBlendingRules     = 0;
				int DoPostScreenSpaceEffects  = 0;
				int DrawModelExecute          = 0;
				int emit_sound                = 0;
				int present2                  = 0;
				int FrameStageNotify          = 0;
				int listLeavesInBox           = 0;
				int LockCursor                = 0;
				int OverrideView              = 0;
				int PaintTraverse             = 0;
				int reset2                    = 0;
				int SceneEnd                  = 0;
				int SendDataGram              = 0;
				int Move                      = 0;
				int writeusercmddeltatobuffer = 0;
				int ReadPacketEntities        = 0;
				int RecvProxy				  = 0;
				int FireEvent                 = 0;
				int StandartBlendingRules	  = 0;
			} UnloadReadyTable;
			bool UIDevMode	  = false;

			CActiveSocket SocketConnection;

			int VersionMajor = 2;
			int VersionMinor = 0;
			int VersionBuild = 1337;
			int ExpireTime	 = 0;
			int ProductID	 = 2; /* 2 */

			const char* GameName = ("Counter-Strike: Global Offensive");

			std::string ClientDLL = ("client.dll");
			std::string UserName = ("unk");

			void LoadDefaults();
	};

	extern CSettings* Settings;
}
