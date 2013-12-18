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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#include "PortCfg/Local/PortCfgLocalObjectManager.h"
#include "PortCfg/Local/PortCfgFCPortDetailedInfoMessage.h"
#include "PortCfg/Local/PortCfgTypes.h"

namespace DcmNs
{

    PortCfgFCPortDetailedInfoMessage::PortCfgFCPortDetailedInfoMessage ()
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTDETAILEDINFO)
    {
    }

    PortCfgFCPortDetailedInfoMessage::PortCfgFCPortDetailedInfoMessage (const string &procInfo,const string &portId,const SI32 &switchPort,const vector<string> &statsRx,const vector<string> &statsTx,const vector<string> &statsErr,const vector<string> &errInfo,const string &lossOfSync,const string &lossOfSig,const string &Frjt,const string &Fbsy,const string &portState,const string &portMediaPresence,const string &portLineSpeedActual,const string &portLineSpeedCfg,const vector<string> &portCfg,const vector<string> &portInfo,const string &portCounters,const string &rateInfo,const string &rateInput,const string &rateOutput,const UI32 &resvbuf,const UI32 &bufusage,const UI32 &neededbuf,const string &linkdist,const UI32 &rembuf,const string &lxmode,const string &txperf,const string &rxperf,const UI32 &bandwidth)
        : ManagementInterfaceMessage (PortCfgLocalObjectManager::getClassName (),PORTCFGFCPORTDETAILEDINFO),
        m_procInfo    (procInfo),
        m_portId    (portId),
        m_switchPort    (switchPort),
        m_statsRx    (statsRx),
        m_statsTx    (statsTx),
        m_statsErr    (statsErr),
        m_errInfo    (errInfo),
        m_lossOfSync    (lossOfSync),
        m_lossOfSig    (lossOfSig),
        m_Frjt    (Frjt),
        m_Fbsy    (Fbsy),
        m_portState    (portState),
        m_portMediaPresence    (portMediaPresence),
        m_portLineSpeedActual    (portLineSpeedActual),
        m_portLineSpeedCfg    (portLineSpeedCfg),
        m_portCfg    (portCfg),
        m_portInfo    (portInfo),
        m_portCounters    (portCounters),
        m_rateInfo    (rateInfo),
        m_rateInput    (rateInput),
        m_rateOutput    (rateOutput),
        m_resvbuf    (resvbuf),
        m_bufusage    (bufusage),
        m_neededbuf    (neededbuf),
        m_linkdist    (linkdist),
        m_rembuf    (rembuf),
        m_lxmode    (lxmode),
        m_txperf    (txperf),
        m_rxperf    (rxperf),
        m_bandwidth    (bandwidth)
    {
    }

    PortCfgFCPortDetailedInfoMessage::~PortCfgFCPortDetailedInfoMessage ()
    {
    }

    void  PortCfgFCPortDetailedInfoMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_procInfo,"procInfo"));
        addSerializableAttribute (new AttributeString(&m_portId,"portId"));
        addSerializableAttribute (new AttributeSI32(&m_switchPort,"switchPort"));
        addSerializableAttribute (new AttributeStringVector(&m_statsRx,"statsRx"));
        addSerializableAttribute (new AttributeStringVector(&m_statsTx,"statsTx"));
        addSerializableAttribute (new AttributeStringVector(&m_statsErr,"statsErr"));
        addSerializableAttribute (new AttributeStringVector(&m_errInfo,"errInfo"));
        addSerializableAttribute (new AttributeString(&m_lossOfSync,"lossOfSync"));
        addSerializableAttribute (new AttributeString(&m_lossOfSig,"lossOfSig"));
        addSerializableAttribute (new AttributeString(&m_Frjt,"Frjt"));
        addSerializableAttribute (new AttributeString(&m_Fbsy,"Fbsy"));
        addSerializableAttribute (new AttributeString(&m_portState,"portState"));
        addSerializableAttribute (new AttributeString(&m_portMediaPresence,"portMediaPresence"));
        addSerializableAttribute (new AttributeString(&m_portLineSpeedActual,"portLineSpeedActual"));
        addSerializableAttribute (new AttributeString(&m_portLineSpeedCfg,"portLineSpeedCfg"));
        addSerializableAttribute (new AttributeStringVector(&m_portCfg,"portCfg"));
        addSerializableAttribute (new AttributeStringVector(&m_portInfo,"portInfo"));
        addSerializableAttribute (new AttributeString(&m_portCounters,"portCounters"));
        addSerializableAttribute (new AttributeString(&m_rateInfo,"rateInfo"));
        addSerializableAttribute (new AttributeString(&m_rateInput,"rateInput"));
        addSerializableAttribute (new AttributeString(&m_rateOutput,"rateOutput"));
        addSerializableAttribute (new AttributeUI32(&m_resvbuf,"resvbuf"));
        addSerializableAttribute (new AttributeUI32(&m_bufusage,"bufusage"));
        addSerializableAttribute (new AttributeUI32(&m_neededbuf,"neededbuf"));
        addSerializableAttribute (new AttributeString(&m_linkdist,"linkdist"));
        addSerializableAttribute (new AttributeUI32(&m_rembuf,"rembuf"));
        addSerializableAttribute (new AttributeString(&m_lxmode,"lxmode"));
        addSerializableAttribute (new AttributeString(&m_txperf,"txperf"));
        addSerializableAttribute (new AttributeString(&m_rxperf,"rxperf"));
        addSerializableAttribute (new AttributeUI32(&m_bandwidth,"bandwidth"));
    }

    void  PortCfgFCPortDetailedInfoMessage::setProcInfo(const string &procInfo)
    {
        m_procInfo  =  procInfo;
    }

    string  PortCfgFCPortDetailedInfoMessage::getProcInfo() const
    {
        return (m_procInfo);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortId(const string &portId)
    {
        m_portId  =  portId;
    }

    string  PortCfgFCPortDetailedInfoMessage::getPortId() const
    {
        return (m_portId);
    }

    void  PortCfgFCPortDetailedInfoMessage::setSwitchPort(const SI32 &switchPort)
    {
        m_switchPort  =  switchPort;
    }

    SI32  PortCfgFCPortDetailedInfoMessage::getSwitchPort() const
    {
        return (m_switchPort);
    }

    void  PortCfgFCPortDetailedInfoMessage::setStatsRx(const vector<string> &statsRx)
    {
        m_statsRx  =  statsRx;
    }

    vector<string>  PortCfgFCPortDetailedInfoMessage::getStatsRx() const
    {
        return (m_statsRx);
    }

    void  PortCfgFCPortDetailedInfoMessage::setStatsTx(const vector<string> &statsTx)
    {
        m_statsTx  =  statsTx;
    }

    vector<string>  PortCfgFCPortDetailedInfoMessage::getStatsTx() const
    {
        return (m_statsTx);
    }

    void  PortCfgFCPortDetailedInfoMessage::setStatsErr(const vector<string> &statsErr)
    {
        m_statsErr  =  statsErr;
    }

    vector<string>  PortCfgFCPortDetailedInfoMessage::getStatsErr() const
    {
        return (m_statsErr);
    }

    void  PortCfgFCPortDetailedInfoMessage::setErrInfo(const vector<string> &errInfo)
    {
        m_errInfo  =  errInfo;
    }

    vector<string>  PortCfgFCPortDetailedInfoMessage::getErrInfo() const
    {
        return (m_errInfo);
    }

    void  PortCfgFCPortDetailedInfoMessage::setLossOfSync(const string &lossOfSync)
    {
        m_lossOfSync  =  lossOfSync;
    }

    string  PortCfgFCPortDetailedInfoMessage::getLossOfSync() const
    {
        return (m_lossOfSync);
    }

    void  PortCfgFCPortDetailedInfoMessage::setLossOfSig(const string &lossOfSig)
    {
        m_lossOfSig  =  lossOfSig;
    }

    string  PortCfgFCPortDetailedInfoMessage::getLossOfSig() const
    {
        return (m_lossOfSig);
    }

    void  PortCfgFCPortDetailedInfoMessage::setFrjt(const string &Frjt)
    {
        m_Frjt  =  Frjt;
    }

    string  PortCfgFCPortDetailedInfoMessage::getFrjt() const
    {
        return (m_Frjt);
    }

    void  PortCfgFCPortDetailedInfoMessage::setFbsy(const string &Fbsy)
    {
        m_Fbsy  =  Fbsy;
    }

    string  PortCfgFCPortDetailedInfoMessage::getFbsy() const
    {
        return (m_Fbsy);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortState(const string &portState)
    {
        m_portState  =  portState;
    }

    string  PortCfgFCPortDetailedInfoMessage::getPortState() const
    {
        return (m_portState);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortMediaPresence(const string &portMediaPresence)
    {
        m_portMediaPresence  =  portMediaPresence;
    }

    string  PortCfgFCPortDetailedInfoMessage::getPortMediaPresence() const
    {
        return (m_portMediaPresence);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortLineSpeedActual(const string &portLineSpeedActual)
    {
        m_portLineSpeedActual  =  portLineSpeedActual;
    }

    string  PortCfgFCPortDetailedInfoMessage::getPortLineSpeedActual() const
    {
        return (m_portLineSpeedActual);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortLineSpeedCfg(const string &portLineSpeedCfg)
    {
        m_portLineSpeedCfg  =  portLineSpeedCfg;
    }

    string  PortCfgFCPortDetailedInfoMessage::getPortLineSpeedCfg() const
    {
        return (m_portLineSpeedCfg);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortCfg(const vector<string> &portCfg)
    {
        m_portCfg  =  portCfg;
    }

    vector<string>  PortCfgFCPortDetailedInfoMessage::getPortCfg() const
    {
        return (m_portCfg);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortInfo(const vector<string> &portInfo)
    {
        m_portInfo  =  portInfo;
    }

    vector<string>  PortCfgFCPortDetailedInfoMessage::getPortInfo() const
    {
        return (m_portInfo);
    }

    void  PortCfgFCPortDetailedInfoMessage::setPortCounters(const string &portCounters)
    {
        m_portCounters  =  portCounters;
    }

    string  PortCfgFCPortDetailedInfoMessage::getPortCounters() const
    {
        return (m_portCounters);
    }

    void  PortCfgFCPortDetailedInfoMessage::setRateInfo(const string &rateInfo)
    {
        m_rateInfo  =  rateInfo;
    }

    string  PortCfgFCPortDetailedInfoMessage::getRateInfo() const
    {
        return (m_rateInfo);
    }

    void  PortCfgFCPortDetailedInfoMessage::setRateInput(const string &rateInput)
    {
        m_rateInput  =  rateInput;
    }

    string  PortCfgFCPortDetailedInfoMessage::getRateInput() const
    {
        return (m_rateInput);
    }

    void  PortCfgFCPortDetailedInfoMessage::setRateOutput(const string &rateOutput)
    {
        m_rateOutput  =  rateOutput;
    }

    string  PortCfgFCPortDetailedInfoMessage::getRateOutput() const
    {
        return (m_rateOutput);
    }

    void  PortCfgFCPortDetailedInfoMessage::setResvbuf(const UI32 &resvbuf)
    {
        m_resvbuf  =  resvbuf;
    }

    UI32  PortCfgFCPortDetailedInfoMessage::getResvbuf() const
    {
        return (m_resvbuf);
    }

    void  PortCfgFCPortDetailedInfoMessage::setBufusage(const UI32 &bufusage)
    {
        m_bufusage  =  bufusage;
    }

    UI32  PortCfgFCPortDetailedInfoMessage::getBufusage() const
    {
        return (m_bufusage);
    }

    void  PortCfgFCPortDetailedInfoMessage::setNeededbuf(const UI32 &neededbuf)
    {
        m_neededbuf  =  neededbuf;
    }

    UI32  PortCfgFCPortDetailedInfoMessage::getNeededbuf() const
    {
        return (m_neededbuf);
    }

    void  PortCfgFCPortDetailedInfoMessage::setLinkdist(const string &linkdist)
    {
        m_linkdist  =  linkdist;
    }

    string  PortCfgFCPortDetailedInfoMessage::getLinkdist() const
    {
        return (m_linkdist);
    }

    void  PortCfgFCPortDetailedInfoMessage::setRembuf(const UI32 &rembuf)
    {
        m_rembuf  =  rembuf;
    }

    UI32  PortCfgFCPortDetailedInfoMessage::getRembuf() const
    {
        return (m_rembuf);
    }

    void  PortCfgFCPortDetailedInfoMessage::setLxmode(const string &lxmode)
    {
        m_lxmode  =  lxmode; 
    }

    string  PortCfgFCPortDetailedInfoMessage::getLxmode() const
    {
        return (m_lxmode);
    }

    void  PortCfgFCPortDetailedInfoMessage::setTxperf(const string &txperf)
    {
        m_txperf  =  txperf;
    }

    string  PortCfgFCPortDetailedInfoMessage::getTxperf() const
    {
        return (m_txperf);
    }

    void  PortCfgFCPortDetailedInfoMessage::setRxperf(const string &rxperf)
    {
        m_rxperf  =  rxperf;
    }

    string  PortCfgFCPortDetailedInfoMessage::getRxperf() const
    {
        return (m_rxperf);
    }

    void  PortCfgFCPortDetailedInfoMessage::setBandwidth(const UI32 &bandwidth)
    {
        m_bandwidth  =  bandwidth;
    }

    UI32  PortCfgFCPortDetailedInfoMessage::getBandwidth() const
    {
        return (m_bandwidth);
    }

}
