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

    K_BOOL ButtonDown();

    K_BOOL ButtonUp();

    K_BOOL LeftDown();

    K_BOOL RightDown();

    K_UCHAR CheckBrickHit(K_USHORT usX_, K_USHORT usY_);

    void UpdateLazer();

    void LoseALife();

    void UpdateBallVelocity( K_SHORT sDelta_);

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
	K_USHORT m_usCounter;
	
	BrBall			m_clBall;		//!< Ball Sprite
	BrPaddle		m_clPaddle;		//!< Paddle sprite defines
	BrPlayField		m_clPlayField;	//!< Bricks, Borders, fixed-hazards
	BrLives			m_clLives;		//!< Lives left + HUD
	BrScore			m_clScore;		//!< Score + HUD
	BrLevel			m_clLevel;		//!< Current level + HUD
    BrLazer         m_clLazer;      //!< Lazor dabs!

	K_CHAR			m_acInitials[4];	//!< High-score initials
	K_UCHAR			m_ucInitialId;		//!< Current initial entered in high-score	
    K_USHORT        m_sBallOffset;     //!< Offset between ball and paddle on start
};


#endif // __BREAKOUT_H__
