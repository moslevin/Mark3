#ifndef __BRCOMMON_H__
#define __BRCOMMON_H__

#include "mark3cfg.h"
#include "kerneltypes.h"
#include "joystick.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "memutil.h"
#include "font_8.h"
#include "drvSound.h"

//---------------------------------------------------------------------------
// Graphical elements - screen layout, etc.
#define SCREEN_WIDTH				(128)
#define SCREEN_HEIGHT				(160)

#define PLAYFIELD_BORDER_WIDTH		(4)
#define PLAYFIELD_BORDER_HEIGHT		(4)
#define HUD_HEIGHT					(16)

#define PLAYFIELD_LEFT_BORDER		(PLAYFIELD_BORDER_WIDTH)
#define PLAYFIELD_RIGHT_BORDER		(SCREEN_WIDTH - PLAYFIELD_BORDER_WIDTH)

#define PLAYFIELD_ACTIVE_WIDTH		(SCREEN_WIDTH - (2 * PLAYFIELD_BORDER_WIDTH))
#define PLAYFIELD_ACTIVE_HEIGHT		(SCREEN_HEIGHT - HUD_HEIGHT - PLAYFIELD_BORDER_HEIGHT)

#define PLAYFIELD_BRICK_MARGIN		(12)

#define LEVEL_NUM_COLS				(12)
#define LEVEL_NUM_ROWS				(10)

#define BRICK_WIDTH					(PLAYFIELD_ACTIVE_WIDTH / LEVEL_NUM_COLS)
#define BRICK_HEIGHT				(5)

#define BALL_SIZE					(4)

#define PADDLE_WIDTH				(24)
#define PADDLE_WIDTH_LONG           (32)
#define PADDLE_HEIGHT				(6)
#define PADDLE_YPOS					(SCREEN_HEIGHT - PADDLE_HEIGHT - 4)

#define PADDLE_MAX_VELOCITY			(6)

#define LIVES_X						(104)
#define LEVEL_X						(84)
#define SCORE_X						(8)

//---------------------------------------------------------------------------
#define ABS(x) ((x) < 0 ? ((x)*-1) : (x))

//---------------------------------------------------------------------------
extern GraphicsDriver *pclDisplay;
extern JoystickDriver *pclJoystick;
extern SoundDriver    *pclSound;
extern Tile_8x8       clMyTile;

#endif
