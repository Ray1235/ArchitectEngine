#include "common.h"
#include "com_discord.h"
#ifdef _USE_DISCORD
#include "com_discord_secret.h"

void __internal_Com_Discord_Ready()
{
	Com_Error(ERR_DROP, "Discord ready!", "");
}

void Com_Discord_Init()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = __internal_Com_Discord_Ready;
	Discord_Initialize(APPLICATION_ID, &handlers, 1, STEAM_APP_ID);
}

void Com_Discord_UpdatePresence()
{
	char buffer[256];
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "In Menu";
	discordPresence.details = "In Default Game";
	discordPresence.largeImageKey = "default-large";
	discordPresence.smallImageKey = "default-small";
	discordPresence.instance = 1;
	Discord_UpdatePresence(&discordPresence);
}

void Com_Discord_Shutdown()
{
	Discord_Shutdown();
}
#endif

void Com_Discord_Init()
{
}

void Com_Discord_UpdatePresence()
{
}

void Com_Discord_Shutdown()
{
}
