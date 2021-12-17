#include "materials.hpp"
void materials::setup() {
	std::ofstream("csgo\\materials\\vitality_regular.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\vitality_rubber.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white"
  "$envmap" "env_cubemap"
  "$normalmapalphaenvmapmask" 1
  "$envmapcontrast" 1
  "$model" 1
  "$selfillum" 1
}
)#";
	std::ofstream("csgo\\materials\\vitality_metallic.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast"             "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
}
void materials::remove() 
{
	std::remove("csgo\\materials\\vitality_regular.vmt");
	std::remove("csgo\\materials\\vitality_rubber.vmt");
	std::remove("csgo\\materials\\vitality_metallic.vmt");
}