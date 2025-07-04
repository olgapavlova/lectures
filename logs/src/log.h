#ifndef LOG_H_LOADED
#define LOG_H_LOADED

#ifdef LOG_I
#define LOG_W
#endif

#ifdef LOG_W
#define LW(x) L("WARN", x)
#else
#define LW(x)
#endif

#ifdef LOG_I
#define LI(x) L("INFO", x)
#else
#define LI(x)
#endif

#ifdef LOG_ON
#define L(lvl, x) fprintf(stderr, "[%s] %s:%d %s %s\n", \
                        __TIME__, __FILE__, __LINE__, __FUNCTION__, x);
#else
#define L(lvl, x)
#endif

#endif
