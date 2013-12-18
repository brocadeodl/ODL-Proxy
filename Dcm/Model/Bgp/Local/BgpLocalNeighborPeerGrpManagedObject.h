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


#ifndef BGPLOCALNEIGHBORPEERGRPMANAGEDOBJECT_H
#define BGPLOCALNEIGHBORPEERGRPMANAGEDOBJECT_H


#include "DcmObjectModel/DcmManagedObject.h"
#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Attributes/Attributes.h"

#include "Bgp/Local/BgpLocalNeighborAttributesCommonManagedObjectBase.h"


using namespace WaveNs;


namespace DcmNs{

class BgpLocalObjectManager;


class BgpLocalNeighborPeerGrpManagedObject : virtual public DcmLocalManagedObject ,virtual public BgpLocalNeighborAttributesCommonManagedObjectBase
{
    private   :
    protected :
                virtual void                                setupAttributesForPersistence                           ();
                virtual void                                setupAttributesForCreate                                ();
    public    :
                                                            BgpLocalNeighborPeerGrpManagedObject                    (BgpLocalObjectManager *pBgpLocalObjectManager);
                                                            BgpLocalNeighborPeerGrpManagedObject                    (BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const string &neighborPeerGrpName,
                                                                                                                const bool &peerGrpFlag);
                static string                               getClassName                                            ();
                virtual void                                setupKeys                                               ();
                virtual void                                validateForUpdateAtThisNode                             (ValidateForUpdateAtThisNodeContext *pContext);
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
                virtual                                     ~BgpLocalNeighborPeerGrpManagedObject                   ();
                void                                        setneighborPeerGrpName                                  (const string &neighborPeerGrpName);
                string                                      getneighborPeerGrpName                                  ();
                void                                        setpeerGrpFlag                                          (const bool &peerGrpFlag);
                bool                                        getpeerGrpFlag                                          ();
                void CleanupAssociatedPeerGroups(UpdateNewAttributeInManagedObjectContext *pContext, Attribute *pAttribute);
                void CleanupAssociatedNeighborPeerGroupMos(UpdateNewAttributeInManagedObjectContext *pContext);
    private   :
    protected :
    public    :
        string                                          m_neighborPeerGrpName;
        bool                                            m_peerGrpFlag;
};
 
}
#endif
