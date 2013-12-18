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
 *   Author : rjain                                                     *
 **************************************************************************/

#ifndef PIMRTRCFGLOCALMANAGEDOBJECT_H
#define PIMRTRCFGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Attributes/AttributeUI32UC.h"
#include "Pim/Local/PimTypes.h"

using namespace WaveNs;

namespace DcmNs
{
    class PimLocalObjectManager;
    class BsrCandidateLocalManagedObject;
    class RpCandidateLocalManagedObject;
	class RpCandidatePrefixLocalManagedObject;
    class StaticRpLocalManagedObject;
    class PimRtrCfgLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
        public:
            PimRtrCfgLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager);
            virtual    ~PimRtrCfgLocalManagedObject ();
            static string getClassName();
            void setPresenceFlag(const bool &presenceFlag);
            bool getPresenceFlag()  const;
            void setVrfId(const string &vrfId);
            string getVrfId()  const;
            void setMaxMcache(const UI32 &maxMcache, bool userConfig = false);
            UI32 getMaxMcache()  const;
            void setHelloTimer(const SI16 &helloTimer);
            SI16 getHelloTimer()  const;
            void setNbrTimeout(const SI16 &NbrTimeout);
            SI16 getNbrTimeout()  const;
            void setInactivityTimer(const SI16 &inactivityTimer);
            SI16 getInactivityTimer()  const;
            void setMsgInterval(const UI32 &msgInterval, bool userConfig = false);
            UI32 getMsgInterval()  const;
            void setSptThresholdInt(const UI32 &sptThreshold);
            UI32 getSptThresholdInt()  const;
            void setSptThresholdInf(const PimSptVal &sptThreshold);
            PimSptVal mapSptIntToEnumVal(const UI32 &sptThreshold);
            PimSptVal getSptThresholdInf()  const;
            void setSptThresholdUnion(const WaveUnion &sptThreshold);
            WaveUnion getSptThresholdUnion()  const;
            UI32 getSptThreshold(const WaveUnion &sptThreshold) const;
            void setBsrCandidateCfg(const vector<WaveManagedObjectPointer
				<BsrCandidateLocalManagedObject> > &BsrCandidateCfg);
            void setRpCandidateCfg(const vector<WaveManagedObjectPointer
				<RpCandidateLocalManagedObject> > &RpCandidateCfg);
			void setRpCandidatePrefixCfg(const vector<WaveManagedObjectPointer
				<RpCandidatePrefixLocalManagedObject> > &RpCandidatePrefixCfg);
            void setStaticRpCfg(const vector<WaveManagedObjectPointer
				<StaticRpLocalManagedObject> > &StaticRpCfg);
            vector<WaveManagedObjectPointer<BsrCandidateLocalManagedObject> > 
				getBsrCandidateCfg()  const;
            vector<WaveManagedObjectPointer<RpCandidateLocalManagedObject> > 
				getRpCandidateCfg()  const;
			vector<WaveManagedObjectPointer
				<RpCandidatePrefixLocalManagedObject> > 
				getRpCandidatePrefixCfg()  const;
            vector<WaveManagedObjectPointer<StaticRpLocalManagedObject> > 
				getStaticRpCfg()  const;
            
            /* Forward declaration of utility functions */
			void addBsrCand(BsrCandidateLocalManagedObject *pMo);
			void delBsrCand(const UI8 intfType, const string intfName);
            BsrCandidateLocalManagedObject* getBsrCandMo(const UI8 intfType, 
                const string intfName);
            void delStaticRp(const IpV4Address rpAddr);
            void addStaticRp(StaticRpLocalManagedObject *pMo);
            StaticRpLocalManagedObject* getStaticRpMo(const IpV4Address rpAddr);
            void delCandRp(const UI8 intfType, const string intfName);
            void addCandRp(RpCandidateLocalManagedObject *pMo);
            RpCandidateLocalManagedObject* getCandRpMo(const UI8 intfType, 
                const string intfName);
			void delCandRpPrefix(const IpV4Address prefixIP, 
				const UI16 prefixLen);
            void addCandRpPrefix(RpCandidatePrefixLocalManagedObject *pMo);
            RpCandidatePrefixLocalManagedObject* getCandRpPrefixMo(
				const IpV4Address prefixIP, const UI16 prefixLen);

// Now the data members
        private:
        protected:
        public:
            bool  m_presenceFlag;
            string  m_vrfId;
            UI32UC  m_maxMcache;
            SI16  m_helloTimer;
            SI16  m_NbrTimeout;
            SI16  m_inactivityTimer;
            UI32UC  m_msgInterval;
            UI32  m_sptThresholdInt;
            PimSptVal m_sptThresholdInf;
            WaveUnion m_sptThresholdUnion;
            vector<WaveManagedObjectPointer
				<BsrCandidateLocalManagedObject> >  m_BsrCandidateCfg;
            vector<WaveManagedObjectPointer
				<RpCandidateLocalManagedObject> >  m_RpCandidateCfg;
            vector<WaveManagedObjectPointer
				<RpCandidatePrefixLocalManagedObject> >  m_RpCandidatePrefixCfg;
			vector<WaveManagedObjectPointer
				<StaticRpLocalManagedObject> >  m_StaticRpCfg;
			
    };
}
#endif                                            //PIMRTRCFGLOCALMANAGEDOBJECT_H
