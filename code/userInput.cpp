
#ifdef	STD_INCLUDE_DECL

//----------
// mouse declarations
//----------

enum MOUSE_BUTTON_ID {
	mouseButton_left,
	mouseButton_right,
	mouseButton_middle,
	
	mouseButton_count,
};

struct MOUSE_BUTTON_STATE {
	void * hotID;
	void * activeID;
	void * doubleClickID;
	boo32 isDown;
	int32 halfTransitionCount;
	flo32 timer_doubleClick;
	boo32 selectBound_isActive;
	vec2 selectPos;
};

struct MOUSE_STATE {
	MOUSE_BUTTON_STATE button[ mouseButton_count ];
	vec2 position;
	flo32 targetTime_doubleClick;
	boo32 control_isDown;
	boo32 shift_isDown;
	int32 wheelClicks;
};

//----------
// keyboard declarations
//----------

enum KEY_STATE_CODE {
	keyCode_null,
	
	keyCode_backspace,
	keyCode_shift,
	keyCode_control,
	keyCode_alt,
	keyCode_capsLock,
	keyCode_escape,
	keyCode_pageUp,
	keyCode_pageDown,
	keyCode_end,
	keyCode_home,
	keyCode_left,
	keyCode_right,
	keyCode_up,
	keyCode_down,
	keyCode_insert,
	keyCode_delete,
	keyCode_enter,
	keyCode_tab,
	keyCode_space,
	keyCode_0,
	keyCode_1,
	keyCode_2,
	keyCode_3,
	keyCode_4,
	keyCode_5,
	keyCode_6,
	keyCode_7,
	keyCode_8,
	keyCode_9,
	keyCode_a,
	keyCode_b,
	keyCode_c,
	keyCode_d,
	keyCode_e,
	keyCode_f,
	keyCode_g,
	keyCode_h,
	keyCode_i,
	keyCode_j,
	keyCode_k,
	keyCode_l,
	keyCode_m,
	keyCode_n,
	keyCode_o,
	keyCode_p,
	keyCode_q,
	keyCode_r,
	keyCode_s,
	keyCode_t,
	keyCode_u,
	keyCode_v,
	keyCode_w,
	keyCode_x,
	keyCode_y,
	keyCode_z,
	keyCode_semicolon,
	keyCode_equal,
	keyCode_comma,
	keyCode_dash,
	keyCode_period,
	keyCode_forwardSlash,
	keyCode_tilde,
	keyCode_openBracket,
	keyCode_backslash,
	keyCode_closeBracket,
	keyCode_quote,
	keyCode_num0,
	keyCode_num1,
	keyCode_num2,
	keyCode_num3,
	keyCode_num4,
	keyCode_num5,
	keyCode_num6,
	keyCode_num7,
	keyCode_num8,
	keyCode_num9,
   keyCode_numAdd,
   keyCode_numSub,
   keyCode_numMul,
   keyCode_numDiv,
   keyCode_numPeriod,
   keyCode_F1,
   keyCode_F2,
   keyCode_F3,
   keyCode_F4,
   keyCode_F5,
   keyCode_F6,
   keyCode_F7,
   keyCode_F8,
   keyCode_F9,
   keyCode_F10,
   keyCode_F11,
   keyCode_F12,
   keyCode_F13,
   keyCode_F14,
   keyCode_F15,
   keyCode_F16,
   keyCode_F17,
   keyCode_F18,
   keyCode_F19,
   keyCode_F20,
   keyCode_F21,
   keyCode_F22,
   keyCode_F23,
   keyCode_F24,
	
	keyCode_count,
};

struct KEY_STATE {
	KEY_STATE_CODE keyCode;
	boo32      isDown;
	uint8      flags;
};

#define KEYBOARD_FLAGS__CONTROL  ( 0x01 )
#define KEYBOARD_FLAGS__ALT      ( 0x02 )
#define KEYBOARD_FLAGS__SHIFT    ( 0x04 )

#define KEYBOARD_STATE__MAX_EVENT_COUNT  ( 16 )
struct KEYBOARD_STATE {
	KEY_STATE event[ KEYBOARD_STATE__MAX_EVENT_COUNT ];
	int32 event_count;
   
   uint8 flags;
};

//----------
// controller declarations
//----------

enum CONTROLLER_BUTTON_ID {
	controllerButton_dPad_up,
	controllerButton_dPad_down,
	controllerButton_dPad_left,
	controllerButton_dPad_right,
   
   controllerButton_start,
   controllerButton_back,
   
   controllerButton_thumb_left,
   controllerButton_thumb_right,
   controllerButton_shoulder_left,
   controllerButton_shoulder_right,
   
   controllerButton_A,
   controllerButton_B,
   controllerButton_X,
   controllerButton_Y,
	
	controllerButton_count,
};

struct CONTROLLER_BUTTON_STATE {
	boo32 isDown;
	int32 halfTransitionCount;
};

struct CONTROLLER_STATE {
	CONTROLLER_BUTTON_STATE button[ controllerButton_count ];
};

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

//----------
// mouse functions
//----------

internal void
MOUSE_endOfFrame( MOUSE_STATE * mouse, flo32 timeElapsed = 0.0f ) {
	mouse->wheelClicks = 0;
	for( uint32 buttonIndex = 0; buttonIndex < mouseButton_count; buttonIndex++ ) {
		MOUSE_BUTTON_STATE * button = &mouse->button[ buttonIndex ];
		button->timer_doubleClick += timeElapsed;
		button->hotID = 0;
		button->halfTransitionCount = 0;
		if( !button->isDown ) {
			button->activeID = 0;
		} else if( button->activeID == 0 ) {
			button->activeID = &button->activeID;
		}
	}
}

internal boo32
isDown( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	boo32 result = mouse->button[ buttonID ].isDown;
	return result;
}

internal boo32
wasButtonPressed( MOUSE_BUTTON_STATE * button ) {
	boo32 result = false;
	if( ( button->isDown && button->halfTransitionCount >= 1 ) ||
      ( !button->isDown && button->halfTransitionCount >= 2 ) ) {
		result = true;
	}
	return result;
}

internal boo32
wasPressed( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
	boo32 result = wasButtonPressed( button );
	return result;
}

internal boo32
wasReleased( MOUSE_BUTTON_STATE * button ) {
	boo32 result = false;
	if( ( !button->isDown && button->halfTransitionCount >= 1 ) ||
      ( button->isDown && button->halfTransitionCount >= 2 ) ) {
		result = true;
	}
	return result;
}

internal boo32
wasReleased( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
	boo32 result = wasReleased( button );
	return result;
}

internal void
processButtonEvent( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID, boo32 isDown ) {
	MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
	button->isDown = isDown;
	button->halfTransitionCount++;
	if( wasButtonPressed( button ) ) {
		button->selectPos = mouse->position;
	}
}

internal boo32
processClick( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID, void * targetID, boo32 isInBound ) {
	boo32 result = false;
	MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
	if( button->hotID == 0 && isInBound ) {
		button->hotID = targetID;
	}
	if( button->activeID == targetID && !button->isDown && isInBound ) {
		result = true;
	} else if( button->hotID == targetID && button->activeID == 0 && button->isDown ) {
		button->activeID = targetID;
	}
	return result;
}

internal boo32
processDoubleClick( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID, void * targetID, boo32 isInBound ) {
	boo32 result = false;
	if( processClick( mouse, buttonID, targetID, isInBound ) ) {
		MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
		if( button->doubleClickID == button->hotID && button->timer_doubleClick <= mouse->targetTime_doubleClick ) {
			result = true;
		} else {
			button->doubleClickID = button->hotID;
		}
		button->timer_doubleClick = 0.0f;
	}
	return result;
}

internal boo32
processButtonDown( MOUSE_BUTTON_STATE * button, void * targetID, boo32 isInBound ) {
	boo32 result = false;
	if( button->hotID == 0 && isInBound ) {
		button->hotID = targetID;
	}
	if( button->hotID == targetID && button->activeID == 0 && button->isDown ) {
		button->activeID = targetID;
		result = true;
	}
	return result;
}

internal boo32
processButtonDown( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID, void * targetID, boo32 isInBound ) {
	boo32 result = false;
	MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
	if( button->hotID == 0 && isInBound ) {
		button->hotID = targetID;
	}
	if( button->hotID == targetID && button->activeID == 0 && button->isDown ) {
		button->activeID = targetID;
		result = true;
	}
	return result;
}

internal boo32
processButtonUp( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID, void * targetID, boo32 isInBound ) {
	boo32 result = false;
	MOUSE_BUTTON_STATE * button = &mouse->button[ buttonID ];
	if( button->hotID == 0 && isInBound ) {
		button->hotID = targetID;
	}
	if( button->activeID == targetID && !button->isDown && isInBound ) {
		button->activeID = 0;
		result = true;
	}
	return result;
}

internal rect
getSelectBound( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	vec2 mousePos = mouse->position;
	vec2 selectPos = mouse->button[ buttonID ].selectPos;
	rect result = {
		minValue( mousePos.x, selectPos.x ),
		minValue( mousePos.y, selectPos.y ),
		maxValue( mousePos.x, selectPos.x ),
		maxValue( mousePos.y, selectPos.y )
	};
	return result;
}

inline vec2
getSelectVector( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	vec2 result = mouse->position - mouse->button[ buttonID ].selectPos;
	return result;
}

inline flo32
getSelectVectorX( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	flo32 result = mouse->position.x - mouse->button[ buttonID ].selectPos.x;
	return result;
}

inline flo32
getSelectVectorY( MOUSE_STATE * mouse, MOUSE_BUTTON_ID buttonID ) {
	flo32 result = mouse->position.y - mouse->button[ buttonID ].selectPos.y;
	return result;
}

//----------
// keyboard functions
//----------

internal KEY_STATE
KeyState( KEY_STATE_CODE keyCode, boo32 isDown, uint8 flags ) {
	KEY_STATE result = {};
	result.keyCode     = keyCode;
	result.isDown      = isDown;
	result.flags       = flags;
	return result;
}

internal boo32
wasPressed( KEYBOARD_STATE * keyboard, KEY_STATE_CODE keyCode, uint8 targetFlags = 0 ) {
	boo32 result = false;
	for( int32 event_index = 0; event_index < keyboard->event_count; event_index++ ) {
		KEY_STATE event = keyboard->event[ event_index ];
		if( ( event.keyCode == keyCode ) && ( event.isDown ) && ( keyboard->flags == targetFlags ) ) {
			result = true;
		}
	}
	return result;
}

internal boo32
wasReleased( KEYBOARD_STATE * keyboard, KEY_STATE_CODE keyCode ) {
	boo32 result = false;
	for( int32 event_index = 0; event_index < keyboard->event_count; event_index++ ) {
		KEY_STATE event = keyboard->event[ event_index ];
		if( ( event.keyCode == keyCode ) && ( !event.isDown ) ) {
			result = true;
		}
	}
	return result;
}

internal void
addKeyEvent( KEYBOARD_STATE * keyboard, KEY_STATE_CODE keyCode, boo32 isDown, uint8 flags ) {
	Assert( keyboard->event_count < KEYBOARD_STATE__MAX_EVENT_COUNT );
	keyboard->event[ keyboard->event_count++ ] = KeyState( keyCode, isDown, flags );
}

//----------
// controller functions
//----------

internal boo32
wasButtonPressed( CONTROLLER_BUTTON_STATE * button ) {
	boo32 result = false;
	if( (  button->isDown && ( button->halfTransitionCount >= 1 ) ) ||
      ( !button->isDown && ( button->halfTransitionCount >= 2 ) ) ) {
		result = true;
	}
	return result;
}

internal boo32
wasPressed( CONTROLLER_STATE * controller, CONTROLLER_BUTTON_ID buttonID ) {
	CONTROLLER_BUTTON_STATE * button = controller->button + buttonID;
	boo32 result = wasButtonPressed( button );
	return result;
}

internal boo32
wasReleased( CONTROLLER_BUTTON_STATE * button ) {
	boo32 result = false;
	if( ( !button->isDown && ( button->halfTransitionCount >= 1 ) ) ||
      (  button->isDown && ( button->halfTransitionCount >= 2 ) ) ) {
		result = true;
	}
	return result;
}

internal boo32
wasReleased( CONTROLLER_STATE * controller, CONTROLLER_BUTTON_ID buttonID ) {
	CONTROLLER_BUTTON_STATE * button = controller->button + buttonID;
	boo32 result = wasReleased( button );
	return result;
}

internal void
updateButton( CONTROLLER_STATE * controller, CONTROLLER_BUTTON_ID buttonID, boo32 isDown ) {
   CONTROLLER_BUTTON_STATE * button = controller->button + buttonID;
   if( (  button->isDown && !isDown ) ||
      ( !button->isDown &&  isDown ) ) {
      button->halfTransitionCount++;
   }
   button->isDown = isDown;
}

#endif	// STD_INCLUDE_FUNC