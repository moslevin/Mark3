/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    @file statemachine.cpp
    @brief Implements a generic and extensible state-machine framework
*/
#include "state_machine.h"
namespace Mark3 {
//---------------------------------------------------------------------------
void StateMachine::SetStates(const State_t* pstStates_, uint16_t u16StateCount_)
{
    m_u16StateCount = u16StateCount_;
    m_pstStateList = pstStates_;
}

//---------------------------------------------------------------------------
void StateMachine::SetContext(void* pvContext_) 
{ 
    m_pvContext = pvContext_; 
}

//---------------------------------------------------------------------------
void* StateMachine::GetContex() 
{ 
    return m_pvContext; 
}

//---------------------------------------------------------------------------
void StateMachine::Begin()
{
    m_u16StackDepth = 1;
    m_bOpcodeSet = false;
    m_au16StateStack[0] = 0;
    
    if (m_pstStateList[0].pfEntry) {
        m_pstStateList[0].pfEntry(this);
    }
}

//---------------------------------------------------------------------------
void StateMachine::PushState(uint16_t u16StateIdx_)
{
    SetOpcode(STATE_OPCODE_PUSH_STATE);
    m_u16NextState = u16StateIdx_;
}

//---------------------------------------------------------------------------
void StateMachine::PopState()
{
    SetOpcode(STATE_OPCODE_POP_STATE);
}

//---------------------------------------------------------------------------
void StateMachine::TransitionState(uint16_t u16StateIdx_)
{
    SetOpcode(STATE_OPCODE_TRANSITION);
    m_u16NextState = u16StateIdx_;
}

//---------------------------------------------------------------------------
bool StateMachine::SetOpcode(StateOpcode_t eOpcode_)
{
    if (m_bOpcodeSet) {
        return false;
    }
    m_bOpcodeSet = true;
    m_eOpcode = eOpcode_;
    return true;
}

//---------------------------------------------------------------------------
bool StateMachine::GetOpcode(StateOpcode_t* peOpcode_)
{
    if (m_bOpcodeSet) {
        *peOpcode_ = m_eOpcode;
        m_bOpcodeSet = false;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void StateMachine::HandleEvent(const void* pvEvent_)
{   
    auto u16StackPtr = m_u16StackDepth;
    auto bDone = false;
    SetOpcode(STATE_OPCODE_RUN_STATE);

    while (!bDone) {
        uint16_t u16State = m_au16StateStack[u16StackPtr - 1];
        
        // Handle the individual statemachine opcodes.  
        StateOpcode_t eOpcode;
        GetOpcode(&eOpcode);
        switch (eOpcode) {
            case STATE_OPCODE_RETURN: {
                // Done processing the event
                bDone = true;
            }
                break;
            case STATE_OPCODE_RUN_STATE: {
                // Must have a run handler...                
                auto eResult = m_pstStateList[u16State].pfRun(this, pvEvent_);
                if (eResult == STATE_RETURN_UNHANDLED) {
                    if (u16StackPtr > 1) {
                        u16StackPtr--;
                        SetOpcode(STATE_OPCODE_RUN_STATE);
                    } else {
                        SetOpcode(STATE_OPCODE_RETURN);
                    }
                } else if (eResult == STATE_RETURN_OK) {
                    SetOpcode(STATE_OPCODE_RETURN);
                } 
                // Implicit - if eResult == STATE_RETURN_TRANSITION, then
                // the m_eOpcode value will have been set before the handler returned, and the  
                // m_u16NextState variable also set.
            }
                break;
            case STATE_OPCODE_PUSH_STATE: {
                while (m_u16StackDepth != u16StackPtr) {
                    uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                    m_u16StackDepth--;
                    if (m_pstStateList[u16TempState].pfExit) {
                        m_pstStateList[u16TempState].pfExit(this);
                    }                    
                }
                
                if (m_pstStateList[m_u16NextState].pfEntry) {
                    m_pstStateList[m_u16NextState].pfEntry(this);
                }
                m_au16StateStack[m_u16StackDepth] = m_u16NextState;
                m_u16StackDepth++;
            }
                break;
            case STATE_OPCODE_POP_STATE: {
                while (m_u16StackDepth != u16StackPtr) {
                    uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                    m_u16StackDepth--;
                    if (m_pstStateList[u16TempState].pfExit) {
                        m_pstStateList[u16TempState].pfExit(this);
                    }                    
                }
                
                uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                m_u16StackDepth--;
                if (m_pstStateList[u16TempState].pfExit) {
                    m_pstStateList[u16TempState].pfExit(this);
                }                    
            }
                break;
            case STATE_OPCODE_TRANSITION: {
                while (m_u16StackDepth != u16StackPtr) {
                    uint16_t u16TempState = m_au16StateStack[m_u16StackDepth - 1];
                    m_u16StackDepth--;
                    if (m_pstStateList[u16TempState].pfExit) {
                        m_pstStateList[u16TempState].pfExit(this);
                    }                    
                }
                
                if (m_pstStateList[u16State].pfExit) {
                    m_pstStateList[u16State].pfExit(this);
                }                    
                if (m_pstStateList[m_u16NextState].pfEntry) {
                    m_pstStateList[m_u16NextState].pfEntry(this);
                }
                m_au16StateStack[m_u16StackDepth] = m_u16NextState;
                
            }
                break;
            default:
                break;
        }       
    }    
}
} //namespace Mark3
