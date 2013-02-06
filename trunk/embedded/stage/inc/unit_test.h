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
	\file unit_test.h
	\brief Unit test class declarations
*/
#ifndef __UNIT_TEST_H__
#define __UNIT_TEST_H__


#include "kerneltypes.h"

//---------------------------------------------------------------------------
/*!
	Class used to implement a simple unit-testing framework
*/
class UnitTest
{
public:
	UnitTest();
	
	/*!
		\fn void SetName( const K_CHAR *szName_ )
		
		Set the name of the test object
		
		\param szName_ Name of the tests associated with this object
		
	*/
	void SetName( const K_CHAR *szName_ ) { m_szName = szName_; }	
	
	/*!
		\fn void Start() 
		
		Start a new test iteration.
	*/
	void Start() { m_bIsActive = 1; }
	
	/*!
		\fn void Pass()
		
		Stop the current iteration (if started), and register that the test
		was successful.		
	*/
	void Pass();
	
	/*!
		\fn void Fail();
		
		Stop the current iterations (if started), and register that the
		current test failed.
	*/
	void Fail();
	
	/*!
		\fn void Complete()
		
		Complete the test.  Once a test has been completed, no new iterations
		can be started (i.e Start()/Pass()/Fail() will have no effect).
	*/
	void Complete() { m_bComplete = 1; }
	
	/*!
		\fn const K_CHAR *GetName()
		
		Get the name of the tests associated with this object
		
		\return Name of the test
	*/
	const K_CHAR *GetName(){ return m_szName; }
	
	/*!
		\fn K_BOOL GetResult()
		
		Return the result of the last test 
		
		\return Status of the last run test (false = fail, true = pass)
	*/
	K_BOOL GetResult() { return m_bStatus; }

	/*!
		\fn K_USHORT GetPassed()
		
		Return the total number of test points/iterations passed
		
		\return Count of all successful test points/iterations
	*/
	K_USHORT GetPassed() { return m_usPassed; }
	
	/*!
		\fn K_USHORT GetFailed()
		
		Return the number of failed test points/iterations
		
		\return Failed test point/iteration count
	*/
	K_USHORT GetFailed() { return m_usIterations - m_usPassed; }
	
	/*!
		\fn K_USHORT GetTotal()
		
		Return the total number of iterations/test-points executed
		
		\return Total number of ierations/test-points executed
	*/
	K_USHORT GetTotal() { return m_usIterations; }
	
private:
	const K_CHAR *m_szName;			//!< Name of the tests performed
	K_BOOL m_bIsActive;				//!< Whether or not the test is active
	K_UCHAR m_bComplete;		//!< Whether or not the test is complete
	K_BOOL m_bStatus;					//!< Status of the last-run test
	K_USHORT m_usIterations;	//!< Number of iterations executed
	K_USHORT m_usPassed;		//!< Number of iterations that have passed
};

#endif
