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
=========================================================================== */
/*!
    \file dcpu.h
    \brief DCPU-16 emulator
 */
#ifndef __DCPU_H__
#define __DCPU_H__

#include "kerneltypes.h"
#include "ll.h"

//---------------------------------------------------------------------------
/*!
    DCPU v1.7 CPU emulator.
    
    Basic opcode format:
        [aaaaaabbbbbooooo]
    
    Where: - aaaaaa 6-bit source argument
           - bbbbb 5-bit destination argument
           - o is the opcode itself in a
           
    If oooo = 0, then it's an "extended" opcode
    
    Extended opcode format:
        [aaaaaaoooooxxxxx]
        
    Where:
        - xxxxx  = all 0's - (basic opcode)
        - ooooo  = an extended opcode
        - aaaaaa = the argument
 */

//---------------------------------------------------------------------------
// Macros to access individual elements from within an opcode
#define DCPU_NORMAL_OPCODE_MASK( x ) \
        ((uint16_t)(x & 0x001F))

#define DCPU_EXTENDED_OPCODE_MASK( x ) \
        ((uint16_t)((x >> 5) & 0x001F))

#define DCPU_A_MASK( x ) \
        ((uint16_t)((x >> 10) & 0x003F))
        
#define DCPU_B_MASK( x ) \
        ((uint16_t)((x >> 5) & 0x001F))

//---------------------------------------------------------------------------
// Macros to emit opcodes in the normal/extended formats
#define DCPU_BUILD_NORMAL( x, y, z ) \
        ( ((uint16_t)(x) & 0x001F) | ((uint16_t)(y) & 0x001F) << 5 | ((uint16_t)(z) & 0x003F) << 10 )

#define DCPU_BUILD_EXTENDED( x, y ) \
        ( ((uint16_t)(x & 0x001F) << 5) | ((uint16_t)(y & 0x003F) << 10) )

//---------------------------------------------------------------------------
/*!
 *  Structure defining the DCPU hardware registers
 */
typedef struct
{
    union
    {
        struct
        {
            uint16_t A;
            uint16_t B;
            uint16_t C;
            uint16_t X;
            uint16_t Y;
            uint16_t Z;
            uint16_t I;
            uint16_t J;
            uint16_t PC;
            uint16_t SP;
            uint16_t EX;
            uint16_t IA;
        };
        uint16_t au16Registers[12];
    };
} DCPU_Registers;

//---------------------------------------------------------------------------
/*!
 *  DCPU Basic Opcodes
 */
typedef enum
{
    OP_NON_BASIC = 0,    //!< special instruction - see below
    OP_SET, //!< b, a | sets b to a
    OP_ADD, //!< b, a | sets b to b+a, sets EX to 0x0001 if there's an overflow, 0x0 otherwise
    OP_SUB, //!< b, a | sets b to b-a, sets EX to 0xffff if there's an underflow, 0x0 otherwise
    OP_MUL, //!< b, a | sets b to b*a, sets EX to ((b*a)>>16)&0xffff (treats b, a as unsigned)
    OP_MLI, //!< b, a | like MUL, but treat b, a as signed
    OP_DIV, //!< b, a | sets b to b/a, sets EX to ((b<<16)/a)&0xffff. if a==0, sets b and EX to 0 instead. (treats b, a as unsigned)
    OP_DVI, //!< b, a | like DIV, but treat b, a as signed. Rounds towards 0
    OP_MOD, //!< b, a | sets b to b%a. if a==0, sets b to 0 instead.
    OP_MDI, //!< b, a | like MOD, but treat b, a as signed. (MDI -7, 16 == -7)
    OP_AND, //!< b, a | sets b to b&a
    OP_BOR, //!< b, a | sets b to b|a
    OP_XOR, //!< b, a | sets b to b^a
    OP_SHR, //!< b, a | sets b to b>>>a, sets EX to ((b<<16)>>a)&0xffff (logical shift)
    OP_ASR, //!< b, a | sets b to b>>a, sets EX to ((b<<16)>>>a)&0xffff (arithmetic shift) (treats b as signed)
    OP_SHL, //!< b, a | sets b to b<<a, sets EX to ((b<<a)>>16)&0xffff
    OP_IFB, //!< b, a | performs next instruction only if (b&a)!=0
    OP_IFC, //!< b, a | performs next instruction only if (b&a)==0
    OP_IFE, //!< b, a | performs next instruction only if b==a 
    OP_IFN, //!< b, a | performs next instruction only if b!=a 
    OP_IFG, //!< b, a | performs next instruction only if b>a 
    OP_IFA, //!< b, a | performs next instruction only if b>a (signed)
    OP_IFL, //!< b, a | performs next instruction only if b<a 
    OP_IFU, //!< b, a | performs next instruction only if b<a (signed)
    OP_18,  //!< **UNDEFINED**
    OP_19,  //!< **UNDEFINED**
    OP_ADX, //!< b, a | sets b to b+a+EX, sets EX to 0x0001 if there is an over-flow, 0x0 otherwise
    OP_SBX, //!< b, a | sets b to b-a+EX, sets EX to 0xFFFF if there is an under-flow, 0x0 otherwise
    OP_1C,  //!< **UNDEFINED**
    OP_1D,  //!< **UNDEFINED**
    OP_STI, //!< b, a | sets b to a, then increases I and J by 1
    OP_STD  //!< b, a | sets b to a, then decreases I and J by 1
} DCPU_OpBasic;

//---------------------------------------------------------------------------
/*!
 *  DCPU Extended opcodes.
 */
typedef enum
{
    OP_EX_RESERVED = 0,
    OP_EX_JSR,      //!< a - pushes the address of the next instruction to the stack, then sets PC to a
    OP_EX_2,        //!< **UNDEFINED**
    OP_EX_3,        //!< **UNDEFINED**
    OP_EX_4,        //!< **UNDEFINED**
    OP_EX_5,        //!< **UNDEFINED**
    OP_EX_6,        //!< **UNDEFINED**
    OP_EX_7,        //!< **UNDEFINED**
    OP_EX_INT,      //!< Invoke software interrupt "a"
    OP_EX_IAG,      //!< Get interrupt address in "a"
    OP_EX_IAS,      //!< Set interrupt address from "a"
    OP_EX_RFI,      //!< Disables interrupt queueing, pops A from the stack, then pops PC from the stack
    OP_EX_IAQ,      //!< if a is nonzero, interrupts will be added to the queue instead of triggered. if a is zero, interrupts will be triggered as normal again
    OP_EX_D,        //!< **UNDEFINED**
    OP_EX_E,        //!< **UNDEFINED**
    OP_EX_F,        //!< **UNDEFINED**
    OP_EX_HWN,      //!< Sets "a" to number of connected HW devices
    OP_EX_HWQ,      //!< Set registers with information about hardware at index "a"
    OP_EX_HWI,      //!< Send an interrupt to hardware interface "a"
    OP_EX_13,       //!< **UNDEFINED**
    OP_EX_14,       //!< **UNDEFINED**
    OP_EX_15,       //!< **UNDEFINED**
    OP_EX_16,       //!< **UNDEFINED**
    OP_EX_17,       //!< **UNDEFINED**
    OP_EX_18,       //!< **UNDEFINED**
    OP_EX_19,       //!< **UNDEFINED**
    OP_EX_1A,       //!< **UNDEFINED**
    OP_EX_1B,       //!< **UNDEFINED**
    OP_EX_1C,       //!< **UNDEFINED**
    OP_EX_1D,       //!< **UNDEFINED**
    OP_EX_1E,       //!< **UNDEFINED**
    OP_EX_1F        //!< **UNDEFINED**
} DCPU_OpExtended;

//---------------------------------------------------------------------------
/*!
    Argument formats
 */

typedef enum
{
    ARG_A = 0,
    ARG_B,
    ARG_C,
    ARG_X,
    ARG_Y,
    ARG_Z,
    ARG_I,
    ARG_J,
    
    ARG_BRACKET_A,
    ARG_BRACKET_B,
    ARG_BRACKET_C,
    ARG_BRACKET_X,
    ARG_BRACKET_Y,
    ARG_BRACKET_Z,
    ARG_BRACKET_I,
    ARG_BRACKET_J,
    
    ARG_WORD_A,
    ARG_WORD_B,
    ARG_WORD_C,
    ARG_WORD_X,
    ARG_WORD_Y,
    ARG_WORD_Z,
    ARG_WORD_I,
    ARG_WORD_J,
    
    ARG_PUSH_POP_SP,
    ARG_PEEK_SP,
    ARG_WORD_SP,
    ARG_SP,
    ARG_PC,
    ARG_EX,
    ARG_NEXT_WORD,
    ARG_NEXT_LITERAL,
    
    ARG_LITERAL_0,
    ARG_LITERAL_1,
    ARG_LITERAL_2,
    ARG_LITERAL_3,
    ARG_LITERAL_4,
    ARG_LITERAL_5,
    ARG_LITERAL_6,
    ARG_LITERAL_7,
    ARG_LITERAL_8,
    ARG_LITERAL_9,
    ARG_LITERAL_A,
    ARG_LITERAL_B,
    ARG_LITERAL_C,
    ARG_LITERAL_D,
    ARG_LITERAL_E,
    ARG_LITERAL_F,
    ARG_LITERAL_10,
    ARG_LITERAL_11,
    ARG_LITERAL_12,
    ARG_LITERAL_13,
    ARG_LITERAL_14,
    ARG_LITERAL_15,
    ARG_LITERAL_16,
    ARG_LITERAL_17,
    ARG_LITERAL_18,
    ARG_LITERAL_19,
    ARG_LITERAL_1A,
    ARG_LITERAL_1B,
    ARG_LITERAL_1C,
    ARG_LITERAL_1D,
    ARG_LITERAL_1E,
    ARG_LITERAL_1F
    
} DCPU_Argument;

//---------------------------------------------------------------------------
class DCPU;    // Forward declaration - required by the plugin class

//---------------------------------------------------------------------------
/*!
 *  Callback function type used to implement HWI for VM->Host communications
 */
typedef void (*DCPU_Callback)(DCPU *pclVM_);

//---------------------------------------------------------------------------
/*!
 *  Class used to provide the hardware device abstraction between the DCPU-16
 *  emulator/VM and the host system.
 */
class DCPUPlugin : public LinkListNode
{
public:    
    /*!
     *  \brief Init
     *
     *  Initialize the DCPU plugin extension.
     *  
     *  \param u16DeviceNumber_ Unique plugin device enumeration associated 
     *                          with this plugin
     *  \param u32HWID_ Unique hardware type identifier 
     *  \param u32VID_  Hardware Vendor ID
     *  \param u16Version_ Version identifier for this hardware piece
     *  \param pfCallback_ Callback function invoked from the VM when a HWI 
     *                      instruction is called on this device.  This is
     *                      essentially the interrupt handler.                
     */
    void Init(  uint16_t u16DeviceNumber_,
                uint32_t u32HWID_, 
                uint32_t u32VID_, 
                uint16_t u16Version_, 
                DCPU_Callback pfCallback_)
    {
        m_u32HWID = u32HWID_;
        m_u32VID = u32VID_;
        m_u16DeviceNumber = u16DeviceNumber_;
        m_u16Version = u16Version_;
        m_pfCallback = pfCallback_;
        ClearNode();
    }

    /*!
     *  \brief Enumerate
     *
     *  Perform hardware enumeration to the target VM specified by the 
     *  register set.
     *  
     *  \param pstRegisters_ Pointer to the VM's CPU registers, which are
     *                       filled with enumeration data.  See the DCPU
     *                       1.7 spec for details.
     */
    void Enumerate( DCPU_Registers *pstRegisters_ )
    {
        pstRegisters_->A = (uint16_t)(m_u32HWID & 0x0000FFFF);
        pstRegisters_->B = (uint16_t)((m_u32HWID >> 16) & 0x0000FFFF);
        pstRegisters_->C = m_u16Version;
        pstRegisters_->X = (uint16_t)(m_u32VID & 0x0000FFFF);
        pstRegisters_->Y = (uint16_t)((m_u32VID >> 16) & 0x0000FFFF);
    }

    /*!
     *  \brief Interrupt
     *
     *  Execute the hardware callback
     *  
     *  \param pclCPU_ Pointer to the VM triggering the interrupt
     */
    void Interrupt( DCPU *pclCPU_ )
    {
        m_pfCallback(pclCPU_);        
    }

    /*!
     *  \brief GetDeviceNumber
     *
     *  Return the device number associated with this plugin
     *  
     *  \return Device number associated with this plugin        
     */
    uint16_t GetDeviceNumber()
    {
        return m_u16DeviceNumber;        
    }
    
    friend class DCPUPluginList;
private:
    uint16_t      m_u16DeviceNumber;   //!< Location of the device on the "bus"
    uint32_t      m_u32HWID;           //!< Hardware ID
    uint32_t      m_u32VID;            //!< Vendor ID
    uint16_t      m_u16Version;        //!< Hardware Version
    
    DCPU_Callback m_pfCallback;        //!< HWI Callback
};

//---------------------------------------------------------------------------
/*!
 *  DCPU emulator, used for running code out of EEPROM, RAM, or
 *  other memory interfaces than FLASH.
 */
class DCPU
{
public:
    /*!        
     *  \brief Init
     *
     *  Initialize the CPU emulator, specifying which driver supplies the
     *  memory read interface.  This allows u16 to abstract RAM/FLASH/EEPROM
     *  or other memory.  The VM must be initialized before any other method in 
     *  the class is run.
     *  
     *  \param pu16RAM_ Pointer to the CPU's RAM buffer
     *  \param u16RAMSize_ Size of the RAM Buffer in words
     *  \param pu16ROM_ Pointer to the CPU's ROM buffer
     *  \param u16ROMSize_ Size of the ROM buffer in words
     */    
    void Init( uint16_t *pu16RAM_, uint16_t u16RAMSize_, const uint16_t *pu16ROM_, uint16_t u16ROMSize_ );
    
    /*!
     *  \brief RunOpcode
     *
     *  Execute the next opcode at the VM's current PC.
     */
    void RunOpcode();
        
    /*!
     *  \brief GetRegisters
     *
     *  Return a pointer to the VM's register structure
     *  
     *  \return Pointer to the VM's register structure
     */
    DCPU_Registers *GetRegisters() { return &m_stRegisters; } 
    
    /*!
     *  \brief SendInterrupt
     *
     *  Send an interrupt to the CPU with a given message
     *  
     *  \param u16Message_ Message to send along with the interrupt
     */
    void SendInterrupt( uint16_t u16Message_ );

    /*!
     *  \brief AddPlugin
     *
     *  Add a plugin to the CPU
     *  
     *  \param pclPlugin_ Pointer to the plugin object to add
     */
    void AddPlugin( DCPUPlugin *pclPlugin_ );
    
private:    

    // Basic opcodes
    void SET();
    void ADD();
    void SUB();
    void MUL();
    void MLI();
    void DIV();
    void DVI();
    void MOD();
    void MDI();
    void AND();
    void BOR();
    void XOR();
    void SHR();
    void ASR();
    void SHL();
    bool IFB();
    bool IFC();
    bool IFE();
    bool IFN();
    bool IFG();
    bool IFA();
    bool IFL();
    bool IFU();
    void ADX();
    void SBX();
    void STI();
    void STD();
    
    // Extended opcodes
    void JSR();
    void INT();
    void IAG();
    void IAS();
    void RFI();
    void IAQ();
    void HWN();
    void HWQ();
    void HWI();
    
    /*!
     *  \brief GetOperand
     *
     *  \param uCopType_ The operand type, as specified in DCPU_Argument
     *  \param pu16Result_ Pointer to the pointer that corresponds to the 
     *                  argument's location in memory.
     */
    uint8_t GetOperand( uint8_t uCopType_, uint16_t **pu16Result_ );
    
    
    /*!
     *  \brief ProcessInterruptQueue
     *
     *  Process the next interrupt in the Queue.        
     */
    void ProcessInterruptQueue();
        
    DCPU_Registers m_stRegisters;    //!< CPU Register file
        
    uint16_t *a;                //!< Temporary "a" operand pointer
    uint16_t *b;                //!< Temporary "b" operand pointer
    
    uint16_t m_u16TempA;         //!< Local-storage for staging literal "a" values
        
    uint16_t *m_pu16RAM;         //!< Pointer to the RAM buffer
    uint16_t m_u16RAMSize;       //!< Size to the RAM (including stack)    
    
    uint16_t *m_pu16ROM;         //!< Pointer to the CPU ROM storage
    uint16_t m_u16ROMSize;       //!< Size of the ROM 
            
    uint32_t  m_u32CycleCount;    //!< Current cycle count
        
    bool   m_bInterruptQueueing;        //!< CPU flag indicating whether or not interrupts are queued
    uint8_t  m_u8QueueLevel;              //!< Current interrupt Queue level
    uint16_t m_au16InterruptQueue[ 8 ];    //!< Interrupt queue
    
    DoubleLinkList m_clPluginList;        //!< Linked-list of plug-ins.
};

#endif
