
#include <stdio.h>

#include "kerneltypes.h"
#include "graphics.h"
#include "draw.h"
#include "tiles.h"
#include "breakout.h"

//---------------------------------------------------------------------------
Breakout::Breakout()
{
    m_eState = GAME_STATE_TITLE;
    m_u16Counter = 0;
}

//---------------------------------------------------------------------------
void Breakout::GameTick()
{
    pclJoystick->Scan();
    m_clPlayField.GetRandomData();
    switch (m_eState)
    {
        case GAME_STATE_TITLE:
            TitleLoop();
            break;
        case GAME_STATE_ATTRACT:
            AttractLoop();
            break;
        case GAME_STATE_NEXT_LEVEL:
            break;
        case GAME_STATE_BALL_WAIT:
            break;
        case GAME_STATE_ACTIVE:
            GameLoop();
            break;
        case GAME_STATE_LOSE_LIFE:
            break;
        case GAME_STATE_GAME_OVER:
            GameOverLoop();
            break;
        case GAME_STATE_HIGH_SCORE:
            HighScoreLoop();
            break;
        case GAME_STATE_LIST_SCORES:
            ListScoreLoop();
            break;
        case GAME_STATE_BEGIN:
            TransitionToState(GAME_STATE_LIST_SCORES);
        default:
            break;
    }
}

//---------------------------------------------------------------------------
void Breakout::TransitionToState( GameState_t eState_ )
{
    // Do what needs to be done to exit old state
    switch (m_eState)
    {
        case GAME_STATE_TITLE:
            break;
        case GAME_STATE_ATTRACT:
            break;
        case GAME_STATE_NEXT_LEVEL:
            break;
        case GAME_STATE_BALL_WAIT:
            break;
        case GAME_STATE_ACTIVE:
            break;
        case GAME_STATE_LOSE_LIFE:
            break;
        case GAME_STATE_GAME_OVER:
            break;
        case GAME_STATE_HIGH_SCORE:
            break;
        case GAME_STATE_LIST_SCORES:
            break;
        default:
            break;
    }

    m_eState = eState_;
    m_u16Counter = 0;

    // Code to prepare for new state.
    switch (eState_)
    {
        case GAME_STATE_TITLE:
            TitleEnter();
            break;
        case GAME_STATE_ATTRACT:
            AttractLoopEnter();
            break;
        case GAME_STATE_NEXT_LEVEL:
            break;
        case GAME_STATE_BALL_WAIT:
            break;
        case GAME_STATE_ACTIVE:
            GameEnter();
            break;
        case GAME_STATE_LOSE_LIFE:
            break;
        case GAME_STATE_GAME_OVER:
            break;
        case GAME_STATE_HIGH_SCORE:
            break;
        case GAME_STATE_LIST_SCORES:
            ListScoreEnter();
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
bool Breakout::ButtonDown()
{
    JoystickReport stCurrent;
    JoystickReport stPrevious;

    pclJoystick->ReadCurrent(&stCurrent);
    pclJoystick->ReadPrevious(&stPrevious);

    if ((stCurrent.bButton1 == 1) && (stPrevious.bButton1 == 0))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool Breakout::ButtonUp()
{
    JoystickReport stCurrent;
    JoystickReport stPrevious;

    pclJoystick->ReadCurrent(&stCurrent);
    pclJoystick->ReadPrevious(&stPrevious);

    if ((stCurrent.bButton1 == 0) && (stPrevious.bButton1 == 1))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------

bool Breakout::LeftDown()
{
    JoystickReport stCurrent;
    JoystickReport stPrevious;

    pclJoystick->ReadCurrent(&stCurrent);
    pclJoystick->ReadPrevious(&stPrevious);

    if ((stCurrent.bLeft == 0) && (stPrevious.bLeft == 1))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------

bool Breakout::RightDown()
{
    JoystickReport stCurrent;
    JoystickReport stPrevious;

    pclJoystick->ReadCurrent(&stCurrent);
    pclJoystick->ReadPrevious(&stPrevious);

    if ((stCurrent.bRight == 0) && (stPrevious.bRight == 1))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
uint8_t Breakout::CheckBrickHit(uint16_t u16X_, uint16_t u16Y_)
{
    uint8_t u8HitFlags = 0;
    if (m_clPlayField.CheckBrickExists(u16X_, u16Y_))
    {
        m_clScore.AddScore(30);
        u8HitFlags = m_clPlayField.ClearBrick(u16X_, u16Y_);
    }
    return u8HitFlags;
}

//---------------------------------------------------------------------------
void Breakout::GameEnter()
{
    // Clear the screen
    pclDisplay->ClearScreen();

    m_u16Counter = 300;

    m_clPaddle.SetPosition(60);
    m_clBall.SetPosition(64, 110);
    m_clBall.SetVelocity(200, 200);
    m_sBallOffset = 6;

    m_clPlayField.LoadLevel(0);
    m_clPlayField.Draw();

    m_clLevel.Reset();
    m_clLives.Reset();
    m_clScore.Reset();

    m_clLevel.Draw();
    m_clLives.Draw();
    m_clScore.Draw();

    m_clPaddle.Init( PADDLE_MODE_NORMAL );
}

//---------------------------------------------------------------------------
void Breakout::GameOverLoop()
{
    if (m_u16Counter == 0)
    {
        m_u16Counter = 90;
    }

    DrawText_t stText;
    stText.pcString = "GAME OVER";
    stText.u16Left = 12;
    stText.u16Top = 48;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    switch (m_u16Counter & 0x03)
    {
        case 0: stText.uColor = COLOR_RED;	break;
        case 1: stText.uColor = COLOR_BLUE; break;
        case 2: stText.uColor = COLOR_WHITE; break;
        case 3: stText.uColor = COLOR_GREEN; break;
        default: break;
    }

    TextFX_t stTextFX;
    stTextFX.u8Flags = TEXTFX_FLAG_SCALE_X | TEXTFX_FLAG_SCALE_Y;
    stTextFX.u16ScaleX100 = 200;
    stTextFX.u16ScaleY100 = 200;
    pclDisplay->TextFX(&stText, &stTextFX);
    m_u16Counter--;

    if (!m_u16Counter)
    {
        HighScore clHS;
        HighScore_t stLastHigh;

        clHS.CheckInit();
        clHS.ReadScore(4, &stLastHigh);

        if (stLastHigh.u32Score < m_clScore.GetScore())
        {
            TransitionToState(GAME_STATE_HIGH_SCORE);
        }
        else
        {
            TransitionToState(GAME_STATE_LIST_SCORES);
        }
    }
}

//---------------------------------------------------------------------------
void Breakout::UpdateBallVelocity( int16_t s16Delta_)
{

    int16_t s16VelX, s16VelY;

    int16_t s16BaseWidth = m_clPaddle.GetWidth() / 5;

    if (ABS(s16Delta_) <= s16BaseWidth)
    {
        s16VelX = 141;
        s16VelY = 141;
    }
    else if (ABS(s16Delta_) <= (s16BaseWidth * 3 / 2))
    {
        s16VelX = 154;
        s16VelY = 117;
    }
    else if (ABS(s16Delta_) <= (s16BaseWidth *2) )
    {
        s16VelX = 167;
        s16VelY = 100;
    }
    else
    {
        s16VelX = 181;
        s16VelY = 85;
    }

    // Difficulty based on stage...
    if (m_clLevel.GetLevel() >= 7)
    {
        s16VelX += 75;
        s16VelY += 75;
    }
    else if (m_clLevel.GetLevel() >= 5)
    {
        s16VelX += 50;
        s16VelY += 50;
    }
    else if (m_clLevel.GetLevel() >= 3)
    {
        s16VelX += 25;
        s16VelY += 25;
    }

    if (m_clPlayField.HasHitCieling())
    {
        s16VelX = ((s16VelX * 5) + 2) / 4;
        s16VelY = ((s16VelY * 5) + 2) / 4;
    }

    // Cut speed in half if the current special mode is SLOW
    if (m_clPlayField.GetCurrentSpecial() == SPECIAL_SLOW)
    {
        s16VelX = (s16VelX * 2) / 3;
        s16VelY = (s16VelY * 2) / 3;
    }

    // Swap sign based on current direction
	s16VelX *= 2;
	s16VelY *= 2;
    if (m_clBall.GetVelX() < 0)
    {
        s16VelX *= -1;
    }
    if (m_clBall.GetVelY() < 0)
    {
        s16VelY *= -1;
    }

    m_clBall.SetVelocity(s16VelX, s16VelY);
}

//---------------------------------------------------------------------------
void Breakout::LoseALife()
{
    bool bContinue = m_clLives.Decrement();
    m_clLazer.Destroy();

    if (!bContinue)
    {
        TransitionToState(GAME_STATE_GAME_OVER);
    }
    else
    {
        m_u16Counter = 300;
        m_sBallOffset = 6;
        m_clPlayField.ResetBonus();
        m_clBall.Clear();
        m_clBall.SetPosition(m_clPaddle.GetLeft() + m_sBallOffset, PADDLE_YPOS - 4);
        m_clBall.SetVelocity(141, 141);
        m_clBall.Draw();
        m_clPaddle.Clear();
        m_clPaddle.SetPosition(m_clPaddle.GetLeft());
        m_clPaddle.Draw();        
    }
}

//---------------------------------------------------------------------------
void Breakout::UpdateLazer()
{
    if (m_clPaddle.GetMode() & PADDLE_MODE_LASER)
    {
        if (ButtonDown())
        {
            m_clLazer.Start( m_clPaddle.GetLeft() + (m_clPaddle.GetWidth() + 1) / 2 );
        }
        else
        {
            m_clLazer.Animate();

            if (m_clLazer.IsActive())
            {
                if (m_clLazer.GetY() <= PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT + 4 ) {
                    m_clLazer.Destroy();
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
void Breakout::GameLoop()
{
    uint16_t u16PixX, u16PixY;
    bool bContinue;
    bool bFirstUpdate = true;

    // Compute velocity to integrate this frame
    m_clBall.Update();

    // Read the current joystick state and update the
    // paddle position
    JoystickReport stCurrent;
    pclJoystick->ReadCurrent(&stCurrent);

    // Update the paddle's position for this frame
    if (stCurrent.bLeft == 1)
    {
        m_clPaddle.MoveLeft();
    }
    else if (stCurrent.bRight == 1)
    {
        m_clPaddle.MoveRight();
    }
    else
    {
        m_clPaddle.Decel();
    }


    // If this is a "normal" frame - and not a "get-ready" frame...
    if (!m_u16Counter)
    {
        // Update the lazer location (if active)
        UpdateLazer();

        m_clPlayField.LevelAction(LEVEL_ACTION_STEP);
        do
        {
            uint8_t u8Flags = 0;

            bool bFlipX = false;
            bool bFlipY = false;

            // Check to see if this is the last ball update to make this frame.
            bContinue = m_clBall.MoveNextPixel(&u16PixX, &u16PixY);
            if (bFirstUpdate)
            {
                m_clBall.Clear();
                bFirstUpdate = false;
            }

            // Hit detection - ball/bricks
            u8Flags |= CheckBrickHit(u16PixX, u16PixY);
            u8Flags |= CheckBrickHit(u16PixX, u16PixY + BALL_SIZE - 1);
            u8Flags |= CheckBrickHit(u16PixX + BALL_SIZE - 1, u16PixY);
            u8Flags |= CheckBrickHit(u16PixX + BALL_SIZE - 1, u16PixY + BALL_SIZE - 1);

            // Hit detection - lazer/bricks
            uint8_t u8LazerFlags = 0;
            {
                uint16_t u16X = m_clLazer.GetX();
                u8LazerFlags|= CheckBrickHit( u16X - 7, m_clLazer.GetY() );
                u8LazerFlags |= CheckBrickHit( u16X + 6, m_clLazer.GetY() );
                if (u8LazerFlags)
                {
                    m_clLazer.Destroy();
                }
            }

            // Check for bonus brick
            if ((u8Flags | u8LazerFlags) & BRICK_HIT_SPECIAL)
            {
                Special_t eBonus = m_clPlayField.SelectBonus();

                // Handle *immediate* changes that take place on certain bonus/special
                // modes.
                switch (eBonus)
                {
                    case SPECIAL_1UP:        m_clLives.Increment(); break;
                    case SPECIAL_BONUS:      m_clScore.AddScore(2000); break;
                    case SPECIAL_LONG:
                        m_clPaddle.Clear();
                        m_clPaddle.Init(PADDLE_MODE_LONG);
                        m_clPaddle.Draw();
                        break;
                    case SPECIAL_FIREPOWER:
                        m_clPaddle.Clear();
                        m_clPaddle.Init(PADDLE_MODE_LASER);
                        m_clPaddle.Draw();
                    default:
                        break;
                }

                // Skip to the next level
                if (eBonus == SPECIAL_SKIP)
                {
                    m_clLazer.Destroy();
                    m_clBall.SetPosition(0,0);
                    m_clBall.Clear();
                    m_clPaddle.Clear();
                    m_clPaddle.Init( PADDLE_MODE_NORMAL );
                    m_clPaddle.Draw();
                    m_clLevel.NextLevel();
                    m_clLevel.Draw();
					m_clPlayField.ResetBonus();
                    m_clPlayField.LoadLevel( m_clLevel.GetLevel() );
                    m_clPlayField.Draw();
                    m_u16Counter = 300;
                    return;
                }
            }

            // Hit detection - playfield boundary
            if (u16PixX >= (PLAYFIELD_RIGHT_BORDER - BALL_SIZE))
            {
                m_clBall.ToggleDirX();                
                m_clBall.SetPosition(PLAYFIELD_RIGHT_BORDER - BALL_SIZE - 1, u16PixY);
                m_clPlayField.LevelAction(LEVEL_ACTION_WALL_HIT);
                bFlipX = true;
            }
            else if (u16PixX <= PLAYFIELD_BORDER_WIDTH)
            {
                m_clBall.ToggleDirX();                
                m_clBall.SetPosition(PLAYFIELD_BORDER_WIDTH + 1, u16PixY);
                m_clPlayField.LevelAction(LEVEL_ACTION_WALL_HIT);
                bFlipX = true;
            }
            if (u16PixY <= (PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT))
            {
                m_clBall.ToggleDirY();
                m_clBall.SetPosition(u16PixX, PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT + 1);
                m_clPlayField.LevelAction(LEVEL_ACTION_CIELING_HIT);
                bFlipY = true;
				{
					SquareWave_t stWave;
					stWave.u8Level = 64;
					stWave.u16Freq = TONE(C,5);
					stWave.u16DurationMS = 300;
					pclSound->Control(SOUND_EVENT_SQUAREWAVE, &stWave, 0,0,0);
				}
            }

            // Hit detection - paddle/ball
            if ((u16PixY > (PADDLE_YPOS - BALL_SIZE)) && (m_clBall.GetVelY() > 0))
            {                
                if ( ((u16PixX + BALL_SIZE - 1) >= m_clPaddle.GetLeft()) &&
                     (u16PixX <= m_clPaddle.GetRight()) )
                {            
                    // Ball hit the paddle, reflect with a specific angle..
                    int16_t s16Delta = (int16_t)u16PixX - (int16_t)((m_clPaddle.GetRight() + m_clPaddle.GetLeft()) / 2);

                    m_clBall.Clear();
                    m_clBall.SetPosition(u16PixX, (PADDLE_YPOS - BALL_SIZE));

                    // Ball is 6 pixels wide - adjust so that we compare absolute values from
                    // Ball's midpoint.
                    if (s16Delta < 0)
                    {
                        s16Delta += 3;
                    }

                    m_clPlayField.LevelAction(LEVEL_ACTION_PADDLE_HIT);

                    // If the ball is in the middle of the platform, and headed in the same direction
                    // (or the paddle is stationary), then reflect in Y only.
                    m_clBall.ToggleDirY();

                    if ( ( ((m_clPaddle.GetVelocity() > 0) && (m_clBall.GetVelX() < 0)) ) ||
                         ( ((m_clPaddle.GetVelocity() < 0) && (m_clBall.GetVelX() > 0)) ) )
                    {
                        m_clBall.ToggleDirX();
                    }
                    // Otherwise, if paddle is stationary, but ball hits too far off-center
                    else if ( ((m_clBall.GetVelX() > 0) && ((s16Delta) <= -6)) ||
                              ((m_clBall.GetVelX() < 0) && ((s16Delta) >= 6)) )
                    {
                        m_clBall.ToggleDirX();
                    }

                    UpdateBallVelocity(s16Delta);

                    // Special handling if we're in "ball catch" mode.
                    if (m_clPlayField.GetCurrentSpecial() == SPECIAL_CATCH)
                    {
                        m_clBall.Update();
                        m_sBallOffset = m_clBall.GetX() - m_clPaddle.GetLeft();

                        m_u16Counter = 300;
                        m_clPaddle.SetPosition(m_clPaddle.GetLeft());
                        m_clPaddle.Draw();
                        return;
                    }

                    m_clBall.Update();

                    m_clPaddle.SetPosition(m_clPaddle.GetLeft());
                    m_clPaddle.Draw();                    

                }
                else if (u16PixY > PADDLE_YPOS)
                {
                    // Lose a life...
                    LoseALife();
                    return;
                }
            }


            // Hit Detection - brick
            if (m_clPlayField.GetType() != BR_LEVEL_BREAKTHROUGH )
            {
                if (!bFlipX)
                {
                    if ((u8Flags & BRICK_HIT_LEFT) && (m_clBall.GetVelX() > 0))
                    {
                        m_clBall.ToggleDirX();
                    }
                    if ((u8Flags & BRICK_HIT_RIGHT) && (m_clBall.GetVelX() < 0))
                    {
                        m_clBall.ToggleDirX();
                    }
                }

                if (!bFlipY)
                {
                    if ((u8Flags & BRICK_HIT_TOP) && (m_clBall.GetVelY() > 0))
                    {
                        m_clBall.ToggleDirY();
                    }
                    if ((u8Flags & BRICK_HIT_BOTTOM) && (m_clBall.GetVelY() < 0))
                    {
                        m_clBall.ToggleDirY();
                    }
                }
            }

        } while (bContinue);
    } else {
        if (m_clPaddle.GetVelocity() != 0)
        {
            m_clBall.Clear();
        }

        m_clBall.SetPosition(m_clPaddle.GetLeft() + m_sBallOffset, PADDLE_YPOS - 4);
        m_clBall.Draw();
        m_u16Counter--;

        if (ButtonDown())
        {
            m_u16Counter = 0;
        }
    }

    m_clPaddle.Update();

    m_clBall.Draw();    
    m_clPaddle.Draw();
    m_clScore.Draw();
    m_clLives.Draw();
    m_clLevel.Draw();

    if (m_clPlayField.IsPlayFieldEmpty())
    {
        m_clLazer.Destroy();
        m_clBall.SetPosition(0,0);
        m_clBall.Clear();
        m_clPaddle.Clear();
        m_clPaddle.Init( PADDLE_MODE_NORMAL );
        m_clLevel.NextLevel();
        m_clLevel.Draw();
        m_clPlayField.LoadLevel( m_clLevel.GetLevel() );
        m_clPlayField.Draw();
        m_clPlayField.ResetBonus();
        m_u16Counter = 300;
    }
}

//---------------------------------------------------------------------------
void Breakout::TitleEnter()
{
    DrawText_t stText;
    DrawRectangle_t stRect;

    pclDisplay->ClearScreen();

    m_u16Counter = 15 * 60;

    stRect.u16Left = 8;
    stRect.u16Right = SCREEN_WIDTH - 9;
    stRect.u16Top = 24;
    stRect.u16Bottom = 80;
    stRect.bFill = true;
    stRect.u32ineColor = RGB_COLOR(MAX_RED / 4, 0, MAX_BLUE / 4);
    stRect.uFillColor = stRect.u32ineColor;

    pclDisplay->Rectangle(&stRect);

    stRect.bFill = true;
    stRect.uFillColor = COLOR_PURPLE;
    stRect.u32ineColor = COLOR_WHITE;

    for (uint8_t u8Col = 0; u8Col < 10; u8Col++)
    {
        uint8_t u8Height;
        switch (u8Col)
        {
        case 0:
            u8Height = 7;
            break;
        case 1:
            u8Height = 3;
            break;
        case 2:
            u8Height = 6;
            break;
        case 3:
            u8Height = 4;
            break;
        case 4:
            u8Height = 8;
            break;
        case 5:
            u8Height = 2;
            break;
        case 6:
            u8Height = 5;
            break;
        case 7:
            u8Height = 1;
            break;
        case 8:
            u8Height = 4;
            break;
        case 9:
            u8Height = 7;
            break;
        default:
            break;
        }

        stRect.u16Left = 8 + (u8Col * (((SCREEN_WIDTH - 16)+5)/10));
        stRect.u16Right = stRect.u16Left + (((SCREEN_WIDTH - 16)+5)/10);

        while (u8Height)
        {
            stRect.uFillColor = RGB_COLOR( (MAX_RED * u8Height) / 10, 0, (MAX_BLUE * u8Height) / 10);
            stRect.u32ineColor = RGB_COLOR( (MAX_RED * u8Height) / 10, (MAX_GREEN * u8Height) / 10, (MAX_BLUE * u8Height) / 10);
            stRect.u16Top = 80 - (u8Height * 6);
            stRect.u16Bottom = stRect.u16Top + 5;

            pclDisplay->Rectangle(&stRect);
            u8Height--;
        }
    }

    TextFX_t stTextFX;

    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.pcString = "Wellesley Rocks!";
    stText.u16Left = 5;
    stText.u16Top = SCREEN_HEIGHT - 8;
    stText.uColor = COLOR_GREEN;

    pclDisplay->Text(&stText);

    stText.pcString = "BRICK";
    stText.u16Top = 41;
    stText.u16Left = 23;
    stText.uColor = COLOR_BLACK;

    stTextFX.u8Flags = TEXTFX_FLAG_SCALE_X | TEXTFX_FLAG_SCALE_Y;
    stTextFX.u16ScaleX100 = 300;
    stTextFX.u16ScaleY100 = 200;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.uColor = COLOR_YELLOW;
    stText.u16Top -= 2;
    stText.u16Left -= 2;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.pcString = "BREAK";

    stText.uColor = COLOR_BLACK;
    stText.u16Top += 20;
    stText.u16Left += 2;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.uColor = COLOR_YELLOW;
    stText.u16Top -= 2;
    stText.u16Left -= 2;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.u16Top = 26;
    stText.u16Left = 10;
    stText.pcString = "SUPER!";
    stText.uColor = COLOR_GREY25;
    pclDisplay->Text(&stText);

    stText.uColor = COLOR_RED;
    stText.u16Top --;
    stText.u16Left --;
    pclDisplay->Text(&stText);
}

//---------------------------------------------------------------------------
void Breakout::TitleExit()
{

}

//---------------------------------------------------------------------------
void Breakout::TitleLoop()
{
    DrawText_t stText;
    stText.pcString = "PRESS START";
    stText.u16Left = 32;
    stText.u16Top = 110;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    if ((m_u16Counter % 120) == 0)
    {
        stText.uColor = COLOR_WHITE;
        pclDisplay->Text(&stText);
    }
    else if ((m_u16Counter % 60) == 0)
    {
        stText.uColor = COLOR_BLACK;
        pclDisplay->Text(&stText);
    }

    m_u16Counter--;

    if (!m_u16Counter)
    {
        TransitionToState(GAME_STATE_ATTRACT);
    }

    if (ButtonDown())
    {
        TransitionToState(GAME_STATE_ACTIVE);
    }
}

//---------------------------------------------------------------------------
void Breakout::AttractLoopEnter()
{

    static uint8_t u8Attract = 0;
    int16_t s16VelX, s16VelY;

    m_u16Counter = 3600; // One minute @ 60fps

    // Clear the screen
    pclDisplay->ClearScreen();

    // Have a few different variants for fun.
    switch (u8Attract)
    {
        case 0:
            s16VelX = 141;
            s16VelY = 141;
            break;
        case 1:
            s16VelX = 154;
            s16VelY = 117;
            break;
        case 2:
            s16VelX = 167;
            s16VelY = 100;
            break;
        case 3:
            s16VelX = 181;
            s16VelY = 85;
            break;
        default:
            break;
    }

    u8Attract = (u8Attract + 1) ;

    m_clPaddle.SetPosition(60);
    m_clBall.SetPosition(64, 130);
    m_clBall.SetVelocity(s16VelX * 2, s16VelY * 2);

    m_clPlayField.LoadLevel(u8Attract * 3);
    m_clPlayField.Draw();

    m_clLevel.Reset();
    m_clLives.Reset();
    m_clScore.Reset();

    m_clLevel.Draw();
    m_clLives.Draw();
    m_clScore.Draw();

}

//---------------------------------------------------------------------------
void Breakout::AttractLoop()
{
    if (ButtonDown())
    {
        TransitionToState(GAME_STATE_TITLE);
        return;
    }


    uint16_t u16PixX, u16PixY;

    if (m_clPlayField.IsPlayFieldEmpty())
    {
        m_clLevel.NextLevel();
        m_clLevel.Draw();
        m_clPlayField.LoadLevel( m_clLevel.GetLevel() );
        m_clPlayField.Draw();
    }

    m_clBall.Update();
    m_clBall.Clear();
    bool bContinue;

    do
    {
        uint8_t u8Flags = 0;
        bContinue = m_clBall.MoveNextPixel(&u16PixX, &u16PixY);

        u8Flags |= CheckBrickHit(u16PixX, u16PixY);
        u8Flags |= CheckBrickHit(u16PixX, u16PixY + BALL_SIZE - 1);
        u8Flags |= CheckBrickHit(u16PixX + BALL_SIZE - 1, u16PixY);
        u8Flags |= CheckBrickHit(u16PixX + BALL_SIZE - 1, u16PixY + BALL_SIZE - 1);

        bool bFlipX = false;
        bool bFlipY = false;

        if (u16PixX >= (PLAYFIELD_RIGHT_BORDER - BALL_SIZE))
        {
            m_clBall.ToggleDirX();
            m_clBall.SetX(PLAYFIELD_RIGHT_BORDER - BALL_SIZE-1);
            bFlipX = true;
        }
        else if (u16PixX <= PLAYFIELD_BORDER_WIDTH)
        {
            m_clBall.ToggleDirX();
            m_clBall.SetX(PLAYFIELD_BORDER_WIDTH + 1);
            bFlipX = true;
        }

        if (u16PixY >= (PADDLE_YPOS - BALL_SIZE))
        {
            m_clBall.ToggleDirY();
            m_clBall.SetY(PADDLE_YPOS - BALL_SIZE - 1);
            bFlipY = true;
        }
        else if (u16PixY <= (PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT))
        {
            m_clBall.ToggleDirY();
            m_clBall.SetY(PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT + 1);
            bFlipY = true;
        }

        if (!bFlipX)
        {
            if ((u8Flags & BRICK_HIT_LEFT) && (m_clBall.GetVelX() > 0))
            {
                m_clBall.ToggleDirX();
            }
            if ((u8Flags & BRICK_HIT_RIGHT) && (m_clBall.GetVelX() < 0))
            {
                m_clBall.ToggleDirX();
            }
        }
        if (!bFlipY)
        {
            if ((u8Flags & BRICK_HIT_TOP) && (m_clBall.GetVelY() > 0))
            {
                m_clBall.ToggleDirY();
            }
            if ((u8Flags & BRICK_HIT_BOTTOM) && (m_clBall.GetVelY() < 0))
            {
                m_clBall.ToggleDirY();
            }
        }

    } while (bContinue);
    if (u16PixX < m_clPaddle.GetLeft())
    {
        m_clPaddle.SetVelocity((int16_t)u16PixX - (int16_t)m_clPaddle.GetLeft());
    }
    if (u16PixX > m_clPaddle.GetRight())
    {
        m_clPaddle.SetVelocity((int16_t)u16PixX - (int16_t)m_clPaddle.GetRight());
    }

    m_clPaddle.Update();
    m_clBall.Draw();
    m_clPaddle.Draw();
    m_clScore.Draw();
    m_clLives.Draw();
    m_clLevel.Draw();

    DrawText_t stText;

    stText.u16Left = 40;
    stText.u16Top = 60;
    stText.uColor = COLOR_RED;

    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.pcString = "GAME OVER";
    pclDisplay->Text(&stText);

    m_u16Counter--;
    if (0 == m_u16Counter)
    {
        static bool bNextIsScores = false;
        if (bNextIsScores)
        {
            TransitionToState(GAME_STATE_LIST_SCORES);
            bNextIsScores = false;
        }
        else
        {
            TransitionToState(GAME_STATE_TITLE);
            bNextIsScores = true;
        }
    }
}


//---------------------------------------------------------------------------
void Breakout::HighScoreLoop()
{
    DrawText_t stText;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    bool bRedraw = true;

    if (!m_u16Counter)
    {
        m_u16Counter = 1800; //3600; // 60 seconds to enter high score;
        m_u8InitialId = 0;

        m_acInitials[0] = 'A';
        m_acInitials[1] = 'A';
        m_acInitials[2] = 'A';
        m_acInitials[3] = '\0';

        pclDisplay->ClearScreen();

        stText.pcString = "TIME";
        stText.u16Left = (SCREEN_WIDTH/2) - 20;
        stText.u16Top = 8;
        stText.uColor = COLOR_RED;
        pclDisplay->Text(&stText);
    }

    // Deal with the current letter
    bool bLeft = LeftDown();
    bool bRight = RightDown();
    bool bExit = false;

    if (bLeft || bRight)
    {
        if (bLeft)
        {
            if (m_acInitials[m_u8InitialId] > 'A')
            {
                m_acInitials[m_u8InitialId]--;
                bRedraw = true;
            }
        }
        if (bRight)
        {
            if (m_acInitials[m_u8InitialId] < 'Z')
            {
                m_acInitials[m_u8InitialId]++;
                bRedraw = true;
            }
        }
    }

    // Check to see if we have to progress to the next letter
    if (ButtonDown())
    {
        m_u8InitialId ++;
        if(m_u8InitialId == 3)
        {
            bExit = true;
        }
        bRedraw = true;
    }

    if (bRedraw && !bExit)
    {
        DrawRectangle_t stRect;

        TextFX_t stTextFX;

        stRect.bFill = true;
        stRect.uFillColor = COLOR_BLACK;
        stRect.u32ineColor = COLOR_BLACK;
        stRect.u16Top = 60;
        stRect.u16Bottom = 76;
        stRect.u16Left = (SCREEN_WIDTH/2) - 18;
        stRect.u16Right = stRect.u16Left + 36;
        pclDisplay->Rectangle(&stRect);

        stTextFX.u8Flags = TEXTFX_FLAG_SCALE_X | TEXTFX_FLAG_SCALE_Y;
        stTextFX.u16ScaleX100 = 200;
        stTextFX.u16ScaleY100 = 200;

        uint8_t i;
        for (i = 0; i < 3; i++)
        {
            char acTmp[2];
            acTmp[0] = m_acInitials[i];
            acTmp[1] = 0;

            stText.pcString = acTmp;
            stText.u16Left = (SCREEN_WIDTH/2) - 18 + (12*i);
            stText.u16Top = 60;
            if (i == m_u8InitialId)
            {
                stText.uColor = COLOR_WHITE;
            }
            else
            {
                stText.uColor = COLOR_RED;
            }

            pclDisplay->TextFX( &stText, &stTextFX );
        }
    }

    if (!bExit)
    {
        stText.pcString = "HIGH SCORE!";
        stText.u16Left = (SCREEN_WIDTH/2) - 28;
        stText.u16Top = 40;
        switch (m_u16Counter % 0x03)
        {
            case 0: stText.uColor = COLOR_RED; break;
            case 1: stText.uColor = COLOR_GREEN; break;
            case 2: stText.uColor = COLOR_BLUE; break;
            case 3: stText.uColor = COLOR_WHITE; break;
        }
        pclDisplay->Text(&stText);
    }

    if (((m_u16Counter % 60) == 0) && !bExit)
    {
        DrawRectangle_t stRect;

        stRect.bFill = true;
        stRect.uFillColor = COLOR_BLACK;
        stRect.u32ineColor = COLOR_BLACK;
        stRect.u16Top = 8;
        stRect.u16Bottom = 15;
        stRect.u16Left = (SCREEN_WIDTH/2);
        stRect.u16Right = stRect.u16Left + 15;
        pclDisplay->Rectangle(&stRect);

        char acTimeStr[4];
        MemUtil::DecimalToString((uint16_t)(m_u16Counter/60), (char*)acTimeStr);

        stText.pcString = acTimeStr;
        stText.u16Left = (SCREEN_WIDTH/2) + 5;
        stText.u16Top = 8;
        stText.uColor = COLOR_RED;
        pclDisplay->Text(&stText);
    }
    m_u16Counter--;

    if (!m_u16Counter)
    {
        bExit = true;
    }

    if (bExit)
    {
        HighScore clHS;
        HighScore_t myScore;
        myScore.u32Score = m_clScore.GetScore();
        myScore.acName[0] = m_acInitials[0];
        myScore.acName[1] = m_acInitials[1];
        myScore.acName[2] = m_acInitials[2];
        myScore.acName[3] = 0;

        clHS.CheckInit();
        clHS.AddNewScore(&myScore);
        TransitionToState(GAME_STATE_LIST_SCORES);
    }
}

//---------------------------------------------------------------------------
void Breakout::ListScoreEnter()
{
    DrawText_t stText;

    m_u16Counter = 600;

    // Draw list of high scores
    pclDisplay->ClearScreen();

    HighScore clHS;
    clHS.CheckInit();

    stText.u16Left = 35;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.uColor = COLOR_WHITE;

    for (uint8_t i = 0; i < 5; i++)
    {
        HighScore_t stScore;
        clHS.ReadScore(i, &stScore);
        stText.u16Top = 64 + (i * 8);
        stText.u16Left = 35;
        stText.pcString = stScore.acName;

        pclDisplay->Text(&stText);

        stText.u16Left = 55;
        stText.pcString = "-";
        pclDisplay->Text(&stText);

        char acStr[11];
        MemUtil::DecimalToString(stScore.u32Score, acStr);
        stText.u16Left = 65;
        stText.pcString = acStr;
        pclDisplay->Text(&stText);
    }
}

//---------------------------------------------------------------------------
void Breakout::ListScoreLoop()
{
    if (ButtonDown())
    {
        TransitionToState(GAME_STATE_TITLE);
        return;
    }

    DrawText_t stText;
    m_u16Counter--;

    stText.pcString = "HIGH SCORES";
    stText.u16Left = 35;
    stText.u16Top = 40;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    switch (m_u16Counter & 0x03)
    {
        case 0: stText.uColor = COLOR_RED;	break;
        case 1: stText.uColor = COLOR_BLUE; break;
        case 3: stText.uColor = COLOR_GREEN; break;
        case 2: stText.uColor = COLOR_WHITE; break;
        default: break;
    }

    pclDisplay->Text(&stText);

    DrawRectangle_t stRect;
    stRect.bFill = true;

    switch (m_u16Counter % 5)
    {
        case 0: stRect.u32ineColor = COLOR_ORANGE; break;
        case 1: stRect.u32ineColor = COLOR_GREEN; break;
        case 2: stRect.u32ineColor = COLOR_BLUE; break;
        case 3: stRect.u32ineColor = COLOR_RED; break;
        case 4: stRect.u32ineColor = COLOR_YELLOW; break;
        default: break;
    }

    stRect.uFillColor = stRect.u32ineColor;

    // Colorful borders - top
    stRect.u16Top = 0;
    stRect.u16Bottom = 7;
    stRect.u16Left = (m_u16Counter << 3) & (SCREEN_WIDTH - 1);
    stRect.u16Right = stRect.u16Left + 7;
    pclDisplay->Rectangle(&stRect);

    // Bottom
    stRect.u16Top = SCREEN_HEIGHT - 8;
    stRect.u16Bottom = SCREEN_HEIGHT - 1;
    stRect.u16Right = (SCREEN_WIDTH - (m_u16Counter << 3)) & (SCREEN_WIDTH - 1);
    if (stRect.u16Right == 0)
    {
        stRect.u16Right = SCREEN_WIDTH - 1;
    }
    else
    {
        stRect.u16Right--;
    }
    stRect.u16Left = stRect.u16Right - 7;
    pclDisplay->Rectangle(&stRect);

    // Left
    stRect.u16Left = 0;
    stRect.u16Right = 7;
    stRect.u16Top = 8 + ((m_u16Counter << 3) % (SCREEN_HEIGHT - 16));
    stRect.u16Bottom = stRect.u16Top + 7;
    pclDisplay->Rectangle(&stRect);

    // Right
    stRect.u16Left = SCREEN_WIDTH - 8;
    stRect.u16Right = SCREEN_WIDTH - 1;
    stRect.u16Top = (SCREEN_HEIGHT - 16) - ((m_u16Counter << 3) % (SCREEN_HEIGHT -16));
    if (0 == stRect.u16Top)
    {
        stRect.u16Top = SCREEN_HEIGHT - 16;
    }
    stRect.u16Bottom = stRect.u16Top + 7;
    pclDisplay->Rectangle(&stRect);
    if (!m_u16Counter)
    {
        TransitionToState(GAME_STATE_ATTRACT);
    }
}
