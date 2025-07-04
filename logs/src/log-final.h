#ifndef LOG_H_LOADED
#define LOG_H_LOADED

#ifdef LOG_T
#define LOG_E
#define LOG_F
#endif

#ifdef LOG_E
#define LOG_F
#endif

#ifdef LOG_T
#define LT(x) L("trace", x)
#else
#define LT(x)
#endif

#ifdef LOG_E
#define LE(x) L("error", x)
#else
#define LE(x)
#endif

#ifdef LOG_F
#define LF(x) L("fatal", x)
#else
#define LF(x)
#endif

#ifdef LOG_ON
#define L(lvl, x) fprintf(stderr, "[%s] • %s:%d • %s | %s %s\n", \
                        __TIME__, __FILE__, __LINE__, __FUNCTION__, lvl, x);
#else
#define L(lvl, x)
#endif


#endif
