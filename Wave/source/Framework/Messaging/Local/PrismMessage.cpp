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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#include "PrismMessage.h"
#include "Framework/Utils/PrismCondition.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/PrismMutex.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Messaging/MessageFactory/PrismMessageFactory.h"
#include "Framework/Messaging/MessageTracker/MessageTracker.h"
#include "Framework/Utils/MemoryUtils.h"
#include "Framework/MultiThreading/PrismThread.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace WaveNs
{

static UI32      s_numberOfMessagesInTheSystemSoFar = 0;
static UI32      s_numberOfMessagesDeletedInTheSystemSoFar = 0;
static PrismMutex *pMessageCreationMutex              = NULL;

PrismMessage::PrismMessageBuffer::PrismMessageBuffer (UI32 size, const void *pBuffer, bool ownedByMessage)
    : m_size (size),
      m_pBuffer ((void *) pBuffer)
{
    if (false == ownedByMessage)
    {
        m_pBuffer = new UI8[size];
        memcpy (m_pBuffer, pBuffer, size);
    }
}

PrismMessage::PrismMessageBuffer::PrismMessageBuffer (const PrismMessage::PrismMessageBuffer &prismMessagebuffer)
{
    m_size    = prismMessagebuffer.m_size;

    m_pBuffer = new UI8[m_size];
    memcpy (m_pBuffer, prismMessagebuffer.m_pBuffer, m_size);
}

PrismMessage::PrismMessageBuffer::~PrismMessageBuffer ()
{
    destroy ();
}

PrismMessage::PrismMessageBuffer &PrismMessage::PrismMessageBuffer::operator = (const PrismMessage::PrismMessageBuffer &prismMessagebuffer)
{
    destroy ();

    m_size    = prismMessagebuffer.m_size;

    m_pBuffer = new UI8[m_size];
    memcpy (m_pBuffer, prismMessagebuffer.m_pBuffer, m_size);

    return (*this);
}

void PrismMessage::PrismMessageBuffer::destroy ()
{
    m_size = 0;

    if (NULL != m_pBuffer)
    {
        delete[] (reinterpret_cast<UI8 *> (m_pBuffer));
        m_pBuffer = NULL;
    }
}

void PrismMessage::PrismMessageBuffer::invalidateBuffer ()
{
    m_size    = 0;
    m_pBuffer = NULL;
}

UI32 PrismMessage::PrismMessageBuffer::getSize () const
{
    return (m_size);
}

void *PrismMessage::PrismMessageBuffer::getPBuffer () const
{
    return (m_pBuffer);
}

void PrismMessage::PrismMessageBuffer::setPBuffer (void *pBuffer)
{
    m_pBuffer = pBuffer;
}

PrismMessage::PrismMessage (PrismServiceId serviceCode, UI32 operationCode)
    : m_serviceCode                             (serviceCode),
      m_operationCode                           (operationCode),
      m_senderLocationId                        (FrameworkToolKit::getThisLocationId ()),
      m_receiverLocationId                      (FrameworkToolKit::getThisLocationId ()),
      m_isOneWayMessage                         (false),
      m_isSynchronousMessage                    (false),
      m_pSynchronizingMutex                     (NULL),
      m_pSynchronizingCondition                 (NULL),
      m_isLastReply                             (true),
      m_isACopy                                 (false),
      m_completionStatus                        (WAVE_MESSAGE_ERROR),
      m_waveClientOriginatingLocationId         (0),
      m_waveNativeClientId                      (0),
      m_waveUserClientId                        (0),
      m_dropReplyAcrossLocations                (false),
      m_isConfigurationChanged                  (false),
      m_isConfigurationFlagSetByUser            (false),
      m_nestedSql                               (""),
      m_transactionCounter                      (0),
      m_surrogatingForLocationId                (0),
      m_needSurrogateSupportFlag                (false),
      m_isMessageBeingSurrogatedFlag            (false),
      m_isMessageSupportedWhenServiceIsPaused   (false),
      m_partitionLocationIdForPropagation       (0),
      m_isPartitionContextPropagated            (false),
      m_isPartitionNameSetByUser                (false),
      m_isConfigurationTimeChanged              (false),
      m_isAConfigurationIntent                  (false),
      m_isConfigurationIntentStored             (false),
      m_isALastConfigReplay                     (false),
      m_timeOutInMilliSeconds                   (600000),
      m_disconnectFromNodeAfterReply            (false),
      m_removeNodeFromKnownLocationAfterReply   (false),
      m_sendForOneWayConnection                 (false)
{
    setIsPreparedForSerialization (false);

    if (NULL == pMessageCreationMutex)
    {
        pMessageCreationMutex = new PrismMutex ();

        prismAssert (NULL != pMessageCreationMutex, __FILE__, __LINE__);
    }

    pMessageCreationMutex->lock ();
    s_numberOfMessagesInTheSystemSoFar++;
    m_waveClientMessageId = m_originalMessageId = m_messageIdAtOriginatingLocation = m_messageId = s_numberOfMessagesInTheSystemSoFar;
    pMessageCreationMutex->unlock ();

    m_type     = WAVE_MESSAGE_TYPE_REQUEST;
    m_priority = WAVE_MESSAGE_PRIORITY_NORMAL;

    MessageTracker::addToMessageTracker (this);

    m_prismMessageCreatorThreadId = PrismThread::getSelf ();
}

PrismMessage::PrismMessage (const PrismMessage &prismMessage)
{
    trace (TRACE_LEVEL_FATAL, "PrismMessage::PrismMessage : Copy Constructing a PrismMessage does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);
}

PrismMessage::~PrismMessage ()
{
    removeAllBuffers ();

    MessageTracker::deleteFromMessageTracker (this);

    pMessageCreationMutex->lock ();

    s_numberOfMessagesDeletedInTheSystemSoFar++;

    if (0 == (s_numberOfMessagesDeletedInTheSystemSoFar % 10000))
    {
        MemoryUtils::releaseUnusedMemoryToSystem ();
    }

    pMessageCreationMutex->unlock ();
}

PrismMessage &PrismMessage::operator = (const PrismMessage &prismMessage)
{
    trace (TRACE_LEVEL_FATAL, "PrismMessage::operator = : Assigning to a PrismMessage does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);

    return (*this);
}

WaveMessageType PrismMessage::getType () const
{
    return (static_cast<WaveMessageType> (m_type));
}

void PrismMessage::setType (WaveMessageType type)
{
    m_type = type;
}

WaveMessagePriority PrismMessage::getPriority () const
{
    return (static_cast<WaveMessagePriority> (m_priority));
}

void PrismMessage::setPriority (WaveMessagePriority priority)
{
    m_priority = priority;
}

PrismServiceId PrismMessage::getServiceCode () const
{
    return (m_serviceCode);
}

void PrismMessage::setServiceCode (const PrismServiceId &serviceCode)
{
    m_serviceCode = serviceCode;
}

UI32 PrismMessage::getOperationCode () const
{
    return (m_operationCode);
}

PrismServiceId PrismMessage::getSenderServiceCode () const
{
    return (m_senderServiceCode);
}

UI32 PrismMessage::getMessageId () const
{
    return (m_messageId);
}

UI32 PrismMessage::getMessageIdAtOriginatingLocation () const
{
    return (m_messageIdAtOriginatingLocation);
}

bool PrismMessage::getIsOneWayMessage () const
{
    return (m_isOneWayMessage);
}

void PrismMessage::setIsOneWayMessage (bool isOneWayMessage)
{
    m_isOneWayMessage = isOneWayMessage;
}

bool PrismMessage::getIsSynchronousMessage () const
{
    return (m_isSynchronousMessage);
}

void PrismMessage::setIsSynchronousMessage (bool isSynchronousMessage)
{
    m_isSynchronousMessage = isSynchronousMessage;
}

ResourceId PrismMessage::getCompletionStatus () const
{
    return (m_completionStatus);
}

PrismMutex *PrismMessage::getPSynchronizingMutex () const
{
    return (m_pSynchronizingMutex);
}

void PrismMessage::setPSynchronizingMutex (PrismMutex *pSynchronizingMutex)
{
    m_pSynchronizingMutex = pSynchronizingMutex;
}

void PrismMessage::setCompletionStatus (const ResourceId completionStatus)
{
    m_completionStatus = completionStatus;
}

PrismCondition *PrismMessage::getPSynchronizingCondition () const
{
    return (m_pSynchronizingCondition);
}

void PrismMessage::setPSynchronizingCondition (PrismCondition *pPrismCondition)
{
    m_pSynchronizingCondition = pPrismCondition;
}

LocationId PrismMessage::getSenderLocationId () const
{
    return (m_senderLocationId);
}

void PrismMessage::setSenderLocationId (const LocationId &locationId)
{
    m_senderLocationId = locationId;
}

LocationId PrismMessage::getReceiverLocationId () const
{
    return (m_receiverLocationId);
}

bool PrismMessage::getIsLastReply () const
{
    return (m_isLastReply);
}

void PrismMessage::setIsLastReply (bool isLastReply)
{
    m_isLastReply = isLastReply;
}

WaveMessageStatus PrismMessage::addBuffer (UI32 tag, UI32 size, const void *pBuffer, bool bufferWillBeOwnedByMessage)
{
    UI32  tempSize;
    void *pTempBuffer = NULL;

    if (NULL == pBuffer)
    {
        cout << "PrismMessage::addBuffer : Cannot add NULL buffer to a PrismMessage." << endl;
        prismAssert (false, __FILE__, __LINE__);

        return (WAVE_MESSAGE_ERROR_ADDING_NULL_BUFFER);
    }

    pTempBuffer = findBuffer (tag, tempSize);

    if (NULL != pTempBuffer)
    {
        cout << "PrismMessage::addBuffer : A buffer already exists with the tag (" << tag << ")." << endl;
        prismAssert (false, __FILE__, __LINE__);

        return (WAVE_MESSAGE_ERROR_BUFFER_WITH_TAG_EXISTS);
    }
    else
    {
        PrismMessageBuffer *pPrismMessageBuffer = new PrismMessageBuffer (size, pBuffer, bufferWillBeOwnedByMessage);

        m_buffers[tag] = pPrismMessageBuffer;
        return (WAVE_MESSAGE_SUCCESS);
    }
}

void *PrismMessage::findBuffer (UI32 tag, UI32 &size) const
{
    map<UI32, PrismMessageBuffer *>::const_iterator limitingElement = m_buffers.end ();
    map<UI32, PrismMessageBuffer *>::const_iterator element         = m_buffers.find (tag);

    if (limitingElement != element)
    {
        PrismMessageBuffer *pPrismMessageBuffer = element->second;

        size = pPrismMessageBuffer->getSize ();

        return (pPrismMessageBuffer->getPBuffer ());
    }
    else
    {
        size = 0;

        return (NULL);
    }
}

void *PrismMessage::transferBufferToUser (UI32 tag, UI32 &size)
{
    map<UI32, PrismMessageBuffer *>::iterator limitingElement = m_buffers.end ();
    map<UI32, PrismMessageBuffer *>::iterator element         = m_buffers.find (tag);

    if (limitingElement != element)
    {
        PrismMessageBuffer *pPrismMessageBuffer = element->second;
        void             *pTempBuffer       = pPrismMessageBuffer->getPBuffer ();

        m_buffers.erase (element);
        size = pPrismMessageBuffer->getSize ();
        pPrismMessageBuffer->invalidateBuffer ();
        delete pPrismMessageBuffer;

        return (pTempBuffer);
    }
    else
    {
        size = 0;

        return (NULL);
    }
}

WaveMessageStatus PrismMessage::removeBuffer (UI32 tag)
{
    UI32  tempSize;
    void *pTempBuffer = NULL;

    pTempBuffer = findBuffer (tag, tempSize);

    if (NULL == pTempBuffer)
    {
        cout << "PrismMessage::addBuffer : A buffer does not exist with the tag (" << tag << ")." << endl;
        prismAssert (false, __FILE__, __LINE__);

        return (WAVE_MESSAGE_ERROR_BUFFER_WITH_TAG_DOES_NOT_EXIST);
    }
    else
    {
        map<UI32, PrismMessageBuffer *>::iterator element = m_buffers.find (tag);

        PrismMessageBuffer *pPrismMessageBuffer = element->second;
        m_buffers.erase (element);
        delete pPrismMessageBuffer;

        return (WAVE_MESSAGE_SUCCESS);
    }
}

void PrismMessage::setupAttributesForSerializationInAttributeOrderFormat ()
{   
    addAttributeNameForOrderToNameMapping ("waveMessageType");
    addAttributeNameForOrderToNameMapping ("messagePriority");
    addAttributeNameForOrderToNameMapping ("messageIdAtOriginatingLocation");
    addAttributeNameForOrderToNameMapping ("serviceId");
    addAttributeNameForOrderToNameMapping ("operationCode");
    addAttributeNameForOrderToNameMapping ("waveClientMessageId");
    addAttributeNameForOrderToNameMapping ("senderLocationId");
    addAttributeNameForOrderToNameMapping ("receiverLocationId");
    addAttributeNameForOrderToNameMapping ("isOneWayMessage");
    addAttributeNameForOrderToNameMapping ("isACopy");
    addAttributeNameForOrderToNameMapping ("originalMessageId");
    addAttributeNameForOrderToNameMapping ("isLastReply");
    addAttributeNameForOrderToNameMapping ("completionStatus");
    addAttributeNameForOrderToNameMapping ("waveClientOriginatingLocationId");
    addAttributeNameForOrderToNameMapping ("waveNativeClientId");
    addAttributeNameForOrderToNameMapping ("waveUserClientId");
    addAttributeNameForOrderToNameMapping ("messageString");
    addAttributeNameForOrderToNameMapping ("isConfigurationChanged");
    addAttributeNameForOrderToNameMapping ("isConfigurationFlagSetByUser");
    addAttributeNameForOrderToNameMapping ("nestedSql");
    addAttributeNameForOrderToNameMapping ("transactionCounter");
    addAttributeNameForOrderToNameMapping ("surrogatingForLocationId");
    addAttributeNameForOrderToNameMapping ("needSurrogateSupportFlag");
    addAttributeNameForOrderToNameMapping ("isMessageBeingSurrogatedFlag");
    addAttributeNameForOrderToNameMapping ("locationsForStatusPropagation");
    addAttributeNameForOrderToNameMapping ("completionStatusForStatusPropagation");
    addAttributeNameForOrderToNameMapping ("localizedCompletionStatusForStatusPropagation");
    addAttributeNameForOrderToNameMapping ("isMessageSupportedWhenServiceIsPaused");
    addAttributeNameForOrderToNameMapping ("xPathStringsVectorForTimestampUpdate");
}

void PrismMessage::setupAttributesForSerialization ()
{
    // Currently there is nothing that happens in the SerializableObject::setupAttributesForSerialization ()
    // method.  So we do not need to call it.  If there is somethign at a later time, the following line
    // must be uncommented.

    //SerializableObject::setupAttributesForSerialization ();

    // WARNING !!! WARNING !!! WARNING

    // The order below must not change.  If it is changed, without making corresponding changes to remote messaging
    // related methods, remote messaing will be utterly broken.  The methods that depend on the order are
    //     getType
    //     getMessageIdAtOriginatingLocation
    //     createAndLoadFromSerializedData2

    addSerializableAttribute (new AttributeWaveMessageType      (&m_type,                                           "waveMessageType"));
    addSerializableAttribute (new AttributeWaveMessagePriority  (&m_priority,                                       "messagePriority"));
    addSerializableAttribute (new AttributeUI32                 (&m_messageIdAtOriginatingLocation,                 "messageIdAtOriginatingLocation"));
    addSerializableAttribute (new AttributePrismServiceId       (&m_serviceCode,                                    "serviceId"));
    addSerializableAttribute (new AttributeUI32                 (&m_operationCode,                                  "operationCode"));
    addSerializableAttribute (new AttributeUI32                 (&m_waveClientMessageId,                            "waveClientMessageId"));
    addSerializableAttribute (new AttributeLocationId           (&m_senderLocationId,                               "senderLocationId"));
    addSerializableAttribute (new AttributeLocationId           (&m_receiverLocationId,                             "receiverLocationId"));
    addSerializableAttribute (new AttributeBool                 (&m_isOneWayMessage,                                "isOneWayMessage"));
    addSerializableAttribute (new AttributeBool                 (&m_isACopy,                                        "isACopy"));
    addSerializableAttribute (new AttributeUI32                 (&m_originalMessageId,                              "originalMessageId"));
    addSerializableAttribute (new AttributeBool                 (&m_isLastReply,                                    "isLastReply"));
    addSerializableAttribute (new AttributeResourceId           (&m_completionStatus,                               "completionStatus"));
    addSerializableAttribute (new AttributeLocationId           (&m_waveClientOriginatingLocationId,                "waveClientOriginatingLocationId"));
    addSerializableAttribute (new AttributeUI32                 (&m_waveNativeClientId,                             "waveNativeClientId"));
    addSerializableAttribute (new AttributeUI32                 (&m_waveUserClientId,                               "waveUserClientId"));
    addSerializableAttribute (new AttributeString               (&m_messageString,                                  "messageString"));
    addSerializableAttribute (new AttributeBool                 (&m_isConfigurationChanged,                         "isConfigurationChanged"));
    addSerializableAttribute (new AttributeBool                 (&m_isConfigurationFlagSetByUser,                   "isConfigurationFlagSetByUser"));
    addSerializableAttribute (new AttributeString               (&m_nestedSql,                                      "nestedSql"));
    addSerializableAttribute (new AttributeUI32                 (&m_transactionCounter,                             "transactionCounter"));
    addSerializableAttribute (new AttributeLocationId           (&m_surrogatingForLocationId,                       "surrogatingForLocationId"));
    addSerializableAttribute (new AttributeBool                 (&m_needSurrogateSupportFlag,                       "needSurrogateSupportFlag"));
    addSerializableAttribute (new AttributeBool                 (&m_isMessageBeingSurrogatedFlag,                   "isMessageBeingSurrogatedFlag"));
    addSerializableAttribute (new AttributeLocationIdVector     (&m_locationsForStatusPropagation,                  "locationsForStatusPropagation"));
    addSerializableAttribute (new AttributeResourceIdVector     (&m_completionStatusForStatusPropagation,           "completionStatusForStatusPropagation"));
    addSerializableAttribute (new AttributeStringVector         (&m_localizedCompletionStatusForStatusPropagation,  "localizedCompletionStatusForStatusPropagation"));
    addSerializableAttribute (new AttributeBool                 (&m_isMessageSupportedWhenServiceIsPaused,          "isMessageSupportedWhenServiceIsPaused"));
    addSerializableAttribute (new AttributeStringVector         (&m_xPathStringsVectorForTimestampUpdate,           "xPathStringsVectorForTimestampUpdate"));

    // Multi-Partition
    addSerializableAttribute (new AttributeString               (&m_partitionName,                                  "wavePartitionName"));
    addSerializableAttribute (new AttributeLocationId           (&m_partitionLocationIdForPropagation,              "partitionLocationIdForPropagation"));
    addSerializableAttribute (new AttributeBool                 (&m_isPartitionContextPropagated,                   "isPartitionContextPropagated"));

    addSerializableAttribute (new AttributeBool                 (&m_isConfigurationTimeChanged,                     "isConfigurationTimeChanged"));

    addSerializableAttribute (new AttributeBool                 (&m_isALastConfigReplay,                            "isALastConfigReplay")); 
}

void PrismMessage::copyFromRemoteResponse (PrismMessage *pRemotePrismMessageResponse)
{
#if 0
//    string serializedData;

//    pRemotePrismMessageResponse->serialize (serializedData);

//    loadFromSerializedData (serializedData);
#else
    // Locd the values from the remote response.

    loadFromSerializableObject (pRemotePrismMessageResponse);
#endif
    // Now handle the buffers.
    // Remove all the exisiting buffers and transfer the buffers from the remote messge response to this message.

    removeAllBuffers ();
    pRemotePrismMessageResponse->transferAllBuffers (this);
}

UI32 PrismMessage::getNumberOfBuffers () const
{
    return (m_buffers.size ());
}

void PrismMessage::getBufferTags (vector<UI32> &bufferTagsVector) const
{
    map<UI32, PrismMessageBuffer *>::const_iterator limitingElement = m_buffers.end ();
    map<UI32, PrismMessageBuffer *>::const_iterator element         = m_buffers.begin ();

    while (element != limitingElement)
    {
        bufferTagsVector.push_back (element->first);

        element++;
    }
}

void PrismMessage::removeAllBuffers ()
{
    map<UI32, PrismMessageBuffer *>::iterator  element           = m_buffers.begin () ;
    map<UI32, PrismMessageBuffer *>::iterator  limitingElement   = m_buffers.end ();
    PrismMessageBuffer                        *pPrismMessageBuffer = NULL;

    while (element != limitingElement)
    {
        pPrismMessageBuffer = element->second;
        delete pPrismMessageBuffer;

        element++;
    }

    m_buffers.clear ();
}

void PrismMessage::transferAllBuffers (PrismMessage *pPrismMessage)
{
    if (this == pPrismMessage)
    {
        prismAssert (false, __FILE__, __LINE__);
        return;
    }

    map<UI32, PrismMessageBuffer *>::iterator  element           = m_buffers.begin () ;
    map<UI32, PrismMessageBuffer *>::iterator  limitingElement   = m_buffers.end ();
    UI32                                     bufferTag;
    PrismMessageBuffer                        *pPrismMessageBuffer = NULL;

    while (element != limitingElement)
    {
        bufferTag         = element->first;
        pPrismMessageBuffer = element->second;

        pPrismMessage->addBuffer (bufferTag, pPrismMessageBuffer->getSize (), pPrismMessageBuffer->getPBuffer (), true);

        pPrismMessageBuffer->setPBuffer (NULL);
        delete pPrismMessageBuffer;

        element++;
    }

    m_buffers.clear ();
}

void PrismMessage::setDropReplyAcrossLocations (const bool &dropReplyAcrossLocations)
{
    m_dropReplyAcrossLocations = dropReplyAcrossLocations;
}

bool PrismMessage::getDropReplyAcrossLocations () const
{
    return (m_dropReplyAcrossLocations);
}

WaveMessageType PrismMessage::getType (const string &serializedData, const UI8 serializationType)  //const string& messageVersion)
{
    // We now the SerializableObjectAttributeId for Message Type is always 1. (This cannot be changed from 1)
    // So the corresponding attribute in the serialized XML will be tagged with A1.  So look for the data
    // tagged between <A1> and </A1>

    SI32 index1            = 0;
    SI32 index2            = 0;
    string attributeTag    = ""; 

    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {
        attributeTag = string ("1");
    }
    else
    {
        attributeTag = string ("waveMessageType");
    }
    UI32 startIndexOffset = attributeTag.size () + strlen ("<A>");

    index1    = serializedData.find (string ("<A") + attributeTag + string (">"));
    index2    = serializedData.find (string ("</A") + attributeTag + string (">"));    
    
    string sotString = serializedData.substr (index1 + startIndexOffset, index2 - index1 - startIndexOffset);
    //UI32   sot       = strtoul (sotString.c_str (), NULL, 10);
    //
    //return ((WaveMessageType) sot);

    WaveMessageType waveMessageType = WAVE_MESSAGE_TYPE_REQUEST;

    AttributeResourceId attributeResourceId (waveMessageType);
    attributeResourceId.fromString (sotString);

    WaveMessageType *pWaveMessageType = (WaveMessageType *) attributeResourceId.getPData ();

    return (*pWaveMessageType);
}

UI32 PrismMessage::getMessageIdAtOriginatingLocation (const string &serializedData, const UI8 serializationType) 
{
    // We now the SerializableObjectAttributeId for Message Id at Originating Location is always 3. (This cannot be changed from 3)
    // So the corresponding attribute in the serialized XML will be tagged with A3.  So look for the data
    // tagged between <A3> and </A3>

    string  attributeTag        = "";
    SI32    index1              = 0;
    SI32    index2              = 0;
    
    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {
        attributeTag = string ("3");
    }
    else
    {
        attributeTag = string ("messageIdAtOriginatingLocation");
    }
    UI32 startIndexOffset = attributeTag.size () + strlen ("<A>");

    index1    = serializedData.find (string ("<A") + attributeTag + string (">"));
    index2    = serializedData.find (string ("</A") + attributeTag + string (">"));

    string sotString = serializedData.substr (index1 + startIndexOffset, index2 - index1 - startIndexOffset);
    UI32   sot       = strtoul (sotString.c_str (), NULL, 10);

    return (sot);
}

UI32 PrismMessage::getWaveClientMessageId (const string &serializedData, const UI8 serializationType)  
{
    // We now the SerializableObjectAttributeId for Message Id at WaveClient is always 6. (This cannot be changed from 6)
    // So the corresponding attribute in the serialized XML will be tagged with A6.  So look for the data
    // tagged between <A6> and </A6>

    string  attributeTag        = "";
    SI32    index1              = 0;
    SI32    index2              = 0;

    
    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {   
        attributeTag = string ("6");
    }
    else
    {   
        attributeTag = string ("waveClientMessageId");
    }
    UI32 startIndexOffset = attributeTag.size () + strlen ("<A>");

    index1    = serializedData.find (string ("<A") + attributeTag + string (">"));
    index2    = serializedData.find (string ("</A") + attributeTag + string (">"));

    string sotString = serializedData.substr (index1 + startIndexOffset, index2 - index1 - startIndexOffset);
    UI32   sot       = strtoul (sotString.c_str (), NULL, 10);

    return (sot);
}

UI32 PrismMessage::getMessageCompletionStatus (const string &serializedData, const UI8 serializationType)  
{
// Now Completion status of a message is at position 13 (This can not be changed form 13 now). 
// So the corresponding attribute in the serialized XML will be tagged with A13. So look for the data
// tagged between <A13> and </A13>

    UI32    startIndexOffset    = 0;
    SI32    index1              = 0;
    SI32    index2              = 0;
    string  attributeTag        = "";

    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {   
        attributeTag = string ("13");
    }
    else
    {
        attributeTag = string ("completionStatus");
    }
    startIndexOffset = attributeTag.size () + strlen ("<A>");
    
    index1    = serializedData.find (string ("<A") + attributeTag + string (">"));
    index2    = serializedData.find (string ("</A") + attributeTag + string (">"));

    string sotString = serializedData.substr (index1 + startIndexOffset, index2 - index1 - startIndexOffset);

    ResourceId  resourceId = WAVE_MESSAGE_SUCCESS;

    AttributeResourceId attributeResourceId (resourceId);
    attributeResourceId.fromString (sotString);

    ResourceId *pResourceId = (ResourceId *) attributeResourceId.getPData ();

    return (*pResourceId);
}

bool PrismMessage::getIsLastReply (const string &serializedData, const UI8 serializationType)  
{
    SI32 index1            = 0; 
    SI32 index2            = 0; 
    string attributeTag    = "";

    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {    
        attributeTag = string ("12");
    }    
    else 
    {    
        attributeTag = string ("isLastReply");
    }    
    UI32 startIndexOffset = attributeTag.size () + strlen ("<A>");

    index1    = serializedData.find (string ("<A") + attributeTag + string (">"));
    index2    = serializedData.find (string ("</A") + attributeTag + string (">"));

    string sotString = serializedData.substr (index1 + startIndexOffset, index2 - index1 - startIndexOffset);

    if (0 == sotString.compare("true"))
    {    
        return(true);
    }    

    return (false);
}  

PrismMessage *PrismMessage::createAndLoadFromSerializedData2 (const string &serializedData, const PrismServiceId &assumedServiceCode, const UI8 serializationType) 
{
    // A4 attribute corresponds to the service code and A5 attribute corresponds to the operation code.
    // We strictly depend on these numbers.  If the order in the setupAttributesForSerialization in PrismMessage
    // changes then these should be adjusted accordingly.

    string  ServiceCodeAttributeTag     = "";
    string  OperationCodeAttributeTag   = "";
    SI32    startIndexForServiceCode    = 0;
    SI32    endIndexForServiceCode      = 0;
    SI32    startIndexForOperationCode  = 0;
    SI32    endIndexForOperationCode    = 0;

    if (SERIALIZE_WITH_ATTRIBUTE_ORDER == serializationType)
    {
        ServiceCodeAttributeTag   = string ("4");
        OperationCodeAttributeTag = string ("5");
    }
    else
    {
        ServiceCodeAttributeTag   = string ("serviceId");
        OperationCodeAttributeTag = string ("operationCode");
    }

    startIndexForServiceCode    = serializedData.find (string ("<A") + ServiceCodeAttributeTag + string (">"));
    endIndexForServiceCode      = serializedData.find (string ("</A") + ServiceCodeAttributeTag + string (">"));
    UI32 startIndexOffsetForServiceCode = ServiceCodeAttributeTag.size () + strlen ("<A>");

    startIndexForOperationCode  = serializedData.find (string ("<A") + OperationCodeAttributeTag + string (">"));
    endIndexForOperationCode    = serializedData.find (string ("</A") + OperationCodeAttributeTag + string (">"));
    UI32 startIndexOffsetForOperationCode = OperationCodeAttributeTag.size () + strlen ("<A>");

    string serviceCodeString    = serializedData.substr (startIndexForServiceCode + startIndexOffsetForServiceCode, endIndexForServiceCode - startIndexForServiceCode - startIndexOffsetForServiceCode);
    UI32   serviceId            = strtoul (serviceCodeString.c_str (), NULL, 10);
    string operationCodeString  = serializedData.substr (startIndexForOperationCode + startIndexOffsetForOperationCode, endIndexForOperationCode - startIndexForOperationCode - startIndexOffsetForOperationCode);
    UI32   operationCode        = strtoul (operationCodeString.c_str (), NULL, 10);

    if (0 != assumedServiceCode)
    {
        serviceId = assumedServiceCode;
    }

    PrismMessage *pPrismMessage = PrismMessageFactory::getMessageInstance (serviceId, operationCode);

    prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);

    if (NULL == pPrismMessage)
    {
        return (NULL);
    }

    // Prepare for serialization and load the attributes from the serialized data.

    pPrismMessage->prepareForSerialization ();
    pPrismMessage->loadFromSerializedData2 (serializedData, serializationType); 

    return (pPrismMessage);
}

PrismMessage *PrismMessage::createAndLoadFromSerializedData2 (const UI8 *pData, const UI32 dataSize)
{
    prismAssert (NULL != pData, __FILE__, __LINE__);
    prismAssert (0 != dataSize, __FILE__, __LINE__);

    if ((NULL == pData) || (0 == dataSize))
    {
        return (NULL);
    }

    char   *pTempString     = new char[dataSize + 1];
    string  serializedData;

    prismAssert (NULL != pTempString, __FILE__, __LINE__);

    if (NULL == pTempString)
    {
        return (NULL);
    }

    strncpy (pTempString, (const char *) pData, dataSize);
    pTempString[dataSize] = '\0';
    serializedData = pTempString;
    delete[] pTempString;

    return (createAndLoadFromSerializedData2 (serializedData));
}

void PrismMessage::setWaveClientOriginatingLocationId (const LocationId &waveClientOriginatingLocationId)
{
    m_waveClientOriginatingLocationId = waveClientOriginatingLocationId;
}

LocationId PrismMessage::getWaveClientOriginatingLocationId () const
{
    return (m_waveClientOriginatingLocationId);
}

void PrismMessage::setWaveNativeClientId (const UI32 &waveNativeClientId)
{
    m_waveNativeClientId = waveNativeClientId;
}

UI32 PrismMessage::getWaveNativeClientId () const
{
    return (m_waveNativeClientId);
}

void PrismMessage::setWaveUserClientId (const UI32 &waveUserClientId)
{
    m_waveUserClientId = waveUserClientId;
}

UI32 PrismMessage::getWaveUserClientId () const
{
    return (m_waveUserClientId);
}

WaveClientSessionContext PrismMessage::getWaveClientSessionContext () const
{
    WaveClientSessionContext waveClientSessionContext;

    waveClientSessionContext.setWaveClientOriginatingLocationId (m_waveClientOriginatingLocationId);
    waveClientSessionContext.setWaveNativeClientId              (m_waveNativeClientId);
    waveClientSessionContext.setWaveUserClientId                (m_waveUserClientId);

    return (waveClientSessionContext);
}

void PrismMessage::setMessageIdAtOriginatingLocation  (const UI32 &messageIdAtOriginatingLocation)
{
    m_messageIdAtOriginatingLocation = messageIdAtOriginatingLocation;
}

UI32 PrismMessage::getOriginalMessageId () const
{
    return (m_originalMessageId);
}

void PrismMessage::setOriginalMessageId (const UI32 &originalMessageId)
{
    m_originalMessageId = originalMessageId;
}

void PrismMessage::copyBuffersFrom (const PrismMessage &prismMessage)
{
    vector<UI32> bufferTagsVector;
    UI32         numberOfBuffers   = 0;
    UI32         i                 = 0;

    prismMessage.getBufferTags (bufferTagsVector);

    numberOfBuffers = bufferTagsVector.size ();

    for (i = 0; i < numberOfBuffers; i++)
    {
        void              *pBuffer    = NULL;
        UI32               bufferSize = 0;
        WaveMessageStatus  status     = WAVE_MESSAGE_ERROR;

        pBuffer = prismMessage.findBuffer (bufferTagsVector[i], bufferSize);

        prismAssert (NULL != pBuffer, __FILE__, __LINE__);
        prismAssert (0 != bufferSize, __FILE__, __LINE__);

        status = addBuffer (bufferTagsVector[i], bufferSize, pBuffer, false);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_FATAL, "PrismMessage::copyBuffersFrom : Copying a Buffer Failed.  Status : " + FrameworkToolKit::localize (status));
            prismAssert (false, __FILE__, __LINE__);
        }
    }
}

PrismMessage *PrismMessage::clone ()
{
    PrismMessage *pClonedPrismMessage            = PrismMessageFactory::getMessageInstance (m_serviceCode, m_operationCode);
    string        serializedData;
    UI32          messageIdAtOriginatingLocation = 0;
    UI32          originalMessageId              = 0;
    UI32          waveClientMessageId            = 0;

    prismAssert (NULL != pClonedPrismMessage, __FILE__, __LINE__);

    if (NULL == pClonedPrismMessage)
    {
        return (NULL);
    }

    // Prepare for serialization on this message.

    prepareForSerialization ();

    // Prepare for serialization and load the attributes from this message into the cloned message.

    messageIdAtOriginatingLocation = pClonedPrismMessage->getMessageIdAtOriginatingLocation ();
    originalMessageId              = pClonedPrismMessage->getOriginalMessageId ();
    waveClientMessageId            = pClonedPrismMessage->getWaveClientMessageId ();

    pClonedPrismMessage->prepareForSerialization ();
    pClonedPrismMessage->loadFromSerializableObject (this);

    pClonedPrismMessage->setMessageIdAtOriginatingLocation (messageIdAtOriginatingLocation);
    pClonedPrismMessage->setOriginalMessageId              (originalMessageId);
    pClonedPrismMessage->setWaveClientMessageId            (waveClientMessageId);

    pClonedPrismMessage->m_isACopy                         = true;


    pClonedPrismMessage->copyBuffersFrom (*this);

    return (pClonedPrismMessage);
}

void PrismMessage::setMessageId (const UI32 &messageId)
{
    m_messageId = messageId;
}


void PrismMessage::setSenderServiceCode(const WaveNs::PrismServiceId& senderServiceCode)
{
    m_senderServiceCode = senderServiceCode;
}

string PrismMessage::getMessageString()
{
    return m_messageString;
}

void PrismMessage::setMessageString(string messageString)
{
    m_messageString = messageString;
}

bool PrismMessage::getIsConfigurationChange () const
{
    return (m_isConfigurationChanged);
}

void PrismMessage::setIsConfigurationChange (const bool &isConfigurationChanged)
{
    m_isConfigurationChanged = isConfigurationChanged;
    m_isConfigurationFlagSetByUser = true;
}

bool PrismMessage::getIsConfigurationTimeChange() const
{
    return (m_isConfigurationTimeChanged);
}

void PrismMessage::setIsConfigurationTimeChange(const bool &isConfigurationTimeChanged)
{
    m_isConfigurationTimeChanged = isConfigurationTimeChanged;
}

bool PrismMessage::getIsConfigurationFlagSetByUser () const
{
    return (m_isConfigurationFlagSetByUser);
}

void PrismMessage::setWaveClientMessageId (const UI32 &waveClientMessageId)
{
    m_waveClientMessageId = waveClientMessageId;
}

UI32 PrismMessage::getWaveClientMessageId () const
{
    return (m_waveClientMessageId);
}

UI32 PrismMessage::getTransactionCounter () const
{
    return m_transactionCounter;
}

void PrismMessage::setTransactionCounter (const UI32 transactionCounter)
{
    m_transactionCounter = transactionCounter;
}

string PrismMessage::getNestedSql () const
{
    return m_nestedSql;
}

void PrismMessage::setNestedSql (const string &nestedSql)
{
    m_nestedSql = nestedSql;
}

void PrismMessage::appendNestedSql (const string &nestedSql)
{
    m_nestedSql += nestedSql;
}

PrismThreadId  PrismMessage::getPrismMessageCreatorThreadId () const
{
    return (m_prismMessageCreatorThreadId);
}

void PrismMessage::setSurrogatingForLocationId (LocationId disconnectedLocation)
{
    m_surrogatingForLocationId = disconnectedLocation;
}

LocationId PrismMessage::getSurrogatingForLocationId () const
{
    return (m_surrogatingForLocationId);
}

void PrismMessage::setNeedSurrogateSupportFlag (bool needSurrogateSupport)
{
    m_needSurrogateSupportFlag = needSurrogateSupport;
}

bool PrismMessage::getNeedSurrogateSupportFlag () const
{
    return (m_needSurrogateSupportFlag);
}

void PrismMessage::setIsMessageBeingSurrogatedFlag (bool isMessageBeingSurrogated)
{
    m_isMessageBeingSurrogatedFlag = isMessageBeingSurrogated;
}

bool PrismMessage::getIsMessageBeingSurrogatedFlag () const
{
    return (m_isMessageBeingSurrogatedFlag);
}

void PrismMessage::updateForCompletionStatusDuringSurrogacy ()
{
    m_completionStatus = WAVE_MESSAGE_SUCCESS;
}
/**
 * Name:        addStatusPropagation
 * Description: Adds the status resource id and localized status of a different message based on the current location id.
 *              The specific purpose is to facilitate propagating sendToWaveCluster partial success
 *              errors back to WyserConfigurationHandlers where logging these error messages can be handled.
 *
 * @param1:     statusResourceId : Resource id of the message status to be added for propagation
 * @param2:     localizedStatus  : Localized string of the message status to be added for propagation
 *
 * @return:     none
 */
void PrismMessage::addStatusPropagation (ResourceId statusResourceId, const string &localizedStatus)
{
    addStatusPropagation (statusResourceId, localizedStatus, FrameworkToolKit::getThisLocationId ());
}

/**
 * Name:        addStatusPropagation
 * Description: Adds the status resource id and localized status of a different message based on location id.
 *              The specific purpose is to facilitate propagating sendToWaveCluster partial success
 *              errors back to WyserConfigurationHandlers where logging these error messages can be handled.
 *
 * @param1:     statusResourceId : Resource id of the message status to be added for propagation
 * @param2:     localizedStatus  : Localized string of the message status to be added for propagation
 * @param3:     locationId       : Location id of where the message was sent to.
 *
 * @return:     none
 */
void PrismMessage::addStatusPropagation (ResourceId statusResourceId, const string &localizedStatus, LocationId locationId)
{
    //trace (TRACE_LEVEL_DEVEL, "PrismMessage::addStatusPropagation : Entering ...");

    vector<LocationId>::iterator it;
    bool                         locationExists = false;

    for (it = m_locationsForStatusPropagation.begin (); it < m_locationsForStatusPropagation.end (); it++)
    {
        if (*it == locationId)
        {
            locationExists = true;
            break;
        }
    }

    if (true == locationExists)
    {
        trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::addStatusPropagation : Propagated status for location id ") + locationId + " already exists, do not add the propagated status");

        return;
    }

    m_completionStatusForStatusPropagation.push_back (statusResourceId);
    m_localizedCompletionStatusForStatusPropagation.push_back (localizedStatus);
    m_locationsForStatusPropagation.push_back (locationId);

    trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::addStatusPropagation : Resource Id : ") + statusResourceId + ", Localized Status : " + localizedStatus + ", Location : " + locationId);
}

/**
 * Name:        addClusterStatusPropagation
 * Description: Batch adds the status resource id and localized status pulled from the sendToWaveClusterContext.
 *              This allows Partial Success errors to be propagated back to WyserEaGW.
 *
 * @param1:     pWaveSendToClusterContext : pointer to WaveSendToClusterContext
 * @param2:     overAllCompletionStatus : overall completion status for send to wave cluster. 
 *
 * @return:     none
 */
void PrismMessage::addClusterStatusPropagation (WaveSendToClusterContext *pWaveSendToClusterContext, ResourceId overAllCompletionStatus)
{
    //trace (TRACE_LEVEL_DEVEL, "PrismMessage::addClusterStatusPropagation : Entering ...");

    bool partialSuccessFlag = pWaveSendToClusterContext->getPartialSuccessFlag ();

    if ((true == partialSuccessFlag) || (WAVE_MESSAGE_ERROR_FAILED_ON_SOME_NODES == overAllCompletionStatus) || (WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES == overAllCompletionStatus))
    {
        // Propagate sendToWaveCluster status on this Prism Message for an all error case, partial error case (when partial success is not used), and partial success case.
        // Phase2 Rollback cases are currently not supported.

        vector<LocationId>  locationsSentForPhase1 = pWaveSendToClusterContext->getLocationsToSendToForPhase1 ();

        for (UI32 i = 0; i < static_cast<UI32> (locationsSentForPhase1.size ()); i++)
        {
            WaveMessageStatus sendStatus = pWaveSendToClusterContext->getSendStatusForPhase1 (locationsSentForPhase1[i]);

            if (WAVE_MESSAGE_SUCCESS != sendStatus)
            {
                addStatusPropagation (sendStatus, FrameworkToolKit::localize (sendStatus), locationsSentForPhase1[i]);
            }
            else
            {
                FrameworkStatus frameworkStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (locationsSentForPhase1[i]);

                if (FRAMEWORK_SUCCESS != frameworkStatus)
                {
                    addStatusPropagation (frameworkStatus, FrameworkToolKit::localize (frameworkStatus), locationsSentForPhase1[i]);
                }
                else
                {
                    ResourceId completionStatus = pWaveSendToClusterContext->getCompletionStatusForPhase1 (locationsSentForPhase1[i]);

                    if (WAVE_MESSAGE_SUCCESS != completionStatus)
                    {
                        PrismMessage   *pResultingMessageForPhase1          = pWaveSendToClusterContext->getResultingMessageForPhase1 (locationsSentForPhase1[i]);
                        string          localizedStatus                     = "";
                        bool            locationFound                       = false;
                        ResourceId      status;


                        locationFound = pResultingMessageForPhase1->getStatusPropagationByLocationId (status, localizedStatus, locationsSentForPhase1[i]);

                        if ((true == locationFound) && ("" != localizedStatus))
                        {
                            // Handle the special case where a variable argument list is used in the completion status of the sendToWaveCluster phase 1 message.

                            addStatusPropagation (completionStatus, localizedStatus, locationsSentForPhase1[i]);

                            trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::addClusterStatusPropagation : Propagated localized status with custom attribute(s).  Resource Id : ") + status + ", Localized Status : " + localizedStatus + ", Location Id : " + locationsSentForPhase1[i]);
                        }
                        else
                        {
                            addStatusPropagation (completionStatus, FrameworkToolKit::localize (completionStatus), locationsSentForPhase1[i]);

                            trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::addClusterStatusPropagation : Propagated error status.  Resource Id : ") + completionStatus + ", Localized Status : " + FrameworkToolKit::localize (completionStatus) + ", Location Id : " + locationsSentForPhase1[i]);
                        }
                    }
                    else
                    {
                        // Propagate the WAVE_MESSAGE_SUCCESS case so that successful locations can be grouped later.

                        addStatusPropagation (completionStatus, FrameworkToolKit::localize (completionStatus), locationsSentForPhase1[i]);

                        trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::addClusterStatusPropagation : Propagated success status.  Resource Id : ") + completionStatus + ", Localized Status : " + FrameworkToolKit::localize (completionStatus) + ", Location Id : " + locationsSentForPhase1[i]);
                    }
                }
            }
        }
    }
    else
    {
        // SendToWaveCluster has completely succeeded or completely failed so do nothing
    }
}

/**
 * Name:        getStatusPropagationByLocationId
 * Description: Retrieve the status resource Id and localized status of a different message by location id.
 *              The specific purpose is to facilitate propagating sendToWaveCluster partial success
 *              errors back to WyserConfigurationHandlers where logging these error messages can be handled.
 *
 * @param1:     statusResourceId :  propagated status of a message when corresponding location id is found.
 * @param2:     localizedStatus  :  propagated localized status of a message when corresponding location id is found.
 * @param3:     locationId       :  the location id by which to search for a propagated status resource id and localized status.
 *
 * @return:     true, if propagated status exists and the provided location id was found.  Otherwise false.
 */
bool PrismMessage::getStatusPropagationByLocationId (ResourceId &statusResourceId, string &localizedStatus, LocationId locationId)
{
    //trace (TRACE_LEVEL_DEVEL, "PrismMessage::getStatusPropagationByLocationId : Entering...");

    UI32 numberOfLocations  = m_locationsForStatusPropagation.size ();
    bool isLocationIdFound  = false;
    bool returnStatus       = false;

    trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::getStatusPropagationByLocationId : Number of locations : ") + numberOfLocations);

    if (0 < numberOfLocations)
    {
        for (UI32 i = 0; i < numberOfLocations; i++)
        {
            if (locationId == m_locationsForStatusPropagation[i])
            {
                statusResourceId    = m_completionStatusForStatusPropagation[i];
                localizedStatus     = m_localizedCompletionStatusForStatusPropagation[i];
                isLocationIdFound   = true;
                returnStatus        = true;

                trace (TRACE_LEVEL_DEBUG, string ("PrismMessage::getStatusPropagationByLocationId : Propagated status found : Resource Id : ") + m_completionStatusForStatusPropagation[i] + ", Localized Status : " + m_localizedCompletionStatusForStatusPropagation[i] + ", Location Id : " + locationId);

                break;
            }
        }

        if (false == isLocationIdFound)
        {
            trace (TRACE_LEVEL_ERROR, string ("PrismMessage::getStatusPropagationByLocationId : Propagated status for location id : ") + locationId + " could not be found.");

            returnStatus = false;
        }
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "PrismMessage::getStatusPropagationByLocationId : Propagated status is currently empty.  Possibly no status has been propagated.");

        returnStatus = false;
    }

    return (returnStatus);
}

/**
 * Name:        getAllStatusPropagation
 * Description: Bulk get of all three vectors where the index number is assumed to be the key
 *              that contains correlated data across the three vectors.
 *              The specific purpose is to facilitate propagating sendToWaveCluster partial success
 *              errors back to WyserConfigurationHandlers where logging these error messages can be handled.
 *
 * @param1:     statusResourceIds : vector of status resource ids where the same index is correlated data from the other two vectors.
 * @param2:     localizedStatus   : vector of localized status where the same index is correlated data from the other two vectors.
 * @param3:     locationIds       : vector of location ids where the same index is correlated data from the other two vectors.
 *
 * @return:     true, if propagated status exists.  Otherwise false.
 */
bool PrismMessage::getAllStatusPropagation (vector<ResourceId> &statusResourceIds, vector<string> &localizedStatus, vector<LocationId> &locationIds)
{
    UI32 numberOfCompletionStatus   = m_completionStatusForStatusPropagation.size ();
    bool returnStatus               = false;

    if (0 < numberOfCompletionStatus)
    {
        statusResourceIds   = m_completionStatusForStatusPropagation;
        localizedStatus     = m_localizedCompletionStatusForStatusPropagation;
        locationIds         = m_locationsForStatusPropagation;

        returnStatus = true;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "PrismMessage::getAllStatusPropagation : Propagated status is currently empty.  There is no valid propagated status to return.");

        returnStatus = false;
    }

    return (returnStatus);
}

/**
 * Name:        clearStatusPropagation
 * Description: Bulk clear of all three vectors used for status propagation.
 *
 * @return:     none
 */
void PrismMessage::clearStatusPropagation ()
{
    m_locationsForStatusPropagation.clear ();
    m_completionStatusForStatusPropagation.clear ();
    m_localizedCompletionStatusForStatusPropagation.clear ();
}

/**
 * Name:        getNumberOfStatusPropagation
 * Description: Check to see if any message status has been added for propagation
 *
 * @return:     Size of the status propagation vectors
 */
UI32 PrismMessage::getNumberOfStatusPropagation () const
{
    UI32 numberOfCompletionStatus = m_completionStatusForStatusPropagation.size ();
    UI32 numberOfLocalizedCompletionStatus = m_localizedCompletionStatusForStatusPropagation.size ();
    UI32 numberOfLocations = m_locationsForStatusPropagation.size ();

    if ((numberOfCompletionStatus != numberOfLocalizedCompletionStatus) || (numberOfCompletionStatus != numberOfLocations))
    {
        trace (TRACE_LEVEL_FATAL, string ("PrismMessage::getNumberOfStatusPropagation : There is a mismatch between the size of the three status propagation vectors which should never happen.  Completion Status Vector Size: ") + numberOfCompletionStatus + ", Localized Completion Status Vector Size: " + numberOfLocalizedCompletionStatus + ", Locations Vector: " + numberOfLocations);

        prismAssert (false, __FILE__, __LINE__);
    }

    return (numberOfCompletionStatus);
}

bool PrismMessage::getIsMessageSupportedWhenServiceIsPaused () const
{
    return (m_isMessageSupportedWhenServiceIsPaused);
}

void PrismMessage::setIsMessageSupportedWhenServiceIsPaused (bool isMessageSupportedWhenServiceIsPaused)
{
    m_isMessageSupportedWhenServiceIsPaused = isMessageSupportedWhenServiceIsPaused;
}

void PrismMessage::addXPathStringsVectorForTimestampUpdate (const vector<string> &xPathStringsVectorForTimestampUpdate)
{
    m_xPathStringsVectorForTimestampUpdate.insert (m_xPathStringsVectorForTimestampUpdate.end (), xPathStringsVectorForTimestampUpdate.begin (), xPathStringsVectorForTimestampUpdate.end ());
}

void PrismMessage::addXPathStringForTimestampUpdate (const string &xPathString)
{
    m_xPathStringsVectorForTimestampUpdate.push_back (xPathString);
}

vector<string> PrismMessage::getXPathStringsVectorForTimestampUpdate () const
{
    return m_xPathStringsVectorForTimestampUpdate;
}

// Multi Partition.
void PrismMessage::setPartitionName (const string &partitionName)
{
    m_isPartitionNameSetByUser = true;

    m_partitionName = partitionName;
}

string PrismMessage::getPartitionName (void) const
{
    return m_partitionName;
}

void PrismMessage::setPartitionLocationIdForPropagation (const LocationId &partitionLocationIdForPropagation)
{
    m_partitionLocationIdForPropagation = partitionLocationIdForPropagation;
}

LocationId PrismMessage::getPartitionLocationIdForPropagation () const
{
    return m_partitionLocationIdForPropagation;
}

void PrismMessage::setIsPartitionContextPropagated (const bool &isPartitionContextPropagated)
{
    m_isPartitionContextPropagated = isPartitionContextPropagated;
}

bool PrismMessage::getIsPartitionContextPropagated () const
{
    return m_isPartitionContextPropagated;
}

bool PrismMessage::getIsPartitionNameSetByUser () const
{
    return m_isPartitionNameSetByUser;
}


vector<string> PrismMessage::getlocalizedCompletionStatusForStatusPropagationVector () const
{
    return (m_localizedCompletionStatusForStatusPropagation);
}

vector<ResourceId> PrismMessage::getcompletionStatusForStatusPropagationVector () const
{
    return (m_completionStatusForStatusPropagation);
}

vector<LocationId> PrismMessage::getlocationsForStatusPropagationVector () const
{
    return (m_locationsForStatusPropagation);
}

void PrismMessage::setlocalizedCompletionStatusForStatusPropagationVector (const vector<string> localizedCompletionStatusForStatusPropagation)
{
    m_localizedCompletionStatusForStatusPropagation= localizedCompletionStatusForStatusPropagation;
}

void PrismMessage::setcompletionStatusForStatusPropagationVector (const vector<ResourceId> completionStatusForStatusPropagation)
{
    m_completionStatusForStatusPropagation = completionStatusForStatusPropagation;
}

void PrismMessage::setlocationsForStatusPropagationVector (const vector<LocationId> locationsForStatusPropagation)
{
    m_locationsForStatusPropagation = locationsForStatusPropagation;
}

bool PrismMessage::getIsAConfigurationIntent () const
{
    return (m_isAConfigurationIntent);
}

void PrismMessage::setIsAConfigurationIntent (bool isAConfigurationIntent)
{
    m_isAConfigurationIntent = isAConfigurationIntent;
}

bool PrismMessage::getIsConfigurationIntentStored () const
{
    return (m_isConfigurationIntentStored);
}

void PrismMessage::setIsConfigurationIntentStored (bool isConfigurationIntentStored)
{
    m_isConfigurationIntentStored = isConfigurationIntentStored;
}

bool PrismMessage::getIsALastConfigReplay () const
{
    return (m_isALastConfigReplay);
}

void PrismMessage::setIsALastConfigReplay (bool isALastConfigReplay)
{
    m_isALastConfigReplay = isALastConfigReplay;
}

void PrismMessage::setTimeOutInMilliSeconds (const UI32 timeOutInMilliSeconds)
{
    m_timeOutInMilliSeconds = timeOutInMilliSeconds;
}

UI32 PrismMessage::getTimeOutInMilliSeconds () const 
{
    return (m_timeOutInMilliSeconds);
}

bool PrismMessage::checkToDisconnectNodeFromLocationAfterReply () const
{
    return (m_disconnectFromNodeAfterReply);
}

void PrismMessage::setDisconnectFromLocationAfterReply (const bool &disconnectFlag)
{
    m_disconnectFromNodeAfterReply = disconnectFlag;
}

bool PrismMessage::checkToRemoveNodeFromKnownLocationAfterReply () const
{
    return (m_removeNodeFromKnownLocationAfterReply);
}

void PrismMessage::setRemoveNodeFromKnownLocationAfterReply (const bool &removeLocation)
{
    m_removeNodeFromKnownLocationAfterReply = removeLocation;
}

bool PrismMessage::checkToSendForOneWayCommunication () const
{
    return (m_sendForOneWayConnection);
}

void PrismMessage::setToAllowSendForOneWayCommunication (const bool &allowSend)
{
    m_sendForOneWayConnection = allowSend;
}

}
