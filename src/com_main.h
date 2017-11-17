enum 
{
	ERR_NONE,			/* Just print to console */
	ERR_FATAL,			/* Kill the game */
	ERR_DROP,			/* Come back to menu */
};

void Com_Init();
void Com_Frame();
void Com_Shutdown();

char * va(const char * fmt, ...);

void Com_Printf(char * fmt, ...);

#define Com_Error(level, fmt, ...) \
	Com_ErrorEx(level, va("%s:%d -> %s()", __FILE__, __LINE__, __FUNCTION__), va(fmt, __VA_ARGS__));

void Com_ErrorEx(int level, char * source, char * msg);