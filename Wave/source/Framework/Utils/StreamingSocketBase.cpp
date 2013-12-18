/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************************
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/StreamingSocketBase.h"
#include "errno.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/SerializableObject.h"

namespace WaveNs
{

StreamingSocketBase::StreamingSocketBase (const SI32 maximumNumberOfConnections)
    : m_maximumNumberOfConnections (maximumNumberOfConnections),
      m_toMessageVersion (""),
      m_toSerializationType (SERIALIZE_WITH_ATTRIBUTE_NAME)
{
    m_socket = -1;
}

StreamingSocketBase::StreamingSocketBase ()
{
    m_socket                     = -1;
    m_maximumNumberOfConnections = 1;
    m_toMessageVersion           = "";
    m_toSerializationType        = SERIALIZE_WITH_ATTRIBUTE_NAME;
}

StreamingSocketBase::~StreamingSocketBase ()
{
    if (true == (isValid ()))
    {
        prismCloseSocket (m_socket);
    }
}

bool StreamingSocketBase::isValid ()
{
    if (-1 != m_socket)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool StreamingSocketBase::listen ()
{
    if (true != (isValid ()))
    {
        return (false);
    }

    SI32 status = ::listen (m_socket, m_maximumNumberOfConnections);

    if (-1 == status)
    {
        tracePrintf (TRACE_LEVEL_INFO, "StreamingSocketBase::listen : errno : %d", errno);
        return (false);
    }
    else
    {
        return (true);
    }
}

SI32 StreamingSocketBase::send (const UI32 &value)
{
    if (true != (isValid ()))
    {
        return (-1);
    }

    UI32 networkValue   = htonl (value);
    SI32 sendStatus     = 0;

    sendStatus = send ((UI8 *)(&networkValue), sizeof (networkValue)); //MSG_NOSIGNAL

    if (-1 == sendStatus)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::send (const UI32 &value) : Value : %lu,  errno : %d", value, errno);
    }

    return (sendStatus);
}

SI32 StreamingSocketBase::send (const FixedSizeBuffer &fixedSizeBuffer)
{
    if (true != (isValid ()))
    {
        return (-1);
    }

    UI32  fixedSizeBufferSize = fixedSizeBuffer.getMaximumSize ();
    UI8  *pBuffer             = fixedSizeBuffer.getPRawBuffer ();
    SI32  sendStatus          = 0;

    if (NULL != pBuffer)
    {
        sendStatus = send (fixedSizeBufferSize);
        if (-1 == sendStatus)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::send (const FixedSizeBuffer &fixedSizeBuffer) : errno : %d", errno);
	        return (sendStatus);
        }

        sendStatus = send (pBuffer, fixedSizeBufferSize); //MSG_NOSIGNAL

        if (-1 == sendStatus)
        {
            tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::send (const FixedSizeBuffer &fixedSizeBuffer) : errno : %d", errno);
        }

        return (sendStatus);
    }
    else
    {
        return (-1);
    }
}

SI32 StreamingSocketBase::send (SerializableObject *pSerializableObject)
{
    if (true != (isValid ()))
    {
        return (-1);
    }

    string dataToBeSent;
    UI32   sizeOfDataToBeSent = 0;
    SI32   sentStatus         = -1;

    prismAssert (NULL != pSerializableObject, __FILE__, __LINE__);

    //pSerializableObject->serialize2 (dataToBeSent, m_toMessageVersion);
    pSerializableObject->serialize2 (dataToBeSent, m_toSerializationType);

    sizeOfDataToBeSent = dataToBeSent.size ();

    sentStatus = send (sizeOfDataToBeSent);

    if (-1 == sentStatus)
    {
        return (sentStatus);
    }

    sentStatus = send (dataToBeSent);

    return (sentStatus);
}

SI32 StreamingSocketBase::send(UI8 *pBuffer, const UI32 maximumBufferLength)
{
	if (true != (isValid ()))
	{
		return (-1);
	}
	SI32  sendStatus          = 0;
	UI32  sendLen			  = 0;
	if (NULL != pBuffer)
	{
		while ( sendLen < maximumBufferLength )
		{
			sendStatus = ::send (m_socket, (char *) (pBuffer +sendLen), (maximumBufferLength-sendLen), 0); //MSG_NOSIGNAL

			tracePrintf (TRACE_LEVEL_DEVEL, "StreamingSocketBase::send(UI8 *pBuffer, const UI32 maximumBufferLength)Sen(Status %d, Len %d, Total %d)",sendStatus, sendLen, maximumBufferLength);

			if (-1 == sendStatus)
			{
				return (sendStatus);
			}

			sendLen += sendStatus;

			if ( sendLen < maximumBufferLength ){
				tracePrintf (TRACE_LEVEL_WARN, "StreamingSocketBase::send(UI8 *pBuffer, const UI32 maximumBufferLength) Fragmented Send");
			}


		}
	}
	return (sendLen);
}

SI32 StreamingSocketBase::send (const string &dataString)
{
    if (true != (isValid ()))
    {
        return (-1);
    }

    SI32 sendStatus = 0;

    sendStatus = send ((UI8*)(dataString.data ()), dataString.size ()); //MSG_NOSIGNAL

    if (-1 == sendStatus)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::send (const string &dataString) : errno : %d", errno);
    }

    return (sendStatus);
}

bool StreamingSocketBase::receive (string &dataString)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    char *pBuffer = new char[maximumDataLengthToReceive + 1];
    SI32  status  = ::recv (m_socket, pBuffer, maximumDataLengthToReceive, 0);

    dataString = "";

    if (-1 == status)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::receive (string &dataString) : Status = %d, errno : %d", status, errno);

        delete[] pBuffer;
        return (false);
    }
    else if (0 == status)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, "StreamingSocketBase::receive (string &dataString) : Status = %d, errno : %d", status, errno);

        delete[] pBuffer;
        return (true);
    }
    else
    {
        //pBuffer[status] = '\0';
        dataString.assign (pBuffer, status);

        delete[] pBuffer;
        return (true);
    }
}

SI32 StreamingSocketBase::receive (UI8 *pBuffer, const UI32 maximumBufferLength)
{
    if (true != (isValid ()))
    {
        return (false);
    }

    SI32 status = ::recv (m_socket, (char *) pBuffer, maximumBufferLength, 0);

    if (-1 == status)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::receive (UI8 *pBuffer, const UI32 maximumBufferLength) : Status = %d, errno : %d", status, errno);
    }
    else if (0 == status)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, "StreamingSocketBase::receive (UI8 *pBuffer, const UI32 maximumBufferLength) : Status = %d, errno : %d", status, errno);
    }

    return (status);
}

SI32 StreamingSocketBase::receiveAll (UI8 *pBuffer, const UI32 maximumBufferLength)
{
    if (true != (isValid ()))
    {
        return (0);
    }

    SI32 status = ::recv (m_socket, (char *) pBuffer, maximumBufferLength, 0); //MSG_WAITALL);

    if (-1 == status)
    {
        tracePrintf (TRACE_LEVEL_ERROR, "StreamingSocketBase::receiveAll (UI8 *pBuffer, const UI32 maximumBufferLength) : Status = %d, errno : %d", status, errno);
    }
    else if (0 == status)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, "StreamingSocketBase::receiveAll (UI8 *pBuffer, const UI32 maximumBufferLength) : Status = %d, errno : %d", status, errno);
    }

    return (status);
}

bool StreamingSocketBase::receiveUI32 (UI32 &value)
{
    UI8 *pTempBuffer                  = reinterpret_cast<UI8 *> (&value);
    UI32 numberOfBytesRemainingToRead = sizeof (value);

    while (numberOfBytesRemainingToRead > 0)
    {
        SI32 numberOfBytesRead = receiveAll (pTempBuffer, numberOfBytesRemainingToRead);

        if (numberOfBytesRead > 0)
        {
            numberOfBytesRemainingToRead -= numberOfBytesRead;
            pTempBuffer                  += numberOfBytesRead;
        }
        else
        {
            //cout << "numberOfBytesRemainingToRead : " << numberOfBytesRemainingToRead << ", numberOfBytesRead : " << numberOfBytesRead << " : " << errno << endl;

            return (false);
        }
    }

    value = ntohl (value);

    return (true);
}

void StreamingSocketBase::setSocket (const SI32 &socket)
{
    m_socket = socket;
}

SI32 StreamingSocketBase::getSocket () const
{
    return (m_socket);
}

void StreamingSocketBase::setMessageVersionAndSerializationType (const string& msgVersion, const UI8& serializationType)
{
    m_toMessageVersion      = msgVersion;
    m_toSerializationType   = serializationType;
}

string StreamingSocketBase::getMessageVersion () const
{
    return (m_toMessageVersion);
}

UI8 StreamingSocketBase::getSerializationType () const
{
    return (m_toSerializationType);
}


}
