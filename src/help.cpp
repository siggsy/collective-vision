#include "ANSI.h"

#define Y(s)	ANSI_YELLOW s ANSI_RESET
#define G(s)	ANSI_GREEN s ANSI_RESET
#define B(s)	ANSI_BOLD s ANSI_RESET

extern const char help[] =
G(B("Usage: ")) Y("%s") " [options]\n"
"  " B("--help")", " B("-h") " ...................... Print program manual.\n"
"  " B("--output") " <path>, " B("-o") " <path> ...... Set output file path for storing binary simulation data for each step.\n"
"                                     Use '-' for stdout.\n"
"  " B("--steps") " <n>, " B("-n") " <n> ............. Number of steps to generate\n"
"  " B("--color") " <params>, " B("-c") " <params> ... Set params for a colored object.\n"
"                                     Format: '<color_1>(<color_2> : <α0>, <α1>, <β0>, <β1>, <γ>)'\n"
"                                      where <color_1> represents the color of the object\n"
"                                      and <color_2> the observed color in the projection field.\n"
"  " B("--boid") " <params>, " B("-b") " <params> .... Define boid type.\n"
"                                     Format: '(<count>, <color>, <size>, <prefSpeed>)'\n"
"                                      <count>      -> number of boids of this type\n"
"                                      <color>      -> color of the boids\n"
"                                      <size>       -> size of the boids\n"
"                                      <prefSpeed>  -> preferred speed of the boids\n"
"\n"
;
