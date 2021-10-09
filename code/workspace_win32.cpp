
// #ifdef	STD_INCLUDE_DECL
// #endif	// STD_INCLUDE_DECL
// #ifdef	STD_INCLUDE_FUNC
// #endif	// STD_INCLUDE_FUNC

#include <math.h> // sqrtf, cosf, sinf
#include <stdint.h> // type definitions
#include <stdlib.h> // rand
#include <time.h> // time
#include <cstdio> // sprintf
#include <intrin.h>
#include <dsound.h>
#include <windows.h>
#include <xinput.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "assetID_generated.h"
#include "camera_config.h"
#include "typeDef.h"
#include "math.cpp"

#define 	STD_INCLUDE_DECL
#include "vector.cpp"
#include "vertex.cpp"
#include "memory.cpp"
#include "font.cpp"
#include "userInput.cpp"
#include "mesh.cpp"
#include "audioSystem.cpp"
#include "renderSystem.cpp"
#include "assetSystem.cpp"
#include "draw.cpp"
#include "direct3d.cpp"
#include "debug_win32.cpp"
#include "profile.cpp"
#include "player.cpp"

#include "win32_platform.cpp"
#undef	STD_INCLUDE_DECL

#define	STD_INCLUDE_FUNC
#include "vector.cpp"
#include "vertex.cpp"
#include "memory.cpp"
#include "font.cpp"
#include "userInput.cpp"
#include "audioSystem.cpp"
#include "renderSystem.cpp"
#include "mesh.cpp"
#include "assetSystem.cpp"
#include "draw.cpp"
#include "direct3d.cpp"
#include "debug_win32.cpp"
#include "profile.cpp"

#include "shader1.cpp"

#include "parser.cpp"
#include "tools.cpp"

#include "undo.cpp"

#include "camera.h"
#include "player.cpp"
#include "texCoord.cpp"
#include "editor_tools.cpp"
#include "editor.cpp"
#include "camera_tools.cpp"
#include "camera.cpp"
#include "defaultFont.cpp"

#include "win32_platform.cpp"
#undef	STD_INCLUDE_FUNC