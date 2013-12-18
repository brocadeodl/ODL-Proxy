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

/**************************************************************************
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#ifndef VCSFABRICLOCALMESSAGES
#define VCSFABRICLOCALMESSAGES

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsFabricGetSwitchIdentifierInfoMessage : public PrismMessage
{
    private :
        void            setupAttributesForSerialization         ();

    protected :
    public :
                        VcsFabricGetSwitchIdentifierInfoMessage ();
        virtual        ~VcsFabricGetSwitchIdentifierInfoMessage ();

        string          getSwitchname                           () const;
        void            setSwitchname                           (string switchname);

        string          getEth0IpAddress                        () const;
        void            setEth0IpAddress                        (string eth0IpAddress);

        string          getEth0IPv6Address                      () const;
        void            setEth0IPv6Address                      (string eth0IPv6Address);


    // Now the data members

    private :
        string          m_switchname;
        string          m_eth0IpAddress;
        string          m_eth0IPv6Address;

    protected :
    public :
};

class VcsNodeResetDomainBitMapMessage : public PrismMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization (); 
    public:

                     VcsNodeResetDomainBitMapMessage (); 
                    ~VcsNodeResetDomainBitMapMessage (); 

                void setIsWarmRecoveryInvolved       (const bool &isWarmRecovery);
                bool getIsWarmRecoveryInvolved       ();

    // Now the data member
    private:
                bool  m_isWarmRecovery;
    protected:
    public:
};

class VcsNodePrepareDomainListAfterHaRecoveryMessage : public PrismMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization (); 
    public:

                     VcsNodePrepareDomainListAfterHaRecoveryMessage ();
                    ~VcsNodePrepareDomainListAfterHaRecoveryMessage ();

                void setIsWarmRecoveryInvolved       (const bool &isWarmRecovery);
                bool getIsWarmRecoveryInvolved       ();

    // Now the data member
    private:
                bool m_isWarmRecovery;
    protected:
    public:
};

class VcsNodeEnableSlotScnHandlingMessage : public PrismMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization (); 
    public:

                     VcsNodeEnableSlotScnHandlingMessage ();
                    ~VcsNodeEnableSlotScnHandlingMessage ();

    // Now the data member
    private:
    protected:
    public:
};

class VcsNodeConfigSnapshotMessage : public PrismMessage
{
    private:
    protected:
        virtual void        setupAttributesForSerialization ();
    public:    
                            VcsNodeConfigSnapshotMessage    (const UI32 &opcode);
                           ~VcsNodeConfigSnapshotMessage    ();
        
                void        setSnapshotId                   (const string& snapshotId );
                string      getSnapshotId                   () const;

                void        setRbridgeId                    (const UI32 &rbridgeId );
                UI32        getRbridgeId                    () const;                                    

                void        setIsNodePrimary                (const bool &isNodePrimary);
                bool        getIsNodePrimary                () const;

    //Now the data Members
    private:
                string      m_snapshotId;
                UI32        m_rbridgeId;
                bool        m_isNodePrimary;
    protected:
    public:

};

class VcsFabricCollectSwitchIdentifierInfoMessage : public PrismMessage
{
    private :
        void            setupAttributesForSerialization         ();

    protected :
    public :
                        VcsFabricCollectSwitchIdentifierInfoMessage ();
        virtual        ~VcsFabricCollectSwitchIdentifierInfoMessage ();

        vector<UI32>    getRbridgeIdVector                      () const;
        void            setRbridgeIdVector                      (vector<UI32> rbridgeIdVector);

        vector<string>  getSwitchnameVector                     () const;
        void            setSwitchnameVector                     (vector<string> switchnameVector);

        vector<string>  getEth0IpAddressVector                  () const;
        void            setEth0IpAddressVector                  (vector<string> eth0IpAddressVector);

        vector<string>  getEth0IPv6AddressVector                () const;
        void            setEth0IPv6AddressVector                (vector<string> eth0IPv6AddressVector);


    // Now the data members

    private :
        vector<UI32>    m_rbridgeIdVector;
        vector<string>  m_switchnameVector;
        vector<string>  m_eth0IpAddressVector;
        vector<string>  m_eth0IPv6AddressVector;

    protected :
    public :
};

class VirtualIpLocalShowVipMessage : public PrismMessage
{
    private:
    protected:
        virtual void setupAttributesForSerialization();
    public:
        VirtualIpLocalShowVipMessage ();
        VirtualIpLocalShowVipMessage (const string &VirtualIpAddress, const UI32 &pNodeId);
        virtual ~VirtualIpLocalShowVipMessage ();
        void setVirtualIpAddress(const string &VirtualIpAddress);
        string getVirtualIpAddress()  const;
        void setpNodeId(const UI32 &pNodeId);
        UI32 getpNodeId()  const;

// Now the data members

    private:
    protected:
    public:
        string  m_VirtualIpAddress;
        UI32    m_pNodeId;
};

class VcsNodeHaSyncConfigToKernelMessaage : public PrismMessage
{   
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeHaSyncConfigToKernelMessaage  (const string &globalConfigs);
                        VcsNodeHaSyncConfigToKernelMessaage  ();
        virtual        ~VcsNodeHaSyncConfigToKernelMessaage  ();

                void    setGlobalConfigs                                        (const string &globalConfigs);
                string  getGlobalConfigs                                        () const;

    // Now the data members

    private :
        string m_globalConfigs;

    protected :
    public :
};

}

#endif // VCSFABRICLOCALMESSAGES

