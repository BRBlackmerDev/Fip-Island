
struct EDITOR_STATE;
typedef void ( UNDO_CALLBACK )( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory );
typedef void ( REDO_CALLBACK )( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory );

struct UNDO_EVENT__TRANSLATE {
   vec3   translate_vector;
   uint32 index_count;
   // struct will be followed in memory by the list of vertex indices
};

struct UNDO_EVENT__ROTATE {
   uint32 nSelect;
   // struct will be followed in memory by:
   // 1) a list of selected vertex indices
   // 2) a list of previous vertex positions
   // 3) a list of new vertex positions
};

struct UNDO_EVENT__DUPLICATE_VERTEX {
   uint32 vertex_count;
   // struct will be followed in memory by the list of vertex indices
};

struct UNDO_EVENT__DUPLICATE_EDGE {
   uint32 vertex_count;
   uint32 edge_count;
   // struct will be followed in memory by the list of vertex indices and a list of edge indices
};

struct UNDO_EVENT__DUPLICATE_FACE {
   uint32 vertex_count;
   uint32 edge_count;
   uint32 face_count;
   // struct will be followed in memory by the list of vertex indices and a list of edge indices and a list of face indices
};

struct UNDO_EVENT__NEW_EDGE {
   EDGE__ edge;
};

struct UNDO_EVENT__NEW_FACE {
   FACE   face;
   uint32 oldEdge_count;
   uint32 newEdge_count;
};

struct UNDO_EVENT__EXTRUDE_VERTEX {
   uint32 nVert;
};

struct UNDO_EVENT__EXTRUDE_EDGE {
   uint32 nSelectEdge;
   
   uint32 mPoints;
   uint32 mEdges;
   uint32 mFaces;
   // struct will be followed in memory by:
   // 1) a list of edge indices that were extruded
   // 2) a list of point indices that were duplicated
   // 3) a list of new edges that were added
   // 4) a list of new faces that were added
};

struct UNDO_EVENT__FLIP_NORMAL {
   uint32 face_count;
   // struct will be followed in memory by a list of face indices
};

struct UNDO_EVENT__DELETE_SELECTION {
   uint32 vertex_count;
   uint32 edge_count;
   uint32 face_count;
};

struct UNDO_EVENT__DELETE_EDGES {
   uint32 edge_count;
   uint32 face_count;
   // struct will be followed in memory by:
   // - a list of edge indices
   // - the deleted edges
   // - a list of face indices
   // - the deleted faces
};

struct UNDO_EVENT__DELETE_FACES {
   uint32 face_count;
   // struct will be followed in memory by:
   // - a list of face indices
   // - the deleted faces
};

// redo translate : same as undo, but negate vector

struct UNDO_EVENT {
   UNDO_CALLBACK * undo_callback;
   REDO_CALLBACK * redo_callback;
   uint8 * data;
   uint32  data_size;
};

#define UNDO_SYSTEM__EVENT_COUNT  ( 256 )
#define UNDO_SYSTEM__BUMP_COUNT   ( UNDO_SYSTEM__EVENT_COUNT / 4 )
#define UNDO_SYSTEM__MEMORY_SIZE  ( _MB( 16 ) )
struct UNDO_SYSTEM {
   UNDO_EVENT event[ UNDO_SYSTEM__EVENT_COUNT ];
   uint32     event_atIndex;
   uint32     event_maxIndex;
   
   boo32   isInitialized;
   uint8 * memory_base;
   uint32  memory_at;
};

