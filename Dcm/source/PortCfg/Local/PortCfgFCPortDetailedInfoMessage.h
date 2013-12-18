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

#ifndef PORTCFGFCPORTDETAILEDINFOMESSAGE_H
#define PORTCFGFCPORTDETAILEDINFOMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgFCPortDetailedInfoMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgFCPortDetailedInfoMessage ();
            PortCfgFCPortDetailedInfoMessage (const string &procInfo,const string &portId,const SI32 &switchPort,const vector<string> &statsRx,const vector<string> &statsTx,const vector<string> &statsErr,const vector<string> &errInfo,const string &lossOfSync,const string &lossOfSig,const string &Frjt,const string &Fbsy,const string &portState,const string &portMediaPresence,const string &portLineSpeedActual,const string &portLineSpeedCfg,const vector<string> &portCfg,const vector<string> &portInfo,const string &portCounters,const string &rateInfo,const string &rateInput,const string &rateOutput,const UI32 &resvbuf,const UI32 &bufusage,const UI32 &neededbuf,const string &linkdist,const UI32 &rembuf,const string &lxmode,const string &txperf,const string &rxperf,const UI32 &bandwidth);
            virtual    ~PortCfgFCPortDetailedInfoMessage ();
            void setProcInfo(const string &procInfo);
            string getProcInfo()  const;
            void setPortId(const string &portId);
            string getPortId()  const;
            void setSwitchPort(const SI32 &switchPort);
            SI32 getSwitchPort()  const;
            void setStatsRx(const vector<string> &statsRx);
            vector<string> getStatsRx()  const;
            void setStatsTx(const vector<string> &statsTx);
            vector<string> getStatsTx()  const;
            void setStatsErr(const vector<string> &statsErr);
            vector<string> getStatsErr()  const;
            void setErrInfo(const vector<string> &errInfo);
            vector<string> getErrInfo()  const;
            void setLossOfSync(const string &lossOfSync);
            string getLossOfSync()  const;
            void setLossOfSig(const string &lossOfSig);
            string getLossOfSig()  const;
            void setFrjt(const string &Frjt);
            string getFrjt()  const;
            void setFbsy(const string &Fbsy);
            string getFbsy()  const;
            void setPortState(const string &portState);
            string getPortState()  const;
            void setPortMediaPresence(const string &portMediaPresence);
            string getPortMediaPresence()  const;
            void setPortLineSpeedActual(const string &portLineSpeedActual);
            string getPortLineSpeedActual()  const;
            void setPortLineSpeedCfg(const string &portLineSpeedCfg);
            string getPortLineSpeedCfg()  const;
            void setPortCfg(const vector<string> &portCfg);
            vector<string> getPortCfg()  const;
            void setPortInfo(const vector<string> &portInfo);
            vector<string> getPortInfo()  const;
            void setPortCounters(const string &portCounters);
            string getPortCounters()  const;
            void setRateInfo(const string &rateInfo);
            string getRateInfo()  const;
            void setRateInput(const string &rateInput);
            string getRateInput()  const;
            void setRateOutput(const string &rateOutput);
            string getRateOutput()  const;
            void setResvbuf(const UI32 &resvbuf);
            UI32 getResvbuf()  const;  
            void setBufusage(const UI32 &bufusage);
            UI32 getBufusage()  const;  
            void setNeededbuf(const UI32 &neededbuf);
            UI32 getNeededbuf()  const;  
            void setLinkdist(const string &linkdist);
            string getLinkdist()  const;  
            void setRembuf(const UI32 &rembuf);
            UI32 getRembuf()  const;
            void setLxmode(const string &lxmode);
            string getLxmode()  const;
            void setTxperf(const string &txperf);
            string getTxperf()  const;
            void setRxperf(const string &rxperf);
            string getRxperf()  const;
            void setBandwidth(const UI32 &bandwidth);
            UI32 getBandwidth()  const;
// Now the data members

        private:
        protected:
        public:
            string  m_procInfo;
            string  m_portId;
            SI32  m_switchPort;
            vector<string>  m_statsRx;
            vector<string>  m_statsTx;
            vector<string>  m_statsErr;
            vector<string>  m_errInfo;
            string  m_lossOfSync;
            string  m_lossOfSig;
            string  m_Frjt;
            string  m_Fbsy;
            string  m_portState;
            string  m_portMediaPresence;
            string  m_portLineSpeedActual;
            string  m_portLineSpeedCfg;
            vector<string>  m_portCfg;
            vector<string>  m_portInfo;
            string  m_portCounters;
            string  m_rateInfo;
            string  m_rateInput;
            string  m_rateOutput;
            UI32  m_resvbuf;
            UI32  m_bufusage;
            UI32  m_neededbuf;
            string  m_linkdist;
            UI32  m_rembuf;
            string  m_lxmode;
            string  m_txperf;
            string  m_rxperf;
            UI32  m_bandwidth;
    };
}
#endif                                            //PORTCFGFCPORTDETAILEDINFOMESSAGE_H
