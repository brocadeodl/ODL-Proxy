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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/


#ifndef BGPLOCALAFIPV4NETWORKMANAGEDOBJECT_H
#define BGPLOCALAFIPV4NETWORKMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{

class BgpLocalObjectManager;


class BgpLocalAfIpv4NetworkManagedObject : virtual public DcmLocalManagedObject
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalAfIpv4NetworkManagedObject                      (BgpLocalObjectManager *pBgpLocalObjectManager);
                                                            BgpLocalAfIpv4NetworkManagedObject                      (BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const IpV4AddressNetworkMask &networkIpv4AddressPrefixMask,
                                                                                                                const UI32 &networkWeight,
                                                                                                                const bool &networkbackDoorFlag,
                                                                                                                const string &networkRouteMapName);
                static string                               getClassName                                            ();
                virtual void                                setupKeys                                               ();
                virtual void                                isAssociatedManagedObjectToBeCreated                    (IsAssociatedManagedObjectToBeCreatedContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForCreate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForUpdate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForDelete                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForCreate                (GetHardwareConfigurationDetailsForCreateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForUpdate                (GetHardwareConfigurationDetailsForUpdateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForDelete                (GetHardwareConfigurationDetailsForDeleteContext *pContext);
                virtual void                                setHardwareConfigurationDetails                         (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap);
                virtual void                                setPostbootHardwareConfigurationDetails                 (map<UI32, vector<string> > &backendAttributesMap, map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap);
                virtual void                                getHardwareConfigurationDetailsForPostboot              (GetHardwareConfigurationDetailsForPostbootContext *pContext);
                virtual                                     ~BgpLocalAfIpv4NetworkManagedObject                     ();
                void                                        setnetworkIpv4AddressPrefixMask                         (const IpV4AddressNetworkMask &networkIpv4AddressPrefixMask);
                IpV4AddressNetworkMask                      getnetworkIpv4AddressPrefixMask                         ();
                void                                        setnetworkWeight                                        (const UI32 &networkWeight);
                UI32                                        getnetworkWeight                                        ();
                void                                        setnetworkbackDoorFlag                                  (const bool &networkbackDoorFlag);
                bool                                        getnetworkbackDoorFlag                                  ();
                void                                        setnetworkRouteMapName                                  (const string &networkRouteMapName);
                string                                      getnetworkRouteMapName                                  ();
    private   :
    protected :
    public    :
        IpV4AddressNetworkMask                          m_networkIpv4AddressPrefixMask;
        UI32                                            m_networkWeight;
        bool                                            m_networkbackDoorFlag;
        string                                          m_networkRouteMapName;
};
 
}
#endif
