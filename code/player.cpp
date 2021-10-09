
#ifdef	STD_INCLUDE_DECL
struct PLAYER_CONTROL {
   boo32 crouch_wasPressed;
   boo32 crouch_wasReleased;
   
   boo32 jump_wasPressed;
   boo32 jump_wasReleased;
   
   boo32 respawn_wasPressed;
   boo32 respawn_wasReleased;
};

struct PLAYER_STATE {
   boo32    crouch_isActive;
   boo32 ledgeGrab_isActive;
   int32 ledgeGrab_index;
   flo32 ledgeGrab_dotZ;
   boo32 canUseLedgeToJump;
   
   boo32 poleGrab_isActive;
   int32 poleGrab_index;
   vec3  poleGrab_normal;
   vec3  poleGrab_direction;
   flo32 poleGrab_timer;
   
   vec3  position;
   vec3  velocity;
   flo32 velocity_grab;
   
   boo32 jump_isAble;
   
   boo32 wallJump_isAble;
   vec3  wallJump_normal;
   
   flo32 radius;
   flo32 height;
   
   boo32 isWalking;
   
   boo32 isSliding;
   vec3  isSliding_normal;
   
   boo32 coyote_useSlideJump;
   flo32 coyote_timer;
   
   uint32 pixie_count[ 4 ];
   
   boo32 isDead;
   flo32 respawn_timer;
   boo32 respawn_isDown;
   
   uint32 lastCheckpoint;
};
#endif	// STD_INCLUDE_DECL

#ifdef	STD_INCLUDE_FUNC

internal ORIENTED_BOX
toOBox( EDITOR_STATE * e, EDITOR__LEDGE ledge ) {
   vec3 P0 = e->point[ ledge.point_index[ 0 ] ];
   vec3 P1 = e->point[ ledge.point_index[ 1 ] ];
   vec3 V = P1 - P0;
   
   vec3 X = getNormal( V );
   vec3 Z = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), X ) );
   vec3 Y = getNormal( cross( X, Z ) );
   
   vec3 halfDim = Vec3( getLength( V ), COLLISION__LEDGE_HEIGHT, COLLISION__LEDGE_DEPTH ) * 0.5f;
   
   ORIENTED_BOX result = {};
   result.xAxis = X;
   result.yAxis = Y;
   result.zAxis = Z;
   result.halfDim = halfDim;
   result.center = P1 - X * halfDim.x - Y * halfDim.y + Z * halfDim.z;
   return result;
}

internal ORIENTED_BOX
toOBox( COLLISION_LEDGE ledge ) {
   vec3 V = ledge.point1 - ledge.point0;
   
   vec3 X = getNormal( V );
   vec3 Z = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), X ) );
   vec3 Y = getNormal( cross( X, Z ) );
   
   vec3 halfDim = Vec3( getLength( V ), COLLISION__LEDGE_HEIGHT, COLLISION__LEDGE_DEPTH ) * 0.5f;
   
   ORIENTED_BOX result = {};
   result.xAxis = X;
   result.yAxis = Y;
   result.zAxis = Z;
   result.halfDim = halfDim;
   result.center = ledge.point1 - X * halfDim.x - Y * halfDim.y + Z * halfDim.z;
   return result;
}

internal vec3
getNormal( COLLISION_LEDGE ledge ) {
   ORIENTED_BOX bound = toOBox( ledge );
   vec3 result = bound.zAxis;
   return result;
}

internal void
updatePlayerState( APP_STATE * appState, flo32 dt ) {
   PLAYER_CONTROL * playerControl = &appState->playerControl;
   PLAYER_STATE   * playerState   = &appState->playerState;
   
   if( playerState->ledgeGrab_isActive ) {
      playerState->isWalking = false;
      playerState->isSliding = false;
      if( ( playerControl->jump_wasPressed ) && ( !playerControl->crouch_wasPressed ) ) {
         playerState->ledgeGrab_isActive = false;
         COLLISION_LEDGE ledge = appState->collisionSystem.ledge[ playerState->ledgeGrab_index ];
         
         if( playerState->ledgeGrab_dotZ > 0.0f ) {
            flo32 degrees = 60.0f;
            vec3 normal   = getNormal( ledge );
            normal.y = tanf( degrees * ( PI / 180.0f ) );
            normal   = getNormal( normal );
            playerState->velocity = normal * 12.0f;
         } else {
            vec3 ledge_vector = getNormal( ledge.point1 - ledge.point0 );
            playerState->velocity.x = ledge_vector.x * playerState->velocity_grab;
            playerState->velocity.z = ledge_vector.z * playerState->velocity_grab;
            playerState->velocity.y = 11.0f;
         }
      }
      if( ( playerControl->crouch_wasPressed ) && ( !playerControl->jump_wasPressed ) ) {
         COLLISION_LEDGE ledge = appState->collisionSystem.ledge[ playerState->ledgeGrab_index ];
         vec3 ledge_vector = getNormal( ledge.point1 - ledge.point0 );
         playerState->velocity = ledge_vector * playerState->velocity_grab;
         playerState->ledgeGrab_isActive = false;
         playerState->crouch_isActive = true;
      }
   } else if( playerState->poleGrab_isActive ) {
      playerState->isWalking = false;
      playerState->isSliding = false;
      if( playerControl->jump_wasPressed && !playerControl->crouch_wasPressed ) {
         flo32 align_ratio = -cosf( 30.0f * ( PI / 180.0f ) );
         flo32 dotDN       = dot( playerState->poleGrab_direction, playerState->poleGrab_normal );
         if( ( getLengthSq( playerState->poleGrab_direction ) <= 0.0001f ) || ( dotDN <= align_ratio ) ) {
            playerState->velocity.y = 9.0f;
         } else {
            vec3 normal = getNormal( playerState->poleGrab_direction );
            if( dotDN < 0.0f ) {
               vec3 dir_axis = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), playerState->poleGrab_normal ) );
               if( dot( normal, dir_axis ) > 0.0f ) {
                  normal = dir_axis;
               } else {
                  normal = -dir_axis;
               }
            }
            flo32 degrees = 55.0f;
            normal.y = tanf( degrees * ( PI / 180.0f ) );
            normal   = getNormal( normal );
            playerState->velocity = normal * 9.0f;
         }
         
         playerState->poleGrab_isActive = false;
         playerState->poleGrab_index    = -1;
         playerState->poleGrab_timer    = 0.0f;
      }
      if( ( playerControl->crouch_wasPressed ) && ( !playerControl->jump_wasPressed ) ) {
         playerState->poleGrab_isActive = false;
         playerState->poleGrab_index    = -1;
         playerState->poleGrab_timer    = 0.0f;
      }
   } else {
      if( playerControl->jump_wasPressed && !playerControl->crouch_wasPressed ) {
         playerState->crouch_isActive = false;
         if( playerState->jump_isAble ) {
            if( ( playerState->isWalking ) || ( !playerState->coyote_useSlideJump ) ) {
               playerState->velocity.y = 12.0f;
            } else if( ( playerState->isSliding ) || ( playerState->coyote_useSlideJump ) ) {
               flo32 degrees = 45.0f;
               vec3 normal   = getNormal( Vec3( playerState->isSliding_normal.x, 0.0f, playerState->isSliding_normal.z ) );
               normal.y = tanf( degrees * ( PI / 180.0f ) );
               normal   = getNormal( normal );
               playerState->velocity = normal * 10.0f;
            }
            
            playerState->isWalking = false;
            playerState->isSliding = false;
         }
         if( playerState->wallJump_isAble ) {
            if( playerState->canUseLedgeToJump ) {
               playerState->velocity.y = 11.0f;
            } else {
               flo32 degrees = 60.0f;
               vec3 normal   = getNormal( Vec3( playerState->wallJump_normal.x, 0.0f, playerState->wallJump_normal.z ) );
               vec3 zAxis    = normal;
               normal.y = tanf( degrees * ( PI / 180.0f ) );
               normal   = getNormal( normal );
               
               vec3 xAxis = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), playerState->wallJump_normal ) );
               vec3 xVel  = dot( playerState->velocity, xAxis ) * xAxis;
               
               vec3  zVel = Vec3( 0.0f, 0.0f, 0.0f );
               flo32 zDot = dot( playerState->velocity, zAxis );
               if( zDot > 0.0f ) {
                  zVel = zDot * zAxis;
               }
               
               playerState->velocity = ( normal * 12.0f ) + xVel + zVel;
            }
            
            playerState->isWalking = false;
            playerState->isSliding = false;
         }
      }
      if( playerControl->crouch_wasPressed && !playerControl->jump_wasPressed ) {
         playerState->crouch_isActive = true;
      }
   }
   
   if( playerControl->crouch_wasReleased ) { playerState->crouch_isActive = false; }
}
#endif	// STD_INCLUDE_FUNC