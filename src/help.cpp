#include "ANSI.h"

#define Y(s)	ANSI_YELLOW s ANSI_RESET
#define G(s)	ANSI_GREEN s ANSI_RESET
#define B(s)	ANSI_BOLD s ANSI_RESET

extern const char help[] =
G(B("Usage: ")) Y("%s") " [options]\n"
"  " B("--help")", " B("-h") " ...................... Print program manual.\n"
"  " B("--color <params>") ", " B("-c <params>")" ... Set params for a colored object.\n"
"                                     Format: '<color>(<α0>,<α1>,<β0>,<β1>,<γ>)'\n"
"\n"
;