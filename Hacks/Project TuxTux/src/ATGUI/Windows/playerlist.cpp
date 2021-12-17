#include "playerlist.h"
#include "../../spacehook.h"
bool PlayerList::showWindow = false;
static uint64_t GetTickCountMs()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t)(ts.tv_nsec / 1000000) + ((uint64_t)ts.tv_sec * 1000ull);
}
inline int sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int result = vsnprintf(buffer, sizeOfBuffer, format, ap);
    va_end(ap);
    return result;
}

template<size_t sizeOfBuffer>
inline int sprintf_s(char (&buffer)[sizeOfBuffer], const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int result = vsnprintf(buffer, sizeOfBuffer, format, ap);
    va_end(ap);
    return result;
}
void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	engine->ClientCmd_Unrestricted(buffer);
}
void votekicks(){

		static int delay;
	//	NameChanger::SetName("\n\xAD\xAD\xAD");
		NameChanger::SetName("\n\n\n\n...\n\n\n Test \n\n\n...\n\n");

 
	/*	if (GetTickCountMs() - delay > 4000)
		{
			delay = GetTickCountMs();
			NameChanger::SetName("\n\xAD\xAD\xAD");
		}*/

}
float onetime;
void teest(){

	if(!onetime){
		NameChanger::SetName("\r");
		onetime = true;
	}
	for (int i = 0; i < 50; i++)
	{
		SayInChat(",");
	}
	
	
}
void PlayerList::RenderWindow()
{
	if( Settings::UI::Windows::Playerlist::reload )
	{
		ImGui::SetNextWindowPos(ImVec2(Settings::UI::Windows::Playerlist::posX, Settings::UI::Windows::Playerlist::posY), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(Settings::UI::Windows::Playerlist::sizeX, Settings::UI::Windows::Playerlist::sizeY), ImGuiSetCond_Always);
		Settings::UI::Windows::Playerlist::reload = false;
		PlayerList::showWindow = Settings::UI::Windows::Playerlist::open;
	}
	else
	{
		ImGui::SetNextWindowPos(ImVec2(Settings::UI::Windows::Playerlist::posX, Settings::UI::Windows::Playerlist::posY), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(Settings::UI::Windows::Playerlist::sizeX, Settings::UI::Windows::Playerlist::sizeY), ImGuiSetCond_FirstUseEver);
	}
	if (!PlayerList::showWindow)
	{
		Settings::UI::Windows::Playerlist::open = false;
		return;
	}

	if (ImGui::Begin(XORSTR("exploit"), &PlayerList::showWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
	{
		panorama::IUIPanel *root = (engine->IsInGame() ? GUI::hudRoot : GUI::menuRoot);
		if(root){
			auto eng = panoramaEngine->AccessUIEngine();
		
			if(ImGui::Button("Stop Matchmaking")){
 			    eng->RunScript(root, "LobbyAPI.StopMatchmaking();", "panorama/layout/base.xml", 8, 10, false);
			}		
			if(ImGui::Button("StartMatchmaking")){
 			    eng->RunScript(root, "LobbyAPI.StartMatchmaking( '', '', '', '' );", "panorama/layout/base.xml", 8, 10, false);
			}
			if(ImGui::Button("Copy FriendCode")){

 			    eng->RunScript(root, "SteamOverlayAPI.CopyTextToClipboard(MyPersonaAPI.GetFriendCode());", "panorama/layout/base.xml", 8, 10, false);

			}
			if(ImGui::Button("test votekicks")){

 			   votekicks();
			}
			static int music = 0;
			const char* musictype[] = {  "Z.E KLICK", "Ant Wan Ikon", "Sinan-1 till 5" };
			ImGui::Combo(XORSTR("music"), (int*)&music, musictype, IM_ARRAYSIZE(musictype));
			if(ImGui::Button("play music")){

				switch (music)
				{
				case 0:
				 	eng->RunScript(root, "SteamOverlayAPI.OpenURL( 'https://www.youtube.com/embed/MOPOl_zwQUc?autoplay=1&loop=1' );", "panorama/layout/base.xml", 8, 10, false);
					break;
				case 1:
				 	eng->RunScript(root, "SteamOverlayAPI.OpenURL( 'https://www.youtube.com/embed/djflfLs7cMo?autoplay=1&loop=1' );", "panorama/layout/base.xml", 8, 10, false);
					break;
				case 2:
				 	eng->RunScript(root, "SteamOverlayAPI.OpenURL( 'https://www.youtube.com/embed/PwzSy6hBFn8?autoplay=1&loop=1' );", "panorama/layout/base.xml", 8, 10, false);
					break;
				default:
					break;
				}
				

			}
			static bool logergetReadyTimeRemainingSeconds;
			if(ImGui::Checkbox("loger getReadyTimeRemainingSeconds",&logergetReadyTimeRemainingSeconds));
			if(logergetReadyTimeRemainingSeconds){
					eng->RunScript(root, "$.Msg('ReadyTimeRemainingSeconds: ' + LobbyAPI.GetReadyTimeRemainingSeconds());", "panorama/layout/base.xml", 8, 10, false);
			}
			if(ImGui::Button("test")){
			//	panoramaEngine->AccessUIEngine()->Shutdown();
			}
			if(SteamClient){
			if(ImGui::Button("test1")){	
				auto GetHSteamPipe	= GetSymbolAddress<HSteamPipe(*)()>(XORSTR("./bin/linux64/libsteam_api.so"), XORSTR("GetHSteamPipe"));
				auto GetHSteamUser	= GetSymbolAddress<HSteamUser(*)()>(XORSTR("./bin/linux64/libsteam_api.so"), XORSTR("GetHSteamUser"));


				SteamMatchmaking = SteamClient->GetISteamMatchmaking(GetHSteamUser(), GetHSteamPipe(), "SteamMatchMaking009");
				 for ( int iLobby = 0; iLobby < 200; iLobby++ )
        		{
				//	if(SteamMatchmaking)
					cvar->ConsoleDPrintf("GetLobbyByIndex %p\n", SteamMatchmaking->GetLobbyByIndex(iLobby));
				}
				
			}
			}
			
			//panoramaEngine->AccessUIEngine()
			static int spamAmount = 10;
			ImGui::SliderInt(XORSTR("##spamAmount"), &spamAmount, 0, 200, XORSTR("spam Amount: %0.f"));
			if(ImGui::Button("Matchmaking spam")){
				for (int i = 0; i < spamAmount; i++)
				{
					 eng->RunScript(root, "LobbyAPI.ChangeTeammateColor();", "panorama/layout/base.xml", 8, 10, false);
					 eng->RunScript(root, "LobbyAPI.StopMatchmaking();", "panorama/layout/base.xml", 8, 10, false);
					 
					 eng->RunScript(root, "LobbyAPI.StartMatchmaking( '', '', '', '' );", "panorama/layout/base.xml", 8, 10, false);
					  eng->RunScript(root, "LobbyAPI.StartMatchmaking( '', '', '', '' );", "panorama/layout/base.xml", 8, 10, false);
					   eng->RunScript(root, "LobbyAPI.StartMatchmaking( '', '', '', '' );", "panorama/layout/base.xml", 8, 10, false);
					    eng->RunScript(root, "LobbyAPI.StartMatchmaking( '', '', '', '' );", "panorama/layout/base.xml", 8, 10, false);
						 eng->RunScript(root, "LobbyAPI.StartMatchmaking( '', '', '', '' );", "panorama/layout/base.xml", 8, 10, false);
					 eng->RunScript(root, "LobbyAPI.StopMatchmaking();", "panorama/layout/base.xml", 8, 10, false);
					
				}
				
 			   
			}		/*





			/*



			/*
			
				LobbyAPI.StartMatchmaking(	MyPersonaAPI.GetMyOfficialTournamentName(),MyPersonaAPI.GetMyOfficialTeamName(),_GetTournamentOpponent(),_GetTournamentStage());
			
			*/	
		}

		ImGui::End();
	}
}
 