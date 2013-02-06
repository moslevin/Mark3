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
	\file kerneltypes.h
	\brief Basic data type primatives used throughout the OS
*/

#ifndef __KERNELTYPES_H__
#define __KERNELTYPES_H__

#if defined(bool)
	#define K_BOOL			bool	
#else
	#define K_BOOL			unsigned char
#endif
	
#define K_CHAR			char
#define K_UCHAR			unsigned char
#define K_USHORT		unsigned short
#define K_SHORT			short
#define K_ULONG			unsigned long
#define K_LONG			long

#endif
