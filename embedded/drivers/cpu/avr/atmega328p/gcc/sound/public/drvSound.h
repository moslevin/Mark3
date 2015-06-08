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

    \file   drvSound.h

    \brief  ATMega328p PWM Sound Driver
	
	This module contains an implementation of a simple sound driver, capable of
	generating accurate monophonic sounds via a Timer in PWM mode.
	
	In order to accurately generate waveforms, we rely on the timer interrupt
	to toggle the Timer's PWM output - and in order to keep the interrupts 
	from firing excessively fast (and chewing up tons of CPU in the process),
	simple square wave output is all we can afford to provide.
	
	Based on this functionality, a few basic sound functions are provided:
	
	Generate Tone: 
		-Output a fixed frequency tone for a period of time
		
	Generate Sweep:
		-Output a nifty frequency up/down effect, with variable speed, range and duration
		
	Play Song:
		-Output a series of notes in sequence, repeating once the sequence is complete.
		 Songs can be played at an arbitrary speed (in BPM), without having to re-encode
		 the note data.
		 
	Tone timing is controlled using a Mark3 Kernel Timer.
*/

#ifndef __drvSound_H__
#define __drvSound_H__

//---------------------------------------------------------------------------
#include "mark3cfg.h"
#include "kerneltypes.h"
#include "driver.h"
#include "timerlist.h"

//---------------------------------------------------------------------------
// Basic math used for tone generation
#define BASE_FREQ			(16000000)		// CPU Frequency in Hz
#define PWM_PRESCALER		(1)				// Timer prescaler used to divide-down CPU Clock to a peripheral clock
#define PWM_DURATION		(256)			// Duration of each PWM output
#define ISR_FREQUENCY		((BASE_FREQ/PWM_PRESCALER)/PWM_DURATION)	// Resulting PWM Interrupt frequency
#define NYQUIST_FREQUENCY	((ISR_FREQUENCY+1)/2)						// Maximum sampling frequency for this interface

//---------------------------------------------------------------------------
// Fundamental frequencies for each note at octave "1"
#define TONE_C_				(32.7032)
#define TONE_CS_			(34.6478)
#define TONE_DF_			TONE_CS_
#define TONE_D_				(36.7081)
#define TONE_DS_			(38.8909)
#define TONE_EF_			TONE_DS_
#define TONE_E_				(41.2034)
#define TONE_F_				(43.6535)
#define TONE_FS_			(46.2493)
#define TONE_GF_			TONE_FS_
#define TONE_G_				(48d.9994)
#define TONE_GS_			(51.9131)
#define TONE_AF_			TONE_GS_
#define TONE_A_				(55.0000)
#define TONE_AS_			(58.2705)
#define TONE_BF_			TONE_AS_
#define TONE_B_				(61.7354)

//---------------------------------------------------------------------------
// Frequency multipliers used to represent tones in each octave
#define OCT_1				(1)
#define OCT_2				(2)
#define OCT_3				(4)
#define OCT_4				(8)
#define OCT_5				(16)
#define OCT_6				(32)
#define OCT_7				(64)
#define OCT_8				(128)

//---------------------------------------------------------------------------
// This macro expands a Note + Octave value into an integer frequency representing
// that note with our hardware.  This allows us to create accurate, easy-to-use,
// compile-time constants for notes...  i.e., you can write TONE(A,4), which 
// evaluates at build-time to a numeric constant representing a 440Hz output,
// adjusted for the hardware timer's configuration.
#define TONE(x,y)			(uint16_t)(0.5 + ( ((double)ISR_FREQUENCY) \
						  / (((double)((TONE_ ## x ##_) * (OCT_ ## y)))) ))

//---------------------------------------------------------------------------
// Event codes used by this object
#define SOUND_EVENT_OFF                     (0x0001)
#define SOUND_EVENT_SQUAREWAVE              (0x0002)
#define SOUND_EVENT_SONG					(0x0003)
#define SOUND_EVENT_SWEEP					(0x0004)

//---------------------------------------------------------------------------
// Beat-based note timing defines
#define NOTE_THIRTYSECOND			(1)
#define NOTE_SIXTEENTH				(2)
#define NOTE_DOTTED_EIGHTH			(3)
#define NOTE_EIGHTH					(4)
#define NOTE_QUARTER				(8)
#define NOTE_HALF					(16)
#define NOTE_DOTTED_HALF			(24)
#define NOTE_WHOLE					(32)

//---------------------------------------------------------------------------
typedef struct
{
	K_USHORT usFreq;		//!< Frequency of the square wave (in Hz)
	K_USHORT usDurationMS;	//!< Duration of the tone (in ms)
	K_UCHAR  ucLevel;		//!< Output level (0 == off, 255 == max)
} SquareWave_t;

//---------------------------------------------------------------------------
// 
typedef struct
{
	K_USHORT usFreq;			//!< Frequency of the note (in Hz)
	K_USHORT usDurationBPM32;	//!< Duration of the note (in beats)
	K_UCHAR	 ucLevel;			//!< Output level (0 == off, 255 == max)
} SongNote_t;

//---------------------------------------------------------------------------
typedef struct  
{
	K_UCHAR ucTempoBPM;			//!< Tempo of the song in BPM
	SongNote_t *astNotes;		//!< Pointer to an array of notes making up the song
} Song_t;

//---------------------------------------------------------------------------
typedef struct  
{
	K_USHORT usFreqStart;		//!< Starting frequency
	K_USHORT usSpeed;			//!< Time (in ms) between steps in the sweep
	K_USHORT usDuration;		//!< Duration of the sweep (in ms)
	K_BOOL  bDir;				//!< Direction of the sweep (false == low, true == high)
	K_UCHAR ucLevel;			//!< Output level (0 == off, 255 == max)
} Sweep_t;

//---------------------------------------------------------------------------
class SoundDriver : public Driver
{
public:
	virtual void Init();

	virtual K_UCHAR Open();
	virtual K_UCHAR Close();

	virtual K_USHORT Read( K_USHORT usBytes_, K_UCHAR *pucData_ )
	{ return 0; }
	virtual K_USHORT Write( K_USHORT usBytes_, K_UCHAR *pucData_ )
	{ return 0; }

	virtual K_USHORT Control( K_USHORT usEvent_, void *pvDataIn_, K_USHORT usSizeIn_, void *pvDataOut_, K_USHORT usSizeOut_ );
	
private:
	void SetTone( SquareWave_t *pstWave_ );
	void StartSong( Song_t *pstSong_ );
	void StartSweep( Sweep_t *pstSweep_ );	
};

#endif
