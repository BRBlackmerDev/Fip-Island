
internal void
APP_updateAndRender( APP_STATE * appState, PLATFORM * platform ) {
    RENDERER * renderer = &platform->renderer;
    
	MOUSE_STATE    * mouse    = &platform->mouse;
	KEYBOARD_STATE * keyboard = &platform->keyboard;
	
	MEMORY * permMemory = &platform->permMemory;
	MEMORY * tempMemory = &platform->tempMemory;
	
    AUDIO_SYSTEM * audioSystem = &platform->audioSystem;
    MUSIC_SYSTEM * musicSystem = &platform->musicSystem;
	ASSET_SYSTEM * assetSystem = &platform->assetSystem;
    
    COLLISION_SYSTEM * collisionSystem = &appState->collisionSystem;
    
    PLAYER_CONTROL * playerControl = &appState->playerControl;
    PLAYER_STATE   * playerState   = &appState->playerState;
    
    flo32 dt = platform->targetSec;
    if( wasPressed( keyboard, keyCode_o ) ) {
        appState->isPaused = !appState->isPaused;
    }
    if( appState->isPaused ) { dt = 0.0f; }
    
    vec2 app_dim     = appState->app_dim;
    vec2 app_halfDim = appState->app_halfDim;
    rect app_bound   = appState->app_bound;
    
    // DISPLAY_VALUE( uint64, tempMemory->used );
	
	if( !appState->isInitialized ) {
		appState->isInitialized = true;
        
        // appState->camera_xAxis = Vec3( 1.0f, 0.0f, 0.0f );
        // appState->camera_yAxis = Vec3( 0.0f, 1.0f, 0.0f );
        // appState->camera_zAxis = Vec3( 0.0f, 0.0f, 1.0f );
        
        // appState->camera_position = Vec3( 0.0f, 1.0f, 0.0f );
        // appState->camera_dist     = 20.0f;
        // appState->camera_vRadians = 0.6f;
        
        appState->camera_fovAngle     = toRadians( 60.0f );
        appState->camera_dist         = 8.0f;
        appState->camera_maxDist      = 8.0f;
        appState->camera_vRadians     = 0.6f;
        
        playerState->radius   = 0.25f;
        playerState->height   = 1.7272f; // 68in -> 1.7272m
        // door width  = 0.9144
        // door height = 2.032
        // wall height = 2.4384
        
        resetLevel( appState, permMemory, tempMemory );
        vec3 pos = appState->player_resetPosition;
        if( appState->spawn_useSpawnPoint ) {
            pos = appState->spawn_spawnPoint;
        }
        playerState->position = getClosestIntersection( collisionSystem, appState->player_resetPosition, Vec3( 0.0f, -1.0f, 0.0f ) );
        playerState->ledgeGrab_index = -1;
        
        // READ_FILE_RESULT file_mesh = readFile( tempMemory, "w:/Projects/camera/art/mesh/mesh0000.x" );
        // X_FILE_DATA xFile_data = parseXFile( tempMemory, file_mesh );
        // appState->renderData[ appState->renderData_count++ ] = addRenderData( permMemory, xFile_data );
        // addCollisionData( collisionSystem, xFile_data );
        // _popSize( tempMemory, getDataSize( xFile_data ) );
        // _popSize( tempMemory, file_mesh.size );
        
        // READ_FILE_RESULT file_mesh = readFile( tempMemory, "w:/Projects/camera/art/entity/ENTITY_0000.ent" );
        // ENTITY_FILE_DATA entityFile_data = parseEntityFile( tempMemory, file_mesh );
        // appState->renderData[ appState->renderData_count++ ] = addRenderData( permMemory, entityFile_data );
        // addCollisionData( collisionSystem, entityFile_data );
        // _popSize( tempMemory, getDataSize( entityFile_data ) );
        // _popSize( tempMemory, file_mesh.size );
        
        // loadEntity( appState, permMemory, tempMemory, 1 );
        // READ_FILE_RESULT file_mesh = readFile( tempMemory, "w:/Projects/camera/art/entity/ENTITY_0001.ent" );
        // if( file_mesh.contents ) {
        // ENTITY_FILE_DATA entityFile_data = parseEntityFile( tempMemory, file_mesh );
        // appState->renderData[ appState->renderData_count++ ] = addRenderData( permMemory, entityFile_data );
        // addCollisionData( collisionSystem, entityFile_data );
        // _popSize( tempMemory, getDataSize( entityFile_data ) );
        // _popSize( tempMemory, file_mesh.size );
        // }
        
        // loadLevel( appState, permMemory, tempMemory );
        
        uint32 breakHere = 10;
        
        appState->modCamera_maxDist  = 8.0f;
        appState->modCamera_fovAngle = toRadians( 60.0f );
        
        //VIEW_LIST * viewList = &appState->viewList;
        //add( viewList, View( Vec3( -4.0f, 0.0f, 8.0f ), 1.0f, 4.0f, 10.0f, 0.0f, 1.0f ) );
        //add( viewList, View( Vec3( 18.0f, 0.0f, 8.0f ), 1.0f, 4.0f, 10.0f, 0.0f, 1.0f ) );
        //add( viewList, View( Vec3( 25.0f, 34.0f, -14.0f ), 3.0f, 30.0f, 22.0f, 0.0f, 4.0f ) );
        //add( viewList, View( Vec3( 18.5f, 42.0f, -74.0f ), 3.0f, 20.0f, 8.0f ) );
        //add( viewList, View( Vec3( 52.4f, 16.0f, -35.25f ), Vec3( 52.4f, 24.0f, -35.25f ), 2.0f, 6.0f, 9.0f, 0.0f, 1.0f ) );
    }
    
    if( wasPressed( keyboard, keyCode_q ) ) {
        resetLevel( appState, permMemory, tempMemory );
    }
    
    flo32 camera_hRadians = appState->camera_hRadians;
    vec3  camera_xAxis = appState->camera_xAxis;
    vec3  camera_yAxis = appState->camera_yAxis;
    vec3  camera_zAxis = appState->camera_zAxis;
    
    flo32 camera_minDist  = 0.20f;
    flo32 camera_fovAngle = toRadians( 60.0f );
    // camera_fovAngle = PI * 0.6667f;
    
    
    vec2 radians = ( mouse->position - app_halfDim ) * 0.002f;
    radians = Vec2( 0.0f, 0.0f );
    
    // if( appState->camera_left  ) { radians.x -= 0.025f; }
    // if( appState->camera_right ) { radians.x += 0.025f; }
    if( appState->camera_left || appState->camera_right ) {
        flo32 ratio = appState->camera_horRatio * appState->camera_horRatio;
        if( appState->camera_left ) { ratio = -ratio; }
        radians.x += ( 0.08f * ratio );
    }
    // if( appState->camera_up    ) { radians.y += 0.025f; }
    // if( appState->camera_down  ) { radians.y -= 0.025f; }
    if( appState->camera_up || appState->camera_down ) {
        flo32 ratio = appState->camera_verRatio * appState->camera_verRatio;
        //ratio *= -1.0f;
        if( appState->camera_down ) { ratio = -ratio; }
        radians.y += ( 0.05f * ratio );
    }
    
    appState->camera_hRadians_dest -= radians.x;
    
    {
        SPRING_COEF h = getSpringCoef( 180.0f, dt );
        flo32 r = appState->camera_hRadians - appState->camera_hRadians_dest;
        flo32 v = appState->camera_hRadians_vel;
        
        appState->camera_hRadians     = h.a * r + h.b * v + appState->camera_hRadians_dest;
        appState->camera_hRadians_vel = h.c * r + h.d * v;
    }
    
    //appState->camera_hRadians -= radians.x;
    appState->camera_vRadians += radians.y;
    
    // appState->camera_dist     = camera_minDist;
    // appState->camera_fovAngle = camera_fovAngle;
    
    flo32 hCos = cosf( appState->camera_hRadians );
    flo32 hSin = sinf( appState->camera_hRadians );
    flo32 vCos = cosf( appState->camera_vRadians );
    flo32 vSin = sinf( appState->camera_vRadians );
    
    appState->camera_zAxis = Vec3( hSin * vCos, vSin,  hCos * vCos );
    appState->camera_xAxis = Vec3( hCos,        0.0f, -hSin        );
    appState->camera_yAxis = getNormal( cross( appState->camera_zAxis, appState->camera_xAxis ) );
    // // DISPLAY_VALUE( vec2, mouse->position );
    
    // // DISPLAY_VALUE( flo32, appState->camera_hRadians );
    // // DISPLAY_VALUE( flo32, appState->camera_vRadians );
    // // DISPLAY_VALUE( vec3,  appState->camera_xAxis );
    // // DISPLAY_VALUE( vec3,  appState->camera_yAxis );
    // // DISPLAY_VALUE( vec3,  appState->camera_zAxis );
    // // DISPLAY_VALUE( vec3,  appState->camera_position );
    // // DISPLAY_VALUE( flo32, appState->camera_dist );
    
    
    
    playerState->canUseLedgeToJump = false;
    if( playerState->ledgeGrab_isActive ) {
        vec2 move_xAxis = Vec2( cosf( camera_hRadians ), -sinf( camera_hRadians ) );
        vec2 move_yAxis = -getPerp( move_xAxis );
        vec2 move_vector = getNormal( ( move_xAxis * appState->input_ratio.x ) + ( move_yAxis * appState->input_ratio.y ) );
        vec3 vector = Vec3( move_vector.x, 0.0f, move_vector.y );
        
        COLLISION_LEDGE ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
        vec3 xAxis = getNormal( ledge.point1 - ledge.point0 );
        vec3 yAxis = Vec3( 0.0f, -1.0f, 0.0f );
        vec3 zAxis = getNormal( cross( xAxis, yAxis ) );
        
        playerState->ledgeGrab_dotZ = dot( vector, zAxis );
        // DISPLAY_VALUE( flo32, playerState->ledgeGrab_dotZ );
    } else {
        vec2 move_xAxis = Vec2( cosf( camera_hRadians ), -sinf( camera_hRadians ) );
        vec2 move_yAxis = -getPerp( move_xAxis );
        vec2 move_vector = getNormal( ( move_xAxis * appState->input_ratio.x ) + ( move_yAxis * appState->input_ratio.y ) );
        
        vec3 pos    = playerState->position;
        pos.y += playerState->height;
        for( uint32 ledge_index = 0; ledge_index < collisionSystem->ledge_count; ledge_index++ ) {
            COLLISION_LEDGE ledge = collisionSystem->ledge[ ledge_index ];
            
            boo32 isValid = isInLedgeBound_ledgeJump( ledge, pos, Vec3( move_vector.x, 0.0f, move_vector.y ) );
            if( isValid ) {
                playerState->canUseLedgeToJump = true;
            }
        }
        
    }
    
    { // update jump state
        vec2 move_xAxis = Vec2( cosf( camera_hRadians ), -sinf( camera_hRadians ) );
        vec2 move_yAxis = -getPerp( move_xAxis );
        vec2 move_accel = getNormal( ( move_xAxis * appState->input_ratio.x ) + ( move_yAxis * appState->input_ratio.y ) );
        vec3 move_vector = Vec3( move_accel.x, 0.0f, move_accel.y );
        
        COLLISION_TRIANGLE floor_triangle = getFirstTriangle( collisionSystem, playerState->position, Vec3( 0.0f, -1.0f, 0.0f ) );
        COLLISION_TRIANGLE  wall_triangle = getWallTriangle ( collisionSystem, playerState->position, move_vector );
        
        playerState->wallJump_isAble = false;
        playerState->jump_isAble = ( ( floor_triangle.isValid ) && ( floor_triangle.isWalkable ) && ( floor_triangle.collision_dist <= COLLISION__JUMP_THRESHOLD ) );
        if( !playerState->jump_isAble ) {
            
            if( ( wall_triangle.isValid ) && ( !wall_triangle.isWalkable ) && ( wall_triangle.collision_dist <= COLLISION__WALL_JUMP_THRESHOLD ) ) {
                playerState->wallJump_isAble = true;
                playerState->wallJump_normal = wall_triangle.collision_normal;
            }
        }
        
        if( ( playerState->isWalking ) || ( playerState->isSliding ) ) {
            playerState->coyote_timer = 0.0f;
            playerState->coyote_useSlideJump = playerState->isSliding;
        } else {
            playerState->coyote_timer += dt;
        }
        
        if( !playerState->jump_isAble && ( playerState->coyote_timer <= JUMP__COYOTE_TIME ) ) {
            playerState->jump_isAble = true;
        }
    }
    
    updatePlayerState( appState, dt );
    
    // DISPLAY_VALUE( boo32, playerState->isWalking );
    // DISPLAY_VALUE( boo32, playerState->isSliding );
    // DISPLAY_VALUE( boo32, playerState->wallJump_isAble );
    // DISPLAY_VALUE( vec3,  playerState->isSliding_normal );
    
    flo32 height_crouch = 0.80f;
    flo32 height_stand  = 1.7272f;
    flo32 crouch_time   = 0.12f;
    flo32 crouch_speed  = ( height_stand - height_crouch ) / crouch_time;
    if( playerState->ledgeGrab_isActive ) {
        flo32 dist = minValue( ( crouch_speed * dt ), height_stand - playerState->height );
        dist = maxValue( dist, 0.0f );
        
        playerState->height     += dist;
        playerState->position.y -= dist;
        
        COLLISION_LEDGE ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
        vec3  xAxis          = ledge.point1 - ledge.point0;
        flo32 xAxis_lengthSq = getLengthSq( xAxis );
        flo32 xAxis_length   = sqrtf( xAxis_lengthSq );
        
        flo32 grab_speed    =  3.0f;
        flo32 grab_friction = 10.0f;
        flo32 grab_accel    =  0.0f;
        
        // flo32 player_horRatio = ( appState->player_horRatio * appState->player_horRatio );
        // flo32 player_verRatio = ( appState->player_verRatio * appState->player_verRatio );
        flo32 player_speedRatio = getInputSpeed( appState->input_speed ); // 1.0f; // player_horRatio + player_verRatio;
        // DISPLAY_VALUE( flo32, player_speedRatio );
        
        vec2 move_xAxis = Vec2( cosf( camera_hRadians ), -sinf( camera_hRadians ) );
        vec2 move_yAxis = -getPerp( move_xAxis );
        // vec2 move_vector = getNormal( ( move_xAxis * appState->player_horRatio ) + ( move_yAxis * appState->player_verRatio ) );
        vec2 move_vector = getNormal( ( move_xAxis * appState->input_ratio.x ) + ( move_yAxis * appState->input_ratio.y ) );
        
        if( getLengthSq( move_vector ) > 0.0f ) {
            vec3 move_direction = Vec3( move_vector.x, 0.0f, move_vector.y );
            grab_accel = dot( getNormal( move_direction ), getNormal( xAxis ) );
            
            // DISPLAY_VALUE( vec3, move_direction );
            // DISPLAY_VALUE( flo32, grab_accel );
        }
        grab_accel = ( grab_accel * grab_speed * grab_friction * player_speedRatio ) - ( playerState->velocity_grab * grab_friction );
        playerState->velocity_grab += ( grab_accel * dt );
        
        flo32 move_dist = ( grab_accel * 0.5f * dt * dt ) + ( playerState->velocity_grab * dt );
        flo32 move_t    = move_dist / xAxis_length;
        
        flo32 player_t    = dot( ( playerState->position + Vec3( 0.0f, playerState->height, 0.0f ) - ledge.point0 ), xAxis ) / xAxis_lengthSq;
        flo32 player_newT = player_t + move_t;
        if( ( player_newT < 0.0f ) || ( player_newT > 1.0f ) ) {
            if( player_newT < 0.0f ) {
                if( ledge.link0 != -1 ) {
                    playerState->ledgeGrab_index = ledge.link0;
                    ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
                    player_newT += 1.0f;
                } else {
                    player_newT = 0.0f;
                }
            }
            if( player_newT > 1.0f ) {
                if( ledge.link1 != -1 ) {
                    playerState->ledgeGrab_index = ledge.link1;
                    ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
                    player_newT -= 1.0f;
                } else {
                    player_newT = 1.0f;
                }
            }
        }
        playerState->position = setPlayerPosOnLedge( ledge, player_newT, playerState->height );
    } else if( playerState->poleGrab_isActive ) {
        flo32 grab_speed    =  3.0f;
        flo32 grab_friction = 10.0f;
        flo32 player_speedRatio = getInputSpeed( appState->input_speedY );
        
        flo32 move_accel = 0.0f;
        if( appState->input_ratio.y < 0.0f ) { move_accel -= 1.0f; }
        if( appState->input_ratio.y > 0.0f ) { move_accel += 1.0f; }
        
        vec2 dir_xAxis = Vec2( cosf( camera_hRadians ), -sinf( camera_hRadians ) );
        vec2 dir_yAxis = -getPerp( dir_xAxis );
        vec2 dir = dir_xAxis * appState->input_ratio.x + dir_yAxis * appState->input_ratio.y;
        vec3 hor_direction = Vec3( dir.x, 0.0f, dir.y );
        if( getLengthSq( hor_direction ) >= 0.1f ) {
            playerState->poleGrab_direction = hor_direction;
        } else {
            playerState->poleGrab_direction = Vec3( 0.0f, 1.0f, 0.0f );
        }
        drawLine( renderer, renderPassID_main_debug, playerState->position, playerState->position + hor_direction, COLOR_CYAN );
        
        flo32 grab_accel = move_accel * player_speedRatio * grab_speed * grab_friction - playerState->velocity_grab * grab_friction;
        playerState->velocity_grab += ( grab_accel * dt );
        
        Assert( playerState->poleGrab_index != -1 );
        COLLISION_POLE pole = collisionSystem->pole[ playerState->poleGrab_index ];
        vec3 pole_vector = pole.point1 - pole.point0;
        
        flo32 move_dist = ( grab_accel * 0.5f * dt * dt ) + ( playerState->velocity_grab * dt );
        flo32 move_t    = move_dist / getLength( pole_vector );
        
        flo32 player_t    = dot( ( playerState->position - pole.point0 ), pole_vector ) / getLengthSq( pole_vector );
        flo32 player_newT = player_t + move_t;
        if( ( player_newT < 0.0f ) || ( player_newT > 1.0f ) ) {
            playerState->poleGrab_isActive = false;
            playerState->poleGrab_index    = -1;
            playerState->velocity.y = playerState->velocity_grab;
        }
        
        playerState->position = pole.point0 + pole_vector * player_newT;
        // if( ( player_newT < 0.0f ) || ( player_newT > 1.0f ) ) {
        // if( player_newT < 0.0f ) {
        // if( ledge.link0 != -1 ) {
        // playerState->ledgeGrab_index = ledge.link0;
        // ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
        // player_newT += 1.0f;
        // } else {
        // player_newT = 0.0f;
        // }
        // }
        // if( player_newT > 1.0f ) {
        // if( ledge.link1 != -1 ) {
        // playerState->ledgeGrab_index = ledge.link1;
        // ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
        // player_newT -= 1.0f;
        // } else {
        // player_newT = 1.0f;
        // }
        // }
        // } 
    } else {
        vec2 move_xAxis = Vec2( cosf( camera_hRadians ), -sinf( camera_hRadians ) );
        vec2 move_yAxis = -getPerp( move_xAxis );
        // flo32 player_horRatio = ( appState->player_horRatio * appState->player_horRatio );
        // flo32 player_verRatio = ( appState->player_verRatio * appState->player_verRatio );
        // move_xAxis *= player_horRatio;
        // move_yAxis *= player_verRatio;
        // vec2 move_accel = {};
        // if( playerControl->move_left     ) { move_accel -= move_xAxis; }
        // if( playerControl->move_right    ) { move_accel += move_xAxis; }
        // if( playerControl->move_backward ) { move_accel -= move_yAxis; }
        // if( playerControl->move_forward  ) { move_accel += move_yAxis; }
        vec2 move_accel = getNormal( ( move_xAxis * appState->input_ratio.x ) + ( move_yAxis * appState->input_ratio.y ) );
        
        flo32 speed_walk    = 3.5f;
        flo32 friction_walk = 15.0f;
        
        flo32 speed_run     = 7.0f;
        flo32 friction_run  = 15.0f;
        
        flo32 speed_crouch    = 2.5f;
        flo32 friction_crouch = 15.0f;
        
        flo32 speed_crouch_run = 5.0f;
        flo32 friction_crouch_run = 8.0f;
        
        flo32 speed_air     = 3.5f;
        speed_air     = 7.0f;
        flo32 friction_air  = 1.9f;
        
        flo32 speed_move    = speed_air;
        flo32 friction_move = friction_air;
        
        if( playerState->isWalking ) {
            if( playerState->crouch_isActive ) {
                speed_move    = speed_crouch_run;
                friction_move = friction_crouch_run;
            } else {
                speed_move    = speed_run;
                friction_move = friction_run;
            }
        } else if( playerState->isSliding ) {
            // if( playerState->run_isActive ) {
            speed_move    = 5.0f;
            friction_move = 2.25f;            
            // } else {
            // speed_move    = 3.5f;
            // friction_move = 2.25f;
            // }
        }
        // else if( playerState->run_isActive ) {
        // // air fast
        // speed_move    = 7.0f;
        // friction_move = 1.9f;
        // }
        
        flo32 player_speedRatio = getInputSpeed( appState->input_speed ); // 1.0f; // player_horRatio + player_verRatio;
        flo32 accel_move = speed_move * friction_move * player_speedRatio;
        // // DISPLAY_VALUE( flo32, speed_move );
        // // DISPLAY_VALUE( flo32, friction_move );
        // // DISPLAY_VALUE( flo32, player_speedRatio );
        
        
        vec2 move_vel = Vec2( playerState->velocity.x, playerState->velocity.z );
        // move_accel = ( getNormal( move_accel ) * accel_move - move_vel * friction_move );
        move_accel = ( getNormal( move_accel ) * accel_move );
        
        vec3 player_accel = Vec3( 0.0f, -28.0f, 0.0f );
        vec2 accel_horDir = getNormal( ( move_xAxis * appState->input_ratio.x ) + ( move_yAxis * appState->input_ratio.y ) );
        if( playerState->isWalking ) {
            // if crouch is active
            flo32 maxSpeed = 7.0f;
            flo32 friction = 15.0f;
            
            vec2 hor_vel   = Vec2( playerState->velocity.x, playerState->velocity.z );
            vec2 hor_accel = accel_horDir * ( player_speedRatio * friction * maxSpeed ) - hor_vel * friction;
            
            player_accel.x += hor_accel.x;
            player_accel.z += hor_accel.y;
        } else if( playerState->isSliding ) {
            vec3 zAxis = playerState->isSliding_normal;
            vec3 xAxis = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), zAxis ) );
            vec3 yAxis = getNormal( cross( zAxis, xAxis ) );
            
            flo32 dotY = dot( playerState->velocity, yAxis );
            // DISPLAY_VALUE( flo32, dotY );
            if( dotY < 0.0f ) {
                flo32 friction_slide = 1.138f;
                vec3 slope_friction = ( friction_slide * dotY ) * yAxis;            
                player_accel -= slope_friction;
            }
            
            flo32 maxSpeed = 10.0f;
            flo32 friction = 2.0f;
            player_accel += xAxis * ( dot( accel_horDir, Vec2( xAxis.x, xAxis.z ) ) * player_speedRatio * friction * maxSpeed );
            player_accel -= xAxis * ( dot( playerState->velocity, xAxis ) * friction );
            
            vec2 hor_vel   = Vec2( playerState->velocity.x, playerState->velocity.z );
            
            vec2  plane_horDir = getNormal( Vec2( playerState->isSliding_normal.x, playerState->isSliding_normal.z ) );
            flo32 dotA         = dot( accel_horDir, plane_horDir );
            
            // vec2 U = move_accel;
            // vec2 V = getNormal( Vec2( playerState->isSliding_normal.x, playerState->isSliding_normal.z ) );
            // flo32 dotUV = dot( U, V );
            // // if( dotUV > 0.0f ) {
            // U -= ( dotUV * V );
            // // }
            // // if( dotUV < 0.0f ) {
            // // flo32 radians_lo = 0.785398f; // 45 degrees
            // // flo32 radians_hi = 1.107149f; // 63 degrees
            // // flo32 radians = acosf( dot( playerState->isSliding_normal, Vec3( 0.0f, 1.0f, 0.0f ) ) );
            // // flo32 radians_t = ( radians - radians_lo ) / ( radians_hi - radians_lo );
            // // flo32 modifier_lo = 0.75f;
            // // flo32 modifier_hi = 1.0f;
            // // flo32 modifier    = lerp( modifier_lo, radians_t, modifier_hi );
            // // U += ( ( dotUV * modifier ) * V );
            // // }
            // move_accel = U;
            
            // vec3 xAxis = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), playerState->isSliding_normal ) );
            // vec3 yAxis = getNormal( cross( playerState->isSliding_normal, xAxis ) );
            
            // // sliding down a slope
            // flo32 dotV = dot( playerState->velocity, yAxis );
            // vec3  slope_friction = yAxis * dotV * dt;
            // if( dotV < 0.0f ) {
            // // playerState->velocity -= slope_friction * 2.0f;
            // }
            
            // sliding up a slope
            // flo32 dotN = dot( playerState->isSliding_normal, Vec3( 0.0f, 1.0f, 0.0f ) );
            // if( ( dotV > 0.0f ) && ( fabs( dotN ) >= 0.000001f ) ) {
            // playerState->velocity -= slope_friction * 4.0f;
            // }
        } else {
            flo32 maxSpeed = 7.0f;
            flo32 friction = 1.9f;
            
            vec2 hor_vel   = Vec2( playerState->velocity.x, playerState->velocity.z );
            vec2 hor_accel = accel_horDir * ( player_speedRatio * friction * maxSpeed ) - hor_vel * friction;
            
            player_accel.x += hor_accel.x;
            player_accel.z += hor_accel.y;
        }
        playerState->velocity += ( player_accel * dt );
        // playerState->velocity.x += ( move_accel.x * dt );
        // playerState->velocity.z += ( move_accel.y * dt );
        
        // // DISPLAY_VALUE( boo32, playerState->isSliding );
        
        // flo32 fall_accel = -28.0f;
        // playerState->velocity.y += ( fall_accel * dt );
        if( playerState->isWalking ) {
            playerState->velocity.y = 0.0f;
        }
        
        if( playerState->crouch_isActive ) {
            playerState->height -= ( crouch_speed * dt );
            playerState->height  = maxValue( playerState->height, height_crouch );
        } else {
            flo32 dist = ( crouch_speed * dt );
            COLLISION_TRIANGLE ceiling = getFirstTriangle( collisionSystem, playerState->position + Vec3( 0.0f, playerState->height, 0.0f ), Vec3( 0.0f, 1.0f, 0.0f ) );
            if( ( ceiling.isValid ) && ( dist > ceiling.collision_dist ) ) {
                dist = ceiling.collision_dist;
            }
            playerState->height += dist;
            playerState->height  = minValue( playerState->height, height_stand );
        }
        
        // DISPLAY_VALUE( boo32, playerState->crouch_isActive );
        // DISPLAY_VALUE( flo32, playerState->height );
        
        if( wasPressed( keyboard, keyCode_i ) ) {
            uint32 breakHere = 10;
        }
        debugCollision( &appState->debugCollisionSystem, collisionSystem, *playerState, dt );
        updateCollision( appState, collisionSystem, playerState, dt );
        
        // updateMoveCollision( appState, dt );
        
        if( playerState->isWalking ) {
            COLLISION_TRIANGLE collision_triangle = getFirstTriangle( collisionSystem, playerState->position, Vec3( 0.0f, -1.0f, 0.0f ) );
            if( ( collision_triangle.isValid ) && ( collision_triangle.isWalkable ) && ( collision_triangle.collision_dist <= COLLISION__IS_WALKING_THRESHOLD ) ) {
                // if player is not jumping or falling
                // and if player is walking/running on sloped terrain, snap to terrain y
                playerState->position.y = collision_triangle.collision_point.y + COLLISION__OFFSET_FROM_COLLISION_PLANE;
                playerState->velocity.y = 0.0f;
            } else {
                playerState->isWalking = false;
            }
        }
        // if( playerState->isSliding ) {
        // playerState->isSliding_timer += dt;
        // if( playerState->isSliding_timer >= playerState->isSliding_targetTime ) {
        // playerState->isSliding = false;
        // }
        // }
        
        // if( playerState->isSliding && ( !collision_triangle.isValid ) || (  collision_triangle.isWalkable ) ) {
        // // TODO: tune dist
        // playerState->isSliding = false;
        // }
        // // DISPLAY_VALUE( vec3,  collision_triangle.collision_point );
        // // DISPLAY_VALUE( flo32, collision_triangle.collision_dist  );
        // if( collision_triangle.vertex ) {
        // debugTriOutline( collision_triangle.vertex[ 0 ], collision_triangle.vertex[ 1 ], collision_triangle.vertex[ 2 ], COLOR_CYAN );
        // }
        // if( !playerState->isWalking ) {
        // updateFallCollision( playerState, dt, collision_triangle );
        // }
        
        if( playerState->velocity.y <= 0.0f ) {
            flo32 ledge_height = COLLISION__LEDGE_HEIGHT;
            flo32 ledge_depth  = COLLISION__LEDGE_DEPTH + COLLISION__LEDGE_DEPTH_EPSILON;
            
            if( playerState->ledgeGrab_index != -1 ) {
                COLLISION_LEDGE ledge = collisionSystem->ledge[ playerState->ledgeGrab_index ];
                boo32 isValid = isInLedgeBound( ledge, playerState->position );
                if( ( !isValid ) && ( ledge.link0 != -1 ) ) {
                    isValid = isInLedgeBound( collisionSystem->ledge[ ledge.link0 ], playerState->position );
                }
                if( ( !isValid ) && ( ledge.link1 != -1 ) ) {
                    isValid = isInLedgeBound( collisionSystem->ledge[ ledge.link1 ], playerState->position );
                }
                if( !isValid ) {
                    playerState->ledgeGrab_index = -1;
                }
            }
            
            vec3 pos = playerState->position + Vec3( 0.0f, playerState->height, 0.0f );
            for( uint32 ledge_index = 0; ( playerState->ledgeGrab_index == -1 ) && ( ledge_index < collisionSystem->ledge_count ); ledge_index++ ) {
                COLLISION_LEDGE ledge = collisionSystem->ledge[ ledge_index ];
                
                ORIENTED_BOX bound = toOBox( ledge );
                bound.halfDim.z += COLLISION__LEDGE_DEPTH_EPSILON;
                boo32 isValid = isInLedgeBound_noAddHeight( ledge, pos );
                isValid = isInBound( pos, bound );
                
                if( isValid ) {
                    vec3 xAxis = Vec3( ledge.point1.x, ledge.point0.y, ledge.point1.z ) - ledge.point0;
                    flo32 dotX = dot( pos - ledge.point0, xAxis ) / getLengthSq( xAxis );
                    
                    playerState->ledgeGrab_isActive = true;
                    playerState->ledgeGrab_index    = ledge_index;
                    
                    playerState->position = setPlayerPosOnLedge( ledge, dotX, playerState->height );
                    playerState->velocity_grab = dot( playerState->velocity, xAxis ) / getLength( xAxis );
                    playerState->velocity = Vec3( 0.0f, 0.0f, 0.0f );
                    
                    playerState->crouch_isActive = false;
                    
                    playerState->isWalking = false;
                    playerState->isSliding = false;
                }
            }
        }
        
        playerState->poleGrab_timer += dt;
        if( ( !playerState->isWalking ) && ( playerState->poleGrab_timer >= PLAYER_CONTROL__POLE_GRAB__TARGET_TIME ) ) {
            for( uint32 pole_index = 0; ( !playerState->poleGrab_isActive ) && ( pole_index < collisionSystem->pole_count ); pole_index++ ) {
                COLLISION_POLE pole = collisionSystem->pole[ pole_index ];
                
                vec3 pole_vector = pole.point1 - pole.point0;
                
                ORIENTED_BOX bound = {};
                bound.center = pole.point0 + pole_vector * 0.5f + pole.normal * ( COLLISION__POLE_DEPTH * 0.5f );
                bound.xAxis = getNormal( cross( pole_vector, pole.normal ) );
                bound.yAxis = getNormal( pole.point1 - pole.point0 );
                bound.zAxis = pole.normal;
                bound.halfDim = Vec3( COLLISION__POLE_WIDTH, getLength( pole_vector ), COLLISION__POLE_DEPTH ) * 0.5f;
                
                bound.center    -= ( COLLISION__POLE_DEPTH_EPSILON * 0.5f ) * pole.normal;
                bound.halfDim.z += ( COLLISION__POLE_DEPTH_EPSILON * 0.5f );
                
                bound.center    -= ( COLLISION__POLE_SUBTRACT_HEIGHT_FOR_ENTRY * 0.5f ) * bound.yAxis;
                bound.halfDim.y -= ( COLLISION__POLE_SUBTRACT_HEIGHT_FOR_ENTRY * 0.5f );
                
                uint32 breakHere = 10;
                
                if( isInBound( playerState->position, bound ) ) {
                    playerState->poleGrab_isActive = true;
                    playerState->poleGrab_index    = pole_index;
                    playerState->poleGrab_normal   = pole.normal;
                    
                    vec3 V     = playerState->position - bound.center;
                    flo32 dotY = dot( V, bound.yAxis );
                    playerState->position = bound.center + bound.yAxis * dotY;
                    
                    playerState->velocity_grab = playerState->velocity.y;
                    playerState->velocity = Vec3( 0.0f, 0.0f, 0.0f );
                }
            }
        }
    }
    
    // DISPLAY_VALUE( boo32, playerState->ledgeGrab_isActive );
    // DISPLAY_VALUE( int32, playerState->ledgeGrab_index );
    // DISPLAY_VALUE( uint32, collisionSystem->ledge_count );
    
    // CONSOLE_VALUE( boo32, playerState->poleGrab_isActive );
    
#if ENABLE_TERRAIN_GENERATION
    TERRAIN_GENERATION * terrain = &appState->terrain_generation;
    // // DISPLAY_VALUE( vec3, terrain->position_start );
    // // DISPLAY_VALUE( vec3, terrain->position_end   );
    if( getLengthSq( terrain->position_end - playerState->position ) < 1.0f ) {
        genTerrain( terrain );
    }
#endif // ENABLE_TERRAIN_GENERATION
    
    //appState->camera_fovAngle = 2.0f * atanf( ( 0.5f * height_stand / camera_heightOnScreen[ appState->camera_heightIndex ] ) / appState->camera_maxDist );
    
    // DISPLAY_VALUE( flo32, appState->camera_fovAngle );
    // DISPLAY_VALUE( flo32, appState->camera_maxDist  );
    // DISPLAY_VALUE( flo32, appState->camera_dist     );
    
    debug_drawLedges( renderer, collisionSystem );
    debug_drawPoles ( renderer, collisionSystem, playerState->poleGrab_isActive );
    
    // {
    // char debug_string[ 128 ] = {};
    // sprintf( debug_string, "Player HeightOnScreen: %s : %f", ( appState->camera_modifyCamera ? "XX" : "--" ), camera_heightOnScreen[ appState->camera_heightIndex ] );
    // // DISPLAY_STRING( debug_string );
    
    // sprintf( debug_string, "Dist: %.02f, FOV Angle: %.02f", appState->camera_dist, appState->camera_fovAngle / PI * 180.0f );
    // // DISPLAY_STRING( debug_string );
    // }
    
    if( wasPressed( keyboard, keyCode_r ) && ( !appState->modifyCamera ) ) {
        playerState->isDead = false;
        playerState->respawn_isDown = false;
        playerState->respawn_timer  = 0.0f;
        playerState->velocity = Vec3( 0.0f, 0.0f, 0.0f );
        
        vec3 pos = appState->player_resetPosition;
        if( appState->spawn_useSpawnPoint ) {
            pos = appState->spawn_spawnPoint;
        }
        playerState->position = getClosestIntersection( collisionSystem, pos, Vec3( 0.0f, -1.0f, 0.0f ) );
        playerState->isWalking = false;
        playerState->isSliding = false;
        playerState->ledgeGrab_isActive = false;
        playerState->poleGrab_isActive  = false;
        // playerState->jump_isActive = false;
        playerState->crouch_isActive = false;
        playerState->height = height_stand;
        playerState->ledgeGrab_index = -1;
        playerState->pixie_count[ 0 ] = 0;
        playerState->pixie_count[ 1 ] = 0;
        playerState->pixie_count[ 2 ] = 0;
        playerState->pixie_count[ 3 ] = 0;
        
        // outputDebugCollision( tempMemory, &appState->debugCollisionSystem );
    }
    
    // vec3 shadow_position = getShadowPosition( collisionSystem, playerState->position, playerState->radius );
    vec3 shadow_position = {};
    COLLISION_TRIANGLE collision_shadow = getFirstTriangle( collisionSystem, playerState->position, Vec3( 0.0f, -1.0f, 0.0f ) );
    if( collision_shadow.isValid ) {
        shadow_position = collision_shadow.collision_point + Vec3( 0.0f, 0.0001f, 0.0f );
    }
    
    if( !appState->modifyCamera ) {
        appState->camera_hOffset  = 0.0f;
        appState->camera_vOffset  = 0.0f;
        appState->camera_maxDist  = 8.0f;
        appState->camera_fovAngle = toRadians( 60.0f );
        
        if( !playerState->isDead ) {
            VIEW_LIST * viewList = &appState->viewList;
            
            vec3 playerPos = playerState->position + Vec3( 0.0f, 1.0f, 0.0f );
            vec3  cameraPos      = playerPos;
            flo32 cameraMaxDist  = appState->camera_maxDist;
            flo32 cameraFOVAngle = appState->camera_fovAngle;
            flo32 cameraHOffset  = appState->camera_hOffset;
            flo32 cameraVOffset  = appState->camera_vOffset;
            
            vec3  P = cameraPos;
            flo32 D = cameraMaxDist;
            flo32 F = cameraFOVAngle;
            flo32 H = cameraHOffset;
            flo32 V = cameraVOffset;
            
            uint32 nContribute      = 0;
            uint32  contribute[ 8 ] = {};
            flo32  tContribute[ 8 ] = {};
            vec3   pContribute[ 8 ] = {};
            flo32  sum = 0.0f;
            for( uint32 iView = 0; iView < viewList->nView; iView++ ) {
                VIEW view = viewList->view[ iView ];
                
                vec3 closestPoint = {};
                switch( view.type ) {
                    case viewType_point: {
                        closestPoint = view.P;
                    } break;
                    
                    case viewType_line: {
                        closestPoint = getClosestPoint_point_lineSegment( playerPos, view.P, view.Q );
                    } break;
                    
                    case viewType_box: {
                        
                    } break;
                    
                    default: {
                        InvalidCodePath;
                    } break;
                }
                
                flo32 dist = getLength( playerPos - closestPoint );
                flo32 t = 1.0f - clamp01( ( dist - view.lo ) / ( view.hi - view.lo ) );
                // dist < 2 , t = 1.0
                // dist > 12, t = 0.0
                if( t > 0.0f ) {
                    Assert( nContribute < 8 );
                    
                    contribute [ nContribute ] = iView;
                    tContribute[ nContribute ] = t;
                    pContribute[ nContribute ] = closestPoint;
                    nContribute++;
                    
                    sum += t;
                }
            }
            
            if( sum > 1.0f ) {
                for( uint32 iContribute = 0; iContribute < nContribute; iContribute++ ) {
                    tContribute[ iContribute ] /= sum;
                }
            }
            
            for( uint32 iContribute = 0; iContribute < nContribute; iContribute++ ) {
                flo32 t    = tContribute[ iContribute ];
                VIEW  view = viewList->view[ contribute[ iContribute ] ];
                
                cameraPos      += ( ( pContribute[ iContribute ] - P ) * t );
                cameraMaxDist  += ( ( view.maxDist  - D ) * t );
                cameraFOVAngle += ( ( view.fovAngle - F ) * t );
                cameraHOffset  += ( ( view.hOffset  - H ) * t );
                cameraVOffset  += ( ( view.vOffset  - V ) * t );
            }
            
            //appState->camera_position = cameraPos;
            appState->camera_destination = cameraPos;
            appState->camera_hOffset  = cameraHOffset;
            appState->camera_vOffset  = cameraVOffset;
            appState->camera_maxDist  = cameraMaxDist;
            appState->camera_fovAngle = cameraFOVAngle;
        }
        
        {
            flo32 w = 8.0f;
            flo32 t = dt;
            
            vec3 P = ( appState->camera_position - appState->camera_destination );
            vec3 V = appState->camera_velocity;
            
            SPRING_COEF xz = getSpringCoef( 8.0f, dt );
            SPRING_COEF y  = ( P.y < 0.0f ) ? getSpringCoef( 5.0f, dt ) : getSpringCoef( 10.0f, dt );
            
            appState->camera_position.x = xz.a * P.x + xz.b * V.x + appState->camera_destination.x;
            appState->camera_velocity.x = xz.c * P.x + xz.d * V.x;
            
            appState->camera_position.y = y.a * P.y + y.b * V.y + appState->camera_destination.y;
            appState->camera_velocity.y = y.c * P.y + y.d * V.y;
            
            appState->camera_position.z = xz.a * P.z + xz.b * V.z + appState->camera_destination.z;
            appState->camera_velocity.z = xz.c * P.z + xz.d * V.z;
        }
        
        // DISPLAY_VALUE( flo32, appState->camera_fovAngle );
        // DISPLAY_VALUE( flo32, appState->camera_maxDist );
        // appState->camera_position = playerState->position + Vec3( 0.0f, 1.8f, 0.0f );
        
        flo32 collision_t = 1.0f;
        vec3 ray_origin = playerState->position + Vec3( 0.0f, 1.0f, 0.0f );
        ray_origin = appState->camera_position;
        vec3 ray_vector = appState->camera_zAxis * appState->camera_maxDist;
        ray_vector += appState->camera_xAxis * appState->camera_hOffset;
        ray_vector += appState->camera_yAxis * appState->camera_vOffset;
        
        for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
            vec3 vertex[ 3 ] = {};
            memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
            vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
            
            flo32 t0 = dot( ray_vector, normal );
            if( t0 < -0.000001f ) {
                flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
                
                if( ( t > 0.0f ) && ( t < collision_t ) ) {
                    vec3 point = ray_origin + ray_vector * t;
                    vec3 normal0 = getNormal( cross( normal, vertex[ 1 ] - vertex[ 0 ] ) );
                    vec3 normal1 = getNormal( cross( normal, vertex[ 2 ] - vertex[ 1 ] ) );
                    vec3 normal2 = getNormal( cross( normal, vertex[ 0 ] - vertex[ 2 ] ) );
                    
                    flo32 dot0 = dot( point - ( vertex[ 0 ] - normal0 * 0.01f ), normal0 );
                    flo32 dot1 = dot( point - ( vertex[ 1 ] - normal1 * 0.01f ), normal1 );
                    flo32 dot2 = dot( point - ( vertex[ 2 ] - normal2 * 0.01f ), normal2 );
                    
                    boo32 isValid = ( ( dot0 >= 0.0f ) && ( dot1 >= 0.0f ) && ( dot2 >= 0.0f ) );
                    if( isValid ) {
                        collision_t = t;
                    }
                }
            }
        }
        
        flo32 dist = clamp( appState->camera_maxDist * collision_t, camera_minDist, appState->camera_maxDist );
        appState->camera_dist = dist;
    }
    
    // DISPLAY_VALUE( vec3, playerState->position );
    // DISPLAY_VALUE( vec3, playerState->velocity );
    
    SECURE_DOOR_LIST * s = &appState->secureDoorList;
    for( uint32 iDoor = 0; iDoor < s->nSecureDoor; iDoor++ ) {
        SECURE_DOOR * door = s->secureDoor + iDoor;
        
        boo32 inActivateBound = isPlayerInBound( playerState->position, playerState->height, door->activate_bound );
        boo32 canActivate     = canActivateSecureDoor( playerState, *door );
        if( inActivateBound ) {
            door->activate_timer += dt;
            if( ( canActivate ) && ( door->activate_timer >= SECURE_DOOR__TARGET_TIME__ACTIVATE ) ) {
                door->collision_isActive = false;
            }
        } else {
            door->collision_isActive = true;
            flo32 maxTime = ( canActivate ) ? SECURE_DOOR__TARGET_TIME__ACTIVATE + SECURE_DOOR__TARGET_TIME__OPEN + 0.4f : SECURE_DOOR__TARGET_TIME__ACTIVATE;
            door->activate_timer = minValue( door->activate_timer, maxTime );
            door->activate_timer = maxValue( door->activate_timer - dt, 0.0f );
        }
    }
    
    KILL_ZONE_LIST * killZoneList = &appState->killZoneList;
    for( uint32 iKillZone = 0; iKillZone < killZoneList->nKillZone; iKillZone++ ) {
        KILL_ZONE killZone = killZoneList->killZone[ iKillZone ];
        if( isInBound( playerState->position, killZone.bound ) ) {
            playerState->isDead = true;
        }
    }
    
    CHECKPOINT_LIST * checkpointList = &appState->checkpointList;
    for( uint32 iCheckpoint = 0; iCheckpoint < checkpointList->nCheckpoint; iCheckpoint++ ) {
        CHECKPOINT checkpoint = checkpointList->checkpoint[ iCheckpoint ];
        if( isInBound( playerState->position, checkpoint.bound ) ) {
            playerState->lastCheckpoint = iCheckpoint;
        }
    }
    
    for( uint32 iDoor = 0; iDoor < s->nSecureDoor; iDoor++ ) {
        SECURE_DOOR door = s->secureDoor[ iDoor ];
        
        boo32 inActivateBound = isPlayerInBound( playerState->position, playerState->height, door.activate_bound );
        boo32 canActivate     = canActivateSecureDoor( playerState, door );
        
        vec4 outline_color = inActivateBound ? COLOR_GREEN : COLOR_RED;
        drawBoxOutline( renderer, renderPassID_main, door.activate_bound, outline_color );
        
        if( canActivate ) {
            flo32 render_t = clamp01( ( door.activate_timer - SECURE_DOOR__TARGET_TIME__ACTIVATE ) / SECURE_DOOR__TARGET_TIME__OPEN );
            door.render_bound.center -= ( door.render_bound.yAxis * ( door.render_bound.halfDim.y * 2.0f * render_t ) );
        }
        
        vec4  render_color = COLOR_GRAY( 0.2f );
        flo32 pixie_t = clamp01( door.activate_timer / SECURE_DOOR__TARGET_TIME__ACTIVATE );
        if( door.activate_timer > 0.0f ) {
            if( canActivate ) {
                render_color = lerp( render_color, pixie_t, COLOR_GRAY( 0.1f ) );
            } else {
                render_color.r = lerp( render_color.r, pixie_t, 0.3f );
            }
        }
        drawBox( renderer, renderPassID_main, door.render_bound, render_color );
        
        uint32 pixie_count = getCost( door );
        vec4   pixie_color[ 12 ] = {};
        
        vec4   color_lo[ 4 ] = PIXIE__COLOR_LO__ARRAY;
        vec4   color_hi[ 4 ] = PIXIE__COLOR_HI__ARRAY;
        flo32 invalid_toggleTime = 0.6f;
        
        uint32 base = 0;
        for( uint32 type_index = 0; type_index < 4; type_index++ ) {
            uint32 slot_cost  = door.pixie_cost[ type_index ];
            uint32 slot_count = playerState->pixie_count[ type_index ];
            for( uint32 slot_index = 0; slot_index < slot_cost; slot_index++ ) {
                if( door.activate_timer > 0.0f ) {
                    if( slot_index < slot_count ) {
                        pixie_color[ base + slot_index ] = lerp( color_lo[ type_index ], pixie_t, color_hi[ type_index ] );
                    } else {
                        if( inActivateBound ) {
                            uint32 value = ( uint32 )( door.activate_timer / invalid_toggleTime );
                            pixie_color[ base + slot_index ] = ( ( value % 2 ) == 1 ) ? Vec4( 0.8f, 0.0f, 0.0f, 1.0f ) : color_lo[ type_index ];
                        } else {
                            pixie_color[ base + slot_index ] = color_lo[ type_index ];
                        }
                    }
                } else {
                    pixie_color[ base + slot_index ] = color_lo[ type_index ];
                }
            }
            base += slot_cost;
        }
        
        {
            vec3 X = door.render_bound.xAxis * 0.15f;
            vec3 Y = door.render_bound.yAxis * 0.15f;
            vec3 atPos = door.render_bound.center + ( door.render_bound.zAxis * ( door.render_bound.halfDim.z + 0.1f ) ) - ( X * ( 2.4f * ( flo32 )pixie_count * 0.5f ) );
            for( uint32 pixie_index = 0; pixie_index < pixie_count; pixie_index++ ) {
                vec3 P[ 4 ] = {};
                P[ 0 ] = atPos - X - Y;
                P[ 1 ] = atPos + X - Y;
                P[ 2 ] = atPos - X + Y;
                P[ 3 ] = atPos + X + Y;
                
                drawQuad( renderer, renderPassID_main_noLighting, P[ 0 ], P[ 1 ], P[ 2 ], P[ 3 ], pixie_color[ pixie_index ] );
                
                atPos += ( X * 2.4f );
            }
        }
        {
            vec3 X = -door.render_bound.xAxis * 0.15f;
            vec3 Y = -door.render_bound.yAxis * 0.15f;
            vec3 atPos = door.render_bound.center + ( -door.render_bound.zAxis * ( door.render_bound.halfDim.z + 0.1f ) ) - ( X * ( 2.4f * ( flo32 )pixie_count * 0.5f ) );
            for( uint32 pixie_index = 0; pixie_index < pixie_count; pixie_index++ ) {
                vec3 P[ 4 ] = {};
                P[ 0 ] = atPos - X - Y;
                P[ 1 ] = atPos + X - Y;
                P[ 2 ] = atPos - X + Y;
                P[ 3 ] = atPos + X + Y;
                
                drawQuad( renderer, renderPassID_main_noLighting, P[ 0 ], P[ 1 ], P[ 2 ], P[ 3 ], pixie_color[ pixie_index ] );
                
                atPos += ( X * 2.4f );
            }
        }
    }
    
    PIXIE_LIST * pixieList = &appState->pixieList;
    int32 remove_index = -1;
    for( uint32 pixie_index = 0; pixie_index < pixieList->nPixie; pixie_index++ ) {
        PIXIE * pixie = pixieList->pixie + pixie_index;
        pixie->xRadians += ( 2.0f * PI / 4.3333f * dt );
        pixie->yRadians += ( 2.0f * PI / 8.0f    * dt );
        pixie->zRadians += ( 2.0f * PI / 2.7132f * dt );
        
        if( isPlayerInBound( playerState->position, playerState->height, pixie->pickUp_bound ) ) {
            remove_index = pixie_index;
        }
    }
    if( remove_index != -1 ) {
        Assert( remove_index < ( int32 )pixieList->nPixie );
        PIXIE pixie = pixieList->pixie[ remove_index ];
        
        pixieList->pixie[ remove_index ] = pixieList->pixie[ --pixieList->nPixie ];
        
        playerState->pixie_count[ pixie.typeID ]++;
    }
    drawPixies( renderer, renderPassID_main, pixieList );
    
    drawCylinder( renderer, renderPassID_main, playerState->position, playerState->height, playerState->radius, COLOR_GREEN ); // Height: 5' 8"
    drawShadow  ( renderer, renderPassID_main, shadow_position, playerState->radius + 0.01f, COLOR_BLACK );
    drawSky     ( renderer );
    
    for( int32 data_index = 0; data_index < appState->renderData_count; data_index++ ) {
        drawRenderData( renderer, renderPassID_main, appState->renderData[ data_index ] );
    }
    
    for( uint32 iKillZone = 0; iKillZone < killZoneList->nKillZone; iKillZone++ ) {
        KILL_ZONE killZone = killZoneList->killZone[ iKillZone ];
        drawBoxOutline( renderer, renderPassID_main, killZone.bound, COLOR_CYAN );
    }
    
    for( uint32 iCheckpoint = 0; iCheckpoint < checkpointList->nCheckpoint; iCheckpoint++ ) {
        CHECKPOINT checkpoint = checkpointList->checkpoint[ iCheckpoint ];
        
        vec4 color = ( playerState->lastCheckpoint == iCheckpoint ) ? COLOR_GREEN : COLOR_RED;
        drawBoxOutline( renderer, renderPassID_main, checkpoint.bound, color );
    }
    
    if( playerControl->respawn_wasPressed ) {
        playerState->respawn_isDown = true;
    }
    if( playerControl->respawn_wasReleased ) {
        playerState->respawn_isDown = false;
        playerState->respawn_timer  = 0.0f;
    }
    if( playerState->isDead ) {
        if( playerState->respawn_isDown ) {
            playerState->respawn_timer += dt;
            if( playerState->respawn_timer >= PLAYER__RESPAWN_TARGET_TIME ) {
                playerState->isDead = false;
                playerState->respawn_isDown = false;
                playerState->respawn_timer  = 0.0f;
                playerState->velocity = Vec3( 0.0f, 0.0f, 0.0f );
                
                vec3 pos = appState->player_resetPosition;
                if( appState->spawn_useSpawnPoint ) {
                    pos = appState->spawn_spawnPoint;
                }
                pos = checkpointList->checkpoint[ playerState->lastCheckpoint ].respawnPos;
                playerState->position = getClosestIntersection( collisionSystem, pos, Vec3( 0.0f, -1.0f, 0.0f ) );
                playerState->isWalking = false;
                playerState->isSliding = false;
                playerState->ledgeGrab_isActive = false;
                playerState->poleGrab_isActive  = false;
                playerState->crouch_isActive = false;
                playerState->height = height_stand;
                playerState->ledgeGrab_index = -1;
                playerState->pixie_count[ 0 ] = 0;
                playerState->pixie_count[ 1 ] = 0;
                playerState->pixie_count[ 2 ] = 0;
                playerState->pixie_count[ 3 ] = 0;
            }
        }
        rect bound = rectCD( app_halfDim, app_dim * 0.4f );
        drawRect( renderer, renderPassID_overlay, bound, COLOR_WHITE );
        
        drawString( renderer, renderPassID_overlay, assetID_font_default, "YOU DIED!", getCenter( bound ), ALIGN_CENTER, COLOR_BLACK );
        drawString( renderer, renderPassID_overlay, assetID_font_default, "HOLD TRIANGLE TO RESTART", getCenter( bound ) + Vec2( 0.0f, -24.0f ), ALIGN_CENTER, COLOR_BLACK );
        
        if( playerState->respawn_timer > 0.0f ) {
            rect respawn_bound = rectCD( getCenter( bound ) + Vec2( 0.0f, -48.0f ), Vec2( 200.0f, 12.0f ) );
            drawRect( renderer, renderPassID_overlay, respawn_bound, COLOR_BLACK );
            
            flo32 tRespawn = clamp01( playerState->respawn_timer / PLAYER__RESPAWN_TARGET_TIME );
            
            rect fill_bound = addRadius( respawn_bound, -1.0f );
            fill_bound.right = fill_bound.left + getWidth( fill_bound ) * tRespawn;
            drawRect( renderer, renderPassID_overlay, fill_bound, COLOR_YELLOW );
        }
    }
    
    if( appState->debug_showFaceIndex ) {
        vec3 camera_position  = appState->camera_position + appState->camera_zAxis * appState->camera_dist;
        mat4 main_camera      = genCameraTransform( camera_position, appState->camera_xAxis, appState->camera_yAxis, appState->camera_zAxis );
        mat4 transform        = mat4_perspective( appState->app_dim, appState->camera_fovAngle, 0.001f, 2000.0f ) * main_camera;
        for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
            vec3 vertex[ 3 ] = {};
            memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
            
            vec4 point    = Vec4( ( vertex[ 0 ] + vertex[ 1 ] + vertex[ 2 ] ) / 3.0f, 1.0f );
            vec4 adjPoint = transform * point;
            vec3 pos = adjPoint.xyz / adjPoint.w;
            vec2 string_pos = pos.xy * app_halfDim + app_halfDim;
            
            char string[ 8 ] = {};
            sprintf( string, "%d", tri_index );
            
            uint32 breakHere = 10;
            drawString( renderer, renderPassID_overlay, assetID_font_default, string, string_pos, ALIGN_CENTER, COLOR_BLACK );
        }
    }
    
    vec3 vel = playerState->velocity;
    vec2 hor_vel = Vec2( vel.x, vel.z );
    
    flo32 player_hSpeed = getLength( hor_vel );
    flo32 player_vSpeed = fabsf( vel.y );
    flo32 player_speed  = getLength( playerState->velocity );
    // DISPLAY_VALUE( flo32, player_hSpeed );
    // DISPLAY_VALUE( flo32, player_vSpeed );
    // DISPLAY_VALUE( flo32, player_speed );
    
    // DISPLAY_VALUE( boo32, appState->spawn_useSpawnPoint );
    // DISPLAY_VALUE( vec3,  appState->spawn_spawnPoint    );
    
    VIEW_LIST * viewList = &appState->viewList;
    for( uint32 iView = 0; iView < viewList->nView; iView++ ) {
        VIEW view = viewList->view[ iView ];
        
        drawView( renderer, renderPassID_main, view, false );
    }
    
    if( wasPressed( keyboard, keyCode_c, KEYBOARD_FLAGS__CONTROL ) ) {
        appState->modifyCamera = !appState->modifyCamera;
    }
    if( appState->modifyCamera ) {
        if( wasPressed( keyboard, keyCode_c ) ) {
            VIEW bView = getClosestView( viewList, playerState->position );
            appState->modCamera_hOffset  = bView.hOffset;
            appState->modCamera_vOffset  = bView.vOffset;
            appState->modCamera_maxDist  = bView.maxDist;
            appState->modCamera_fovAngle = bView.fovAngle;
        }
        if( wasPressed( keyboard, keyCode_r ) ) {
            appState->modCamera_hOffset  = 0.0f;
            appState->modCamera_vOffset  = 0.0f;
            appState->modCamera_maxDist  = 8.0f;
            appState->modCamera_fovAngle = toRadians ( 60.0f );
        }
        flo32 lo[ 4 ] = { -10.0f, -10.0f,   1.0f,  30.0f };
        flo32 hi[ 4 ] = {  10.0f,  10.0f, 100.0f, 170.0f };
        
        flo32 height = 18.0f;
        rect  pBound = rectBLD( Vec2( 0.0f, 0.0f ), Vec2( app_dim.x, height * 4.0f ) );
        
        rect sBound[ 4 ] = {};
        vec2 atPos = getTL( pBound ) + Vec2( 200.0f, 0.0f );
        for( uint32 iter = 0; iter < 4; iter++ ) {
            rect bound = Rect( atPos.x, atPos.y - height, app_bound.right, atPos.y );
            bound = addRadius( bound, -1.0f );
            sBound[ iter ] = bound;
            
            atPos.y -= height;
        }
        
        if( isInBound( mouse->position, pBound ) ) {
            if( wasPressed( mouse, mouseButton_left ) ) {
                appState->modCamera_modValue = true;
                appState->modCamera_modIndex = clamp( ( int32 )( ( pBound.top - mouse->position.y ) / height ), 0, 3 );
            }
        }
        if( appState->modCamera_modValue ) {
            rect  b = sBound[ appState->modCamera_modIndex ];
            flo32 t = clamp01( ( mouse->position.x - b.left ) / getWidth( b ) );
            switch( appState->modCamera_modIndex ) {
                case 0: {
                    appState->modCamera_hOffset  = roundTo( lerp( lo[ 0 ], t, hi[ 0 ] ), 0.1f );
                } break;
                
                case 1: {
                    appState->modCamera_vOffset  = roundTo( lerp( lo[ 1 ], t, hi[ 1 ] ), 0.1f );
                } break;
                
                case 2: {
                    appState->modCamera_maxDist  = roundTo( lerp( lo[ 2 ], t, hi[ 2 ] ), 0.1f );
                } break;
                
                case 3: {
                    appState->modCamera_fovAngle = toRadians( roundTo( lerp( lo[ 3 ], t, hi[ 3 ] ), 1.0f ) );
                } break;
                
                default: {
                    InvalidCodePath;
                } break;
            }
        }
        if( wasReleased( mouse, mouseButton_left ) ) {
            appState->modCamera_modValue = false;
        }
        
        drawRect( renderer, renderPassID_overlay, pBound, COLOR_GRAY( 0.94f ) );
        
        char * label[ 4 ] = {
            "hOffset",
            "vOffset",
            "dist",
            "fovAngle",
        };
        
        flo32 val[ 4 ] = {
            appState->modCamera_hOffset,
            appState->modCamera_vOffset,
            appState->modCamera_maxDist,
            toDegrees( appState->modCamera_fovAngle ),
        };
        
        atPos = getTL( pBound );
        for( uint32 iter = 0; iter < 4; iter++ ) {
            char str[ 64 ] = {};
            sprintf( str, "%-12s: %f", label[ iter ], val[ iter ] );
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, atPos + Vec2( 4.0f, -2.0f ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            drawRect( renderer, renderPassID_overlay, sBound[ iter ], COLOR_GRAY( 0.1f ) );
            
            flo32 t = ( val[ iter ] - lo[ iter ] ) / ( hi[ iter ] - lo[ iter ] );
            vec2 cursorPos = getP( sBound[ iter ], Vec2( t, 0.0f ) );
            rect cBound = rectBCD( cursorPos, Vec2( 2.0f, height ) );
            drawRect( renderer, renderPassID_overlay, cBound, COLOR_YELLOW );
            
            atPos.y -= height;
        }
        
        appState->camera_hOffset  = appState->modCamera_hOffset;
        appState->camera_vOffset  = appState->modCamera_vOffset;
        appState->camera_maxDist  = appState->modCamera_maxDist;
        appState->camera_dist     = appState->camera_maxDist;
        appState->camera_fovAngle = appState->modCamera_fovAngle;
        
        vec3 cameraPos = appState->camera_position + appState->camera_xAxis * appState->camera_hOffset + appState->camera_yAxis * appState->camera_vOffset;
        // DISPLAY_VALUE( vec3, cameraPos );
    }
}