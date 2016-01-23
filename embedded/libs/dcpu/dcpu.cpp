/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012-2016 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file dcpu.cpp
    \brief Portable DCPU-16 CPU emulator

    The DCPU-16 is the in-game CPU used in the upcoming game 0x10^c, from the
    creators of the wildly successful Minecraft.  While the DCPU is supposed
    to be part of the game, it has serious potential for use in all sorts of
    embedded applications.
    
    The fact that DCPU is a very lightweight VM to implement and contains
    built-in instructions for accessing hardware peripheras and handling 
    external interrupts lends itself to being used on microcontrollers.  
    
    Unlike a lot of embedded CPUs, DCPU-16 assembly is extremely simple to 
    learn, since it has a very limited number of opcodes (37), each of which
    provide the same register/memory addressing modes for all operands.  
    There are also only 2 opcode formats which make interpreting opcodes very
    efficient.
    
    The DCPU-16 is extended using a variable number of "external hardware
    devices" which communicate with the CPU core using interrupts.  These
    devices are enumerated on startup, and since there is no defined format
    for how these devices work, we can hijack this interface to provide a 
    way for the DCPU to access resources supplied by the OS (i.e Timers, 
    Drivers), or the hardware directly.  This also lends itself to inter-VM
    communications (multiple DCPUs communicating with eachother in different
    OS threads).  There's an immense amount of flexibility here - applications
    from debugging to scripting to runtime-configuration are all easily 
    supported by this machine.
    
    But what is a platform without tools support?  Fortunately, the hype around
    0x10c is building - and a development community for this platform has 
    grown immensely.  There are a number of compilers, assemblers, and IDEs,
    many of which support virtualized hardware extensions.  One of the 
    compilers is a CLANG/LLVM backend, which should allow for very good C
    language support.
    
    I had attempted to do something similar by creating a VM based on the 8051
    (see the Funk51 project on sourceforge), but that project was at least
    four times as large - and the tools support was very spotty.  There were
    C compilers, but there was a lot of shimming required to produce output
    that was suitable for the VM.  Also, the lack of a native host interface
    (interrupts, hardware bus enumerations, etc.) forced a non-standard
    approach to triggering native methods by writing commands to a reserved
    chunk of memory and writing to a special "trigger" address to invoke the
    native system.  using a DCPU-16 based simulator addresses this in a nice,
    clean way by providing modern tools, and a VM infrastruture tailored to
    be interfaced with a host.
    
    Regarding this version of the DCPU emulator - it's very simple to use.  
    Program binaries are loaded into buffers in the host CPU's RAM, with
    the host also providing a separate buffer for DCPU RAM.  The size of the
    DCPU RAM buffer will contain both the RAM area, as well as the program
    stack, so care must be taken to ensure that the stack doesn't overflow.
    The DCPU specification allows for 64K words (128KB) of RAM and ROM each,
    but this implementation allows u16 to tailor the CPU for more efficient
    or minimal environments.

    In the future, this emulator will be extended to provide a mechanism to 
    allow programs to be run out of flash, EEPROM, or other interfaces via
    the Mark3 Drivers API.
    
    Once the program has been loaded into the host's address space, the 
    DCPU class can be initialized.
    
    \code
    // use 16-bit words for 16-bit emulator.
    uint16_t au16[A-Z]AM[ RAM_SIZE ];
    uint16_t au16[A-Z]OM[ ROM_SIZE ];
    {
        class DCPU clMyDCPU;
        
        // Read program code into au16[A-Z]OM buffer here
        
        // Initialize the DCPU emulator
        clMyDCPU.Init( au16[A-Z]OM, RAM_SIZE, au16[A-Z]OM, ROM_SIZE );
    }
    \endcode
    
    Once the emulator has been initialized, the VM can be run one opcode at a
    time, as in the following example.
    
    \code
    while(1)
    {
        clMyCPU.RunOpcode();
    }
    \endcode
    
    To inspect the contents of the VM's registers, call the GetRegisters() 
    method.  This is useful for printing the CPU state on a regular basis, or
    using the PC value to determine when to end execution, or to provide an
    offset for disassembling the current opcode.  
    
    \code 
    DCPU_Registers *pstRegisters;
    pstRegisters = clMyCPU.GetRegisters();
    \endcode 

*/
#include "dcpu.h"
#include "kerneltypes.h"
#include "ll.h"

#define CORE_DEBUG 0

//---------------------------------------------------------------------------
#if CORE_DEBUG

  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>

  #define DBG_PRINT(...)        printf(__VA_ARGS__)
#else
  #define DBG_PRINT(...)
#endif

//---------------------------------------------------------------------------
/*!
    Define the number of cycles that each "basic" opcode takes to execute.
*/
static const uint8_t aucBasicOpcodeCycles[] = 
{
    0,        // OP_NON_BASIC = 0
    1,        // OP_SET
    2,        // OP_ADD
    2,        // OP_SUB
    2,        // OP_MUL
    2,        // OP_MLI
    3,        // OP_DIV
    3,        // OP_DVI, 
    3,        // OP_MOD, 
    3,        // OP_MDI, 
    1,        // OP_AND, 
    1,        // OP_BOR, 
    1,        // OP_XOR, 
    1,        // OP_SHR, 
    1,        // OP_ASR, 
    1,        // OP_SHL, 
    2,        // OP_IFB, 
    2,        // OP_IFC, 
    2,        // OP_IFE, 
    2,        // OP_IFN, 
    2,        // OP_IFG, 
    2,        // OP_IFA, 
    2,        // OP_IFL, 
    2,        // OP_IFU, 
    0,        // OP_18,     
    0,        // OP_19,    
    3,        // OP_ADX,    
    3,        // OP_SBX,    
    0,        // OP_1C,    
    0,        // OP_1D,    
    2,        // OP_STI,    
    2,        // OP_STD     
};

//---------------------------------------------------------------------------
/*!
    Define the number of cycles that each "extended" opcode takes to execute.
*/
static const uint8_t aucExtendedOpcodeCycles[] = 
{
    0,    // "RESERVED",
    3,    // "JSR",        
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    4,    // "INT",        
    1,    // "IAG",        
    1,    // "IAS",        
    3,    // "RFI",        
    2,    // "IAQ",        
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    2,    // "HWN",        
    4,    // "HWQ",        
    4,    // "HWI",        
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
    0,    // "UNDEFINED"
};

//---------------------------------------------------------------------------
void DCPU::SET()
{
    DBG_PRINT("SET\n");
    *b = *a;
}

//---------------------------------------------------------------------------
void DCPU::ADD()
{
    uint32_t u32Temp;
    DBG_PRINT("ADD\n");
    
    u32Temp = (uint32_t)*a + (uint32_t)*b;    
    if (u32Temp >= 65536) 
    {
        m_stRegisters.EX = 0x0001;
    }
    else
    {
        m_stRegisters.EX = 0;
    }
    
    *b = *b + *a;
}

//---------------------------------------------------------------------------
void DCPU::SUB()
{
    int32_t lTemp;
    DBG_PRINT("SUB\n");
    
    lTemp = (int32_t)*b - (int32_t)*a;    
    if (lTemp < 0) 
    {        
        m_stRegisters.EX = 0xFFFF;
    }
    else
    {
        m_stRegisters.EX = 0;
    }
    
    *b = *b - *a;
}

//---------------------------------------------------------------------------
void DCPU::MUL()
{
    uint32_t u32Temp;
    
    DBG_PRINT("MUL\n");
    u32Temp = (((uint32_t)*a * (uint32_t)*b));    
    m_stRegisters.EX = (uint16_t)(u32Temp >> 16);
    *b = (uint16_t)(u32Temp & 0x0000FFFF);
}

//---------------------------------------------------------------------------
void DCPU::MLI()
{
    int32_t lTemp;
    
    DBG_PRINT("MLI\n");
    lTemp = ((int32_t)(*(int16_t*)a) * (int32_t)(*(int16_t*)b));
    m_stRegisters.EX = (uint16_t)(lTemp >> 16);
    *b = (uint16_t)(lTemp & 0x0000FFFF);
}

//---------------------------------------------------------------------------
void DCPU::DIV()
{
    uint16_t u16Temp;
    
    DBG_PRINT("DIV\n");
    if (*a == 0)
    {
        *b = 0;
        m_stRegisters.EX = 0;
    }
    else
    {
        u16Temp = (uint16_t)((((uint32_t)*b) << 16) / (uint32_t)*a);        
        *b = *b / *a;
        m_stRegisters.EX = u16Temp;
    }
}

//---------------------------------------------------------------------------
void DCPU::DVI()
{
    uint16_t u16Temp;
    
    DBG_PRINT("DVI\n");
    if (*a == 0)
    {
        *b = 0;
        m_stRegisters.EX = 0;
    }
    else
    {
        u16Temp = (uint16_t)((((int32_t)*((int16_t*)b)) << 16) / (int32_t)(*(int16_t*)a));        
        *b = (uint16_t)(*(int16_t*)b / *(int16_t*)a);
        m_stRegisters.EX = u16Temp;
        
    }
}

//---------------------------------------------------------------------------
void DCPU::MOD()
{
    DBG_PRINT("MOD\n");
    if (*a == 0)
    {
        *b = 0;
    }
    else
    {
        *b = *b % *a;
    }
}

//---------------------------------------------------------------------------
void DCPU::MDI()
{
    DBG_PRINT("MDI\n");
    if (*b == 0)
    {
        *a = 0;
    }
    else
    {
        *b = (uint16_t)(*((int16_t*)b) % *((int16_t*)a));
    }
}

//---------------------------------------------------------------------------
void DCPU::AND()
{
    DBG_PRINT("AND\n");
    *b = *b & *a;
}

//---------------------------------------------------------------------------
void DCPU::BOR()
{
    DBG_PRINT("BOR\n");
    *b = *b | *a;
}

//---------------------------------------------------------------------------
void DCPU::XOR()
{
    DBG_PRINT("XOR\n");
    *b = *b ^ *a;
}

//---------------------------------------------------------------------------
void DCPU::SHR()
{
    uint16_t u16Temp = (uint16_t)((((uint32_t)*b) << 16) >> (uint32_t)*a);
    
    DBG_PRINT("SHR\n");
    *b = *b >> *a;
    m_stRegisters.EX = u16Temp;
}

//---------------------------------------------------------------------------
void DCPU::ASR()
{
    uint16_t u16Temp = (uint16_t)((((int32_t)*b) << 16) >> (int32_t)*a);
    
    DBG_PRINT("ASR\n");
    *b = (uint16_t)(*(int16_t*)b >> *(int16_t*)a);
    m_stRegisters.EX = u16Temp;
}
//---------------------------------------------------------------------------
void DCPU::SHL()
{
    uint16_t u16Temp = (uint16_t)((((uint32_t)*b) << (uint32_t)*a) >> 16);
    
    DBG_PRINT("SHL\n");
    *b = *b << *a;
    m_stRegisters.EX = u16Temp;
}

//---------------------------------------------------------------------------
bool DCPU::IFB()
{
    DBG_PRINT("IFB\n");    
    if ((*b & *a) != 0)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFC()
{
    DBG_PRINT("IFC\n");
    if ((*b & *a) == 0)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFE()
{
    DBG_PRINT("IFE\n");
    if (*b == *a)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFN()
{
    DBG_PRINT("IFN\n");
    if (*b != *a)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFG()
{
    DBG_PRINT("IFG\n");
    if (*b > *a)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFA()
{
    DBG_PRINT("IFA\n");
    if (*((int16_t*)b) > *((int16_t*)a))
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFL()
{
    DBG_PRINT("IFL\n");
    if (*b < *a)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool DCPU::IFU()
{
    DBG_PRINT("IFU\n");
    if (*(int16_t*)b < *(int16_t*)a)
    {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
void DCPU::ADX()
{
    uint32_t u32Temp;
    DBG_PRINT("ADX\n");
    u32Temp = (uint32_t)*b + (uint32_t)*a + (uint32_t)m_stRegisters.EX;
    if (u32Temp >= 0x10000)
    {
        m_stRegisters.EX = 1;
    }
    else
    {
        m_stRegisters.EX = 0;
    }
    
    *b = ((uint16_t)(u32Temp & 0x0000FFFF));    
}

//---------------------------------------------------------------------------
void DCPU::SBX()
{
    int32_t lTemp;
    DBG_PRINT("SBX\n");
    lTemp = (int32_t)*b - (int32_t)*a + (int32_t)m_stRegisters.EX;
    if (lTemp < 0 )
    {
        m_stRegisters.EX = 0xFFFF;
    }
    else
    {
        m_stRegisters.EX = 0;
    }
    
    *b = ((uint16_t)(lTemp & 0x0000FFFF));    
}

//---------------------------------------------------------------------------
void DCPU::STI()
{
    DBG_PRINT("STI\n");
    *b = *a;
    m_stRegisters.I++;
    m_stRegisters.J++;
}

//---------------------------------------------------------------------------
void DCPU::STD()
{
    DBG_PRINT("STD\n");
    *b = *a;
    m_stRegisters.I--;
    m_stRegisters.J--;
}

//---------------------------------------------------------------------------
void DCPU::JSR()
{
    DBG_PRINT("JSR 0x%04X\n", *a);
    m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.PC;
    m_stRegisters.PC = *a;
}

//---------------------------------------------------------------------------
void DCPU::INT()
{
    DBG_PRINT("INT\n");

    if (m_stRegisters.IA == 0)
    {
        // If IA is not set, return out.
        return;
    }
    
    // Either acknowledge the interrupt immediately, or queue it.
    if (m_bInterruptQueueing == false)
    {    
        m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.PC;
        m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.A;
        
        m_stRegisters.A = *a;
        m_stRegisters.PC = m_stRegisters.IA;    
        m_bInterruptQueueing = true;
    }
    else
    {
        // Add interrupt message to the queue
        m_au16InterruptQueue[ ++m_u8QueueLevel ] = *a;
    }
}

//---------------------------------------------------------------------------
void DCPU::ProcessInterruptQueue()
{
    // If there's an interrupt address specified, queueing is disabled, and 
    // the queue isn't empty
    if (m_stRegisters.IA && !m_bInterruptQueueing && m_u8QueueLevel)
    {
        m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.PC;
        m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.A;
        
        m_stRegisters.A = m_au16InterruptQueue[ m_u8QueueLevel-- ];
        m_stRegisters.PC = m_stRegisters.IA;    
        
        m_bInterruptQueueing = true;
    }
}


//---------------------------------------------------------------------------
void DCPU::IAG()
{
    DBG_PRINT("IAG\n");
    
    *a = m_stRegisters.IA;
}

//---------------------------------------------------------------------------
void DCPU::IAS()
{
    DBG_PRINT("IAS\n");
    
    m_stRegisters.IA = *a;
}

//---------------------------------------------------------------------------
void DCPU::RFI()
{
    DBG_PRINT("RFI\n");
    
    /*! Disables interrupt queueing, pop A from the stack, then pops PC from
        the stack.  By disabling interrupt Queueing, we're essentially 
        re-enabling interrupts. */
    m_bInterruptQueueing = false;

    m_stRegisters.A  = m_pu16RAM[ ++m_stRegisters.SP ];
    m_stRegisters.PC = m_pu16RAM[ ++m_stRegisters.SP ];
    
}

//---------------------------------------------------------------------------
void DCPU::IAQ()
{
    DBG_PRINT("IAQ\n");
    
    /*! Add an interrupt to the interrupt queue if non-zero, if a = 0 then
        interrupts will be triggered as normal */
        
    if (*a)
    {
        m_bInterruptQueueing = true;    /*! Interrupts queued */
    }
    else
    {
        m_bInterruptQueueing = false;    /*! Interrups triggered */
    }
}

//---------------------------------------------------------------------------
void DCPU::HWN()
{        
    LinkListNode *pclNode;
    
    DBG_PRINT("HWN\n");
    m_u16TempA = 0;
    /*! Returns the number of connected hardware devices to "a" */
    pclNode = m_clPluginList.GetHead();
    while (pclNode)
    {
        m_u16TempA++;
        pclNode = pclNode->GetNext();
    }
    
    *a = m_u16TempA;
}

//---------------------------------------------------------------------------
void DCPU::HWQ()
{
    DBG_PRINT("HWQ\n");
    DCPUPlugin *pclPlugin;
    pclPlugin = (DCPUPlugin*)m_clPluginList.GetHead();
    
    while (pclPlugin)
    {
        if (pclPlugin->GetDeviceNumber() == *a)
        {
            pclPlugin->Enumerate(&m_stRegisters);
            break;
        }        
        pclPlugin = (DCPUPlugin*)pclPlugin->GetNext();
    }
}

//---------------------------------------------------------------------------
void DCPU::HWI()
{
    DBG_PRINT("HWI\n");
    
    DCPUPlugin *pclPlugin;
    pclPlugin = (DCPUPlugin*)m_clPluginList.GetHead();
    
    while (pclPlugin)
    {        
        if (pclPlugin->GetDeviceNumber() == *a)
        {
            pclPlugin->Interrupt(this);
            break;
        }
        pclPlugin = (DCPUPlugin*)pclPlugin->GetNext();
    }
}

//---------------------------------------------------------------------------
void DCPU::Init(     uint16_t *pu16RAM_, uint16_t u16RAMSize_,
                    const uint16_t *pu16ROM_, uint16_t u16ROMSize_ )
{
    m_stRegisters.PC = 0;
    m_stRegisters.SP = u16RAMSize_ ;
    m_stRegisters.A = 0;
    m_stRegisters.B = 0;
    m_stRegisters.C = 0;
    m_stRegisters.X = 0;
    m_stRegisters.Y = 0;
    m_stRegisters.Z = 0;
    m_stRegisters.I = 0;
    m_stRegisters.J = 0;
    m_stRegisters.EX = 0;
    m_stRegisters.IA = 0;
    m_u32CycleCount = 0;
    
    m_pu16ROM = (uint16_t*)pu16ROM_;
    m_u16ROMSize = u16ROMSize_;
    
    m_pu16RAM = pu16RAM_;
    m_u16RAMSize = u16RAMSize_;
}

//---------------------------------------------------------------------------
uint8_t DCPU::GetOperand( uint8_t uCopType_, uint16_t **pu16Result_ )
{
    uint8_t u8RetVal = 0;
    switch (uCopType_)
    {
        case ARG_A:    case ARG_B:    case ARG_C:    case ARG_X:    
        case ARG_Y:    case ARG_Z:    case ARG_I:    case ARG_J:
            *pu16Result_ = &m_stRegisters.au16Registers[ uCopType_ - ARG_A ];
            break;
                
        case ARG_BRACKET_A:    case ARG_BRACKET_B:    case ARG_BRACKET_C:    case ARG_BRACKET_X:            
        case ARG_BRACKET_Y:    case ARG_BRACKET_Z:    case ARG_BRACKET_I:    case ARG_BRACKET_J:
            *pu16Result_ = &m_pu16RAM[ m_stRegisters.au16Registers[ uCopType_ - ARG_BRACKET_A ] ];
            break;
        
        case ARG_WORD_A: case ARG_WORD_B: case ARG_WORD_C: case ARG_WORD_X: 
        case ARG_WORD_Y: case ARG_WORD_Z: case ARG_WORD_I: case ARG_WORD_J:
        {
            uint16_t u16Temp = m_pu16ROM[ m_stRegisters.PC++ ];
            u16Temp += m_stRegisters.au16Registers[ uCopType_ - ARG_WORD_A ];
            *pu16Result_ = &m_pu16RAM[ u16Temp ];
            u8RetVal = 1;
        }
            break;
        case ARG_PUSH_POP_SP:
            if (*pu16Result_ == a)
            {
                a = &m_pu16RAM[ ++m_stRegisters.SP ];
            }
            else
            {
                b = &m_pu16RAM[ m_stRegisters.SP-- ];
            }
            break;    
        case ARG_PEEK_SP:
            *pu16Result_ = &m_pu16RAM[ m_stRegisters.SP ];            
            break;
        case ARG_WORD_SP:
        {
            uint16_t u16Temp = m_pu16ROM[ ++m_stRegisters.PC ];
            u16Temp += m_stRegisters.SP;
            *pu16Result_ = &m_pu16RAM[ u16Temp ];
            u8RetVal++;
        }
            break;        
        case ARG_SP:            
            *pu16Result_ = &(m_stRegisters.SP);
            break;
        case ARG_PC:            
            *pu16Result_ = &(m_stRegisters.PC);
            break;
        case ARG_EX:            
            *pu16Result_ = &(m_stRegisters.EX);
            break;
        case ARG_NEXT_WORD:
            *pu16Result_ = &m_pu16RAM[ m_pu16ROM[ m_stRegisters.PC++ ] ];
            u8RetVal++;
            break;
        case ARG_NEXT_LITERAL:
            *pu16Result_ = &m_pu16ROM[ m_stRegisters.PC++ ];
            u8RetVal++;
            break;
            
        case ARG_LITERAL_0:
            *pu16Result_ = &m_u16TempA;
            m_u16TempA = (uint16_t)(-1);        
            break;
        case ARG_LITERAL_1:    case ARG_LITERAL_2:    case ARG_LITERAL_3:    case ARG_LITERAL_4:
        case ARG_LITERAL_5:    case ARG_LITERAL_6: case ARG_LITERAL_7:    case ARG_LITERAL_8:
        case ARG_LITERAL_9:    case ARG_LITERAL_A:    case ARG_LITERAL_B:    case ARG_LITERAL_C:
        case ARG_LITERAL_D:    case ARG_LITERAL_E:    case ARG_LITERAL_F:    case ARG_LITERAL_10:
        case ARG_LITERAL_11: case ARG_LITERAL_12: case ARG_LITERAL_13: case ARG_LITERAL_14:
        case ARG_LITERAL_15: case ARG_LITERAL_16: case ARG_LITERAL_17: case ARG_LITERAL_18:
        case ARG_LITERAL_19: case ARG_LITERAL_1A: case ARG_LITERAL_1B: case ARG_LITERAL_1C:
        case ARG_LITERAL_1D: case ARG_LITERAL_1E: case ARG_LITERAL_1F:
            *pu16Result_ = &m_u16TempA;
            m_u16TempA = (uint16_t)(uCopType_ - ARG_LITERAL_1);    
            break;
        default:
            break;
    }
    return u8RetVal;
}

//---------------------------------------------------------------------------
void DCPU::RunOpcode()
{
    // Fetch the opcode @ the current program counter
    uint16_t u16Word = m_pu16ROM[ m_stRegisters.PC++ ];        
    uint8_t uCop = (uint8_t)DCPU_NORMAL_OPCODE_MASK(u16Word);
    uint8_t u8A = (uint8_t)DCPU_A_MASK(u16Word);
    uint8_t u8B = (uint8_t)DCPU_B_MASK(u16Word);
    uint8_t u8Size = 1;

    DBG_PRINT("0x%04X: %04X\n", m_stRegisters.PC - 1, u16Word );

    // Decode the opcode 
    if (uCop)
    {
        bool bRunNext = true;
        
        a = &m_u16TempA;
        b = 0;
        
        // If this is a "basic" opcode, decode "a" and "b"
        u8Size += GetOperand( u8A , &a );
        u8Size += GetOperand( u8B, &b );

        // Add the cycles to the runtime clock
        m_u32CycleCount += (uint32_t)aucBasicOpcodeCycles[ uCop ];
        m_u32CycleCount += (u8Size - 1);
        
        // Execute the instruction once we've decoded the opcode and 
        // processed the arguments.
        switch (DCPU_NORMAL_OPCODE_MASK(u16Word))
        {
            case OP_SET:    SET();        break;
            case OP_ADD:    ADD();        break;
            case OP_SUB:    SUB();        break;

            case OP_MUL:    MUL();        break;
            case OP_MLI:    MLI();        break;            
            case OP_DIV:    DIV();        break;
            case OP_DVI:    DVI();        break;
            case OP_MOD:    MOD();        break;
            case OP_MDI:    MDI();        break;
            case OP_AND:    AND();        break;
            case OP_BOR:    BOR();        break;
            case OP_XOR:    XOR();        break;
            case OP_SHR:    SHR();        break;
            case OP_ASR:    ASR();        break;
            case OP_SHL:    SHL();        break;
            case OP_IFB:    bRunNext = IFB();    break;            
            case OP_IFC:    bRunNext = IFC();    break;
            case OP_IFE:    bRunNext = IFE();    break;                
            case OP_IFN:    bRunNext = IFN();    break;
            case OP_IFG:    bRunNext = IFG();    break;            
            case OP_IFA:    bRunNext = IFA();    break;
            case OP_IFL:    bRunNext = IFL();    break;
            case OP_IFU:    bRunNext = IFU();    break;
            case OP_ADX:    ADX();        break;
            case OP_SBX:    SBX();        break;
            case OP_STI:    STI();        break;
            case OP_STD:    STD();        break;
            default:    break;
        }
        
        // If we're not supposed to run the next instruction (i.e. skip it 
        // due to failed condition), adjust the PC.
        if (!bRunNext)
        {            
            // Skipped branches take an extra cycle 
            m_u32CycleCount++;
            
            // Skip the next opcode
            u16Word = m_pu16ROM[ m_stRegisters.PC++ ];            
            if (DCPU_NORMAL_OPCODE_MASK(u16Word))
            {
                DBG_PRINT( "Skipping Basic Opcode: %X\n", DCPU_NORMAL_OPCODE_MASK(u16Word));
                // If this is a "basic" opcode, decode "a" and "b" - we do this to make sure our
                // PC gets adjusted properly.
                GetOperand( DCPU_A_MASK(u16Word), &a );
                GetOperand( DCPU_B_MASK(u16Word), &b );
            }
            else
            {
                DBG_PRINT( "Skipping Extended Opcode: %X\n", DCPU_EXTENDED_OPCODE_MASK(u16Word));
                GetOperand( DCPU_A_MASK(u16Word), &a );
            }
        }
    }
    else
    {        
        // Extended opcode.  These only have a single argument, stored in the 
        // "a" field.
        GetOperand( u8A, &a );
        m_u32CycleCount++;

        // Execute the "extended" instruction now that the opcode has been
        // decoded, and the arguments processed.
        switch (u8B)
        {
            case OP_EX_JSR:        JSR();    break;
            case OP_EX_INT:        INT();    break;
            case OP_EX_IAG:        IAG();    break;
            case OP_EX_IAS:        IAS();    break;
            case OP_EX_RFI:        RFI();    break;
            case OP_EX_IAQ:        IAQ();    break;
            case OP_EX_HWN:        HWN();    break;
            case OP_EX_HWQ:        HWQ();    break;
            case OP_EX_HWI:        HWI();    break;
            default:    break;
        }
    }
    
    // Process an interrupt from the queue (if there is one)
    ProcessInterruptQueue();    
}

//---------------------------------------------------------------------------
void DCPU::SendInterrupt( uint16_t u16Message_ )
{
    if (m_stRegisters.IA == 0)
    {
        // If IA is not set, return out.
        return;
    }
    
    // Either acknowledge the interrupt immediately, or queue it.
    if (m_bInterruptQueueing == false)
    {    
        m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.PC;
        m_pu16RAM[ m_stRegisters.SP-- ] = m_stRegisters.A;
        
        m_stRegisters.A = u16Message_;
        m_stRegisters.PC = m_stRegisters.IA;    
        m_bInterruptQueueing = true;
    }
    else
    {
        // Add interrupt message to the queue
        m_au16InterruptQueue[ ++m_u8QueueLevel ] = u16Message_;
    }    
}

//---------------------------------------------------------------------------
void DCPU::AddPlugin( DCPUPlugin *pclPlugin_ )
{
    m_clPluginList.Add( (LinkListNode*)pclPlugin_ );    
}
