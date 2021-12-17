#include "hooks.h"
//  virtual int RunScript(panorama::IUIPanel *panel,char const *entireJSCode,char const *pathToXMLContext,int,int,bool alreadyCompiled) = 0;

int Hooks::RunScript( panorama::UIEngine *thisptr, panorama::IUIPanel *panel, const char *entireJSCode, const char *pathToXMLContext, int int1, int int2, bool alreadyCompiled ) {
      //   uiEngineVMT->GetOriginalMethod<RunScriptFn>(110)( thisptr, panel, entireJSCode, pathToXMLContext, int1, int2, alreadyCompiled );
      if(entireJSCode)
      Util::Log( "--RunScript--\n Str1: %s\n", entireJSCode);
       //  Util::Log( "--RunScript--\n Panel: %s Str1: %s, Str2: (%s), int1: %d, int2: %d, bool1(%s)\n", panel->GetID(), entireJSCode, pathToXMLContext, int1, int2, alreadyCompiled? "yes" : "no" );
       if( strstr( pathToXMLContext, "popup_accept_match.xml" ) ){
         panorama::IUIPanel *root = (engine->IsInGame() ? GUI::hudRoot : GUI::menuRoot);
	    	  if(root){
 			    panoramaEngine->AccessUIEngine()->RunScript(root, "LobbyAPI.SetLocalPlayerReady( 'accept' );", "panorama/layout/base.xml", 8, 10, false);
     
          }
       }


     int code =  uiEngineVMT->GetOriginalMethod<RunScriptFn>(110)( thisptr, panel, entireJSCode, pathToXMLContext, int1, int2, alreadyCompiled );
     cvar->ConsoleDPrintf("RETURN CODE: %d\n", code);
     return code;
}