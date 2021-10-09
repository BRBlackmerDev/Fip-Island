
//----------
// GOALS
//  - 3D Platformer (SuperMario64)
//  - robust 3D collision
//  - robust 3D camera
//  - controller support
// 
// TODO
// - build sample level
// - smoother movement and camera motion
// - camera collision
//
// SAMPLE LEVEL
//  - small cabin
//  - covered bridge
//  - cliff vista
//  - canyon/trench
//  - cliff climb
//
// TERRAIN GENERATION PARAMETERS
//  - direction
//  - distance between
//  - size
//  - height variance
//  - shape
//  - slope direction
//  - slope angle
//  - recovery
//
// SAMPLE LEVEL : MOVEMENT
//  - continuous wall jump
//  - multiple sliding angles
//  - slide -> wall jump
//  - collision for player head
//  - wall jump around corner
//
// - add error checking to mesh import
// - import mesh to editor
// - mark up geometry in editor
// - [optional] improve texturing in blender
// - [optional] pop up panels for game parameters
//
// - if tex file exists, read tex coords from tex file
// - if tex file does not exist
//  - if tex coords exist in x file, read tex coords from x file, write tex coords to tex file
//  - if tex coords do not exist in x file, gen tex coords, write tex coords to tex file
// - tex file format: binary, uint32 texCoord_count, vec2 texCoord[] (tex coords are organized into triangles, meaning texCoord_count == ( face_count * 3 ) )
// - NOTE OF INTEREST : if an x file is saved from Blender, then position coords and tex coords will already have been unfolded into triangles, meaning we can ignore faces

// shared edge means that the 2 indices of the edge vertices are the same (if they don't have the same endpoints, then the edge is not shared (ie. edges are adjacent, but one edge is longer than the other)
// neighbor face : if normals align then non shared edges are added to island
//----------

struct INT_RANGE {
    int32 min;
    int32 max;
};

struct FLOAT_RANGE {
    flo32 min;
    flo32 max;
};

struct TEXTURE_DATA {
    int32 width;
    int32 height;
    void * memory;
};

struct COLLISION_OVERHANG {
    vec3 point0;
    vec3 point1;
};

internal COLLISION_OVERHANG
CollisionOverhang( vec3 point0, vec3 point1 ) {
    COLLISION_OVERHANG result = {};
    result.point0 = point0;
    result.point1 = point1;
    return result;
}

struct COLLISION_LEDGE {
    vec3 point0;
    vec3 point1;
    int32 link0;
    int32 link1;
};

internal COLLISION_LEDGE
CollisionLedge( vec3 point0, vec3 point1, int32 link0, int32 link1 ) {
    COLLISION_LEDGE result = {};
    result.point0 = point0;
    result.point1 = point1;
    result.link0  = link0;
    result.link1  = link1;
    return result;
}

struct COLLISION_POLE {
    vec3 point0;
    vec3 point1;
    vec3 normal;
};

internal COLLISION_POLE
CollisionPole( vec3 point0, vec3 point1, vec3 normal ) {
    COLLISION_POLE result = {};
    result.point0 = point0;
    result.point1 = point1;
    result.normal = normal;
    return result;
}

#define COLLISION__TRI_COUNT   ( 65536 )
#define COLLISION__EDGE_COUNT  ( 8192 )
#define COLLISION__OVERHANG_COUNT  ( 128 )
#define COLLISION__LEDGE_COUNT     ( 128 )
struct COLLISION_SYSTEM {
    vec3  tri_vertex[ COLLISION__TRI_COUNT * 3 ];
    int32 tri_count;
    
    vec3  edge_vertex[ COLLISION__EDGE_COUNT * 2 ];
    int32 edge_count;
    
    uint32             overhang_count;
    COLLISION_OVERHANG overhang[ COLLISION__OVERHANG_COUNT ];
    
    uint32          ledge_count;
    COLLISION_LEDGE ledge[ COLLISION__LEDGE_COUNT ];
    
#define COLLISION__POLE_COUNT  ( 128 )
    uint32          pole_count;
    COLLISION_POLE  pole[ COLLISION__POLE_COUNT ];
};

struct DEBUG_COLLISION_DATA {
    uint32 iter;
    boo32  isWalking;
    
    vec3 pos;
    vec3 dPos;
    
    uint32 tri_index;
    boo32  isValid;
    vec3   vertex[ 3 ];
    flo32  t;
    flo32  dist;
    vec3   point;
    vec3   normal;
    boo32  isWalkable;
};

struct DEBUG_COLLISION_QUERY {
    flo32 t0;
    flo32 t_;
    flo32 t_length;
    flo32 t;
    
    flo32 u;
    flo32 v;
    flo32 w;
    
    flo32 dist_u;
    flo32 dist_v;
    flo32 dist_w;
};

#define DEBUG_COLLISION_SYSTEM__QUERY_COUNT  ( 8 )
#define DEBUG_COLLISION_SYSTEM__DATA_COUNT  ( 8 * 60 * 4 )
struct DEBUG_COLLISION_SYSTEM {
    uint32 index;
    DEBUG_COLLISION_DATA  data [ DEBUG_COLLISION_SYSTEM__DATA_COUNT  ];
    
    uint32 query_count;
    DEBUG_COLLISION_QUERY query[ DEBUG_COLLISION_SYSTEM__QUERY_COUNT ][ DEBUG_COLLISION_SYSTEM__DATA_COUNT  ];
};

struct COLLISION_TRIANGLE {
    uint32 index;
    boo32  isValid;
    
    vec3 * vertex;
    vec3   collision_point;
    vec3   collision_normal;
    flo32  collision_t;
    flo32  collision_dist;
    
    boo32  isWalkable;
};

#define TERRAIN_GENERATION__TERRAIN_COUNT      ( 10 )
#define TERRAIN_GENERATION__VERTEX_PER_TERRAIN ( 30 )
struct TERRAIN_GENERATION {
    uint32 terrain_count;
    
    VERTEX1 * render_vertex;
    vec3    * collision_vertex;
    uint32 vertex_count;
    
    vec3 position_start;
    vec3 position_end;
};

#define SECURE_DOOR_LIST__SECURE_DOOR_COUNT  ( 16 )
struct SECURE_DOOR_LIST {
    uint32     nSecureDoor;
    SECURE_DOOR secureDoor[ SECURE_DOOR_LIST__SECURE_DOOR_COUNT ];
};

internal void
addSecureDoor( SECURE_DOOR_LIST * s, SECURE_DOOR door ) {
    Assert( s->nSecureDoor < SECURE_DOOR_LIST__SECURE_DOOR_COUNT );
    s->secureDoor[ s->nSecureDoor++ ] = door;
}

internal ORIENTED_BOX
transformOBox( mat4 transform, ORIENTED_BOX b ) {
    ORIENTED_BOX result = b;
    result.center = ( transform * Vec4( b.center, 1.0f ) ).xyz;
    result.xAxis  = ( transform * Vec4( b.xAxis,  0.0f ) ).xyz;
    result.yAxis  = ( transform * Vec4( b.yAxis,  0.0f ) ).xyz;
    result.zAxis  = ( transform * Vec4( b.zAxis,  0.0f ) ).xyz;
    return result;
}

internal SECURE_DOOR
transformSecureDoor( mat4 transform, SECURE_DOOR door ) {
    SECURE_DOOR result = door;
    
    ORIENTED_BOX b = transformOBox( transform, result.render_bound );
    result.render_bound    = b;
    result.collision_bound = b;
    result.activate_bound  = b;
    result.activate_bound.halfDim.z = door.activate_bound.halfDim.z;
    
    return result;
}

internal boo32
canActivateSecureDoor( PLAYER_STATE * playerState, SECURE_DOOR door ) {
    boo32 result = true;
    for( uint32 type_index = 0; type_index < 4; type_index++ ) {
        if( playerState->pixie_count[ type_index ] < door.pixie_cost[ type_index ] ) {
            result = false;
        }
    }
    return result;
}

internal uint32
getCost( SECURE_DOOR door ) {
    uint32 result = 0;
    for( uint32 type_index = 0; type_index < 4; type_index++ ) {
        result += door.pixie_cost[ type_index ];
    }
    return result;
}

#define PIXIE_LIST__PIXIE_COUNT  ( 128 )
struct PIXIE_LIST {
    uint32 nPixie;
    PIXIE   pixie[ PIXIE_LIST__PIXIE_COUNT ];
};

internal void
add( PIXIE_LIST * pixieList, PIXIE pixie ) {
    Assert( pixieList->nPixie < PIXIE_LIST__PIXIE_COUNT );
    pixieList->pixie[ pixieList->nPixie++ ] = pixie;
}

#define KILL_ZONE_LIST__KILL_ZONE_COUNT  ( 128 )
struct KILL_ZONE_LIST {
    uint32 nKillZone;
    KILL_ZONE   killZone[ KILL_ZONE_LIST__KILL_ZONE_COUNT ];
};

internal void
add( KILL_ZONE_LIST * killZoneList, KILL_ZONE killZone ) {
    Assert( killZoneList->nKillZone < KILL_ZONE_LIST__KILL_ZONE_COUNT );
    killZoneList->killZone[ killZoneList->nKillZone++ ] = killZone;
}

#define CHECKPOINT_LIST__CHECKPOINT_COUNT  ( 128 )
struct CHECKPOINT_LIST {
    uint32 nCheckpoint;
    CHECKPOINT   checkpoint[ CHECKPOINT_LIST__CHECKPOINT_COUNT ];
};

internal void
add( CHECKPOINT_LIST * checkpointList, CHECKPOINT checkpoint ) {
    Assert( checkpointList->nCheckpoint < CHECKPOINT_LIST__CHECKPOINT_COUNT );
    checkpointList->checkpoint[ checkpointList->nCheckpoint++ ] = checkpoint;
}

#define VIEW_LIST__VIEW_COUNT  ( 128 )
struct VIEW_LIST {
    uint32 nView;
    VIEW   view[ VIEW_LIST__VIEW_COUNT ];
};

internal void
add( VIEW_LIST * viewList, VIEW view ) {
    Assert( viewList->nView < VIEW_LIST__VIEW_COUNT );
    viewList->view[ viewList->nView++ ] = view;
}

struct APP_STATE {
	boo32 isRunning;
	boo32 isInitialized;
    boo32 isPaused;
    
    vec2  mouse_position;
    boo32 mouseLeft_wasPressed;
    boo32 mouseLeft_wasReleased;
    boo32 mouseRight_wasPressed;
    boo32 mouseRight_wasReleased;
    boo32 mouseMiddle_wasPressed;
    boo32 mouseMiddle_wasReleased;
    int32 mouseMiddle_wheelClicks;
	
	vec2 app_dim;
	vec2 app_halfDim;
	rect app_bound;
    
    flo32 camera_hRadians;
    flo32 camera_vRadians;
    boo32 camera_moveCamera;
    boo32 camera_rotateCamera;
    vec3  camera_position;
    vec3  camera_velocity;
    vec3  camera_destination;
    vec3  camera_xAxis;
    vec3  camera_yAxis;
    vec3  camera_zAxis;
    flo32 camera_hOffset;
    flo32 camera_vOffset;
    flo32 camera_dist;
    flo32 camera_maxDist;
    flo32 camera_fovAngle;
    
    flo32 camera_hRadians_dest;
    flo32 camera_hRadians_vel;
    
    boo32  modifyCamera;
    boo32  modCamera_modValue;
    uint32 modCamera_modIndex;
    flo32  modCamera_hOffset;
    flo32  modCamera_vOffset;
    flo32  modCamera_maxDist;
    flo32  modCamera_fovAngle;
    
    boo32 camera_left;
    boo32 camera_right;
    boo32 camera_up;
    boo32 camera_down;
    
    boo32 camera_moveLeft;
    boo32 camera_moveRight;
    boo32 camera_moveForward;
    boo32 camera_moveBackward;
    
    flo32 camera_horRatio;
    flo32 camera_verRatio;
    
    vec2   input_ratio;
    uint32 input_speed;
    uint32 input_speedX;
    uint32 input_speedY;
    
    PLAYER_CONTROL playerControl;
    PLAYER_STATE   playerState;
    vec3           player_resetPosition;
    
    boo32 spawn_useSpawnPoint;
    vec3  spawn_spawnPoint;
    
#define COLLISION__RENDER_DATA_COUNT ( 8192 )
    RENDER_DATA renderData[ COLLISION__RENDER_DATA_COUNT ];
    int32       renderData_count;
    
    COLLISION_SYSTEM collisionSystem;
    DEBUG_COLLISION_SYSTEM debugCollisionSystem;
    
    SECURE_DOOR_LIST secureDoorList;
    PIXIE_LIST       pixieList;
    KILL_ZONE_LIST   killZoneList;
    CHECKPOINT_LIST  checkpointList;
    VIEW_LIST        viewList;
    
    TERRAIN_GENERATION terrain_generation;
    
    flo32 maxHeight;
    
    boo32 debug_showFaceIndex;
    flo32 debug_distFromWall;
};

enum SELECT_MODE {
    selectMode_vertex,
    selectMode_edge,
    selectMode_face,
    
    selectMode_count,
};

enum EDITOR_MODE {
    editorMode_default,
    
    editorMode_setEntityID,
    editorMode_openEntity,
    
    editorMode_addLevel,
    
    editorMode_utilityPanel_textBox,
    
    editorMode_translateSelection,
    editorMode_rotateSelection,
    editorMode_scaleSelection,
    
    editorMode_placeEntity,
    editorMode_rotateDoor,
    editorMode_sizeDoor,
    
    editorMode_rotateKillZone,
    editorMode_sizeKillZone,
    
    editorMode_rotateCheckpoint,
    editorMode_sizeCheckpoint,
    
    editorMode_sizeView_hi,
    editorMode_sizeView_lo,
};

enum ENTITY_MODE {
    entityMode_spawn,
    
    entityMode_ledge,
    entityMode_door,
    entityMode_pixie,
    entityMode_killZone,
    entityMode_checkpoint,
    entityMode_view,
};

enum TRANSLATE_MODE {
    translateMode_twoAxis,
    translateMode_oneAxis,
};

#define  INPUT_STRING__CHAR_COUNT  ( 32 )
struct INPUT_STRING {
    uint32 maxCount;
    uint32 count;
    uint32 cursor;
    char   string[ INPUT_STRING__CHAR_COUNT ];
};

internal void
addChar( INPUT_STRING * input_string, char c ) {
    if( ( input_string->count < input_string->maxCount ) && ( input_string->cursor < input_string->maxCount ) ) {
        for( int32 iter = ( int32 )input_string->count; iter >= ( int32 )input_string->cursor; iter-- ) {
            input_string->string[ iter + 1 ] = input_string->string[ iter ];
        }
        
        input_string->string[ input_string->cursor ] = c;
        input_string->cursor++;
        input_string->string[ ++input_string->count ] = 0;
    }
}

internal void
deleteChar_backspace( INPUT_STRING * input_string ) {
    if( ( input_string->count > 0 ) && ( input_string->cursor > 0 ) ) {
        for( uint32 iter = input_string->cursor; iter <= input_string->count; iter++ ) {
            input_string->string[ iter - 1 ] = input_string->string[ iter ];
        }
        input_string->string[ --input_string->count ] = 0;
        input_string->cursor--;
    }
}

internal void
deleteChar_delete( INPUT_STRING * input_string ) {
    if( ( input_string->count > 0 ) && ( input_string->cursor < input_string->count ) ) {
        for( uint32 iter = input_string->cursor; iter < ( input_string->count - 1 ); iter++ ) {
            input_string->string[ iter ] = input_string->string[ iter + 1 ];
        }
        input_string->string[ --input_string->count ] = 0;
    }
}

internal void
clear( INPUT_STRING * input_string ) {
    memset( input_string->string, 0, input_string->count );
    input_string->count = 0;
}

internal void
init( INPUT_STRING * input_string, uint32 maxCount = INPUT_STRING__CHAR_COUNT ) {
    Assert( maxCount <= INPUT_STRING__CHAR_COUNT );
    
    clear( input_string );
    input_string->maxCount = maxCount;
    input_string->count    = 0;
    input_string->cursor   = 0;
}

internal void
setCursor( INPUT_STRING * input_string, int32 cursor_index ) {
    input_string->cursor = clamp( cursor_index, 0, input_string->count );
}

internal void
setCursor( INPUT_STRING * i, vec2 pos, rect bound, flo32 char_width ) {
    int32 cursor_index = ( int32 )( ( pos.x - bound.left ) / char_width );
    setCursor( i, cursor_index );
}

struct CENTROID {
    boo32 isValid;
    vec3  position;
};

struct DIRECTION {
    boo32 isValid;
    vec3  direction;
};

struct TEXT_BOX_F32 {
    INPUT_STRING * input;
    flo32        * output;
    
    rect  bound;
};

internal TEXT_BOX_F32
TextBox( rect bound, flo32 * output ) {
    Assert( output );
    
    TEXT_BOX_F32 result = {};
    result.output = output;
    result.bound  = bound;
    return result;
}

enum UTILITY_PANEL__TEXT_BOX_ID {
    utilityPanel_textBoxID_centroidX,
    utilityPanel_textBoxID_centroidY,
    utilityPanel_textBoxID_centroidZ,
    
    utilityPanel_textBoxID_scale,
    utilityPanel_textBoxID_degrees,
    
    utilityPanel_textBoxID_pivotX,
    utilityPanel_textBoxID_pivotY,
    utilityPanel_textBoxID_pivotZ,
    
    utilityPanel_textBoxID_directionX,
    utilityPanel_textBoxID_directionY,
    utilityPanel_textBoxID_directionZ,
    
    utilityPanel_textBoxID_count,
};

struct UTILITY_PANEL {
    rect bound;
    
    boo32 grid_isActive;
    rect  grid_isActive_bound;
    
    boo32 pivot_isActive;
    rect  pivot_isActive_bound;
    rect  pivot_setPivot_bound;
    
    rect  direction_setDirection_bound;
    
    CENTROID centroid;
    flo32 scale;
    flo32 degrees;
    vec3  pivot;
    vec3  direction;
    
    flo32 text_charWidth;
    TEXT_BOX_F32 * active;
    
    TEXT_BOX_F32 textBox[ utilityPanel_textBoxID_count ];
};

struct LEVEL {
    uint32 entityID;
    
    uint32 bPoint;
    uint32 nPoint;
    
    uint32 bFace;
    uint32 nFace;
    
    uint32 bDoor;
    uint32 nDoor;
    uint32 bKillZone;
    uint32 nKillZone;
    uint32 bCheckpoint;
    uint32 nCheckpoint;
    
    vec3 translation;
    vec3 rotation;
};

struct LEVEL_PANEL {
    rect bound;
    vec2 addButton_dim;
    
    vec2 entry_dim;
    
#define LEVEL_PANEL__LEVEL_COUNT  ( 16 )
    uint32 nLevel;
    LEVEL   level           [ LEVEL_PANEL__LEVEL_COUNT ];
    boo32   level_isSelected[ LEVEL_PANEL__LEVEL_COUNT ];
};

internal void
addLevel( LEVEL_PANEL * l, LEVEL level ) {
    Assert( l->nLevel < LEVEL_PANEL__LEVEL_COUNT );
    l->level[ l->nLevel++ ] = level;
}

internal void
init( INPUT_STRING * i, TEXT_BOX_F32 * t, UTILITY_PANEL * u, MOUSE_STATE * mouse, uint32 max ) {
    if( u->active ) {
        u->active->input = 0;
    }
    
    t->input  = i;
    u->active = t;
    
    Assert( t->output );
    sprintf( i->string, "%f", *( t->output ) );
    uint32 cursor_index = ( uint32 )( ( mouse->position.x - t->bound.left ) / u->text_charWidth );cursor_index = clamp( cursor_index, 0, max );
    
    i->maxCount = max;
    i->count    = ( uint32 )strlen( i->string );
    i->cursor   = cursor_index;
}

internal flo32
roundTo( flo32 value, flo32 scale ) {
    Assert( scale > 0.0f );
    
    flo32 offset = scale * 0.5f;
    if( value < 0 ) { offset = -offset; }
    
    int32 count  = ( int32 )( ( value + offset ) / scale );
    // DISPLAY_VALUE( int32, count );
    flo32 result = ( flo32 )count * scale;
    return result;
}

#define ENTITY_PANEL__BUTTON_COUNT  ( 32 )
struct ENTITY_PANEL {
    rect bound;
    
    int32 hover;
    int32 active;
    
    int32 nButton;
    rect  button_bound[ ENTITY_PANEL__BUTTON_COUNT ];
};

struct CAMERA {
    vec3  position;
    flo32 fovAngle;
    flo32 nearPlane;
    flo32 farPlane;
    
    flo32 hRadians;
    flo32 vRadians;
    vec3  xAxis;
    vec3  yAxis;
    vec3  zAxis;
    flo32 dist;
    
    boo32 isOrthographic;
    vec2  ortho_center;
    vec2  ortho_dim;
    
    boo32 mouseOrbit_isActive;
    boo32 mouseMove_isActive;
    
    boo32 moveSlow_isActive;
    boo32 move_left;
    boo32 move_right;
    boo32 move_forward;
    boo32 move_backward;
    boo32 move_up;
    boo32 move_down;
};

struct EDITOR__SECURE_DOOR {
    uint32 point_index;
    flo32  height;
    flo32  radians;
    
    uint32 pixie_cost[ 4 ];
};

struct EDITOR__PIXIE {
    uint32        point_index;
    PIXIE_TYPE_ID typeID;
};

struct EDITOR__LEDGE {
    uint32 point_index[ 2 ];
};

struct EDITOR__KILL_ZONE {
    uint32 point_index;
    vec3   halfDim;
    flo32  radians;
};

struct EDITOR__CHECKPOINT {
    uint32 point_index;
    uint32 respawn_index;
    vec3   halfDim;
    flo32  radians;
};

struct EDITOR__VIEW {
    VIEW_TYPE type;
    
    uint32 pIndex;
    uint32 qIndex;
    flo32  height;
    flo32  radians;
    
    flo32 lo;
    flo32 hi;
    flo32 maxDist;
    flo32 hOffset;
    flo32 vOffset;
    flo32 fovAngle;
};

struct EDITOR_STATE {
	boo32 isInitialized;
    
    vec2 mouse_position;
    vec2 mouse_prevPos;
    boo32 mouseLeft_wasPressed;
    boo32 mouseLeft_wasReleased;
    boo32 mouseRight_wasPressed;
    boo32 mouseRight_wasReleased;
    boo32 mouseMiddle_wasPressed;
    boo32 mouseMiddle_wasReleased;
    int32 mouseMiddle_wheelClicks;
	
	vec2 app_dim;
	vec2 app_halfDim;
	rect app_bound;
    
    CAMERA camera;
    
    flo32 modCamera_maxDist;
    flo32 modCamera_hOffset;
    flo32 modCamera_vOffset;
    flo32 modCamera_fovAngle;
    
    boo32 boxSelect_isEnabled;
    boo32 boxSelect_isActive;
    vec2  boxSelect_position;
    rect  boxSelect_bound;
    
#define EDITOR__POINT_COUNT  ( 4096 )
    int32 point_count;
    vec3  point           [ EDITOR__POINT_COUNT ];
    boo32 point_isSelected[ EDITOR__POINT_COUNT ];
    
#define EDITOR__EDGE_COUNT  ( 4096 )
    int32         edge_count;
    EDGE__        edge           [ EDITOR__EDGE_COUNT ];
    vec3          edge_centroid  [ EDITOR__EDGE_COUNT ];
    boo32         edge_isSelected[ EDITOR__EDGE_COUNT ];
    
#define EDITOR__FACE_COUNT  ( 2048 )
    int32         face_count;
    FACE          face           [ EDITOR__FACE_COUNT ];
    vec3          face_normal    [ EDITOR__FACE_COUNT ];
    vec3          face_centroid  [ EDITOR__FACE_COUNT ];
    boo32         face_isSelected[ EDITOR__FACE_COUNT ];
    uint32        face_color     [ EDITOR__FACE_COUNT ];
    
#define EDITOR__OVERHANG_COUNT  ( 1024 )
    int32    overhang_count;
    OVERHANG overhang           [ EDITOR__OVERHANG_COUNT ];
    boo32    overhang_isSelected[ EDITOR__OVERHANG_COUNT ];
    
#define EDITOR__LEDGE_COUNT  ( 1024 )
    uint32       nLedge;
    EDITOR__LEDGE ledge[ EDITOR__LEDGE_COUNT ];
    
#define EDITOR__POLE_COUNT  ( 1024 )
    int32 pole_count;
    POLE  pole           [ EDITOR__POLE_COUNT ];
    boo32 pole_isSelected[ EDITOR__POLE_COUNT ];
    
#define EDITOR__SECURE_DOOR_COUNT  ( 32 )
    uint32             nSecureDoor;
    EDITOR__SECURE_DOOR secureDoor_temp;
    EDITOR__SECURE_DOOR secureDoor           [ EDITOR__SECURE_DOOR_COUNT ];
    
#define EDITOR__PIXIE_COUNT  ( 32 )
    uint32       nPixie;
    EDITOR__PIXIE pixie[ EDITOR__PIXIE_COUNT ];
    
#define EDITOR__KILL_ZONE_COUNT  ( 32 )
    uint32           nKillZone;
    EDITOR__KILL_ZONE killZone_temp;
    EDITOR__KILL_ZONE killZone[ EDITOR__KILL_ZONE_COUNT ];
    uint32            sizeKillZone_axis;
    
#define EDITOR__CHECKPOINT_COUNT  ( 32 )
    uint32            nCheckpoint;
    EDITOR__CHECKPOINT checkpoint_temp;
    EDITOR__CHECKPOINT checkpoint[ EDITOR__CHECKPOINT_COUNT ];
    uint32             sizeCheckpoint_axis;
    
#define EDITOR__VIEW_COUNT  ( 128 )
    uint32      nView;
    EDITOR__VIEW view_temp;
    EDITOR__VIEW view[ EDITOR__VIEW_COUNT ];
    uint32       sizeView_axis;
    
    rect  displayWindow_bound;
    rect  toolWindow_bound;
    flo32 toolWindow_translationValue;
    rect  textBox_bound;
    rect  doorPanel_bound;
    
    UTILITY_PANEL utilityPanel;
    ENTITY_PANEL  entityPanel;
    LEVEL_PANEL   levelPanel;
    
    char  entityDir[ 32 ];
    int32 entityFile_count;
    int32 entityID;
    boo32 entity_showUnsavedChangesMessage;
    boo32 entity_hasUnsavedChanges;
    boo32 entity_isNewEntity;
    
    int32 entity_select;
    
    UNDO_SYSTEM undoSystem;
    
    EDITOR_MODE    editorMode;
    ENTITY_MODE    entityMode;
    SELECT_MODE    selectMode;
    TRANSLATE_MODE translateMode;
    
    INPUT_STRING input_string;
    
    vec3   translate_origin;
    vec3   translate_axis;
    vec3   transform_basePos;
    vec3 * translate_tempPos;
    
    vec3  translate_xAxis;
    vec3  translate_yAxis;
    
    boo32 scale_selectBasePoint;
    vec3  scale_basePoint;
    boo32 scale_axisIsEnabled[ 3 ];
    
    boo32 spawn_useSpawnPoint;
    vec3  spawn_spawnPoint;
    
    boo32 drawFaces;
    
    boo32 levelEditor_isActive;
};