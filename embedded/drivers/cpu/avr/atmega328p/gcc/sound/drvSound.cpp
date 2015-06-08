/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!

    \file   drvSound.cpp

    \brief  ATMega328p PWM Sound Driver
*/

#include "mark3cfg.h"
#include "kerneltypes.h"
#include "kerneltimer.h"
#include "threadport.h"
#include "drvSound.h"

#include <avr/io.h>
#include <avr/interrupt.h>

//---------------------------------------------------------------------------
static volatile K_USHORT usBeatTimer = 0;
static volatile K_USHORT usNoteIndex = 0;
static volatile K_USHORT usSweepIdx = 0;
static volatile K_USHORT usSweepSteps = 0;

static volatile K_USHORT target = 0;
static volatile K_USHORT count = 0;
static volatile K_UCHAR	level = 0;

static Timer clTimer;

//---------------------------------------------------------------------------
ISR(TIMER2_COMPB_vect)
{
	if( ++count >= target )
	{		
		count = 0;
		if( !OCR2B ) {
			OCR2B = level;
			} else {
			OCR2B = 0;
		}
	}
}

//---------------------------------------------------------------------------
static void ToneCallback( Thread *pclOwner_, void *pvData_ )
{
	OCR2B = 0;
	TIMSK2 &= ~(1 << OCIE2B);
	clTimer.Stop();
}

//---------------------------------------------------------------------------
void SoundDriver::SetTone( SquareWave_t *pstWave_ )
{
	clTimer.Stop();
	Open();
	
	CS_ENTER();	
	target = pstWave_->usFreq / 2;
	level = pstWave_->ucLevel;
	OCR2B = level;
	CS_EXIT();
	
	clTimer.Start( false, pstWave_->usDurationMS, ToneCallback, 0 );	
}

//---------------------------------------------------------------------------
void SongCallback( Thread *pclOwner_, void *pvData_ )
{
	Song_t *pstSong = (Song_t*)pvData_;
	usBeatTimer++;
	if( usBeatTimer >= pstSong->astNotes[usNoteIndex].usDurationBPM32 )
	{
		usBeatTimer = 0;
		usNoteIndex++;
		
		if (0 == pstSong->astNotes[usNoteIndex].usFreq)	
		{
			usNoteIndex = 0;
		}
		
		CS_ENTER();	
		target = pstSong->astNotes[usNoteIndex].usFreq / 2;
		level = pstSong->astNotes[usNoteIndex].ucLevel;
		OCR2B = level;
		CS_EXIT();
	}
}

//---------------------------------------------------------------------------
void SweepCallback( Thread *pstOwner_, void *pvData_ )
{
	K_USHORT usFreq;
	Sweep_t *pstSweep = (Sweep_t*)pvData_;

	if (usSweepIdx < usSweepSteps)
	{
		usSweepIdx++;	

		if (pstSweep->bDir)
		{
			usFreq = pstSweep->usFreqStart + usSweepIdx;	
		}
		else
		{
			usFreq = pstSweep->usFreqStart - usSweepIdx;	
		}

		CS_ENTER();		
		target = usFreq / 2;
		level = pstSweep->ucLevel;		
		CS_EXIT();
		
		if (usSweepIdx == usSweepSteps)
		{
			OCR2B = 0;
			TIMSK2 &= ~(1 << OCIE2B);
			clTimer.Stop();
		}		
	}	
}
//---------------------------------------------------------------------------
void SoundDriver::StartSong( Song_t *pstSong_ )
{
	K_ULONG ulBeatTimeMS32 = ((60000)/(pstSong_->ucTempoBPM * 8));
	
	Open();
	
	clTimer.Stop();
	
	CS_ENTER();
	count = 0;
	target = pstSong_->astNotes[0].usFreq / 2;
	level = pstSong_->astNotes[0].ucLevel;
	OCR2B = level;
	CS_EXIT();
    
	clTimer.Start( true, ulBeatTimeMS32, SongCallback, pstSong_ );	
	usBeatTimer = 0;
	usNoteIndex = 0;
}

//---------------------------------------------------------------------------
void SoundDriver::StartSweep( Sweep_t *pstSweep_ )
{
	clTimer.Stop();
	
	Open();
	
	usSweepIdx = 0;
	usSweepSteps = (pstSweep_->usDuration) / pstSweep_->usSpeed;
	
	CS_ENTER();	
	target = pstSweep_->usFreqStart / 2;
	level = pstSweep_->ucLevel;
	OCR2B = level;
	CS_EXIT();
	
	clTimer.Start( true, pstSweep_->usSpeed, SweepCallback, pstSweep_ );
}

//---------------------------------------------------------------------------
void SoundDriver::Init()
{
	clTimer.Init();
	
	TCCR2A = 0;
	TCCR2A |= (1 << COM2B1) // Non inverted mode
	| (1 << WGM21)  // Fast PWM
	| (1 << WGM20)  // Fast PWM (contd)
	;
	TCCR2B = 0;
	TCCR2B |= (1 << CS20)	// Div-by-1
	;
	
	DDRD |= (1 << 3);		// Port D3 as output
	PORTD = 0;
	OCR2B = 0;
}

//---------------------------------------------------------------------------
K_UCHAR SoundDriver::Open()
{		
	OCR2B = 0;
	TIMSK2 |= (1 << OCIE2B);
	return 0;
}

//---------------------------------------------------------------------------
K_UCHAR SoundDriver::Close()
{
	TIMSK2 &= ~(1 << OCIE2B);
	OCR2B = 0;
	return 0;
}

//---------------------------------------------------------------------------
K_USHORT SoundDriver::Control( K_USHORT usEvent_, void *pvDataIn_, K_USHORT usSizeIn_, void *pvDataOut_, K_USHORT usSizeOut_ )
{
	switch (usEvent_)
	{
		case SOUND_EVENT_OFF:
			clTimer.Stop();			
			Close();
			break;
		case SOUND_EVENT_SQUAREWAVE:
		{
			SquareWave_t *pstSW = (SquareWave_t*)pvDataIn_;
			SetTone(pstSW);
			break;
		}
		case SOUND_EVENT_SONG:
		{
			Song_t *pstSong = (Song_t*)pvDataIn_;
			StartSong(pstSong);
			break;
		}
		case SOUND_EVENT_SWEEP:
		{
			Sweep_t *pstSweep = (Sweep_t*)pvDataIn_;
			StartSweep(pstSweep);
			break;
		}
		default:
			break;
	}
	return 0;
}
