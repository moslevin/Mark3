#ifndef __BREAKOUT_H__
#define __BREAKOUT_H__

#include "graphics.h"
#include "mark3cfg.h"
#include "kerneltypes.h"
#include "font_8.h"
#include "memutil.h"
#include "joystick.h"
#include "tiles.h"
#include "brcommon.h"
#include "brscore.h"
#include "brball.h"
#include "brpaddle.h"
#include "brlevel.h"
#include "brlives.h"
#include "brplayfield.h"
#include "highscore.h"
#include "brlazer.h"

//---------------------------------------------------------------------------
typedef enum
{
    GAME_STATE_TITLE,
	GAME_STATE_ATTRACT,
	GAME_STATE_NEXT_LEVEL,
	GAME_STATE_BALL_WAIT,
	GAME_STATE_ACTIVE,
	GAME_STATE_LOSE_LIFE,
	GAME_STATE_GAME_OVER,
	GAME_STATE_HIGH_SCORE,
	GAME_STATE_LIST_SCORES,
    GAME_STATE_BEGIN,
	//--
	GAME_STATES
} GameState_t;

//---------------------------------------------------------------------------
class Breakout
{
public:
	
    Breakout();

    void GameTick();

    void TransitionToState( GameState_t eState_ );

private:

    bool ButtonDown();

    bool ButtonUp();

    bool LeftDown();

    bool RightDown();

    uint8_t CheckBrickHit(uint16_t u16X_, uint16_t u16Y_);

    void UpdateLazer();

    void LoseALife();

    void UpdateBallVelocity( int16_t s16Delta_);

    void GameEnter();

    void GameLoop();

    void GameOverLoop();

    void TitleEnter();

    void TitleExit();

    void TitleLoop();

    void AttractLoopEnter();

    void AttractLoop();

    void HighScoreLoop();

    void ListScoreEnter();

    void ListScoreLoop();

	GameState_t m_eState;
	uint16_t m_u16Counter;
	
	BrBall			m_clBall;		//!< Ball Sprite
	BrPaddle		m_clPaddle;		//!< Paddle sprite defines
	BrPlayField		m_clPlayField;	//!< Bricks, Borders, fixed-hazards
	BrLives			m_clLives;		//!< Lives left + HUD
	BrScore			m_clScore;		//!< Score + HUD
	BrLevel			m_clLevel;		//!< Current level + HUD
    BrLazer         m_clLazer;      //!< Lazor dabs!

	char			m_acInitials[4];	//!< High-score initials
	uint8_t			m_u8InitialId;		//!< Current initial entered in high-score	
    uint16_t        m_sBallOffset;     //!< Offset between ball and paddle on start
};


#endif // __BREAKOUT_H__
