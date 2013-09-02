// Example AtoD Source

#include <avr/io.h>
#include "kernel.h"
#include "thread.h"
#include "memutil.h"
#include "driver.h"
#include "drvUART.h"

extern "C" {void __cxa_pure_virtual() {} }

//---------------------------------------------------------------------------
static Thread clAppThread;
static Thread clIdleThread;

//---------------------------------------------------------------------------
#define APP_STACK_SIZE		(256)
#define IDLE_STACK_SIZE		(256)

//---------------------------------------------------------------------------
static K_UCHAR aucAppStack[APP_STACK_SIZE];
static K_UCHAR aucIdleStack[IDLE_STACK_SIZE];

//---------------------------------------------------------------------------
void AppThread(void *unused_);
void IdleThread(void *unused_);

//---------------------------------------------------------------------------
#define UART_TX_SIZE				(64)
#define UART_RX_SIZE				(24)

//---------------------------------------------------------------------------
static ATMegaUART clUART;
static K_UCHAR aucRxData[UART_RX_SIZE];
static K_UCHAR aucTxData[UART_TX_SIZE];

//---------------------------------------------------------------------------
int main(void)
{
	Kernel::Init();

	clAppThread.Init(aucAppStack, APP_STACK_SIZE, 1, AppThread, NULL);
	clIdleThread.Init(aucIdleStack, IDLE_STACK_SIZE, 0, IdleThread, NULL);
	
	clAppThread.Start();
	clIdleThread.Start();
	
    clUART.SetName("/dev/tty");
    clUART.Init();

    DriverList::Add(&clUART);

	Kernel::Start();
}

//---------------------------------------------------------------------------
void AnalogInit(void)
{
	ADCSRA =  (1 << ADPS0)	// Prescalar = 128
			| (1 << ADPS1)
			| (1 << ADPS2)
			| (1 << ADEN);  // Enable AtoD
}

//---------------------------------------------------------------------------
K_USHORT AnalogRead(K_UCHAR ucChannel_)
{
    K_UCHAR ucLow;
    K_UCHAR ucHigh;
    K_USHORT usRet;

    // Set reference to AVCC, and the channel
    ADMUX = (1 << REFS0) | (ucChannel_ & 0x0F);

    // Start the conversion, and spin until the register is cleared, indicating
    // completion.
	ADCSRA |= (1 << ADSC);
	while ( (ADCSRA & ( 1 << ADSC)) ) { /* do nothing */ };

    // Read the results from the registers, and assemble into a single int
    ucLow  = ADCL;
    ucHigh = ADCH;

    usRet = (ucHigh << 8) | ucLow;
    return usRet;
}

//---------------------------------------------------------------------------
void AppThread(void *unused_)
{    
    K_USHORT usRetVal;
    K_UCHAR ucChannel = 0;
    K_CHAR acStrBuf[6];

    // Grab the serial driver
    Driver *my_uart = DriverList::FindByPath("/dev/tty");

    // Set up the serial driver's buffers
    my_uart->Control( CMD_SET_BUFFERS, aucRxData, UART_RX_SIZE,
                                         aucTxData, UART_TX_SIZE);
    // Open the driver
    my_uart->Open();

    // Initialize the AtoD code
    AnalogInit();
	
    // Loop through each of the AtoD channels in sequence and print out the
    // values to the serial port
	while(1)
    {
        usRetVal = AnalogRead(ucChannel);

        MemUtil::DecimalToString(ucChannel, acStrBuf);
        my_uart->Write( MemUtil::StringLength(acStrBuf), (K_UCHAR*)acStrBuf);
        my_uart->Write(1, (K_UCHAR*)(" "));

        MemUtil::DecimalToString(usRetVal, acStrBuf);
        my_uart->Write( MemUtil::StringLength(acStrBuf), (K_UCHAR*)acStrBuf);
        my_uart->Write(1, (K_UCHAR*)("\n"));

        ucChannel = ( ucChannel + 1 ) & 0x0F;

        Thread::Sleep(10);
	}
}
//---------------------------------------------------------------------------
void IdleThread(void *unused_)
{
	while(1)
	{
		/* Do Idle Tasks */
	}
}



