#include "MetaInfo.h"
#include "Utilities.h"

void PrintMetaHeader()
{
	printf("                                  Av");
	Utilities::SetConsoleColor(FOREGROUND_INTENSE_GREEN);
	printf("oz\n");
	Utilities::SetConsoleColor(FOREGROUND_WHITE);
	Utilities::Log("Build Date: %s", __DATE__);
	Utilities::Log("Setting Up Mirror for %s", META_GAME);
}