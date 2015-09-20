/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2015 Funkenstein Software Consulting, all rights reserved.
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
        \fn void SetName( const char *szName_ )
        
        Set the name of the test object
        
        \param szName_ Name of the tests associated with this object
        
    */
    void SetName( const char *szName_ ) { m_szName = szName_; }    
    
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
    
    void ExpectTrue( bool bExpression_ )
        { bExpression_ ? Pass() : Fail(); }

    void ExpectFalse( bool bExpression_ )
        { !bExpression_ ? Pass() : Fail(); }

    void ExpectEquals( bool bVal_, bool bExpression_ )
        { (bVal_ == bExpression_) ? Pass() : Fail(); }

    void ExpectEquals( uint8_t u8Val_, uint8_t u8Expression_ )
        { (u8Val_ == u8Expression_) ? Pass() : Fail(); }

    void ExpectEquals( uint16_t u16Val_, uint16_t usexpression_ )
        { (u16Val_ == usexpression_) ? Pass() : Fail(); }

    void ExpectEquals( uint32_t u32Val_, uint32_t u32Expression_ )
        { (u32Val_ == u32Expression_) ? Pass() : Fail(); }

    void ExpectEquals( char cVal_, char cExpression_ )
        { (cVal_ == cExpression_) ? Pass() : Fail(); }

    void ExpectEquals( int16_t s16Val_, int16_t s16Expression_ )
        { (s16Val_ == s16Expression_) ? Pass() : Fail(); }

    void ExpectEquals( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ == lExpression_) ? Pass() : Fail(); }

    void ExpectEquals( void* pvVal_, void* pvExpression_ )
        { (pvVal_ == pvExpression_) ? Pass() : Fail(); }


    void ExpectFailTrue( bool bExpression_ )
        { bExpression_ ? Fail() : Pass(); }

    void ExpectFailFalse( bool bExpression_ )
        { !bExpression_ ? Fail() : Pass(); }

    void ExpectFailEquals( bool bVal_, bool bExpression_ )
        { (bVal_ == bExpression_) ? Fail() : Pass(); }

    void ExpectFailEquals( uint8_t u8Val_, uint8_t u8Expression_ )
        { (u8Val_ == u8Expression_) ? Fail() : Pass(); }

    void ExpectFailEquals( uint16_t u16Val_, uint16_t usexpression_ )
        { (u16Val_ == usexpression_) ? Fail() : Pass(); }

    void ExpectFailEquals( uint32_t u32Val_, uint32_t u32Expression_ )
        { (u32Val_ == u32Expression_) ? Fail() : Pass(); }

    void ExpectFailEquals( char cVal_, char cExpression_ )
        { (cVal_ == cExpression_) ? Fail() : Pass(); }

    void ExpectFailEquals( int16_t s16Val_, int16_t s16Expression_ )
        { (s16Val_ == s16Expression_) ? Fail() : Pass(); }

    void ExpectFailEquals( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ == lExpression_) ? Fail() : Pass(); }

    void ExpectFailEquals( void* pvVal_, void* pvExpression_ )
        { (pvVal_ == pvExpression_) ? Fail() : Pass(); }

    void ExpectGreaterThan( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ > lExpression_) ? Pass() : Fail(); }

    void ExpectLessThan( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ < lExpression_) ? Pass() : Fail(); }

    void ExpectGreaterThanEquals( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ >= lExpression_) ? Pass() : Fail(); }

    void ExpectLessThanEquals( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ <= lExpression_) ? Pass() : Fail(); }

    void ExpectFailGreaterThan( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ > lExpression_) ? Fail() : Pass(); }

    void ExpectFailLessThan( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ < lExpression_) ? Fail() : Pass(); }

    void ExpectFailGreaterThanEquals( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ >= lExpression_) ? Fail() : Pass(); }

    void ExpectFailLessThanEquals( int32_t lVal_, int32_t lExpression_ )
        { (lVal_ <= lExpression_) ? Fail() : Pass(); }

    /*!
        \fn void Complete()
        
        Complete the test.  Once a test has been completed, no new iterations
        can be started (i.e Start()/Pass()/Fail() will have no effect).
    */
    void Complete() { m_bComplete = 1; }
    
    /*!
        \fn const char *GetName()
        
        Get the name of the tests associated with this object
        
        \return Name of the test
    */
    const char *GetName(){ return m_szName; }
    
    /*!
        \fn bool GetResult()
        
        Return the result of the last test 
        
        \return Status of the last run test (false = fail, true = pass)
    */
    bool GetResult() { return m_bStatus; }

    /*!
        \fn uint16_t GetPassed()
        
        Return the total number of test points/iterations passed
        
        \return Count of all successful test points/iterations
    */
    uint16_t GetPassed() { return m_u16Passed; }
    
    /*!
        \fn uint16_t GetFailed()
        
        Return the number of failed test points/iterations
        
        \return Failed test point/iteration count
    */
    uint16_t GetFailed() { return m_u16Iterations - m_u16Passed; }
    
    /*!
        \fn uint16_t GetTotal()
        
        Return the total number of iterations/test-points executed
        
        \return Total number of ierations/test-points executed
    */
    uint16_t GetTotal() { return m_u16Iterations; }
    
private:
    const char *m_szName;        //!< Name of the tests performed
    bool m_bIsActive;            //!< Whether or not the test is active
    bool m_bComplete;        //!< Whether or not the test is complete
    bool m_bStatus;            //!< Status of the last-run test
    uint16_t m_u16Iterations;    //!< Number of iterations executed
    uint16_t m_u16Passed;        //!< Number of iterations that have passed
};

#endif
