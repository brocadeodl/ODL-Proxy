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
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#ifndef VRFUTILS_H
#define VRFUTILS_H

#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Vrf/Local/VrfLocalMessages.h"
#include "Vrf/Local/VrfAfIpv4UcastManagedObject.h"
#include "Vrf/Local/VrfRouteTargetManagedObject.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForMultiPartitionCleanup.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"

using namespace WaveNs;
using namespace std;

#define MAX_VRF_COUNT 127
#define MIN_VRF_COUNT 31 

namespace DcmNs
{
    class VrfLocalManagedObject;

    typedef enum RT_ACTION {
        RT_PROCEED = 0,
        RT_EXISTS
    }RT_ACTION_T;

class VrfUtils: public WaveObjectManagerToolKit
{
    private :
        VrfUtils ();

    protected :
    public :
        static ResourceId   backendVRFCreate(const string &vrfName);
        static ResourceId   backendVRFDelete(const string &vrfName);
        static ResourceId   sendVrfInstance(const string &vrfName, uint32_t opcode);
        static ResourceId   getVrfObjectIdByName(string vrfname, ObjectId &objid, const bool validate=true, LocationId location = 0);
        static string       getVrfNameByObjectId(const ObjectId objid);
        static ResourceId getObjectIdByVrfName(const string &vrfName, ObjectId &oid, LocationId location = 0);
        static ResourceId getVrfNameByObjectId(const ObjectId &oid, string &vrfName);
        static bool isVrfExists(const string &vrfName, LocationId location = 0);
        static bool isRdExists(const string &vrfName, LocationId location = 0);
        static ResourceId backendVRFUpdate (VrfLocalAddVrfMessage *m);
        static ResourceId sendToBackEnd(DcmManagementInterfaceMessage *m,const char *client);
        static ResourceId getVrfMo(string vrfName, VrfLocalManagedObject *&mo, LocationId location = 0);
        static ResourceId getVrfAfIpv4Mo(string vrfName, VrfAfIpv4UcastManagedObject *&mo, LocationId location = 0);
        static ResourceId getVrfAfRtMo(string vrfName, RtmMessageConfigRtType rtType, string rt, VrfRouteTargetAfManagedObject *mo[], LocationId location = 0); 
        static ResourceId getVrfAfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage, VrfRouteTargetAfManagedObject *mo[], LocationId location = 0);  
        static ResourceId getVrfRtMo(string vrfName, RtmMessageConfigRtType rtType, string rt, VrfRouteTargetManagedObject *mo[], LocationId location = 0) ;
        static ResourceId getVrfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage, VrfRouteTargetManagedObject *mo[], LocationId location = 0); 
        static ResourceId getVrfObjectIdByNameAndValidate(string vrfName, ObjectId &objid,LocationId locId);
        static void restoreVrfRtmConfig(VrfLocalManagedObject *pMo);
        static void restoreRd(VrfLocalManagedObject *pMo);
        static void restoreRouterId(VrfLocalManagedObject *pMo);
        static void restoreVrfAfIpv4(VrfAfIpv4UcastManagedObject *mo,  const  string &vrfName);
        static void restoreVrfAfIpv4Rt(vector<WaveManagedObjectPointer<VrfRouteTargetAfManagedObject> > rt);
        static void restoreVrfRt(vector<WaveManagedObjectPointer<VrfRouteTargetManagedObject> > rt);
        static ObjectId getVrfMoObjIdByPartitionMoObjId(ObjectId &ownerPartitionManagedObjectId);
        static RT_ACTION_T recalculateRtType(const string &className, const string &vrfName, const string &rt, RtmMessageConfigRtType &rtType, LocationId location);
        static string getVrfNameFromOwnerPartitionManagedObjectId(const ObjectId & oid);
        static void setPartitionName(PrismMessage *pPrismMessage, const string  & partitionName);
        static ResourceId validateVRFBinding(const ObjectId &oid, LocationId locationi = 0);

    // Now the data members
    private :
    protected :
    public :
};

class MultiPartitionCleanup :public WaveLocalObjectManager
{
 public:
     MultiPartitionCleanup(const string objectManagerName,vector<string> managedClass);
     ~MultiPartitionCleanup();
     virtual void multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);
 private:
   vector<string> m_managedClasses;
 protected:
};

}

#endif // VRFUTILS_H
