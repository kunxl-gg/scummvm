#ifndef __QDSCR_KEYWORDS_H__
#define __QDSCR_KEYWORDS_H__

enum qdscrKeywordID
{
	QDSCR_BLOCK_BEG = 1,
	QDSCR_BLOCK_END,

	// Common
	QDSCR_ID,
	QDSCR_X,
	QDSCR_Y,
	QDSCR_SX,
	QDSCR_SY,
	QDSCR_POS2D,
	QDSCR_POS3D,
	QDSCR_SRC_POS,
	QDSCR_DEST_POS,
	QDSCR_FILE,
	QDSCR_START_TIME,
	QDSCR_PERIOD,
	QDSCR_LENGTH,
	QDSCR_SPEED,
	QDSCR_INTERPOLATION_TIME,
	QDSCR_SCALE,
	QDSCR_NAME,
	QDSCR_FLAG,
	QDSCR_RND,
	QDSCR_MASK_SIZE,
	QDSCR_MASK_ATTRIBUTES,
	QDSCR_MASK_HEIGHTS,
	QDSCR_BOUND,
	QDSCR_TYPE,
	QDSCR_SIZE,
	QDSCR_STATE,
	QDSCR_TEXT,
	QDSCR_SHORT_TEXT,

	QDSCR_SCREEN_SIZE,

	// Contours
	QDSCR_CONTOUR_RECTANGLE,
	QDSCR_CONTOUR_CIRCLE,
	QDSCR_CONTOUR_POLYGON,

	// Named Object Reference
	QDSCR_NAMED_OBJECT,

	// Grid zone
	QDSCR_GRID_ZONE,
	QDSCR_GRID_ZONE_CELLS,
	QDSCR_GRID_ZONE_HEIGHT,
	QDSCR_GRID_ZONE_CONTOUR,

	// Trigger Element
	QDSCR_TRIGGER_ELEMENT,
	QDSCR_TRIGGER_ELEMENT_LINK,
	QDSCR_TRIGGER_ELEMENT_TITLE,
	QDSCR_TRIGGER_ELEMENT_CELL_NUMBER,
	QDSCR_TRIGGER_PARENT_LINK_OWNER_OFFSET,
	QDSCR_TRIGGER_PARENT_LINK_CHILD_OFFSET,
	QDSCR_TRIGGER_CHILD_LINK_OWNER_OFFSET,
	QDSCR_TRIGGER_CHILD_LINK_CHILD_OFFSET,

	// Trigger Chain
	QDSCR_TRIGGER_CHAIN,
	QDSCR_TRIGGER_CHAIN_ROOT,
	QDSCR_TRIGGER_CHAIN_WORK_AREA,
	QDSCR_TRIGGER_CHAIN_LAYOUT,

	QDSCR_TRIGGER_BOUND,

	// Sound
	QDSCR_SOUND,

	// Video
	QDSCR_VIDEO,
	QDSCR_VIDEO_POSITION,
	QDSCR_VIDEO_BACKGROUND,

	// Animation
	QDSCR_ANIMATION_FRAME,
	QDSCR_ANIMATION_INFO,
	QDSCR_ANIMATION_FILE,
	QDSCR_ANIMATION,

	// AnimationSet
	QDSCR_ANIMATION_SET,

	// Coords Animation
	QDSCR_COORDS_ANIMATION_POINT,
	QDSCR_COORDS_ANIMATION,

	// Game Object
	QDSCR_PARALLAX_OFFSET,

	// Static Object
	QDSCR_STATIC_OBJECT,

	// Animated Object
	QDSCR_ANIMATED_OBJECT,
	QDSCR_OBJECT_STATE,
	QDSCR_OBJECT_MOVEMENT_STATES,
	QDSCR_OBJECT_MOVEMENT_STATE,

	QDSCR_OBJECT_DIRECTION,
	QDSCR_OBJECT_DEFAULT_POS,
	QDSCR_OBJECT_DEFAULT_DIRECTION,

	QDSCR_OBJECT_STATE_STATIC,
	QDSCR_OBJECT_STATE_WALK,
	QDSCR_OBJECT_STATE_MASK,
	QDSCR_OBJECT_STATE_MASK_POS,
	QDSCR_OBJECT_STATE_MASK_SIZE,
	QDSCR_OBJECT_STATE_MASK_DATA,
	QDSCR_OBJECT_STATE_MASK_PARENT,
	QDSCR_OBJECT_STATE_MASK_CONTOUR,
	QDSCR_OBJECT_STATE_CENTER_OFFSET,
	QDSCR_OBJECT_STATE_DELAY,

	// State conditions
	QDSCR_CONDITION,
	QDSCR_CONDITION_DATA_INT,
	QDSCR_CONDITION_DATA_FLOAT,
	QDSCR_CONDITION_DATA_STRING,
	QDSCR_CONDITIONS_MODE,

	// Object Scale
	QDSCR_OBJ_SCALE_INFO,

	// Mouse object
	QDSCR_MOUSE_OBJECT,

	// Moving Object
	QDSCR_MOVEMENT_ANIMATION_NUMBER,
	QDSCR_MOVEMENT_ANIMATIONS,
	QDSCR_STATIC_ANIMATIONS,
	QDSCR_MOVING_OBJECT,

	// Inventory
	QDSCR_INVENTORY_CELL_TYPE,
	QDSCR_INVENTORY_CELL_SET,
	QDSCR_INVENTORY_CELL_SET_SIZE,
	QDSCR_INVENTORY_CELL_SET_POS,
	QDSCR_INVENTORY,

	// Camera & grid
	QDSCR_CAMERA,
	QDSCR_CAMERA_GRID_SIZE,
	QDSCR_CAMERA_GRID_HEIGHT,
	QDSCR_CAMERA_GRID_ATTRIBUTES,
	QDSCR_CAMERA_GRID_CENTER,
	QDSCR_CAMERA_CELL_SIZE,
	QDSCR_CAMERA_FOCUS,
	QDSCR_CAMERA_ANGLES,
	QDSCR_CAMERA_SCREEN_SIZE,
	QDSCR_CAMERA_SCREEN_OFFSET,
	QDSCR_CAMERA_SCREEN_CENTER,

	// Scene
	QDSCR_SCENE,

	// Location
	QDSCR_LOCATION,

	QDSCR_MAX_KEYWORD_ID
};

// Data types...
enum qdscrDataType
{
	QDSCR_DATA_VOID = 0,
	QDSCR_DATA_INT,
	QDSCR_DATA_DOUBLE,
	QDSCR_DATA_CHAR
};

struct qdscrKeyword
{
	int ID;
	char* name;

	int dataType;
	int dataSize;
};

#endif /* __QDSCR_KEYWORDS_H__ */