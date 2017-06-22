/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 - 2017 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   socket.cpp

    \brief  Synchronous/Asynchronous buffered sockets
*/

#include "mark3.h"
#include "driver.h"
#include "socket.h"

//---------------------------------------------------------------------------
void ServerSocket::Init()
{
	m_bBlocking = true;
	m_bOpen = false;

	m_clNotifyClientRead.Init();
	m_clNotifyClientWrite.Init();
}

//---------------------------------------------------------------------------
uint8_t ServerSocket::Open()
{
	m_bOpen = true;

	m_pclNotifyServerRead->Init();
	m_pclNotifyServerWrite->Init();

	return static_cast<uint8_t>(m_bOpen);
}

//---------------------------------------------------------------------------
uint8_t ServerSocket::Close()
{
	m_bOpen = false;
	m_clNotifyClientWrite.Signal();
	m_clNotifyClientRead.Signal();
	m_pclNotifyServerRead->Signal();
	m_pclNotifyServerWrite->Signal();
	return 0;
}

//---------------------------------------------------------------------------
uint16_t ServerSocket::Read(uint16_t u16Bytes_, uint8_t* pu8Data_)
{
	if (!m_bOpen) {
        return SOCKET_ERROR;
	}

	uint16_t u16ToRead = u16Bytes_;
	uint8_t* pu8Out = pu8Data_;
	uint16_t u16Read;
	if (m_bBlocking) {
		while (u16ToRead != 0u) {
			if (m_clInput.CanRead()) {
				m_clInput.Read(pu8Out);
				u16ToRead--;
				pu8Out++;
			} else {
				m_pclNotifyServerRead->Wait(0);
				if (!m_bOpen) {
                    return SOCKET_ERROR;
				}
			}
		}
		u16Read = u16Bytes_;
	} else {
		while (u16ToRead != 0u) {
			if (!m_clInput.Read(pu8Out)) {
				break;
			}
			u16ToRead--;
			pu8Out++;
		}
		u16Read = u16Bytes_ - u16ToRead;
	}
	m_clNotifyClientWrite.Signal();
	return u16Read;
}

//---------------------------------------------------------------------------
uint16_t ServerSocket::Write(uint16_t u16Bytes_, uint8_t* pu8Data_)
{
	if (!m_bOpen) {
        return SOCKET_ERROR;
	}

	uint16_t u16ToWrite = u16Bytes_;
	uint8_t* pu8Out = pu8Data_;
	uint16_t u16Written;
	if (m_bBlocking) {
		while (u16ToWrite != 0u) {
			if (m_clOutput.CanWrite()) {
				m_clOutput.Write(*pu8Out);
				pu8Out++;
				u16ToWrite--;
			} else {
				m_pclNotifyServerWrite->Wait(0);
				if (!m_bOpen) {
                    return SOCKET_ERROR;
				}
			}
		}
		u16Written = u16Bytes_;
	} else {
		while (u16ToWrite != 0u) {
			if (!m_clOutput.Write(*pu8Out)) {
				break;
			}
			pu8Out++;
			u16ToWrite--;
		}
		u16Written = u16Bytes_ - u16ToWrite;
	}
	m_clNotifyClientRead.Signal();
	return u16Written;
}

//---------------------------------------------------------------------------
uint16_t ServerSocket::Control(uint16_t u16Event_, void* pvDataIn_, uint16_t u16SizeIn_, void* pvDataOut_, uint16_t u16SizeOut_)
{
	switch (u16Event_) {
		case SOCKET_CONTROL_SET_BUFFERS:
			m_clInput.Init(static_cast<uint8_t*>(pvDataIn_), u16SizeIn_);
			m_clOutput.Init(static_cast<uint8_t*>(pvDataOut_), u16SizeOut_);
			break;
		case SOCKET_CONTROL_SET_BLOCKING:
			m_bBlocking = true;
			break;
		case SOCKET_CONTROL_SET_NONBLOCKING:
			m_bBlocking = false;
			break;
		case SOCKET_CONTROL_SET_NOTIFIERS:
			m_pclNotifyServerRead = static_cast<Notify*>(pvDataIn_);
			m_pclNotifyServerWrite = static_cast<Notify*>(pvDataOut_);
			break;
		case SOCKET_CONTROL_PRIVATE_CLIENT_READ:
			return ClientRead(u16SizeOut_, static_cast<uint8_t*>(pvDataOut_), u16SizeIn_ != 0);
		case SOCKET_CONTROL_PRIVATE_CLIENT_WRITE:
			return ClientWrite(u16SizeIn_, static_cast<uint8_t*>(pvDataIn_), u16SizeOut_ != 0);
		case SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_READ: {
			if (!m_bOpen) {
                return SOCKET_ERROR;
			}
#if defined(KERNEL_USE_TIMEOUTS)
			uint32_t u32Timeout = 0;
			if (pvDataIn_ != 0) {
				u32Timeout = *static_cast<uint32_t*>(pvDataIn_);
			}
			m_clNotifyClientRead.Wait(u32Timeout, 0);
#else
			m_clNotifyClientRead.Wait(0);
#endif
		}
			break;
		case SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_WRITE: {
			if (!m_bOpen) {
                return SOCKET_ERROR;
			}
#if defined(KERNEL_USE_TIMEOUTS)
			uint32_t u32Timeout = 0;
			if (pvDataIn_ != 0) {
				u32Timeout = *static_cast<uint32_t*>(pvDataIn_);
			}
			m_clNotifyClientWrite.Wait(u32Timeout, 0);
#else
			m_clNotifyClientWrite.Wait(0);
#endif
		}
			break;
		case SOCKET_CONTROL_PRIVATE_CLIENT_CLOSED:
			m_pclNotifyServerRead->Signal();
			m_pclNotifyServerWrite->Signal();
			m_clNotifyClientRead.Signal();
			m_clNotifyClientWrite.Signal();
			break;
		default:
			break;
	}

	if (m_bOpen) {
        return SOCKET_ERROR;
	}
	return 0;
}

//---------------------------------------------------------------------------
uint16_t ServerSocket::ClientRead(uint16_t u16Size_, uint8_t* pu8Data_, bool bBlocking_)
{
	if (!m_bOpen) {
        return SOCKET_ERROR;
	}

	uint8_t* pu8Out = pu8Data_;
	uint16_t u16ToRead = u16Size_;
	uint16_t u16Read;
	if (bBlocking_) {
		while (u16ToRead != 0u) {
			if (m_clOutput.CanRead()) {
				m_clOutput.Read(pu8Out);
				pu8Out++;
				u16ToRead--;
			} else {
				m_clNotifyClientRead.Wait(0);
				if (!m_bOpen) {
                    return SOCKET_ERROR;
				}
			}
		}
		u16Read = u16Size_;
	} else {
		while (u16ToRead != 0u) {
			if (!m_clOutput.Read(pu8Out)) {
				break;
			}
			u16ToRead--;
		}
		u16Read = u16Size_ - u16ToRead;
	}
	m_pclNotifyServerWrite->Signal();
	return u16Read;
}

//---------------------------------------------------------------------------
uint16_t ServerSocket::ClientWrite(uint16_t u16Size_, uint8_t* pu8Data_, bool bBlocking_)
{
	if (!m_bOpen) {
        return SOCKET_ERROR;
	}

	uint8_t* pu8In = pu8Data_;
	uint16_t u16ToWrite = u16Size_;
	uint16_t u16Written;
	if (bBlocking_) {
		while (u16ToWrite != 0u) {
			if (m_clInput.CanWrite()) {
				m_clInput.Write(*pu8In);
				pu8In++;
				u16ToWrite--;
			} else {
				m_clNotifyClientWrite.Wait(0);
				if (!m_bOpen) {
                    return SOCKET_ERROR;
				}
			}
		}
		u16Written = u16Size_;
	} else {
		while (u16ToWrite != 0u) {
			if (!m_clInput.Write(*pu8In)) {
				break;
			}
			u16ToWrite--;
			pu8In++;
		}
		u16Written = u16Size_ - u16ToWrite;
	}
	m_pclNotifyServerRead->Signal();
	return u16Written;
}

//---------------------------------------------------------------------------
void ServerSocket::SetBlocking(bool bBlocking_)
{
	m_bBlocking = bBlocking_;
}

//---------------------------------------------------------------------------
void ServerSocket::SetBuffers(uint8_t* pu8In_, K_ADDR kaInSize_, uint8_t* pu8Out_, K_ADDR kaOutSize_)
{
	Control(SOCKET_CONTROL_SET_BUFFERS, pu8In_, kaInSize_, pu8Out_, kaOutSize_);
}

//---------------------------------------------------------------------------
void ServerSocket::SetNotifiers(Notify* pclNotifyIn_, Notify* pclNotifyOut_)
{
	m_pclNotifyServerRead = pclNotifyIn_;
	m_pclNotifyServerWrite = pclNotifyOut_;
}

//---------------------------------------------------------------------------
bool ServerSocket::WaitOnInput()
{
	m_pclNotifyServerRead->Wait(0);
	return m_bOpen;
}

//---------------------------------------------------------------------------
bool ServerSocket::WaitOnOutput()
{
	m_pclNotifyServerWrite->Wait(0);
	return m_bOpen;
}

#if defined(KERNEL_USE_TIMEOUTS)
//---------------------------------------------------------------------------
bool ServerSocket::WaitOnInput(uint32_t u32WaitTimeMS_)
{
	m_pclNotifyServerRead->Wait(u32WaitTimeMS_, 0);
	return m_bOpen;
}

//---------------------------------------------------------------------------
bool ServerSocket::WaitOnOutput(uint32_t u32WaitTimeMS_)
{
	m_pclNotifyServerWrite->Wait(u32WaitTimeMS_, 0);
	return m_bOpen;
}
#endif

//---------------------------------------------------------------------------
void ClientSocket::Init()
{
	m_bBlocking = true;
}

//---------------------------------------------------------------------------
uint8_t ClientSocket::Open()
{
	return static_cast<uint8_t>(m_pclSocket->IsOpen());
}

//---------------------------------------------------------------------------
uint8_t ClientSocket::Close()
{
	m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_CLOSED, 0, 0, 0, 0);
	return 0;
}

//---------------------------------------------------------------------------
uint16_t ClientSocket::Read(uint16_t u16Bytes_, uint8_t* pu8Data_)
{
	return m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_READ,
								0,
								static_cast<uint16_t>(m_bBlocking),
								pu8Data_,
								u16Bytes_);
}

//---------------------------------------------------------------------------
uint16_t ClientSocket::Write(uint16_t u16Bytes_, uint8_t* pu8Data_)
{
	return m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_WRITE,
								pu8Data_,
								u16Bytes_,
								0,
								static_cast<uint16_t>(m_bBlocking));
}

//---------------------------------------------------------------------------
uint16_t ClientSocket::Control(uint16_t u16Event_, void* pvDataIn_, uint16_t  /*u16SizeIn_*/, void*  /*pvDataOut_*/, uint16_t  /*u16SizeOut_*/)
{
	if (!m_pclSocket->IsOpen()) {
        return SOCKET_ERROR;
	}

	switch (u16Event_) {
		case SOCKET_CONTROL_SET_BLOCKING:
			m_bBlocking = true;
			break;
		case SOCKET_CONTROL_SET_NONBLOCKING:
			m_bBlocking = false;
			break;
		case SOCKET_CONTROL_CONNECT_TO_SERVER:
			m_pclSocket = static_cast<ServerSocket*>(pvDataIn_);
		default:
			break;
	}
	if (!m_pclSocket->IsOpen()) {
        return SOCKET_ERROR;
	}
	return 0;
}

//---------------------------------------------------------------------------
void ClientSocket::SetBlocking(bool bBlocking_)
{
	m_bBlocking = bBlocking_;
}

//---------------------------------------------------------------------------
bool ClientSocket::WaitOnInput()
{
	return 0 == m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_READ, 0, 0, 0, 0);
}

//---------------------------------------------------------------------------
bool ClientSocket::WaitOnOutput()
{
	return 0 == m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_WRITE, 0, 0, 0, 0);
}

#if defined(KERNEL_USE_TIMEOUTS)
//---------------------------------------------------------------------------
bool ClientSocket::WaitOnInput(uint32_t u32WaitTimeMS_)
{
	uint32_t u32Time = u32WaitTimeMS_;
	return 0 == m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_READ, &u32Time, 0, 0, 0);
}

//---------------------------------------------------------------------------
bool ClientSocket::WaitOnOutput(uint32_t u32WaitTimeMS_)
{
	uint32_t u32Time = u32WaitTimeMS_;
	return 0 == m_pclSocket->Control(SOCKET_CONTROL_PRIVATE_CLIENT_BLOCK_WRITE, &u32Time, 0, 0, 0);
}

#endif
