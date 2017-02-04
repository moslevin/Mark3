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

    \file   socket.h

    \brief  Synchronous/Asynchronous buffered sockets
*/

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "mark3.h"
#include "driver.h"
#include "streamer.h"

//---------------------------------------------------------------------------
#define SOCKET_ERROR			(0xFFFF)

//---------------------------------------------------------------------------
// Socket control event IDs
typedef enum {
	SOCKET_CONTROL_SET_BUFFERS,
	SOCKET_CONTROL_SET_BLOCKING,
	SOCKET_CONTROL_SET_NONBLOCKING,	
	SOCKET_CONTROL_SET_NOTIFIERS,
	SOCKET_CONTROL_CONNECT_TO_SERVER,	
	SOCKET_CONTROL_PRIVATE_CLIENT_READ,
	SOCKET_CONTROL_PRIVATE_CLIENT_WRITE,
	SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_READ,
	SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_WRITE,
	SOCKET_CONTROL_PRIVATE_CLIENT_CLOSED,
} SocketControl_t;

//---------------------------------------------------------------------------
/*!
 * \brief The ServerSocket class
 *
 * Provides a local socket (i.e. BSD-style domain socket), supporting buffered
 * synchronous or asynchronous communications between server and client threads.
 */
class ServerSocket : public Driver
{
public:
    // Implements driver-object functions.
	virtual void Init();	
	virtual uint8_t Open();
	virtual uint8_t Close();
    virtual uint16_t Read(uint16_t u16Bytes_, uint8_t* pu8Data_);
    virtual uint16_t Write(uint16_t u16Bytes_, uint8_t* pu8Data_);
    virtual uint16_t Control(uint16_t u16Event_, void* pvDataIn_, uint16_t u16SizeIn_, void* pvDataOut_, uint16_t u16SizeOut_);
	
    /*!
     * \brief SetBlocking
     *
     * Sets the IO mode of the socket to blocking or non-blocking.
     * Non-blocking sockets return early from reads or writes when
     * there is not enough data in the buffer to satisfy the read
     * or write request.  Blocking mode will cause the calleing
     * Thread to block until it has read/written all the data
     * requested.
     *
     * \param bBlocking_ true - make server socket blocking
     *                   false - make server non-blocking
     */
	void SetBlocking(bool bBlocking_);
	
    /*!
     * \brief SetBuffers
     *
     * Set the physical memory buffers used for socket reads
     * and writes.  These buffers must persist for the duration
     * of the socket's lifespan.
     *
     * \param pu8In_     Pointer to socket's input (read) buffer
     * \param kaInSize_  size of input buffer in bytes
     * \param pu8Out_    Pointer to the socket's output (write) buffer
     * \param kaOutSize_ size of output buffer in bytes
     */
	void SetBuffers(uint8_t* pu8In_, K_ADDR kaInSize_, uint8_t* pu8Out_, K_ADDR kaOutSize_);

    /*!
     * \brief SetNotifiers
     *
     * Set the notification objects used for unblocking the server
     * thread when there is data to be read or free space to be
     * written to.  These are set manually in order to allow a server
     * to block on multiple sockets simultaneously.
     *
     * \param pclNotifyIn_  Server input (read) notification object
     * \param pclNotifyOut_ Server output (write) notification object
     */
	void SetNotifiers(Notify* pclNotifyIn_, Notify* pclNotifyOut_);
	
    /*!
     * \brief WaitOnInput
     * Block until there is input available to be read on the socket
     *
     * \return true on success, false on socket error
     */
	bool WaitOnInput();

    /*!
     * \brief WaitOnOutput
     * Block until there is free space in the socket's output buffer
     * to be written to.
     *
     * \return true on success, false on socket error
     */
	bool WaitOnOutput();
	
#if defined(KERNEL_USE_TIMEOUTS)
    /*!
     * \brief WaitOnInput
     * Block until there is input available to be read on the socket,
     * or until the operation timed out.
     *
     * \param u32WaitTimeMS_ Maximum time to wait for data (in ms)
     * \return true on success, false on socket timeout.
     */
	bool WaitOnInput(uint32_t u32WaitTimeMS_);

    /*!
     * \brief WaitOnOutput
     * Block until there is free space in the socket's output buffer
     * to be written to, or until the operation timed out.
     *
     * \param u32WaitTimeMS_ Maximum time to wait for data (in ms)
     * \return true on success, false on socket timeout.
     * \return
     */
	bool WaitOnOutput(uint32_t u32WaitTimeMS_);
#endif
	
    /*!
     * \brief IsOpen
     * Return the socket's current state
     * \return true - socket is open, false - socket is closed
     */
	bool IsOpen() { return m_bOpen; }
	
private:	

    /*!
     * \brief ClientRead
     *
     * Perform a client-side read on the socket.  This is invoked by ClientSocket
     * through the use of Control() messages.
     *
     * \param u16Size_   Bytes of data to read
     * \param pu8Data_   Buffer to hold the received data
     * \param bBlocking_ true - call is blocking, false - call is non-blocking
     * \return Number of bytes read, or SOCKET_ERROR on error.
     */
	uint16_t ClientRead(uint16_t u16Size_, uint8_t* pu8Data_, bool bBlocking_);

    /*!
     * \brief ClientWrite
     *
     * Perform a client-side write on the socket.  This is invoked by ClientSocket
     * through the use of Control() messages.
     *
     * \param u16Size_  Number of bytes to write
     * \param pu8Data_  Buffer containing data to be written
     * \param bBlocking_ true - call is blocking, false - call is non-blocking
     * \return Number of bytes written, or SOCKET_ERROR on error.
     */
	uint16_t ClientWrite(uint16_t u16Size_, uint8_t* pu8Data_, bool bBlocking_);

    bool	 m_bBlocking;               //!< Whether or not the socket is blocking or non-blocking
    bool	 m_bOpen;                   //!< Socket-open state
	
    Streamer m_clInput;                 //!< Client->Server stream
    Streamer m_clOutput;                //!< Server->Client stream
	
    Notify	 m_clNotifyClientRead;      //!< Object used by client for "can-read" notifications.
    Notify	 m_clNotifyClientWrite;     //!< Object used by client for "can-write" notifications.
	
    Notify*	 m_pclNotifyServerRead;     //!< Pointer to the local "can-read" notification
    Notify*	 m_pclNotifyServerWrite;	//!< Pointer to the local "can-write" notification
};

//---------------------------------------------------------------------------
/*!
 * \brief The ClientSocket class
 */
class ClientSocket : public Driver
{
public:
    // Implements driver-object functions.
	virtual void Init();	
	virtual uint8_t Open();
	virtual uint8_t Close();
	virtual uint16_t Read(uint16_t u16Bytes_, uint8_t* pu8Data_);
	virtual uint16_t Write(uint16_t u16Bytes_, uint8_t* pu8Data_);
	virtual uint16_t Control(uint16_t u16Event_, void* pvDataIn_, uint16_t u16SizeIn_, void* pvDataOut_, uint16_t u16SizeOut_);

    /*!
     * \brief SetBlocking
     * Select whether or not the socket performs blocking or non-blocking
     * IO operations.
     *
     * \param bBlocking_ true - make socket blocking, false - make socket non-
     *                   blocking.
     */
	void SetBlocking(bool bBlocking_);

    /*!
     * \brief ConnectToServer
     *
     * Attache the client socket to an existing server socket.  A client socket
     * Must be connected to a server for it to function.
     *
     * \param pclSocket_ Pointer to the server socket object to connect to.
     */
    void ConnectToServer(ServerSocket* pclSocket_) { m_pclSocket = pclSocket_; }

    /*!
     * \brief WaitOnInput
     * Block until there is input available to be read on the socket
     *
     * \return true on success, false on socket error
     */
    bool WaitOnInput();

    /*!
     * \brief WaitOnOutput
     * Block until there is free space in the socket's output buffer
     * to be written to.
     *
     * \return true on success, false on socket error
     */
    bool WaitOnOutput();
	
#if defined(KERNEL_USE_TIMEOUTS)	
    /*!
     * \brief WaitOnInput
     * Block until there is input available to be read on the socket,
     * or until the operation timed out.
     *
     * \param u32WaitTimeMS_ Maximum time to wait for data (in ms)
     * \return true on success, false on socket timeout.
     */
    bool WaitOnInput(uint32_t u32WaitTimeMS_);

    /*!
     * \brief WaitOnOutput
     * Block until there is free space in the socket's output buffer
     * to be written to, or until the operation timed out.
     *
     * \param u32WaitTimeMS_ Maximum time to wait for data (in ms)
     * \return true on success, false on socket timeout.
     */
    bool WaitOnOutput(uint32_t u32WaitTimeMS_);
#endif

private:	
    ServerSocket* m_pclSocket;  //!< Pointer to the server-side socket object
    bool		  m_bBlocking;  //!< Whether or not the socket is blocking or non-blocking
};


#endif //__SOCKET_H__
