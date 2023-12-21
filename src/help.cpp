#include "ANSI.h"

#define Y(s)	ANSI_YELLOW s ANSI_RESET
#define G(s)	ANSI_GREEN s ANSI_RESET
#define B(s)	ANSI_BOLD s ANSI_RESET

extern const char help[] =
G(B("Usage: ")) Y("%s") " [options]\n"
"  " B("--help")", " B("-h") " ...................... Print program manual.\n"
"  " B("--output") " <path>, " B("-o") " <path> ...... Set output file path for storing binary simulation data for each step.\n"
"                                     Use '-' for stdout.\n"
"  " B("--color <params>") ", " B("-c <params>")" ... Set params for a colored object.\n"
"                                     Format: '<color_1>(<color_2> : <α0>, <α1>, <β0>, <β1>, <γ>)'\n"
"                                      where <color_1> represents the color of the object\n"
"                                      and <color_2> the observed color in the projection field.\n"
"  " B("--prey") " <n>, " B("-a") " <n>\n"
"  " B("--predator") " <n>, " B("-b") " <n>\n"
"\n"
;