
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
    m_usCounter = 0;
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
    m_usCounter = 0;

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
K_BOOL Breakout::ButtonDown()
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
K_BOOL Breakout::ButtonUp()
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

K_BOOL Breakout::LeftDown()
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

K_BOOL Breakout::RightDown()
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
K_UCHAR Breakout::CheckBrickHit(K_USHORT usX_, K_USHORT usY_)
{
    K_UCHAR ucHitFlags = 0;
    if (m_clPlayField.CheckBrickExists(usX_, usY_))
    {
        m_clScore.AddScore(30);
        ucHitFlags = m_clPlayField.ClearBrick(usX_, usY_);
    }
    return ucHitFlags;
}

//---------------------------------------------------------------------------
void Breakout::GameEnter()
{
    // Clear the screen
    pclDisplay->ClearScreen();

    m_usCounter = 300;

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
    if (m_usCounter == 0)
    {
        m_usCounter = 90;
    }

    DrawText_t stText;
    stText.pcString = "GAME OVER";
    stText.usLeft = 12;
    stText.usTop = 48;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    switch (m_usCounter & 0x03)
    {
        case 0: stText.uColor = COLOR_RED;	break;
        case 1: stText.uColor = COLOR_BLUE; break;
        case 2: stText.uColor = COLOR_WHITE; break;
        case 3: stText.uColor = COLOR_GREEN; break;
        default: break;
    }

    TextFX_t stTextFX;
    stTextFX.ucFlags = TEXTFX_FLAG_SCALE_X | TEXTFX_FLAG_SCALE_Y;
    stTextFX.usScaleX100 = 200;
    stTextFX.usScaleY100 = 200;
    pclDisplay->TextFX(&stText, &stTextFX);
    m_usCounter--;

    if (!m_usCounter)
    {
        HighScore clHS;
        HighScore_t stLastHigh;

        clHS.CheckInit();
        clHS.ReadScore(4, &stLastHigh);

        if (stLastHigh.ulScore < m_clScore.GetScore())
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
void Breakout::UpdateBallVelocity( K_SHORT sDelta_)
{

    K_SHORT sVelX, sVelY;

    K_SHORT sBaseWidth = m_clPaddle.GetWidth() / 5;

    if (ABS(sDelta_) <= sBaseWidth)
    {
        sVelX = 141;
        sVelY = 141;
    }
    else if (ABS(sDelta_) <= (sBaseWidth * 3 / 2))
    {
        sVelX = 154;
        sVelY = 117;
    }
    else if (ABS(sDelta_) <= (sBaseWidth *2) )
    {
        sVelX = 167;
        sVelY = 100;
    }
    else
    {
        sVelX = 181;
        sVelY = 85;
    }

    // Difficulty based on stage...
    if (m_clLevel.GetLevel() >= 7)
    {
        sVelX += 75;
        sVelY += 75;
    }
    else if (m_clLevel.GetLevel() >= 5)
    {
        sVelX += 50;
        sVelY += 50;
    }
    else if (m_clLevel.GetLevel() >= 3)
    {
        sVelX += 25;
        sVelY += 25;
    }

    if (m_clPlayField.HasHitCieling())
    {
        sVelX = ((sVelX * 5) + 2) / 4;
        sVelY = ((sVelY * 5) + 2) / 4;
    }

    // Cut speed in half if the current special mode is SLOW
    if (m_clPlayField.GetCurrentSpecial() == SPECIAL_SLOW)
    {
        sVelX = (sVelX * 2) / 3;
        sVelY = (sVelY * 2) / 3;
    }

    // Swap sign based on current direction
	sVelX *= 2;
	sVelY *= 2;
    if (m_clBall.GetVelX() < 0)
    {
        sVelX *= -1;
    }
    if (m_clBall.GetVelY() < 0)
    {
        sVelY *= -1;
    }

    m_clBall.SetVelocity(sVelX, sVelY);
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
        m_usCounter = 300;
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
    K_USHORT usPixX, usPixY;
    K_BOOL bContinue;
    K_BOOL bFirstUpdate = true;

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
    if (!m_usCounter)
    {
        // Update the lazer location (if active)
        UpdateLazer();

        m_clPlayField.LevelAction(LEVEL_ACTION_STEP);
        do
        {
            K_UCHAR ucFlags = 0;

            K_BOOL bFlipX = false;
            K_BOOL bFlipY = false;

            // Check to see if this is the last ball update to make this frame.
            bContinue = m_clBall.MoveNextPixel(&usPixX, &usPixY);
            if (bFirstUpdate)
            {
                m_clBall.Clear();
                bFirstUpdate = false;
            }

            // Hit detection - ball/bricks
            ucFlags |= CheckBrickHit(usPixX, usPixY);
            ucFlags |= CheckBrickHit(usPixX, usPixY + BALL_SIZE - 1);
            ucFlags |= CheckBrickHit(usPixX + BALL_SIZE - 1, usPixY);
            ucFlags |= CheckBrickHit(usPixX + BALL_SIZE - 1, usPixY + BALL_SIZE - 1);

            // Hit detection - lazer/bricks
            K_UCHAR ucLazerFlags = 0;
            {
                K_USHORT usX = m_clLazer.GetX();
                ucLazerFlags|= CheckBrickHit( usX - 7, m_clLazer.GetY() );
                ucLazerFlags |= CheckBrickHit( usX + 6, m_clLazer.GetY() );
                if (ucLazerFlags)
                {
                    m_clLazer.Destroy();
                }
            }

            // Check for bonus brick
            if ((ucFlags | ucLazerFlags) & BRICK_HIT_SPECIAL)
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
                    m_usCounter = 300;
                    return;
                }
            }

            // Hit detection - playfield boundary
            if (usPixX >= (PLAYFIELD_RIGHT_BORDER - BALL_SIZE))
            {
                m_clBall.ToggleDirX();                
                m_clBall.SetPosition(PLAYFIELD_RIGHT_BORDER - BALL_SIZE - 1, usPixY);
                m_clPlayField.LevelAction(LEVEL_ACTION_WALL_HIT);
                bFlipX = true;
            }
            else if (usPixX <= PLAYFIELD_BORDER_WIDTH)
            {
                m_clBall.ToggleDirX();                
                m_clBall.SetPosition(PLAYFIELD_BORDER_WIDTH + 1, usPixY);
                m_clPlayField.LevelAction(LEVEL_ACTION_WALL_HIT);
                bFlipX = true;
            }
            if (usPixY <= (PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT))
            {
                m_clBall.ToggleDirY();
                m_clBall.SetPosition(usPixX, PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT + 1);
                m_clPlayField.LevelAction(LEVEL_ACTION_CIELING_HIT);
                bFlipY = true;
				{
					SquareWave_t stWave;
					stWave.ucLevel = 64;
					stWave.usFreq = TONE(C,5);
					stWave.usDurationMS = 300;
					pclSound->Control(SOUND_EVENT_SQUAREWAVE, &stWave, 0,0,0);
				}
            }

            // Hit detection - paddle/ball
            if ((usPixY > (PADDLE_YPOS - BALL_SIZE)) && (m_clBall.GetVelY() > 0))
            {                
                if ( ((usPixX + BALL_SIZE - 1) >= m_clPaddle.GetLeft()) &&
                     (usPixX <= m_clPaddle.GetRight()) )
                {            
                    // Ball hit the paddle, reflect with a specific angle..
                    K_SHORT sDelta = (K_SHORT)usPixX - (K_SHORT)((m_clPaddle.GetRight() + m_clPaddle.GetLeft()) / 2);

                    m_clBall.Clear();
                    m_clBall.SetPosition(usPixX, (PADDLE_YPOS - BALL_SIZE));

                    // Ball is 6 pixels wide - adjust so that we compare absolute values from
                    // Ball's midpoint.
                    if (sDelta < 0)
                    {
                        sDelta += 3;
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
                    else if ( ((m_clBall.GetVelX() > 0) && ((sDelta) <= -6)) ||
                              ((m_clBall.GetVelX() < 0) && ((sDelta) >= 6)) )
                    {
                        m_clBall.ToggleDirX();
                    }

                    UpdateBallVelocity(sDelta);

                    // Special handling if we're in "ball catch" mode.
                    if (m_clPlayField.GetCurrentSpecial() == SPECIAL_CATCH)
                    {
                        m_clBall.Update();
                        m_sBallOffset = m_clBall.GetX() - m_clPaddle.GetLeft();

                        m_usCounter = 300;
                        m_clPaddle.SetPosition(m_clPaddle.GetLeft());
                        m_clPaddle.Draw();
                        return;
                    }

                    m_clBall.Update();

                    m_clPaddle.SetPosition(m_clPaddle.GetLeft());
                    m_clPaddle.Draw();                    

                }
                else if (usPixY > PADDLE_YPOS)
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
                    if ((ucFlags & BRICK_HIT_LEFT) && (m_clBall.GetVelX() > 0))
                    {
                        m_clBall.ToggleDirX();
                    }
                    if ((ucFlags & BRICK_HIT_RIGHT) && (m_clBall.GetVelX() < 0))
                    {
                        m_clBall.ToggleDirX();
                    }
                }

                if (!bFlipY)
                {
                    if ((ucFlags & BRICK_HIT_TOP) && (m_clBall.GetVelY() > 0))
                    {
                        m_clBall.ToggleDirY();
                    }
                    if ((ucFlags & BRICK_HIT_BOTTOM) && (m_clBall.GetVelY() < 0))
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
        m_usCounter--;

        if (ButtonDown())
        {
            m_usCounter = 0;
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
        m_usCounter = 300;
    }
}

//---------------------------------------------------------------------------
void Breakout::TitleEnter()
{
    DrawText_t stText;
    DrawRectangle_t stRect;

    pclDisplay->ClearScreen();

    m_usCounter = 15 * 60;

    stRect.usLeft = 8;
    stRect.usRight = SCREEN_WIDTH - 9;
    stRect.usTop = 24;
    stRect.usBottom = 80;
    stRect.bFill = true;
    stRect.uLineColor = RGB_COLOR(MAX_RED / 4, 0, MAX_BLUE / 4);
    stRect.uFillColor = stRect.uLineColor;

    pclDisplay->Rectangle(&stRect);

    stRect.bFill = true;
    stRect.uFillColor = COLOR_PURPLE;
    stRect.uLineColor = COLOR_WHITE;

    for (K_UCHAR ucCol = 0; ucCol < 10; ucCol++)
    {
        K_UCHAR ucHeight;
        switch (ucCol)
        {
        case 0:
            ucHeight = 7;
            break;
        case 1:
            ucHeight = 3;
            break;
        case 2:
            ucHeight = 6;
            break;
        case 3:
            ucHeight = 4;
            break;
        case 4:
            ucHeight = 8;
            break;
        case 5:
            ucHeight = 2;
            break;
        case 6:
            ucHeight = 5;
            break;
        case 7:
            ucHeight = 1;
            break;
        case 8:
            ucHeight = 4;
            break;
        case 9:
            ucHeight = 7;
            break;
        default:
            break;
        }

        stRect.usLeft = 8 + (ucCol * (((SCREEN_WIDTH - 16)+5)/10));
        stRect.usRight = stRect.usLeft + (((SCREEN_WIDTH - 16)+5)/10);

        while (ucHeight)
        {
            stRect.uFillColor = RGB_COLOR( (MAX_RED * ucHeight) / 10, 0, (MAX_BLUE * ucHeight) / 10);
            stRect.uLineColor = RGB_COLOR( (MAX_RED * ucHeight) / 10, (MAX_GREEN * ucHeight) / 10, (MAX_BLUE * ucHeight) / 10);
            stRect.usTop = 80 - (ucHeight * 6);
            stRect.usBottom = stRect.usTop + 5;

            pclDisplay->Rectangle(&stRect);
            ucHeight--;
        }
    }

    TextFX_t stTextFX;

    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.pcString = "(C)2014 - FUNKENSTEIN";
    stText.usLeft = 5;
    stText.usTop = SCREEN_HEIGHT - 8;
    stText.uColor = COLOR_GREY25;

    pclDisplay->Text(&stText);

    stText.pcString = "BRICK";
    stText.usTop = 41;
    stText.usLeft = 23;
    stText.uColor = COLOR_BLACK;

    stTextFX.ucFlags = TEXTFX_FLAG_SCALE_X | TEXTFX_FLAG_SCALE_Y;
    stTextFX.usScaleX100 = 300;
    stTextFX.usScaleY100 = 200;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.uColor = COLOR_YELLOW;
    stText.usTop -= 2;
    stText.usLeft -= 2;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.pcString = "BREAK";

    stText.uColor = COLOR_BLACK;
    stText.usTop += 20;
    stText.usLeft += 2;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.uColor = COLOR_YELLOW;
    stText.usTop -= 2;
    stText.usLeft -= 2;

    pclDisplay->TextFX(&stText, &stTextFX);

    stText.usTop = 26;
    stText.usLeft = 10;
    stText.pcString = "SUPER!";
    stText.uColor = COLOR_GREY25;
    pclDisplay->Text(&stText);

    stText.uColor = COLOR_RED;
    stText.usTop --;
    stText.usLeft --;
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
    stText.usLeft = 32;
    stText.usTop = 110;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    if ((m_usCounter % 120) == 0)
    {
        stText.uColor = COLOR_WHITE;
        pclDisplay->Text(&stText);
    }
    else if ((m_usCounter % 60) == 0)
    {
        stText.uColor = COLOR_BLACK;
        pclDisplay->Text(&stText);
    }

    m_usCounter--;

    if (!m_usCounter)
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

    static K_UCHAR ucAttract = 0;
    K_SHORT sVelX, sVelY;

    m_usCounter = 3600; // One minute @ 60fps

    // Clear the screen
    pclDisplay->ClearScreen();

    // Have a few different variants for fun.
    switch (ucAttract)
    {
        case 0:
            sVelX = 141;
            sVelY = 141;
            break;
        case 1:
            sVelX = 154;
            sVelY = 117;
            break;
        case 2:
            sVelX = 167;
            sVelY = 100;
            break;
        case 3:
            sVelX = 181;
            sVelY = 85;
            break;
        default:
            break;
    }

    ucAttract = (ucAttract + 1) ;

    m_clPaddle.SetPosition(60);
    m_clBall.SetPosition(64, 130);
    m_clBall.SetVelocity(sVelX * 2, sVelY * 2);

    m_clPlayField.LoadLevel(ucAttract * 3);
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


    K_USHORT usPixX, usPixY;

    if (m_clPlayField.IsPlayFieldEmpty())
    {
        m_clLevel.NextLevel();
        m_clLevel.Draw();
        m_clPlayField.LoadLevel( m_clLevel.GetLevel() );
        m_clPlayField.Draw();
    }

    m_clBall.Update();
    m_clBall.Clear();
    K_BOOL bContinue;

    do
    {
        K_UCHAR ucFlags = 0;
        bContinue = m_clBall.MoveNextPixel(&usPixX, &usPixY);

        ucFlags |= CheckBrickHit(usPixX, usPixY);
        ucFlags |= CheckBrickHit(usPixX, usPixY + BALL_SIZE - 1);
        ucFlags |= CheckBrickHit(usPixX + BALL_SIZE - 1, usPixY);
        ucFlags |= CheckBrickHit(usPixX + BALL_SIZE - 1, usPixY + BALL_SIZE - 1);

        K_BOOL bFlipX = false;
        K_BOOL bFlipY = false;

        if (usPixX >= (PLAYFIELD_RIGHT_BORDER - BALL_SIZE))
        {
            m_clBall.ToggleDirX();
            m_clBall.SetX(PLAYFIELD_RIGHT_BORDER - BALL_SIZE-1);
            bFlipX = true;
        }
        else if (usPixX <= PLAYFIELD_BORDER_WIDTH)
        {
            m_clBall.ToggleDirX();
            m_clBall.SetX(PLAYFIELD_BORDER_WIDTH + 1);
            bFlipX = true;
        }

        if (usPixY >= (PADDLE_YPOS - BALL_SIZE))
        {
            m_clBall.ToggleDirY();
            m_clBall.SetY(PADDLE_YPOS - BALL_SIZE - 1);
            bFlipY = true;
        }
        else if (usPixY <= (PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT))
        {
            m_clBall.ToggleDirY();
            m_clBall.SetY(PLAYFIELD_BORDER_HEIGHT + HUD_HEIGHT + 1);
            bFlipY = true;
        }

        if (!bFlipX)
        {
            if ((ucFlags & BRICK_HIT_LEFT) && (m_clBall.GetVelX() > 0))
            {
                m_clBall.ToggleDirX();
            }
            if ((ucFlags & BRICK_HIT_RIGHT) && (m_clBall.GetVelX() < 0))
            {
                m_clBall.ToggleDirX();
            }
        }
        if (!bFlipY)
        {
            if ((ucFlags & BRICK_HIT_TOP) && (m_clBall.GetVelY() > 0))
            {
                m_clBall.ToggleDirY();
            }
            if ((ucFlags & BRICK_HIT_BOTTOM) && (m_clBall.GetVelY() < 0))
            {
                m_clBall.ToggleDirY();
            }
        }

    } while (bContinue);
    if (usPixX < m_clPaddle.GetLeft())
    {
        m_clPaddle.SetVelocity((K_SHORT)usPixX - (K_SHORT)m_clPaddle.GetLeft());
    }
    if (usPixX > m_clPaddle.GetRight())
    {
        m_clPaddle.SetVelocity((K_SHORT)usPixX - (K_SHORT)m_clPaddle.GetRight());
    }

    m_clPaddle.Update();
    m_clBall.Draw();
    m_clPaddle.Draw();
    m_clScore.Draw();
    m_clLives.Draw();
    m_clLevel.Draw();

    DrawText_t stText;

    stText.usLeft = 40;
    stText.usTop = 60;
    stText.uColor = COLOR_RED;

    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.pcString = "GAME OVER";
    pclDisplay->Text(&stText);

    m_usCounter--;
    if (0 == m_usCounter)
    {
        static K_BOOL bNextIsScores = false;
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

    if (!m_usCounter)
    {
        m_usCounter = 1800; //3600; // 60 seconds to enter high score;
        m_ucInitialId = 0;

        m_acInitials[0] = 'A';
        m_acInitials[1] = 'A';
        m_acInitials[2] = 'A';
        m_acInitials[3] = '\0';

        pclDisplay->ClearScreen();

        stText.pcString = "TIME";
        stText.usLeft = (SCREEN_WIDTH/2) - 20;
        stText.usTop = 8;
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
            if (m_acInitials[m_ucInitialId] > 'A')
            {
                m_acInitials[m_ucInitialId]--;
                bRedraw = true;
            }
        }
        if (bRight)
        {
            if (m_acInitials[m_ucInitialId] < 'Z')
            {
                m_acInitials[m_ucInitialId]++;
                bRedraw = true;
            }
        }
    }

    // Check to see if we have to progress to the next letter
    if (ButtonDown())
    {
        m_ucInitialId ++;
        if(m_ucInitialId == 3)
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
        stRect.uLineColor = COLOR_BLACK;
        stRect.usTop = 60;
        stRect.usBottom = 76;
        stRect.usLeft = (SCREEN_WIDTH/2) - 18;
        stRect.usRight = stRect.usLeft + 36;
        pclDisplay->Rectangle(&stRect);

        stTextFX.ucFlags = TEXTFX_FLAG_SCALE_X | TEXTFX_FLAG_SCALE_Y;
        stTextFX.usScaleX100 = 200;
        stTextFX.usScaleY100 = 200;

        K_UCHAR i;
        for (i = 0; i < 3; i++)
        {
            K_CHAR acTmp[2];
            acTmp[0] = m_acInitials[i];
            acTmp[1] = 0;

            stText.pcString = acTmp;
            stText.usLeft = (SCREEN_WIDTH/2) - 18 + (12*i);
            stText.usTop = 60;
            if (i == m_ucInitialId)
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
        stText.usLeft = (SCREEN_WIDTH/2) - 28;
        stText.usTop = 40;
        switch (m_usCounter % 0x03)
        {
            case 0: stText.uColor = COLOR_RED; break;
            case 1: stText.uColor = COLOR_GREEN; break;
            case 2: stText.uColor = COLOR_BLUE; break;
            case 3: stText.uColor = COLOR_WHITE; break;
        }
        pclDisplay->Text(&stText);
    }

    if (((m_usCounter % 60) == 0) && !bExit)
    {
        DrawRectangle_t stRect;

        stRect.bFill = true;
        stRect.uFillColor = COLOR_BLACK;
        stRect.uLineColor = COLOR_BLACK;
        stRect.usTop = 8;
        stRect.usBottom = 15;
        stRect.usLeft = (SCREEN_WIDTH/2);
        stRect.usRight = stRect.usLeft + 15;
        pclDisplay->Rectangle(&stRect);

        K_CHAR acTimeStr[4];
        MemUtil::DecimalToString((K_USHORT)(m_usCounter/60), (K_CHAR*)acTimeStr);

        stText.pcString = acTimeStr;
        stText.usLeft = (SCREEN_WIDTH/2) + 5;
        stText.usTop = 8;
        stText.uColor = COLOR_RED;
        pclDisplay->Text(&stText);
    }
    m_usCounter--;

    if (!m_usCounter)
    {
        bExit = true;
    }

    if (bExit)
    {
        HighScore clHS;
        HighScore_t myScore;
        myScore.ulScore = m_clScore.GetScore();
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

    m_usCounter = 600;

    // Draw list of high scores
    pclDisplay->ClearScreen();

    HighScore clHS;
    clHS.CheckInit();

    stText.usLeft = 35;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;
    stText.uColor = COLOR_WHITE;

    for (K_UCHAR i = 0; i < 5; i++)
    {
        HighScore_t stScore;
        clHS.ReadScore(i, &stScore);
        stText.usTop = 64 + (i * 8);
        stText.usLeft = 35;
        stText.pcString = stScore.acName;

        pclDisplay->Text(&stText);

        stText.usLeft = 55;
        stText.pcString = "-";
        pclDisplay->Text(&stText);

        K_CHAR acStr[11];
        MemUtil::DecimalToString(stScore.ulScore, acStr);
        stText.usLeft = 65;
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
    m_usCounter--;

    stText.pcString = "HIGH SCORES";
    stText.usLeft = 35;
    stText.usTop = 40;
    stText.pstFont = &fntPrint_Char_21_6_False_False_False_;

    switch (m_usCounter & 0x03)
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

    switch (m_usCounter % 5)
    {
        case 0: stRect.uLineColor = COLOR_ORANGE; break;
        case 1: stRect.uLineColor = COLOR_GREEN; break;
        case 2: stRect.uLineColor = COLOR_BLUE; break;
        case 3: stRect.uLineColor = COLOR_RED; break;
        case 4: stRect.uLineColor = COLOR_YELLOW; break;
        default: break;
    }

    stRect.uFillColor = stRect.uLineColor;

    // Colorful borders - top
    stRect.usTop = 0;
    stRect.usBottom = 7;
    stRect.usLeft = (m_usCounter << 3) & (SCREEN_WIDTH - 1);
    stRect.usRight = stRect.usLeft + 7;
    pclDisplay->Rectangle(&stRect);

    // Bottom
    stRect.usTop = SCREEN_HEIGHT - 8;
    stRect.usBottom = SCREEN_HEIGHT - 1;
    stRect.usRight = (SCREEN_WIDTH - (m_usCounter << 3)) & (SCREEN_WIDTH - 1);
    if (stRect.usRight == 0)
    {
        stRect.usRight = SCREEN_WIDTH - 1;
    }
    else
    {
        stRect.usRight--;
    }
    stRect.usLeft = stRect.usRight - 7;
    pclDisplay->Rectangle(&stRect);

    // Left
    stRect.usLeft = 0;
    stRect.usRight = 7;
    stRect.usTop = 8 + ((m_usCounter << 3) % (SCREEN_HEIGHT - 16));
    stRect.usBottom = stRect.usTop + 7;
    pclDisplay->Rectangle(&stRect);

    // Right
    stRect.usLeft = SCREEN_WIDTH - 8;
    stRect.usRight = SCREEN_WIDTH - 1;
    stRect.usTop = (SCREEN_HEIGHT - 16) - ((m_usCounter << 3) % (SCREEN_HEIGHT -16));
    if (0 == stRect.usTop)
    {
        stRect.usTop = SCREEN_HEIGHT - 16;
    }
    stRect.usBottom = stRect.usTop + 7;
    pclDisplay->Rectangle(&stRect);
    if (!m_usCounter)
    {
        TransitionToState(GAME_STATE_ATTRACT);
    }
}
