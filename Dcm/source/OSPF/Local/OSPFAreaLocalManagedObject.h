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
 *   Author : kverma                                                     *
 **************************************************************************/

#ifndef OSPFAREALOCALMANAGEDOBJECT_H
#define OSPFAREALOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/Types/IpV4Address.h"

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Framework/Attributes/AttributeBoolUC.h"

using namespace WaveNs;

namespace DcmNs
{

    class OSPFLocalObjectManager;
    class OSPFAreaRangeLocalManagedObject;
    class OSPFVirtualLinkLocalManagedObject;
	class OSPFAreaMessage;
	class OSPFAreaLocalMessage;

    class OSPFAreaLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
            virtual void setupKeys();
        public:
            OSPFAreaLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager);
            OSPFAreaLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const string &areaId,const bool &Normal, const bool &isStub,const UI32 &stubMetric,const bool &stubNoSummary,const bool &isNSSA,const UI32 &NSSAMetric,const bool &NSSADefInfoOrig,const bool &NSSANoSummary,const vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> > &range,const vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> > &virtualLink);
            virtual    ~OSPFAreaLocalManagedObject ();
            static string getClassName();
            void setAreaId(const string &areaId);
            string getAreaId()  const;
            void setNormal(const bool &Normal, const bool &set);
            bool getNormal()  const;
            void setIsStub(const bool &isStub);
            bool getIsStub()  const;
            void setStubMetric(const UI32 &stubMetric, const bool &set);
            UI32 getStubMetric()  const;
            void setStubNoSummary(const bool &stubNoSummary, const bool &set);
            bool getStubNoSummary()  const;
            void setIsNSSA(const bool &isNSSA);
            bool getIsNSSA()  const;
            void setNSSAMetric(const UI32 &NSSAMetric, const bool &set);
            UI32 getNSSAMetric()  const;
            void setNSSADefInfoOrig(const bool &NSSADefInfoOrig, const bool &set);
            bool getNSSADefInfoOrig()  const;
            void setNSSANoSummary(const bool &NSSANoSummary, const bool &set);
            bool getNSSANoSummary()  const;
            void setAreaIdInIntFormat();
            UI64 convertOSPFAreaToInteger();
            UI64 getAreaIdInIntFormat() const;
            void setRange(const vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> > &range);
			OSPFAreaRangeLocalManagedObject * getAreaRangeLocalMO(IpV4Address rangeIp, IpV4Address rangeMask);
			void setAreaRangeLocalMO(OSPFAreaRangeLocalManagedObject *areaRangeMO);
			void deleteAreaRangeMO(OSPFAreaRangeLocalManagedObject *rangeMO);

            vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> > getRange()  const;
            void setVirtualLink(const vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> > &virtualLink);
            vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> > getVirtualLink()  const;
            void setVirtualLinkLocalMO(OSPFVirtualLinkLocalManagedObject *virtualLinkMO);
            OSPFVirtualLinkLocalManagedObject * getVirtualLinkLocalMO(IpV4Address &virtualLinkRouterId);
            void deleteVirtualLinkLocalMO(OSPFVirtualLinkLocalManagedObject *pVirtualLinkLocalMO);

			void updateMOFromLocalMessage(OSPFAreaLocalMessage *pMsg);
			void updateClientMessageForReplay(OSPFAreaMessage *pMsg);

// Now the data members

        private:
        protected:
        public:
			string m_areaId;
            BoolUC  m_Normal;
            bool  m_isStub;
            UI32UC  m_stubMetric;
            BoolUC  m_stubNoSummary;
            bool  m_isNSSA;
            UI32UC  m_NSSAMetric;
            BoolUC  m_NSSADefInfoOrig;
            BoolUC  m_NSSANoSummary;
            UI64    m_areaIdInIntFormat;
            vector<WaveManagedObjectPointer<OSPFAreaRangeLocalManagedObject> >  m_range;
            vector<WaveManagedObjectPointer<OSPFVirtualLinkLocalManagedObject> >  m_virtualLink;
    };
}
#endif                                            //OSPFAREALOCALMANAGEDOBJECT_H
