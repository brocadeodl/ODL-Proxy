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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : statpatt                                                     *
 **************************************************************************/


#ifndef UDLDGLOBALCONFIGMANAGEDOBJECT_H
#define UDLDGLOBALCONFIGMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Attributes/Attributes.h"



using namespace WaveNs;


namespace DcmNs{

class UdldGlobalObjectManager;


class UdldGlobalConfigManagedObject : virtual public DcmManagedObject
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            UdldGlobalConfigManagedObject                           (UdldGlobalObjectManager *pUdldGlobalObjectManager);
                                                            UdldGlobalConfigManagedObject                           (UdldGlobalObjectManager *pUdldGlobalObjectManager,
                                                                                                                const bool &UdldPresent,
                                                                                                                const UI32 &Hello,
                                                                                                                const UI32 &Multiplier,
                                                                                                                const bool &Shutdown);
                static string                               getClassName                                            ();
                virtual void                                setupKeys                                               ();
                virtual bool                                updateHardwareBeforeWaveManagedObject                   (const vector<Attribute *> &attributeVector);
                virtual bool                                getPluginDetailsForDistribution                         (ObjectId &newOperateOnWaveManagedObjectId, PrismServiceId &prismServiceId, vector<LocationId> &locationIds, bool &isNeedSurrogateSupportFlag, bool &isPartialSuccessFlag);
                virtual void                                isAssociatedManagedObjectToBeCreated                    (IsAssociatedManagedObjectToBeCreatedContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForCreate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForUpdate                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                addNewAttributeToBeUpdatedForDelete                     (UpdateNewAttributeInManagedObjectContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForCreate                (GetHardwareConfigurationDetailsForCreateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForUpdate                (GetHardwareConfigurationDetailsForUpdateContext *pContext);
                virtual void                                getHardwareConfigurationDetailsForDelete                (GetHardwareConfigurationDetailsForDeleteContext *pContext);
                virtual UI32                                getManagedObjectUserTag                                 () const;
                virtual void                                setHardwareConfigurationDetails                         (map <string, UI32> &attributeNamesGroupCodeMap, map <UI32, string> &groupCodeBackendNamesMap, map <UI32, vector <string> > &groupCodeAttributeGroupMap);
                virtual void                                setPostbootHardwareConfigurationDetails                 (map<UI32, vector<string> > &backendAttributesMap, map<UI32, string > &groupCodeMap, map<UI32, bool> &groupTypeMap, map<UI32, map<UI32, vector<UI32> > > &choiceToCasesMap);
                virtual void                                getHardwareConfigurationDetailsForPostboot              (GetHardwareConfigurationDetailsForPostbootContext *pContext);
                virtual                                     ~UdldGlobalConfigManagedObject                          ();
                void                                        setUdldPresent                                          (const bool &UdldPresent);
                bool                                        getUdldPresent                                          ();
                void                                        setHello                                                (const UI32 &Hello);
                UI32                                        getHello                                                ();
                void                                        setMultiplier                                           (const UI32 &Multiplier);
                UI32                                        getMultiplier                                           ();
                void                                        setShutdown                                             (const bool &Shutdown);
                bool                                        getShutdown                                             ();

                // Added to notify NSM upon "no proto udld"
                virtual void isManagedObjectsAssociatedToCurrentMONeedsToBeDeleted (WaveManagedObjectDeleteContext *pContext);
    
    private   :
    protected :
    public    :
        bool                                            m_UdldPresent;
        UI32                                            m_Hello;
        UI32                                            m_Multiplier;
        bool                                            m_Shutdown;
};
 
}
#endif
