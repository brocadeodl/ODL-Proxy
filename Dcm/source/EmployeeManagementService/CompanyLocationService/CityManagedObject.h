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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 **************************************************************************/

#ifndef CITYMANAGEDOBJECT_H
#define CITYMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/IpV6Address.h"   
#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/IpVxAddress.h"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "Framework/Types/IpV6AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"   
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"   
#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"   
#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "CityLocalObjectManager.h"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class CityObjectManager;

    class CityManagedObject : virtual public DcmManagedObject
    {
        private:
                    void        getAttributeGroupsInformation               (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeattributeGroupMap, map <string, string> &attributeNameBackendNameMap); 

        protected:
            virtual void        setupAttributesForPersistence               ();
            virtual void        setupAttributesForCreate                    ();
        public:
                                CityManagedObject                           (CityObjectManager *pCityObjectManager);
            virtual            ~CityManagedObject                           ();
            static  string      getClassName                                ();

                    void        setCityName                                 (const string &cityName);
                    string      getCityName                                 ()  const;

                    void        setCityCode                                 (const UI32 cityCode);
                    UI32        getCityCode                                 ()  const;

                    void        setPopulation                               (const UI64 &population);
                    UI64        getPopulation                               () const;

                    void        setIsMetro                                  (const bool isMetro);
                    bool        getIsMetro                                  () const;

                    void                     setCityIpV4Address             (const IpV4Address cityIpV4Address);                                         
                    IpV4Address              getCityIpV4Address             () const;

                    void                     setCityIpV4AddressNetworkMask  (const IpV4AddressNetworkMask cityIpV4AddressNetworkMask);                
                    IpV4AddressNetworkMask   getCityIpV4AddressNetworkMask  () const;  
                    
                    void                     setCityIpV6Address             (const IpV6Address cityIpV6Address);                                       
                    IpV6Address              getCityIpV6Address             () const;                                                                 
                   
                    void                     setCityIpV6AddressNetworkMask  (const IpV6AddressNetworkMask cityIpV6AddressNetworkMask);  
                    IpV6AddressNetworkMask   getCityIpV6AddressNetworkMask  () const;   
                    
                    void                     setCityIpVxAddress             (const IpVxAddress cityIpVxAddress);   
                    IpVxAddress              getCityIpVxAddress             () const; 

            virtual void        validateForDeleteAtThisNode                 (ValidateForCreateAtThisNodeContext *pContext);
            virtual bool        updateHardwareBeforeWaveManagedObject       (const vector<Attribute *> &attributeVector);

        virtual void            getHardwareConfigurationDetailsForUpdate    (GetHardwareConfigurationDetailsForUpdateContext *pContext);
        virtual void            getHardwareConfigurationDetailsForCreate    (GetHardwareConfigurationDetailsForCreateContext *pContext);
        virtual void            getHardwareConfigurationDetailsForDelete    (GetHardwareConfigurationDetailsForDeleteContext *pContext);
        virtual void            getHardwareConfigurationDetailsForPostboot  (GetHardwareConfigurationDetailsForPostbootContext *pContext);
        virtual void            addNewAttributeToBeUpdatedForCreate         (UpdateNewAttributeInManagedObjectContext *pUpdateNewAttributeInManagedObjectContext);
        virtual bool            getPluginDetailsForDistribution             (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag);
        
// Now the data members

        private:
        protected:
        public:
                    string      m_cityName;
                    UI32UC      m_cityCode;
                    UI64        m_population;
                    BoolUC      m_isMetro;
                    IpV4Address                m_cityIpV4Address;
                    IpV4AddressNetworkMask     m_cityIpV4AddressNetworkMask;
                    IpV6Address                m_cityIpV6Address;      
                    IpV6AddressNetworkMask     m_cityIpV6AddressNetworkMask;
                    IpVxAddress                m_cityIpVxAddress;
    };
}
#endif //CITYMANAGEDOBJECT_H

