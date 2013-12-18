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

#include "Pim/Local/PimRtrCfgLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/BsrCandidateLocalManagedObject.h"
#include "Pim/Local/RpCandidateLocalManagedObject.h"
#include "Pim/Local/RpCandidatePrefixLocalManagedObject.h"
#include "Pim/Local/StaticRpLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "vcs.h"

namespace DcmNs
{

    PimRtrCfgLocalManagedObject::PimRtrCfgLocalManagedObject(
        PimLocalObjectManager *pPimLocalObjectManager)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (PimRtrCfgLocalManagedObject::getClassName (), 
            DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager)
    {
		/* Copy the default values in the fields when the MO gets created */
        m_maxMcache = PIM_DFLT_MCACHE;
        m_helloTimer = PIM_DFLT_HELLO_TIMER;
        m_NbrTimeout = PIM_DFLT_NBR_TO;
        m_inactivityTimer = PIM_DFLT_INACT_TO;
        m_msgInterval = PIM_DFLT_MSG_INTVL;
    }

    PimRtrCfgLocalManagedObject::~PimRtrCfgLocalManagedObject ()
    {
    }

    string  PimRtrCfgLocalManagedObject::getClassName()
    {
        return ("PimRtrCfgLocalManagedObject");
    }

    void  PimRtrCfgLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_presenceFlag,"presenceFlag"));
        addPersistableAttribute (new AttributeString(&m_vrfId,"vrfId"));
        addPersistableAttribute (new AttributeUI32UC(&m_maxMcache, true, "maxMcache"));
        addPersistableAttribute (new AttributeSI16(&m_helloTimer,"helloTimer"));
        addPersistableAttribute (new AttributeSI16(&m_NbrTimeout,"NbrTimeout"));
        addPersistableAttribute (new AttributeSI16(&m_inactivityTimer,"inactivityTimer"));
        addPersistableAttribute (new AttributeUI32UC(&m_msgInterval, true, "msgInterval"));
        addPersistableAttribute (new AttributeUI32(&m_sptThresholdInt,
            "sptThresholdInt"));
        addPersistableAttribute (new AttributeEnum((UI32*)(&m_sptThresholdInf),
            PIM_SPT_VAL_USR_DEF, "sptThresholdInf"));
        addPersistableAttribute (new AttributeUnion(&m_sptThresholdUnion,
            "sptThresholdUnion"));

		/* Removing support for BSR, RP-CAND AND PREFIX */


        //addPersistableAttribute (new AttributeManagedObjectVectorComposition
        //    <BsrCandidateLocalManagedObject>(&m_BsrCandidateCfg, 
        //    "BsrCandidateCfg", getClassName (), getObjectId (), 
        //    BsrCandidateLocalManagedObject::getClassName ()));
		//addPersistableAttribute (new AttributeManagedObjectVectorComposition
        //    <RpCandidateLocalManagedObject>(&m_RpCandidateCfg, "RpCandidateCfg", 
        //    getClassName (), getObjectId (), 
        //    RpCandidateLocalManagedObject::getClassName ()));
		//addPersistableAttribute (new AttributeManagedObjectVectorComposition
        //    <RpCandidatePrefixLocalManagedObject>(&m_RpCandidatePrefixCfg, 
		//	"RpCandidatePrefixCfg", getClassName (), getObjectId (),
        //    RpCandidatePrefixLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition
            <StaticRpLocalManagedObject>(&m_StaticRpCfg, "StaticRpCfg", 
            getClassName (), getObjectId (), 
            StaticRpLocalManagedObject::getClassName ()));

        /* Adding the XML tag mappings */
        setUserTagForAttribute("presenceFlag", pim_pim);
        //setUserTagForAttribute("vrfId", );
        setUserTagForAttribute("maxMcache", pim_max_mcache);
        setUserTagForAttribute("helloTimer", pim_hello_interval);
        setUserTagForAttribute("NbrTimeout", pim_nbr_timeout);
        setUserTagForAttribute("inactivityTimer", pim_inactivity_timer);
        setUserTagForAttribute("msgInterval", pim_message_interval);
        setUserTagForAttribute("sptThresholdUnion", pim_spt_threshold);
        setUserTagForAttribute("StaticRpCfg", pim_rp_address);
		//setUserTagForAttribute("BsrCandidateCfg", pim_bsr_candidate);
        //setUserTagForAttribute("RpCandidateCfg", pim_rp_candidate);
		//setUserTagForAttribute("RpCandidatePrefixCfg", pim_rp_cand_grp_prefix);
    }

    void  PimRtrCfgLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(
			&m_presenceFlag,"presenceFlag"));
        addPersistableAttributeForCreate  (new AttributeString(
			&m_vrfId,"vrfId"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_maxMcache,
            true, "maxMcache"));
        addPersistableAttributeForCreate  (new AttributeSI16(
			&m_helloTimer,"helloTimer"));
        addPersistableAttributeForCreate  (new AttributeSI16(
			&m_NbrTimeout,"NbrTimeout"));
        addPersistableAttributeForCreate  (new AttributeSI16(
			&m_inactivityTimer,"inactivityTimer"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_msgInterval, 
            true, "msgInterval"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_sptThresholdInt,
            "sptThresholdInt"));
        addPersistableAttributeForCreate (new AttributeEnum(
            (UI32*)(&m_sptThresholdInf), PIM_SPT_VAL_USR_DEF, 
			"sptThresholdInf"));
        addPersistableAttributeForCreate (new AttributeUnion(
			&m_sptThresholdUnion, "sptThresholdUnion"));
        //addPersistableAttributeForCreate(
		//	new AttributeManagedObjectVectorComposition
        //    <BsrCandidateLocalManagedObject>(&m_BsrCandidateCfg,
		//	"BsrCandidateCfg", getClassName (), getObjectId (), 
        //    BsrCandidateLocalManagedObject::getClassName ()));
		//addPersistableAttributeForCreate(
		//	new AttributeManagedObjectVectorComposition
        //    <RpCandidateLocalManagedObject>(&m_RpCandidateCfg,"RpCandidateCfg",
        //    getClassName (), getObjectId (), 
        //    RpCandidateLocalManagedObject::getClassName ()));
		//addPersistableAttributeForCreate(
		//	new AttributeManagedObjectVectorComposition
        //    <RpCandidatePrefixLocalManagedObject>(&m_RpCandidatePrefixCfg,
		//	"RpCandidatePrefixCfg", getClassName (), getObjectId (),
        //    RpCandidatePrefixLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate(
			new AttributeManagedObjectVectorComposition
            <StaticRpLocalManagedObject>(&m_StaticRpCfg,"StaticRpCfg", 
            getClassName (), getObjectId (), 
            StaticRpLocalManagedObject::getClassName ()));
    }

    void  PimRtrCfgLocalManagedObject::setPresenceFlag(const bool 
			&presenceFlag)
    {
        m_presenceFlag  =  presenceFlag;
    }

    bool  PimRtrCfgLocalManagedObject::getPresenceFlag() const
    {
        return (m_presenceFlag);
    }

    void  PimRtrCfgLocalManagedObject::setVrfId(const string &vrfId)
    {
        m_vrfId  =  vrfId;
    }

    string  PimRtrCfgLocalManagedObject::getVrfId() const
    {
        return (m_vrfId);
    }

    void  PimRtrCfgLocalManagedObject::setMaxMcache(const UI32 &maxMcache,
        bool userConfig)
    {
        m_maxMcache.setIsUserConfigured(userConfig);
        m_maxMcache.setUI32Value(maxMcache);
    }

    UI32  PimRtrCfgLocalManagedObject::getMaxMcache() const
    {
        if(m_maxMcache.getIsUserConfigured())
            return(m_maxMcache.getUI32Value());
        else
            return(PIM_NOT_USR_CFGED_VAL);
    }

    void  PimRtrCfgLocalManagedObject::setHelloTimer(const SI16 &helloTimer)
    {
        m_helloTimer  =  helloTimer;
    }

    SI16  PimRtrCfgLocalManagedObject::getHelloTimer() const
    {
        return (m_helloTimer);
    }

    void  PimRtrCfgLocalManagedObject::setNbrTimeout(const SI16 &NbrTimeout)
    {
        m_NbrTimeout  =  NbrTimeout;
    }

    SI16  PimRtrCfgLocalManagedObject::getNbrTimeout() const
    {
        return (m_NbrTimeout);
    }

    void  PimRtrCfgLocalManagedObject::setInactivityTimer(const SI16 &inactivityTimer)
    {
        m_inactivityTimer  =  inactivityTimer;
    }

    SI16  PimRtrCfgLocalManagedObject::getInactivityTimer() const
    {
        return (m_inactivityTimer);
    }

    void  PimRtrCfgLocalManagedObject::setMsgInterval(const UI32 &msgInterval,
        bool userConfig)
    {
        m_msgInterval.setIsUserConfigured(userConfig);
        m_msgInterval.setUI32Value(msgInterval);
    }

    UI32  PimRtrCfgLocalManagedObject::getMsgInterval() const
    {
        if(m_msgInterval.getIsUserConfigured())
            return (m_msgInterval.getUI32Value());
        else
            return(PIM_NOT_USR_CFGED_VAL);
    }

    UI32  PimRtrCfgLocalManagedObject::getSptThreshold(const 
        WaveUnion &sptThreshold) const
    {
        if(sptThreshold == "sptThresholdInt")
            return(m_sptThresholdInt);
        else
            return(m_sptThresholdInf);
    }
    
    void  PimRtrCfgLocalManagedObject::setSptThresholdInt(const UI32 &sptThreshold)
    {
        m_sptThresholdUnion = "sptThresholdInt";
        m_sptThresholdInt  =  sptThreshold;
        m_sptThresholdInf = PIM_SPT_VAL_USR_DEF;
    }
    
    UI32 PimRtrCfgLocalManagedObject::getSptThresholdInt()  const
    {
        return (m_sptThresholdInt);
    }

    void  PimRtrCfgLocalManagedObject::setSptThresholdInf(const PimSptVal &sptThreshold)
    {
        m_sptThresholdUnion = "sptThresholdInf";
        m_sptThresholdInt  =  0;
        m_sptThresholdInf = sptThreshold;
    }
    
    PimSptVal PimRtrCfgLocalManagedObject::getSptThresholdInf()  const
    {
        return (m_sptThresholdInf);
    }
    
    PimSptVal PimRtrCfgLocalManagedObject::mapSptIntToEnumVal(const UI32 &sptThreshold)
    {
        PimSptVal sptVal = PIM_SPT_VAL_USR_DEF;
        if(sptThreshold == PIM_SPT_VAL_INFINITY)
            sptVal = PIM_SPT_VAL_INFINITY;
        
        return(sptVal);
    }
    
    void  PimRtrCfgLocalManagedObject::setSptThresholdUnion(const 
        WaveUnion &sptThreshold)
    {
        m_sptThresholdUnion = sptThreshold;
    }
    
    WaveUnion PimRtrCfgLocalManagedObject::getSptThresholdUnion()  const
    {
        return (m_sptThresholdUnion);
    }
    
    void  PimRtrCfgLocalManagedObject::setBsrCandidateCfg(const vector<WaveManagedObjectPointer<BsrCandidateLocalManagedObject> > &BsrCandidateCfg)
    {
        m_BsrCandidateCfg  =  BsrCandidateCfg;
    }

    vector<WaveManagedObjectPointer<BsrCandidateLocalManagedObject> >  PimRtrCfgLocalManagedObject::getBsrCandidateCfg() const
    {
        return (m_BsrCandidateCfg);
    }

    void  PimRtrCfgLocalManagedObject::setRpCandidateCfg(const 
		vector<WaveManagedObjectPointer<RpCandidateLocalManagedObject> > 
		&RpCandidateCfg)
    {
        m_RpCandidateCfg  =  RpCandidateCfg;
    }

    vector<WaveManagedObjectPointer<RpCandidateLocalManagedObject> >  
		PimRtrCfgLocalManagedObject::getRpCandidateCfg() const
    {
        return (m_RpCandidateCfg);
    }

    void  PimRtrCfgLocalManagedObject::setRpCandidatePrefixCfg(const 
		vector<WaveManagedObjectPointer<RpCandidatePrefixLocalManagedObject> > 
		&RpCandidatePrefixCfg)
    {
        m_RpCandidatePrefixCfg  =  RpCandidatePrefixCfg;
    }

    vector<WaveManagedObjectPointer<RpCandidatePrefixLocalManagedObject> >  
		PimRtrCfgLocalManagedObject::getRpCandidatePrefixCfg() const
    {
        return (m_RpCandidatePrefixCfg);
    }

    void  PimRtrCfgLocalManagedObject::setStaticRpCfg(const 
		vector<WaveManagedObjectPointer<StaticRpLocalManagedObject> > 
		&StaticRpCfg)
    {
        m_StaticRpCfg  =  StaticRpCfg;
    }

    vector<WaveManagedObjectPointer<StaticRpLocalManagedObject> >  PimRtrCfgLocalManagedObject::getStaticRpCfg() const
    {
        return (m_StaticRpCfg);
    }
	
	/* Function to add an instance of the BSR candidate config into the 
     * PIM router MO. The instance is added to the vector */
    void 
    PimRtrCfgLocalManagedObject::addBsrCand(BsrCandidateLocalManagedObject *pMo)
    {
	    WaveManagedObjectPointer<BsrCandidateLocalManagedObject> BsrCandMo(pMo);
	    m_BsrCandidateCfg.push_back(BsrCandMo);
    }
    
    /* Function to delete an instance of the BSR candidate config from the 
     * PIM router MO. The instance is removed from the vector */
    void 
    PimRtrCfgLocalManagedObject::delBsrCand(const UI8 intfType, const string intfName)
    {
	    for (UI32 i = 0; i < m_BsrCandidateCfg.size();i++)
	    {
		    if((m_BsrCandidateCfg[i]->getIntfType() == intfType) && 
               (m_BsrCandidateCfg[i]->getIntfName() == intfName)) {
			    trace(TRACE_LEVEL_DEBUG,
                    string("PimRtrCfgLocalManagedObject::delBsrCand: ") +
                    string("Found BSR Cand ") + pimGetIntfTypeStr(intfType) +
                    intfName);
			    m_BsrCandidateCfg.erase (m_BsrCandidateCfg.begin() + i);
			    return;
		    }
	    }
        trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgLocalManagedObject::delBsrCand: ")
            + string("BSR cand ") +  pimGetIntfTypeStr(intfType) + intfName
            + string(" not found"));
        return;
    }
    
    /* Function to get an instance of the BSR candidate config from the
     * PIM router MO given the interface details */
    BsrCandidateLocalManagedObject*
    PimRtrCfgLocalManagedObject::getBsrCandMo(const UI8 intfType, 
        const string intfName)
    {
        BsrCandidateLocalManagedObject *pMo = NULL;

        for (UI32 i = 0; i < m_BsrCandidateCfg.size(); i++) {
            if (m_BsrCandidateCfg[i]->getIntfType() == intfType &&
                m_BsrCandidateCfg[i]->getIntfName() == intfName) {
                trace(TRACE_LEVEL_DEBUG,
                    string("PimRtrCfgLocalManagedObject::getBsrCandMo: ") +
                    string("Found BSR Cand ") + pimGetIntfTypeStr(intfType) +
                    intfName);
                pMo = (WaveManagedObjectPointer<BsrCandidateLocalManagedObject>
                          (m_BsrCandidateCfg[i])).operator->();
                return pMo;
            }
        }
        trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgLocalManagedObject::getBsrCandMo: ")
            + string("BSR cand ") +  pimGetIntfTypeStr(intfType) + intfName
            + string(" not found"));
        return pMo;
    }

    /* Function to delete an instance of the static RP config from the 
     * PIM router MO. The instance is removed from the vector */
    void
    PimRtrCfgLocalManagedObject::delStaticRp(const IpV4Address rpAddr)
    {
        for (UI32 i = 0; i < m_StaticRpCfg.size(); i++) {
            if (m_StaticRpCfg[i]->getRpAddr() == rpAddr) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimRtrCfgLocalManagedObject::delStaticRp: ") +
                    string("Found RP Addr ") + rpAddr.toString());
                m_StaticRpCfg.erase(m_StaticRpCfg.begin() + i);
                return;
            }
        }
        trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgLocalManagedObject::delStaticRp: ")
            + string("RP addr ") + rpAddr.toString() + string(" not found"));
        return;
    }

    /* Function to add an instance of the static RP config into the 
     * PIM router MO. The instance is added to the vector */
    void 
    PimRtrCfgLocalManagedObject::addStaticRp(StaticRpLocalManagedObject *pMo)
    {
        WaveManagedObjectPointer<StaticRpLocalManagedObject> staticRpMo(pMo);
        m_StaticRpCfg.push_back(staticRpMo);
    }
    
    /* Function to get an instance of the static RP config from the 
     * PIM router MO given the RP address */
    StaticRpLocalManagedObject*
    PimRtrCfgLocalManagedObject::getStaticRpMo(const IpV4Address rpAddr)
    {
        StaticRpLocalManagedObject *pMo = NULL;
        
        for (UI32 i = 0; i < m_StaticRpCfg.size(); i++) {
            if (m_StaticRpCfg[i]->getRpAddr() == rpAddr) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimRtrCfgLocalManagedObject::getStaticRpMo: ") +
                    string("Found RP Addr ") + rpAddr.toString());
                pMo = (WaveManagedObjectPointer<StaticRpLocalManagedObject>
                          (m_StaticRpCfg[i])).operator->();
                return pMo;
            }
        }
        trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgLocalManagedObject::getStaticRpMo: ")
            + string("RP addr ") + rpAddr.toString() + string(" not found"));
        return pMo;
    }


    /* Function to delete an instance of the RP Candidate Prefix config from the 
     * PIM router MO. The instance is removed from the vector */
    void
    PimRtrCfgLocalManagedObject::delCandRpPrefix(const IpV4Address prefixIP, 
		const UI16 prefixLen)
    {
        for (UI32 i = 0; i < m_RpCandidatePrefixCfg.size(); i++) {
            if (m_RpCandidatePrefixCfg[i]->getPrefixIP() == prefixIP && 
				m_RpCandidatePrefixCfg[i]->getPrefixLen() == prefixLen) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimRtrCfgLocalManagedObject::delCandRpPrefix: ") +
                    string("Found Prefix IP ") + prefixIP.toString() +
				    string("Prefix Length ") + prefixLen);
				m_RpCandidatePrefixCfg.erase(m_RpCandidatePrefixCfg.begin()+i);
                return;
            }
        }
        trace(TRACE_LEVEL_DEBUG, 
			string("PimRtrCfgLocalManagedObject::delCandRpPrefix: ")
            + string("Prefix IP ") + prefixIP.toString() + 
			string("Prefix Length ") + prefixLen + string(" not found"));
        return;
    }

    /* Function to add an instance of the RP Candidate Prefix config into the 
     * PIM router MO. The instance is added to the vector */
    void 
    PimRtrCfgLocalManagedObject::addCandRpPrefix(
		RpCandidatePrefixLocalManagedObject *pMo)
    {
        WaveManagedObjectPointer<RpCandidatePrefixLocalManagedObject> 
			candRpPrefixMo(pMo);
		m_RpCandidatePrefixCfg.push_back(candRpPrefixMo);
    }
    
    /* Function to get an instance of the RP Candidate Prefix config from the 
     * PIM router MO given the Prefix and Action */
    RpCandidatePrefixLocalManagedObject*
    PimRtrCfgLocalManagedObject::getCandRpPrefixMo(const IpV4Address prefixIP, 
		const UI16 prefixLen)
    {
        RpCandidatePrefixLocalManagedObject *pMo = NULL;
        
        for (UI32 i = 0; i < m_RpCandidatePrefixCfg.size(); i++) {
            if (m_RpCandidatePrefixCfg[i]->getPrefixIP() == prefixIP &&
				m_RpCandidatePrefixCfg[i]->getPrefixLen() == prefixLen) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimRtrCfgLocalManagedObject::getCandRpPrefixMo: ")+
                    string("Found Prefix IP ") + prefixIP.toString() + 
					string("Prefix Length ") + prefixLen);
                pMo = (WaveManagedObjectPointer
					<RpCandidatePrefixLocalManagedObject>
                    (m_RpCandidatePrefixCfg[i])).operator->();
                return pMo;
            }
        }
        trace(TRACE_LEVEL_DEBUG, 
			string("PimRtrCfgLocalManagedObject::getCandRpPrefixMo: ")
            + string("Prefix IP ") + prefixIP.toString() + 
			string("Prefix Length ") + prefixLen + string(" not found"));
        return pMo;
    }
 

    
    /* Function to delete an instance of the RP candidate config from the 
     * PIM router MO. The instance is removed from the vector */
    void
    PimRtrCfgLocalManagedObject::delCandRp(const UI8 intfType, 
        const string intfName)
    {
        for (UI32 i = 0; i < m_RpCandidateCfg.size(); i++) {
            if (m_RpCandidateCfg[i]->getIntfType() == intfType &&
                m_RpCandidateCfg[i]->getIntfName() == intfName) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimRtrCfgLocalManagedObject::delCandRp: ") +
                    string("Found RP Cand ") + pimGetIntfTypeStr(intfType) + 
                    intfName);
                m_RpCandidateCfg.erase(m_RpCandidateCfg.begin() + i);
                return;
            }
        }
        trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgLocalManagedObject::delCandRp: ")
            + string("RP cand ") +  pimGetIntfTypeStr(intfType) + intfName 
            + string(" not found"));
        return;
    }

    /* Function to add an instance of the RP candidate config into the 
     * PIM router MO. The instance is added to the vector */
    void 
    PimRtrCfgLocalManagedObject::addCandRp(RpCandidateLocalManagedObject *pMo)
    {
        WaveManagedObjectPointer<RpCandidateLocalManagedObject> candRpMo(pMo);
        m_RpCandidateCfg.push_back(candRpMo);
    }
    
    /* Function to get an instance of the RP candidate config from the 
     * PIM router MO given the interface details */
    RpCandidateLocalManagedObject*
    PimRtrCfgLocalManagedObject::getCandRpMo(const UI8 intfType, 
        const string intfName)
    {
        RpCandidateLocalManagedObject *pMo = NULL;
        
        for (UI32 i = 0; i < m_RpCandidateCfg.size(); i++) {
            if (m_RpCandidateCfg[i]->getIntfType() == intfType &&
                m_RpCandidateCfg[i]->getIntfName() == intfName) {
                trace(TRACE_LEVEL_DEBUG, 
                    string("PimRtrCfgLocalManagedObject::getCandRpMo: ") +
                    string("Found RP Cand ") + pimGetIntfTypeStr(intfType) + 
                    intfName);
                pMo = (WaveManagedObjectPointer<RpCandidateLocalManagedObject>
                          (m_RpCandidateCfg[i])).operator->();
                return pMo;
            }
        }
        trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgLocalManagedObject::getCandRpMo: ")
            + string("RP cand ") +  pimGetIntfTypeStr(intfType) + intfName 
            + string(" not found"));
        return pMo;
    }
}
