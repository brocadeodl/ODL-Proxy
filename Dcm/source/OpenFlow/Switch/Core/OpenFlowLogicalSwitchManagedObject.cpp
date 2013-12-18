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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "OpenFlow/Switch/Core/OpenFlowLogicalSwitchManagedObject.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeUri.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManager.h"
#include "Framework/Utils/ClientStreamingSocket.h"
#include "OpenFlow/Interface/OpenFlow.h"
#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Utils/Integer.h"
#include "OpenFlow/Switch/Core/OpenFlowSwitchObjectManagerAddFlowMessage.h"

#include <map>
#include <string>

using namespace WaveNs;
using namespace std;

namespace OpenFlowNs
{

OpenFlowLogicalSwitchManagedObject::OpenFlowLogicalSwitchManagedObject (WaveObjectManager *pWaveObjectManager)
: PrismElement           (pWaveObjectManager),
  PrismPersistableObject (OpenFlowLogicalSwitchManagedObject::getClassName (), DcmManagedObject::getClassName ()),
  WaveManagedObject      (pWaveObjectManager),
  DcmManagedObject       (pWaveObjectManager)
{
}

OpenFlowLogicalSwitchManagedObject::~OpenFlowLogicalSwitchManagedObject ()
{
}

string OpenFlowLogicalSwitchManagedObject::getClassName ()
{
    return ("OpenFlowLogicalSwitchManagedObject");
}

void OpenFlowLogicalSwitchManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeString                                                          (&m_openFlowLogicalSwitchName, "openFlowLogicalSwitchName"));
    addPersistableAttribute (new AttributeUri                                                             (&m_openFlowLogicalSwitchUri,  "openFlowLogicalSwitchUri"));
    addPersistableAttribute (new AttributeManagedObjectVectorComposition<OpenFlowControllerManagedObject> (&m_openFlowControllers,       "m_openFlowControllers",       getClassName (), getObjectId (), OpenFlowControllerManagedObject::getClassName ()));
}

void OpenFlowLogicalSwitchManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeString                                                          (&m_openFlowLogicalSwitchName, "openFlowLogicalSwitchName"));
    addPersistableAttributeForCreate (new AttributeUri                                                             (&m_openFlowLogicalSwitchUri,  "openFlowLogicalSwitchUri"));
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition<OpenFlowControllerManagedObject> (&m_openFlowControllers,       "m_openFlowControllers",       getClassName (), getObjectId (), OpenFlowControllerManagedObject::getClassName ()));
}

string OpenFlowLogicalSwitchManagedObject::getOpenFlowLogicalSwitchName () const
{
    return (m_openFlowLogicalSwitchName);
}

void OpenFlowLogicalSwitchManagedObject::setOpenFlowLogicalSwitchName (const string &openFlowLogicalSwitchName)
{
    m_openFlowLogicalSwitchName = openFlowLogicalSwitchName;
}

Uri OpenFlowLogicalSwitchManagedObject::getOpenFlowLogicalSwitchUri () const
{
    return (m_openFlowLogicalSwitchUri);
}

void OpenFlowLogicalSwitchManagedObject::setOpenFlowLogicalSwitchUri (const Uri &openFlowLogicalSwitchUri)
{
    m_openFlowLogicalSwitchUri = openFlowLogicalSwitchUri;
}

bool OpenFlowLogicalSwitchManagedObject::isAKnownOpenFlowController (const IpV4AddressNetworkMask &openFlowControllerIpV4AddressNetworkMask, const UI32 &openFlowControllerPort)
{
    UI32 numberOfKnownOpenFlowControllers = m_openFlowControllers.size ();
    UI32 i                                = 0;
    bool isKnown                          = false;

    for (i = 0; i < numberOfKnownOpenFlowControllers; i++)
    {
        OpenFlowControllerManagedObject *pOpenFlowControllerManagedObject = (m_openFlowControllers[i]).operator-> ();

        prismAssert (NULL != pOpenFlowControllerManagedObject, __FILE__, __LINE__);

        const IpV4AddressNetworkMask tempOpenFlowControllerIpV4AddressNetworkMask = pOpenFlowControllerManagedObject->getIpPrefix ();
        const UI32                   tempOpenFlowControllerPort                   = pOpenFlowControllerManagedObject->getPort     ();

        if ((openFlowControllerIpV4AddressNetworkMask == tempOpenFlowControllerIpV4AddressNetworkMask) && (openFlowControllerPort == tempOpenFlowControllerPort))
        {
            isKnown = true;

            break;
        }
    }

    return (isKnown);
}

void OpenFlowLogicalSwitchManagedObject::addOpenFlowController (const IpV4AddressNetworkMask &openFlowControllerIpV4AddressNetworkMask, const UI32 &openFlowControllerPort, const OpenFlowChannelConnectionProtocol &openFlowChannelConnectionProtocol)
{
    bool isKnown = isAKnownOpenFlowController (openFlowControllerIpV4AddressNetworkMask, openFlowControllerPort);

    if (true != isKnown)
    {
        OpenFlowControllerManagedObject *pOpenFlowControllerManagedObject = new OpenFlowControllerManagedObject (reinterpret_cast<OpenFlowSwitchObjectManager *> (getPWaveObjectManager()));

        prismAssert (NULL != pOpenFlowControllerManagedObject, __FILE__, __LINE__);

        pOpenFlowControllerManagedObject->setIpPrefix (openFlowControllerIpV4AddressNetworkMask);
        pOpenFlowControllerManagedObject->setPort     (openFlowControllerPort);
        pOpenFlowControllerManagedObject->setProtocol (openFlowChannelConnectionProtocol);

        prismAssert (NULL != pOpenFlowControllerManagedObject, __FILE__, __LINE__);

        m_openFlowControllers.push_back (WaveManagedObjectPointer<OpenFlowControllerManagedObject> (pOpenFlowControllerManagedObject));
    }
}

void OpenFlowLogicalSwitchManagedObject::connectToKnownOpenFlowControllers ()
{
    UI32 numberOfKnownOpenFlowControllers = m_openFlowControllers.size ();
    UI32 i                                = 0;

//    static UI64 cookie1 = 0;
//    static UI64 cookie2 = 0;
    //static ofp100_match match1;
    //static ofp100_match match2;
    static UI64 temp1 = 0;
    static map<UI32, ofp100_match> s_ofpMatchMap;
    static map<UI32, UI64>         s_cookieMap;

    for (i = 0; i < numberOfKnownOpenFlowControllers; i++)
    {
        OpenFlowControllerManagedObject *pOpenFlowControllerManagedObject = (m_openFlowControllers[i]).operator-> ();

        prismAssert (NULL != pOpenFlowControllerManagedObject, __FILE__, __LINE__);

        const IpV4AddressNetworkMask openFlowControllerIpV4AddressNetworkMask = pOpenFlowControllerManagedObject->getIpPrefix         ();
        const UI32                   openFlowControllerPort                   = pOpenFlowControllerManagedObject->getPort             ();
        const string                 openFlowControllerIpV4Address            = openFlowControllerIpV4AddressNetworkMask.getIpAddress ();

        ClientStreamingSocket *pOpenFlowControllerClientStreamingSocket = new ClientStreamingSocket (openFlowControllerIpV4Address, (SI32) openFlowControllerPort, 1);

        prismAssert (NULL != pOpenFlowControllerClientStreamingSocket, __FILE__, __LINE__);

        ofp_hello openFlowHelloMessage;

        openFlowHelloMessage.header.version = OpenFlow::m_openFlow10Version;
        openFlowHelloMessage.header.type    = OFPT_HELLO;
        openFlowHelloMessage.header.length  = ntohs (sizeof (openFlowHelloMessage));
        openFlowHelloMessage.header.xid     = 0;

        bool isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowHelloMessage, sizeof (openFlowHelloMessage));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for hello ");
        }

        FixedSizeBuffer fixedSizeBufferForOpenFlowHelloMessage (sizeof (openFlowHelloMessage), &openFlowHelloMessage, false);

        isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> fixedSizeBufferForOpenFlowHelloMessage;

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Reading failed for hello ");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "    Type : " + (Integer (openFlowHelloMessage.header.type)).toString ());
        }

        fixedSizeBufferForOpenFlowHelloMessage.reset ();

        isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> fixedSizeBufferForOpenFlowHelloMessage;

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Reading failed for hello 2");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "    Type   : " + (Integer (openFlowHelloMessage.header.type)).toString ());
            trace (TRACE_LEVEL_WARN, "    Length : " + (Integer (htons (openFlowHelloMessage.header.length))).toString ());
        }

        ofp_switch_features openFlowSwitchFeaturesMessage;
        ofp_phy_port        openFlowSwitchFeaturesMessagePhyPort1;
        ofp_phy_port        openFlowSwitchFeaturesMessagePhyPort2;

        openFlowSwitchFeaturesMessage.header.version = OpenFlow::m_openFlow10Version;
        openFlowSwitchFeaturesMessage.header.type    = OFPT_FEATURES_REPLY;
        openFlowSwitchFeaturesMessage.header.length  = htons (sizeof (openFlowSwitchFeaturesMessage) + sizeof (openFlowSwitchFeaturesMessagePhyPort1) + sizeof (openFlowSwitchFeaturesMessagePhyPort2));
        openFlowSwitchFeaturesMessage.header.xid     = openFlowHelloMessage.header.xid;
        openFlowSwitchFeaturesMessage.datapath_id    = 0xF123456789ABCDEFULL;
        openFlowSwitchFeaturesMessage.n_buffers      = htonl (1024);
        openFlowSwitchFeaturesMessage.n_tables       = 1;
        openFlowSwitchFeaturesMessage.auxiliary_id   = 0;
        openFlowSwitchFeaturesMessage.capabilities   = htonl ( OFPC_FLOW_STATS);

        openFlowSwitchFeaturesMessagePhyPort1.port_no     = htons (1);
        openFlowSwitchFeaturesMessagePhyPort1.hw_addr[0]  = 0x11;
        openFlowSwitchFeaturesMessagePhyPort1.hw_addr[1]  = 0x20;
        openFlowSwitchFeaturesMessagePhyPort1.hw_addr[2]  = 0x33;
        openFlowSwitchFeaturesMessagePhyPort1.hw_addr[3]  = 0x44;
        openFlowSwitchFeaturesMessagePhyPort1.hw_addr[4]  = 0x50;
        openFlowSwitchFeaturesMessagePhyPort1.hw_addr[5]  = 0x10;
        strncpy (openFlowSwitchFeaturesMessagePhyPort1.name, "port-1", 16);
        openFlowSwitchFeaturesMessagePhyPort1.config      = 0;
        openFlowSwitchFeaturesMessagePhyPort1.state       = 0;
        openFlowSwitchFeaturesMessagePhyPort1.curr        = htonl (OFPPF_1GB_FD);
        openFlowSwitchFeaturesMessagePhyPort1.advertised  = htonl (OFPPF_1GB_FD);
        openFlowSwitchFeaturesMessagePhyPort1.supported   = htonl (OFPPF_1GB_FD);
        openFlowSwitchFeaturesMessagePhyPort1.peer        = htonl (OFPPF_1GB_FD);

        openFlowSwitchFeaturesMessagePhyPort2.port_no     = htons (1);
        openFlowSwitchFeaturesMessagePhyPort2.hw_addr[0]  = 0x22;
        openFlowSwitchFeaturesMessagePhyPort2.hw_addr[1]  = 0x20;
        openFlowSwitchFeaturesMessagePhyPort2.hw_addr[2]  = 0x33;
        openFlowSwitchFeaturesMessagePhyPort2.hw_addr[3]  = 0x44;
        openFlowSwitchFeaturesMessagePhyPort2.hw_addr[4]  = 0x50;
        openFlowSwitchFeaturesMessagePhyPort2.hw_addr[5]  = 0x20;
        strncpy (openFlowSwitchFeaturesMessagePhyPort2.name, "port-2", 16);
        openFlowSwitchFeaturesMessagePhyPort2.config      = 0;
        openFlowSwitchFeaturesMessagePhyPort2.state       = 0;
        openFlowSwitchFeaturesMessagePhyPort2.curr        = htonl (OFPPF_1GB_FD);
        openFlowSwitchFeaturesMessagePhyPort2.advertised  = htonl (OFPPF_1GB_FD);
        openFlowSwitchFeaturesMessagePhyPort2.supported   = htonl (OFPPF_1GB_FD);
        openFlowSwitchFeaturesMessagePhyPort2.peer        = htonl (OFPPF_1GB_FD);

        isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchFeaturesMessage,         sizeof (openFlowSwitchFeaturesMessage));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for Features 1");
        }

        isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchFeaturesMessagePhyPort1, sizeof (openFlowSwitchFeaturesMessagePhyPort1));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for Features 2");
        }

        isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchFeaturesMessagePhyPort2, sizeof (openFlowSwitchFeaturesMessagePhyPort2));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for Features 3");
        }

        fixedSizeBufferForOpenFlowHelloMessage.reset ();

        isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> fixedSizeBufferForOpenFlowHelloMessage;

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Reading failed for hello 3");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "    Type : " + (Integer (openFlowHelloMessage.header.type)).toString ());
        }

        openFlowHelloMessage.header.length = ntohs (openFlowHelloMessage.header.length);

        if (openFlowHelloMessage.header.length > (sizeof (openFlowHelloMessage)))
        {
            UI32 remainingLengthToBeRead = openFlowHelloMessage.header.length - (sizeof (openFlowHelloMessage));

            FixedSizeBuffer tempBuffer (remainingLengthToBeRead);

            isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> tempBuffer;

            if (true != isSuccessful)
            {
                trace (TRACE_LEVEL_ERROR, " Reading failed for temp 3");
            }
        }

        fixedSizeBufferForOpenFlowHelloMessage.reset ();

        isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> fixedSizeBufferForOpenFlowHelloMessage;

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Reading failed for hello 4");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "    Type : " + (Integer (openFlowHelloMessage.header.type)).toString ());
        }

        openFlowHelloMessage.header.length = ntohs (openFlowHelloMessage.header.length);

        if (openFlowHelloMessage.header.length > (sizeof (openFlowHelloMessage)))
        {
            UI32 remainingLengthToBeRead = openFlowHelloMessage.header.length - (sizeof (openFlowHelloMessage));

            FixedSizeBuffer tempBuffer (remainingLengthToBeRead);

            isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> tempBuffer;

            if (true != isSuccessful)
            {
                trace (TRACE_LEVEL_ERROR, " Reading failed for temp 4");
            }
        }

        ofp_switch_config openFlowSwitchConfiguration;

        openFlowSwitchConfiguration.header.version = OpenFlow::m_openFlow10Version;
        openFlowSwitchConfiguration.header.type    = OFPT_GET_CONFIG_REPLY;
        openFlowSwitchConfiguration.header.length  = htons (sizeof (openFlowSwitchConfiguration));
        openFlowSwitchConfiguration.header.xid     = openFlowHelloMessage.header.xid;
        openFlowSwitchConfiguration.flags          = 0;
        openFlowSwitchConfiguration.miss_send_len  = 0xffff;

        //isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchConfiguration, 8);
        isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchConfiguration, sizeof (openFlowSwitchConfiguration));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for get config reply");
        }

        fixedSizeBufferForOpenFlowHelloMessage.reset ();

        isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> fixedSizeBufferForOpenFlowHelloMessage;

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Reading failed for hello 5");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, "    Type : " + (Integer (openFlowHelloMessage.header.type)).toString ());
        }

        openFlowHelloMessage.header.length = ntohs (openFlowHelloMessage.header.length);

        if (openFlowHelloMessage.header.length > (sizeof (openFlowHelloMessage)))
        {
            UI32 remainingLengthToBeRead = openFlowHelloMessage.header.length - (sizeof (openFlowHelloMessage));

            FixedSizeBuffer tempBuffer (remainingLengthToBeRead);

            isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> tempBuffer;

            if (true != isSuccessful)
            {
                trace (TRACE_LEVEL_ERROR, " Reading failed for temp 5");
            }
        }

        UI8 *pBuffer = new UI8[sizeof (ofp_multipart_reply) + sizeof (ofp_desc)];
        ofp_multipart_reply *pOpenFlowMultiPartReply = (ofp_multipart_reply *) pBuffer;
        ofp_desc *pOpenFlowDescription = (ofp_desc *) (pBuffer + sizeof (ofp_multipart_reply));

        pOpenFlowMultiPartReply->header.version = OpenFlow::m_openFlow10Version;
        pOpenFlowMultiPartReply->header.type     = 17;
        pOpenFlowMultiPartReply->header.length   = htons (sizeof (ofp_multipart_reply) + sizeof (ofp_desc));
        pOpenFlowMultiPartReply->header.xid      = openFlowHelloMessage.header.xid;
        pOpenFlowMultiPartReply->type            = 0;
        pOpenFlowMultiPartReply->flags           = 0;

        strncpy (pOpenFlowDescription->mfr_desc,   "Brocade VDX Logical Chassis Based Open Flow Logical Switch", 256);
        strncpy (pOpenFlowDescription->hw_desc,    "Brocade VDX Logical Chassis Based Open Flow Logical Switch", 256);
        strncpy (pOpenFlowDescription->sw_desc,    "NOS Software Based Platform", 256);
        strncpy (pOpenFlowDescription->serial_num, "1.4.0", 32);
        strncpy (pOpenFlowDescription->dp_desc,    "NOS Software Based experimental Open Flow Logical Switch", 256);

        isSuccessful = pOpenFlowControllerClientStreamingSocket->send (pBuffer, sizeof (ofp_multipart_reply) + sizeof (ofp_desc));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for stats (desc) reply");
        }

        ofp_port_status openFlowPortStatusMessage;

        openFlowPortStatusMessage.header.version   = OpenFlow::m_openFlow10Version;
        openFlowPortStatusMessage.header.type      = OFPT_PORT_STATUS;
        openFlowPortStatusMessage.header.length    = htons (sizeof (openFlowPortStatusMessage));
        openFlowPortStatusMessage.header.xid       = 0;
        openFlowPortStatusMessage.reason           = OFPPR_ADD;
        openFlowPortStatusMessage.desc.port_no     = htons (1);
        openFlowPortStatusMessage.desc.hw_addr[0]  = 0x11;
        openFlowPortStatusMessage.desc.hw_addr[1]  = 0x20;
        openFlowPortStatusMessage.desc.hw_addr[2]  = 0x33;
        openFlowPortStatusMessage.desc.hw_addr[3]  = 0x44;
        openFlowPortStatusMessage.desc.hw_addr[4]  = 0x50;
        openFlowPortStatusMessage.desc.hw_addr[5]  = 0x10;
        strncpy (openFlowPortStatusMessage.desc.name, "port-1", 16);
        openFlowPortStatusMessage.desc.config     = 0;
        openFlowPortStatusMessage.desc.state      = 0;
        openFlowPortStatusMessage.desc.curr       = htonl (OFPPF_1GB_FD);
        openFlowPortStatusMessage.desc.advertised = htonl (OFPPF_1GB_FD);
        openFlowPortStatusMessage.desc.supported  = htonl (OFPPF_1GB_FD);
        openFlowPortStatusMessage.desc.peer       = htonl (OFPPF_1GB_FD);

        isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowPortStatusMessage, sizeof (openFlowPortStatusMessage));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for Port  1 Status");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, " Sending succeeded for Port 1 Status ");
        }

        openFlowPortStatusMessage.header.version   = OpenFlow::m_openFlow10Version;
        openFlowPortStatusMessage.header.type      = OFPT_PORT_STATUS;
        openFlowPortStatusMessage.header.length    = htons (sizeof (openFlowPortStatusMessage));
        openFlowPortStatusMessage.header.xid       = 0;
        openFlowPortStatusMessage.reason           = OFPPR_ADD;
        openFlowPortStatusMessage.desc.port_no     = htons (2);
        openFlowPortStatusMessage.desc.hw_addr[0]  = 0x22;
        openFlowPortStatusMessage.desc.hw_addr[1]  = 0x20;
        openFlowPortStatusMessage.desc.hw_addr[2]  = 0x33;
        openFlowPortStatusMessage.desc.hw_addr[3]  = 0x44;
        openFlowPortStatusMessage.desc.hw_addr[4]  = 0x50;
        openFlowPortStatusMessage.desc.hw_addr[5]  = 0x20;
        strncpy (openFlowPortStatusMessage.desc.name, "port-2", 16);
        openFlowPortStatusMessage.desc.config     = 0;
        openFlowPortStatusMessage.desc.state      = 0;
        openFlowPortStatusMessage.desc.curr       = htonl (OFPPF_1GB_FD);
        openFlowPortStatusMessage.desc.advertised = htonl (OFPPF_1GB_FD);
        openFlowPortStatusMessage.desc.supported  = htonl (OFPPF_1GB_FD);
        openFlowPortStatusMessage.desc.peer       = htonl (OFPPF_1GB_FD);

        isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowPortStatusMessage, sizeof (openFlowPortStatusMessage));

        if (true != isSuccessful)
        {
            trace (TRACE_LEVEL_ERROR, " Sending failed for Port  2 Status");
        }
        else
        {
            trace (TRACE_LEVEL_WARN, " Sending succeeded for Port 2 Status ");
        }

        while (true)
        {
            fixedSizeBufferForOpenFlowHelloMessage.reset ();

            isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> fixedSizeBufferForOpenFlowHelloMessage;

            if (true != isSuccessful)
            {
                trace (TRACE_LEVEL_ERROR, " Reading failed for hello 6");
                exit (0);
            }
            else
            {
                trace (TRACE_LEVEL_WARN, "    Type : " + (Integer (openFlowHelloMessage.header.type)).toString ());
            }

            openFlowHelloMessage.header.length = ntohs (openFlowHelloMessage.header.length);

            trace (TRACE_LEVEL_WARN, "    length : " + (Integer (openFlowHelloMessage.header.length)).toString ());

            FixedSizeBuffer *pTempBuffer = NULL;

            if (openFlowHelloMessage.header.length > (sizeof (openFlowHelloMessage)))
            {
                UI32 remainingLengthToBeRead = openFlowHelloMessage.header.length - (sizeof (openFlowHelloMessage));

                 pTempBuffer = new FixedSizeBuffer (openFlowHelloMessage.header.length );

                 pTempBuffer->incrementCurrentSize (sizeof (openFlowHelloMessage));

                trace (TRACE_LEVEL_WARN, " Reading Remainign length : " + (Integer (remainingLengthToBeRead)).toString ());

                isSuccessful = (*pOpenFlowControllerClientStreamingSocket) >> (*pTempBuffer);

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, " Reading failed for temp 6");
                }
            }

            if (OFPT_ECHO_REQUEST == openFlowHelloMessage.header.type)
            {
                openFlowHelloMessage.header.length = htons (openFlowHelloMessage.header.length);

                openFlowHelloMessage.header.type = OFPT_ECHO_REPLY;

                isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowHelloMessage, sizeof (openFlowHelloMessage));

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, " Sending failed for echo reply ");
                }
                else
                {
                    trace (TRACE_LEVEL_WARN, " Sending succeededd for echo reply ");
                }

                openFlowHelloMessage.header.type = OFPT_ECHO_REQUEST;

                isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowHelloMessage, sizeof (openFlowHelloMessage));

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, " Sending failed for echo request ");
                }
                else
                {
                    trace (TRACE_LEVEL_WARN, " Sending succeededd for echo request ");
                }

            }
            else if (OFPT_FLOW_MOD == openFlowHelloMessage.header.type)
            {
                ofp100_flow_mod *pOpenFlowFlowModMessage = (ofp100_flow_mod *) pTempBuffer->getPRawBuffer ();

                trace (TRACE_LEVEL_WARN, " FLOW MOD LENGTH : " + (Integer (sizeof (ofp100_flow_mod)).toString ()));

                pOpenFlowFlowModMessage->priority = htons (pOpenFlowFlowModMessage->priority);

                if (0 == pOpenFlowFlowModMessage->match.in_port)
                {
                    //cookie1 = pOpenFlowFlowModMessage->cookie;
                    //match1  = pOpenFlowFlowModMessage->match;
                }
                else
                {
                    //cookie2 = pOpenFlowFlowModMessage->cookie;
                    //match2  = pOpenFlowFlowModMessage->match;

                    s_ofpMatchMap[pOpenFlowFlowModMessage->priority] = pOpenFlowFlowModMessage->match;
                    s_cookieMap[pOpenFlowFlowModMessage->priority]   = pOpenFlowFlowModMessage->cookie;
                }

                pOpenFlowFlowModMessage->out_port      = ntohs (pOpenFlowFlowModMessage->out_port);
                pOpenFlowFlowModMessage->command       = ntohs (pOpenFlowFlowModMessage->command);
                pOpenFlowFlowModMessage->match.in_port = ntohs (pOpenFlowFlowModMessage->match.in_port);
                pOpenFlowFlowModMessage->match.tp_src  = ntohs (pOpenFlowFlowModMessage->match.tp_src);
                pOpenFlowFlowModMessage->match.tp_dst  = ntohs (pOpenFlowFlowModMessage->match.tp_dst);

                char sourceIpAddress[64];
                char destinationIpAddress[64];

                inet_ntop (AF_INET, &(pOpenFlowFlowModMessage->match.nw_src), sourceIpAddress, 64);
                inet_ntop (AF_INET, &(pOpenFlowFlowModMessage->match.nw_dst), destinationIpAddress, 64);

                trace (TRACE_LEVEL_WARN, "        (Output Port)    : " + (Integer (pOpenFlowFlowModMessage->out_port)).toString ());
                trace (TRACE_LEVEL_WARN, "    Command              : " + (Integer (pOpenFlowFlowModMessage->command )).toString ());
                trace (TRACE_LEVEL_WARN, "        INPUT PORT       : " + (Integer (pOpenFlowFlowModMessage->match.in_port )).toString ());
                trace (TRACE_LEVEL_WARN, "        SOURCE IP        : " + string (sourceIpAddress));
                trace (TRACE_LEVEL_WARN, "        SOURCE PORT      : " + (Integer (pOpenFlowFlowModMessage->match.tp_src )).toString ());
                trace (TRACE_LEVEL_WARN, "        DESTINATION IP   : " + string (destinationIpAddress));
                trace (TRACE_LEVEL_WARN, "        DESTINATION PORT : " + (Integer (pOpenFlowFlowModMessage->match.tp_dst )).toString ());

                ofp_action_header *pOpenFlowActionHeader = (ofp_action_header *) (pTempBuffer->getPRawBuffer () + sizeof (ofp100_flow_mod));

                prismAssert (NULL != pOpenFlowActionHeader, __FILE__, __LINE__);

                pOpenFlowActionHeader->type = htons (pOpenFlowActionHeader->type);

                trace (TRACE_LEVEL_WARN, "        ACTION           : " + (Integer (pOpenFlowActionHeader->type)).toString ());

                if (OFPAT_OUTPUT == pOpenFlowActionHeader->type)
                {
                    trace (TRACE_LEVEL_WARN, "        ACTION           : OFPAT_OUTPUT");

                    ofp100_action_output *pOpenFlowActionOutput = (ofp100_action_output *) pOpenFlowActionHeader;

                    pOpenFlowActionOutput->port = htons (pOpenFlowActionOutput->port);

                    trace (TRACE_LEVEL_WARN, "        OUTPUT PORT      : " + (Integer (pOpenFlowActionOutput->port )).toString ());

                    OpenFlowSwitchObjectManagerAddFlowMessage    message;

                    message.setOpenFlowIngressIfIndex (411533447);
                    message.setOpenFlowEgressIfIndex (411566214);
                    message.setOpenFlowSrcIpV4Address (pOpenFlowFlowModMessage->match.nw_src);
                    message.setOpenFlowSrcIpV4AddressMask (0xFFFFFFFF);
                    message.setOpenFlowDstIpV4Address (pOpenFlowFlowModMessage->match.nw_dst);
                    message.setOpenFlowDstIpV4AddressMask (0xFFFFFFFF);
                    message.setOpenFlowAppType ("6");
                    message.setOpenFlowSrcPort (pOpenFlowFlowModMessage->match.tp_src);
                    message.setOpenFlowDstPort (pOpenFlowFlowModMessage->match.tp_dst);

                    ResourceId status = sendSynchronously (&message);

                    if (WAVE_MESSAGE_SUCCESS != status)
                    {
                        trace (TRACE_LEVEL_ERROR, "ERROR in sending the message to add a flow.");
                    }
                    else
                    {
                        status = message.getCompletionStatus ();

                        if (WAVE_MESSAGE_SUCCESS != status)
                        {
                            trace (TRACE_LEVEL_ERROR, "ERROR in processing the message to add a flow.");
                        }
                        else
                        {
                            trace (TRACE_LEVEL_SUCCESS, "Successfully added the flow.");
                        }
                    }
                }
            }
            else if (OFPT_FEATURES_REQUEST == openFlowHelloMessage.header.type)
            {
                trace (TRACE_LEVEL_WARN, "Serving Features Reply...");

                openFlowSwitchFeaturesMessage.header.version = OpenFlow::m_openFlow10Version;
                openFlowSwitchFeaturesMessage.header.type    = OFPT_FEATURES_REPLY;
                openFlowSwitchFeaturesMessage.header.length  = htons (sizeof (openFlowSwitchFeaturesMessage) + sizeof (openFlowSwitchFeaturesMessagePhyPort1) + sizeof (openFlowSwitchFeaturesMessagePhyPort2));
                openFlowSwitchFeaturesMessage.header.xid     = openFlowHelloMessage.header.xid;
                openFlowSwitchFeaturesMessage.datapath_id    = 0xF123456789ABCDEFULL;
                openFlowSwitchFeaturesMessage.n_buffers      = htonl (1024);
                openFlowSwitchFeaturesMessage.n_tables       = 1;
                openFlowSwitchFeaturesMessage.auxiliary_id   = 0;
                openFlowSwitchFeaturesMessage.capabilities   = htonl ( OFPC_FLOW_STATS);

                openFlowSwitchFeaturesMessagePhyPort1.port_no     = htons (1);
                openFlowSwitchFeaturesMessagePhyPort1.hw_addr[0]  = 0x11;
                openFlowSwitchFeaturesMessagePhyPort1.hw_addr[1]  = 0x20;
                openFlowSwitchFeaturesMessagePhyPort1.hw_addr[2]  = 0x33;
                openFlowSwitchFeaturesMessagePhyPort1.hw_addr[3]  = 0x44;
                openFlowSwitchFeaturesMessagePhyPort1.hw_addr[4]  = 0x50;
                openFlowSwitchFeaturesMessagePhyPort1.hw_addr[5]  = 0x10;
                strncpy (openFlowSwitchFeaturesMessagePhyPort1.name, "port-1", 16);
                openFlowSwitchFeaturesMessagePhyPort1.config      = 0;
                openFlowSwitchFeaturesMessagePhyPort1.state       = 0;
                openFlowSwitchFeaturesMessagePhyPort1.curr        = htonl (OFPPF_1GB_FD);
                openFlowSwitchFeaturesMessagePhyPort1.advertised  = htonl (OFPPF_1GB_FD);
                openFlowSwitchFeaturesMessagePhyPort1.supported   = htonl (OFPPF_1GB_FD);
                openFlowSwitchFeaturesMessagePhyPort1.peer        = htonl (OFPPF_1GB_FD);

                openFlowSwitchFeaturesMessagePhyPort2.port_no     = htons (2);
                openFlowSwitchFeaturesMessagePhyPort2.hw_addr[0]  = 0x22;
                openFlowSwitchFeaturesMessagePhyPort2.hw_addr[1]  = 0x20;
                openFlowSwitchFeaturesMessagePhyPort2.hw_addr[2]  = 0x33;
                openFlowSwitchFeaturesMessagePhyPort2.hw_addr[3]  = 0x44;
                openFlowSwitchFeaturesMessagePhyPort2.hw_addr[4]  = 0x50;
                openFlowSwitchFeaturesMessagePhyPort2.hw_addr[5]  = 0x20;
                strncpy (openFlowSwitchFeaturesMessagePhyPort2.name, "port-2", 16);
                openFlowSwitchFeaturesMessagePhyPort2.config      = 0;
                openFlowSwitchFeaturesMessagePhyPort2.state       = 0;
                openFlowSwitchFeaturesMessagePhyPort2.curr        = htonl (OFPPF_1GB_FD);
                openFlowSwitchFeaturesMessagePhyPort2.advertised  = htonl (OFPPF_1GB_FD);
                openFlowSwitchFeaturesMessagePhyPort2.supported   = htonl (OFPPF_1GB_FD);
                openFlowSwitchFeaturesMessagePhyPort2.peer        = htonl (OFPPF_1GB_FD);

                isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchFeaturesMessage,         sizeof (openFlowSwitchFeaturesMessage));

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, " Sending failed for Features 1 1");
                }

                isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchFeaturesMessagePhyPort1, sizeof (openFlowSwitchFeaturesMessagePhyPort1));

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, " Sending failed for Features 1 2");
                }

                isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchFeaturesMessagePhyPort2, sizeof (openFlowSwitchFeaturesMessagePhyPort2));

                if (true != isSuccessful)
                {
                    trace (TRACE_LEVEL_ERROR, " Sending failed for Features 1 3");
                }
            }
            else if (16 == openFlowHelloMessage.header.type) // OFPT_STATS_REQUEST
            {
                ofp_stats_request *pOpenFlowStatsRequest = (ofp_stats_request *) pTempBuffer->getPRawBuffer ();

                pOpenFlowStatsRequest->type  = ntohs (pOpenFlowStatsRequest->type);
                pOpenFlowStatsRequest->flags = ntohs (pOpenFlowStatsRequest->flags);

                trace (TRACE_LEVEL_WARN, string ("     STATS TYPE : ") + pOpenFlowStatsRequest->type);

                if (0 == pOpenFlowStatsRequest->type)
                {
                    trace (TRACE_LEVEL_WARN, "Serving Stats Reply 1 ...");

                    ofp_stats_reply openFlowSwitchStatsReplyMessage;
                    ofp_desc_stats  openFlowSwitchDescriptionStatsMessage;

                    openFlowSwitchStatsReplyMessage.header.version = OpenFlow::m_openFlow10Version;
                    openFlowSwitchStatsReplyMessage.header.type    = 17;
                    openFlowSwitchStatsReplyMessage.header.length  = htons (sizeof (openFlowSwitchStatsReplyMessage) + sizeof (openFlowSwitchDescriptionStatsMessage));
                    openFlowSwitchStatsReplyMessage.header.xid     = openFlowHelloMessage.header.xid;

                    openFlowSwitchStatsReplyMessage.type  = htons (pOpenFlowStatsRequest->type);
                    openFlowSwitchStatsReplyMessage.flags = htons (pOpenFlowStatsRequest->flags);

                    strncpy (openFlowSwitchDescriptionStatsMessage.mfr_desc,   "Brocade VDX Logical Chassis Based Open Flow Logical Switch", 256);
                    strncpy (openFlowSwitchDescriptionStatsMessage.hw_desc,    "Brocade VDX Logical Chassis Based Open Flow Logical Switch", 256);
                    strncpy (openFlowSwitchDescriptionStatsMessage.sw_desc,    "NOS Software Based Platform", 256);
                    strncpy (openFlowSwitchDescriptionStatsMessage.serial_num, "1.4.0", 32);
                    strncpy (openFlowSwitchDescriptionStatsMessage.dp_desc,    "NOS Software Based experimental Open Flow Logical Switch", 256);

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchStatsReplyMessage,       sizeof (openFlowSwitchStatsReplyMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 1 1");
                    }

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchDescriptionStatsMessage, sizeof (openFlowSwitchDescriptionStatsMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 1 2");
                    }
                }
                else if (1 == pOpenFlowStatsRequest->type)
                {
                    UI32                       numberOfOpenFlowFlows                  = s_ofpMatchMap.size ();
                    ofp100_flow_stats_request *pOpenFlowSwitchFlowStatsRequestMessage = (ofp100_flow_stats_request *) pTempBuffer->getPRawBuffer ();
                    ofp100_flow_stats          *pOpenFlowSwitchFlowStatsReplyMessage  = new ofp100_flow_stats[numberOfOpenFlowFlows];
                    ofp_stats_reply            openFlowSwitchStatsReplyMessage;

                    prismAssert (NULL != pOpenFlowSwitchFlowStatsRequestMessage, __FILE__, __LINE__);

                    openFlowSwitchStatsReplyMessage.header.version = OpenFlow::m_openFlow10Version;
                    openFlowSwitchStatsReplyMessage.header.type    = 17;
                    openFlowSwitchStatsReplyMessage.header.length  = htons (sizeof (openFlowSwitchStatsReplyMessage) + sizeof (ofp100_flow_stats) * numberOfOpenFlowFlows);
                    openFlowSwitchStatsReplyMessage.header.xid     = openFlowHelloMessage.header.xid;

                    openFlowSwitchStatsReplyMessage.type  = htons (pOpenFlowStatsRequest->type);
                    openFlowSwitchStatsReplyMessage.flags = htons (pOpenFlowStatsRequest->flags);

                    UI64 temp11;

                    temp1 += 1023;
                    temp11 = temp1 * 8;

                    UI64 tempp = temp1;
                    UI64 tempb = temp11;

                    UI32 endianTestValue = 0x78563412;
                    char *p              = (char *) &endianTestValue;

                    if (0x12 == p[0])
                    {
                        UI32 temp2 = 0;

                        temp2 = temp1 & 0xFFFFFFFF;
                        temp2 = htonl (temp2);

                        tempp = temp2;

                        tempp <<= 32;

                        temp2 = (temp1 & 0xFFFFFFFF00000000ULL) >> 32;
                        temp2 = htonl (temp2);

                        tempp |= temp2;



                        temp2 = temp11 & 0xFFFFFFFF;
                        temp2 = htonl (temp2);

                        tempb = temp2;

                        tempb <<= 32;

                        temp2 = (temp11 & 0xFFFFFFFF00000000ULL) >> 32;
                        temp2 = htonl (temp2);

                        tempb |= temp2;
                    }

                    map<UI32, ofp100_match>::iterator element    = s_ofpMatchMap.begin ();
                    map<UI32, ofp100_match>::iterator endElement = s_ofpMatchMap.end   ();

                    UI32 flowIndex = 0;

                    while (element != endElement)
                    {
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].length = htons (sizeof (ofp100_flow_stats));
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].table_id = pOpenFlowSwitchFlowStatsRequestMessage->table_id;
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].match = element->second;
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].duration_sec = htonl (100);
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].duration_nsec = htonl (0);
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].priority = htons (element->first);
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].idle_timeout = htons (0);
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].hard_timeout = htons (0);

                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].cookie       = s_cookieMap[element->first];

                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].packet_count = tempp;
                        pOpenFlowSwitchFlowStatsReplyMessage[flowIndex].byte_count = tempb;

                        flowIndex++;
                        element++;
                    }

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchStatsReplyMessage,       sizeof (openFlowSwitchStatsReplyMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 2 1");
                    }

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) pOpenFlowSwitchFlowStatsReplyMessage, sizeof (ofp100_flow_stats) * numberOfOpenFlowFlows);

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 2 2");
                    }
                }
                else if (2 == pOpenFlowStatsRequest->type)
                {
                    trace (TRACE_LEVEL_WARN, "Serving Stats Reply 2 ...");

                    ofp_stats_reply openFlowSwitchStatsReplyMessage;
                    ofp_aggregate_stats_reply  openFlowSwitchAggregateStatsMessage;

                    openFlowSwitchStatsReplyMessage.header.version = OpenFlow::m_openFlow10Version;
                    openFlowSwitchStatsReplyMessage.header.type    = 17;
                    openFlowSwitchStatsReplyMessage.header.length  = htons (sizeof (openFlowSwitchStatsReplyMessage) + sizeof (openFlowSwitchAggregateStatsMessage));
                    openFlowSwitchStatsReplyMessage.header.xid     = openFlowHelloMessage.header.xid;

                    openFlowSwitchStatsReplyMessage.type  = htons (pOpenFlowStatsRequest->type);
                    openFlowSwitchStatsReplyMessage.flags = htons (pOpenFlowStatsRequest->flags);

                    openFlowSwitchAggregateStatsMessage.packet_count = 0;
                    openFlowSwitchAggregateStatsMessage.byte_count   = 0;
                    openFlowSwitchAggregateStatsMessage.flow_count   = htonl (2);

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchStatsReplyMessage,       sizeof (openFlowSwitchStatsReplyMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 1 1");
                    }

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchAggregateStatsMessage, sizeof (openFlowSwitchAggregateStatsMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 1 2");
                    }
                }
                else if (4 == pOpenFlowStatsRequest->type)
                {
                    ofp100_flow_stats_request *pOpenFlowSwitchFlowStatsRequestMessage = (ofp100_flow_stats_request *) pTempBuffer->getPRawBuffer ();
                    ofp100_port_stats          openFlowSwitchPortStatsReplyMessage[2];
                    ofp_stats_reply openFlowSwitchStatsReplyMessage;

                    prismAssert (NULL != pOpenFlowSwitchFlowStatsRequestMessage, __FILE__, __LINE__);

                    openFlowSwitchStatsReplyMessage.header.version = OpenFlow::m_openFlow10Version;
                    openFlowSwitchStatsReplyMessage.header.type    = 17;
                    openFlowSwitchStatsReplyMessage.header.length  = htons (sizeof (openFlowSwitchStatsReplyMessage) + sizeof (ofp100_port_stats) * 2);
                    openFlowSwitchStatsReplyMessage.header.xid     = openFlowHelloMessage.header.xid;

                    openFlowSwitchStatsReplyMessage.type  = htons (pOpenFlowStatsRequest->type);
                    openFlowSwitchStatsReplyMessage.flags = htons (pOpenFlowStatsRequest->flags);

                    memset (&openFlowSwitchPortStatsReplyMessage, 0, sizeof (openFlowSwitchPortStatsReplyMessage));

                    UI64 temp11;

                    temp1 += 1023;
                    temp11 = temp1 * 8;

                    UI64 tempp = temp1;
                    UI64 tempb = temp11;

                    UI32 endianTestValue = 0x78563412;
                    char *p              = (char *) &endianTestValue;

                    if (0x12 == p[0])
                    {
                        UI32 temp2 = 0;

                        temp1 += 1023;
                        temp11 = temp1 * 8;

                        temp2 = temp1 & 0xFFFFFFFF;
                        temp2 = htonl (temp2);

                        tempp = temp2;

                        tempp <<= 32;

                        temp2 = (temp1 & 0xFFFFFFFF00000000ULL) >> 32;
                        temp2 = htonl (temp2);

                        tempp |= temp2;

                        temp2 = temp11 & 0xFFFFFFFF;
                        temp2 = htonl (temp2);

                        tempb = temp2;

                        tempb <<= 32;

                        temp2 = (temp11 & 0xFFFFFFFF00000000ULL) >> 32;
                        temp2 = htonl (temp2);

                        tempb |= temp2;
                    }

                    openFlowSwitchPortStatsReplyMessage[0].port_no = htons (1);
                    openFlowSwitchPortStatsReplyMessage[0].rx_packets = tempp;
                    openFlowSwitchPortStatsReplyMessage[0].rx_bytes = tempb;
                    openFlowSwitchPortStatsReplyMessage[0].tx_packets = tempp;
                    openFlowSwitchPortStatsReplyMessage[0].tx_bytes = tempb;

                    openFlowSwitchPortStatsReplyMessage[1].port_no = htons (2);
                    openFlowSwitchPortStatsReplyMessage[1].rx_packets = tempp;
                    openFlowSwitchPortStatsReplyMessage[1].rx_bytes = tempb;
                    openFlowSwitchPortStatsReplyMessage[1].tx_packets = tempp;
                    openFlowSwitchPortStatsReplyMessage[1].tx_bytes = tempb;


                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchStatsReplyMessage,       sizeof (openFlowSwitchStatsReplyMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Port 3 1");
                    }

                    isSuccessful = pOpenFlowControllerClientStreamingSocket->send ((UI8 *) &openFlowSwitchPortStatsReplyMessage, sizeof (openFlowSwitchPortStatsReplyMessage));

                    if (true != isSuccessful)
                    {
                        trace (TRACE_LEVEL_ERROR, " Sending failed for Stats 3 2");
                    }
                }
            }
        }
    }
}

}
