#ifndef DEBUG_MARK
#define DEBUG_MARK
#define HALFMARK /

#ifdef enable_DEBUG_all
#define DEBUG HALFMARKHALFMARK
#ifndef enable_DEBUG_all DEBUG {}
#endif

#endif