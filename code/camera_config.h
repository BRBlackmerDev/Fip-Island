
// F9 : Toggle Editor
#define APP_targetFPS			60.0f
#define APP_permMemorySize		_MB( 256 )
#define APP_tempMemorySize		_MB( 256 )

#define APP_width         ( 1200.0f )
#define APP_height        (  708.0f )
#define APP_isFullscreen  ( 0 )

#define MUSIC__MIX_VOLUME  ( 1.0f )
#define AUDIO__MIX_VOLUME  ( 1.0f )

#define DIRECTION_TO_LIGHT  ( getNormal( Vec3( -0.2f, 1.0f, 0.5f ) ) )

#define PLAYER__HEIGHT_STAND  ( 1.7272f )
#define PLAYER__RESPAWN_TARGET_TIME  ( 1.0f )

#define DOT__DEGENERATE_EPSILON  ( 0.000001f )
#define DOT__COLLINEAR_EPSILON   ( 0.000001f )
#define DOT__ORTHOGONAL_EPSILON  ( 0.000001f )

#define COLLISION__BORDER_EPSILON               ( 0.0005f )
#define COLLISION__OFFSET_FROM_COLLISION_PLANE  ( 0.0006f )
#define COLLISION__JUMP_THRESHOLD               ( 0.00075f )
#define COLLISION__WALL_JUMP_THRESHOLD          ( 0.035f )
#define COLLISION__IS_WALKING_THRESHOLD         ( 0.1f )

#define COLLISION__LEDGE_HEIGHT                 ( 0.5f )
#define COLLISION__LEDGE_DEPTH                  ( 0.5f )
#define COLLISION__LEDGE_DEPTH_EPSILON          ( 0.001f )
#define COLLISION__LEDGE_JUMP_HEIGHT            ( 0.25f )
#define COLLISION__LEDGE_OFFSET_FROM_WALL       ( 0.001f )

#define COLLISION__POLE_WIDTH            ( 0.5f )
#define COLLISION__POLE_DEPTH            ( 0.5f )
#define COLLISION__POLE_DEPTH_EPSILON    ( 0.001f )
#define COLLISION__POLE_SUBTRACT_HEIGHT_FOR_ENTRY  ( 0.5f )
#define COLLISION__POLE_OFFSET_BOTTOM     ( 0.25f )

#define JUMP__COYOTE_TIME                ( 0.1f )

#define SECURE_DOOR__MAX_PIXIE_COST        ( 12 )
#define SECURE_DOOR__MAX_WIDTH             ( 12.0f )
#define SECURE_DOOR__MAX_HALF_WIDTH        ( SECURE_DOOR__MAX_WIDTH * 0.5f )
#define SECURE_DOOR__ACTIVATE__DEPTH       ( 10.0f )
#define SECURE_DOOR__ACTIVATE__HALF_DEPTH  ( SECURE_DOOR__ACTIVATE__DEPTH * 0.5f )
#define SECURE_DOOR__RENDER__DEPTH         ( 1.0f )
#define SECURE_DOOR__RENDER__HALF_DEPTH    ( SECURE_DOOR__RENDER__DEPTH * 0.5f )

#define PLAYER_CONTROL__POLE_GRAB__TARGET_TIME  ( 0.25f )

#define ENABLE_TERRAIN_GENERATION  ( 0 )

#define PIXIE__COLOR_HI__GROW    ( Vec4( 0.1f, 1.0f, 0.1f, 1.0f ) )
#define PIXIE__COLOR_HI__SIGHT   ( COLOR_YELLOW )
#define PIXIE__COLOR_HI__MEMORY  ( COLOR_ORANGE )
#define PIXIE__COLOR_HI__EMPTY   ( Vec4( 0.2f, 0.2f, 1.0f, 1.0f ) )

#define PIXIE__COLOR_LO__GROW    ( Vec4( 0.0f, 0.4f,  0.0f, 1.0f ) )
#define PIXIE__COLOR_LO__SIGHT   ( Vec4( 0.4f, 0.4f,  0.0f, 1.0f ) )
#define PIXIE__COLOR_LO__MEMORY  ( Vec4( 0.4f, 0.2f, 0.0f, 1.0f ) )
#define PIXIE__COLOR_LO__EMPTY   ( Vec4( 0.0f, 0.0f,  0.6f, 1.0f ) )

#define PIXIE__COLOR_LO__ARRAY  { PIXIE__COLOR_LO__GROW, PIXIE__COLOR_LO__SIGHT, PIXIE__COLOR_LO__MEMORY, PIXIE__COLOR_LO__EMPTY }
#define PIXIE__COLOR_HI__ARRAY  { PIXIE__COLOR_HI__GROW, PIXIE__COLOR_HI__SIGHT, PIXIE__COLOR_HI__MEMORY, PIXIE__COLOR_HI__EMPTY }

#define PIXIE__PICK_UP__HALF_DIM  ( Vec3( 1.0f, 1.2f, 1.0f ) )

#define RAY_SELECT__RANGE  ( 0.15f )

// accel time     : walk : 0.25f
// accel time     : run  : 0.30f
// low jump dist  : walk :
// low jump dist  : run  :
// high jump dist : walk :
// high jump dist : run  : 