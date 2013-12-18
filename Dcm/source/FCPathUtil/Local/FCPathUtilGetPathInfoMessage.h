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
 *   Author : ravikuma                                                     *
 **************************************************************************/

#ifndef FCPATHUTILGETPATHINFOMESSAGE_H
#define FCPATHUTILGETPATHINFOMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FCPathUtilGetPathInfoMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FCPathUtilGetPathInfoMessage ();
            FCPathUtilGetPathInfoMessage (const SI32 &FID,const SI32 &destDomainId,const SI32 &sourcePort,const SI32 &destPort,const string &basicStats,const string &extendedStats,const string &reversePath,const string &ErrMsg,const vector<UI32> &HopCount,const vector<UI32> &InPort,const vector<UI32> &OutPort,const vector<UI32> &RBridgeId,const vector<UI32> &Bandwidth,const vector<UI32> &IsReversePath,const vector<UI32> &Cost,const vector<UI32> &ShortIntvl,const vector<UI32> &LongIntvl,const vector<UI32> &InTxShortBps,const vector<UI32> &InRxShortBps,const vector<UI32> &OutTxShortBps,const vector<UI32> &OutRxShortBps,const vector<UI32> &InTxLongBps,const vector<UI32> &InRxLongBps,const vector<UI32> &OutTxLongBps,const vector<UI32> &OutRxLongBps,const vector<UI32> &InTxShortTxCrdz,const vector<UI32> &InRxShortTxCrdz,const vector<UI32> &OutTxShortTxCrdz,const vector<UI32> &OutRxShortTxCrdz,const vector<UI32> &InTxLongTxCrdz,const vector<UI32> &InRxLongTxCrdz,const vector<UI32> &OutTxLongTxCrdz,const vector<UI32> &OutRxLongTxCrdz,const vector<UI32> &InTxShortFps,const vector<UI32> &InRxShortFps,const vector<UI32> &OutTxShortFps,const vector<UI32> &OutRxShortFps,const vector<UI32> &InTxLongFps,const vector<UI32> &InRxLongFps,const vector<UI32> &OutTxLongFps,const vector<UI32> &OutRxLongFps,const vector<string> &InTxWords,const vector<string> &InRxWords,const vector<string> &OutTxWords,const vector<string> &OutRxWords,const vector<string> &InTxFrames,const vector<string> &InRxFrames,const vector<string> &OutTxFrames,const vector<string> &OutRxFrames,const vector<UI32> &InTxErrors,const vector<UI32> &InRxErrors,const vector<UI32> &OutTxErrors,const vector<UI32> &OutRxErrors, const vector<string> &inputTuples, const vector<string> &outputTuples, const vector<UI32> &fullHop);
            virtual    ~FCPathUtilGetPathInfoMessage ();
            void setFID(const SI32 &FID);
            SI32 getFID()  const;
            void setDestDomainId(const SI32 &destDomainId);
            SI32 getDestDomainId()  const;
            void setSourcePort(const SI32 &sourcePort);
            SI32 getSourcePort()  const;
            void setDestPort(const SI32 &destPort);
            SI32 getDestPort()  const;
            void setBasicStats(const string &basicStats);
            string getBasicStats()  const;
            void setExtendedStats(const string &extendedStats);
            string getExtendedStats()  const;
            void setReversePath(const string &reversePath);
            string getReversePath()  const;
            void setErrMsg(const string &ErrMsg);
            string getErrMsg()  const;
            void setHopCount(const vector<UI32> &HopCount);
            vector<UI32> getHopCount()  const;
            void setInPort(const vector<UI32> &InPort);
            vector<UI32> getInPort()  const;
            void setOutPort(const vector<UI32> &OutPort);
            vector<UI32> getOutPort()  const;
            void setRBridgeId(const vector<UI32> &RBridgeId);
            vector<UI32> getRBridgeId()  const;
            void setBandwidth(const vector<UI32> &Bandwidth);
            vector<UI32> getBandwidth()  const;
            void setIsReversePath(const vector<UI32> &IsReversePath);
            vector<UI32> getIsReversePath()  const;
            void setCost(const vector<UI32> &Cost);
            vector<UI32> getCost()  const;
            void setShortIntvl(const vector<UI32> &ShortIntvl);
            vector<UI32> getShortIntvl()  const;
            void setLongIntvl(const vector<UI32> &LongIntvl);
            vector<UI32> getLongIntvl()  const;
            void setInTxShortBps(const vector<UI32> &InTxShortBps);
            vector<UI32> getInTxShortBps()  const;
            void setInRxShortBps(const vector<UI32> &InRxShortBps);
            vector<UI32> getInRxShortBps()  const;
            void setOutTxShortBps(const vector<UI32> &OutTxShortBps);
            vector<UI32> getOutTxShortBps()  const;
            void setOutRxShortBps(const vector<UI32> &OutRxShortBps);
            vector<UI32> getOutRxShortBps()  const;
            void setInTxLongBps(const vector<UI32> &InTxLongBps);
            vector<UI32> getInTxLongBps()  const;
            void setInRxLongBps(const vector<UI32> &InRxLongBps);
            vector<UI32> getInRxLongBps()  const;
            void setOutTxLongBps(const vector<UI32> &OutTxLongBps);
            vector<UI32> getOutTxLongBps()  const;
            void setOutRxLongBps(const vector<UI32> &OutRxLongBps);
            vector<UI32> getOutRxLongBps()  const;
            void setInTxShortTxCrdz(const vector<UI32> &InTxShortTxCrdz);
            vector<UI32> getInTxShortTxCrdz()  const;
            void setInRxShortTxCrdz(const vector<UI32> &InRxShortTxCrdz);
            vector<UI32> getInRxShortTxCrdz()  const;
            void setOutTxShortTxCrdz(const vector<UI32> &OutTxShortTxCrdz);
            vector<UI32> getOutTxShortTxCrdz()  const;
            void setOutRxShortTxCrdz(const vector<UI32> &OutRxShortTxCrdz);
            vector<UI32> getOutRxShortTxCrdz()  const;
            void setInTxLongTxCrdz(const vector<UI32> &InTxLongTxCrdz);
            vector<UI32> getInTxLongTxCrdz()  const;
            void setInRxLongTxCrdz(const vector<UI32> &InRxLongTxCrdz);
            vector<UI32> getInRxLongTxCrdz()  const;
            void setOutTxLongTxCrdz(const vector<UI32> &OutTxLongTxCrdz);
            vector<UI32> getOutTxLongTxCrdz()  const;
            void setOutRxLongTxCrdz(const vector<UI32> &OutRxLongTxCrdz);
            vector<UI32> getOutRxLongTxCrdz()  const;
            void setInTxShortFps(const vector<UI32> &InTxShortFps);
            vector<UI32> getInTxShortFps()  const;
            void setInRxShortFps(const vector<UI32> &InRxShortFps);
            vector<UI32> getInRxShortFps()  const;
            void setOutTxShortFps(const vector<UI32> &OutTxShortFps);
            vector<UI32> getOutTxShortFps()  const;
            void setOutRxShortFps(const vector<UI32> &OutRxShortFps);
            vector<UI32> getOutRxShortFps()  const;
            void setInTxLongFps(const vector<UI32> &InTxLongFps);
            vector<UI32> getInTxLongFps()  const;
            void setInRxLongFps(const vector<UI32> &InRxLongFps);
            vector<UI32> getInRxLongFps()  const;
            void setOutTxLongFps(const vector<UI32> &OutTxLongFps);
            vector<UI32> getOutTxLongFps()  const;
            void setOutRxLongFps(const vector<UI32> &OutRxLongFps);
            vector<UI32> getOutRxLongFps()  const;
            void setInTxWords(const vector<string> &InTxWords);
            vector<string> getInTxWords()  const;
            void setInRxWords(const vector<string> &InRxWords);
            vector<string> getInRxWords()  const;
            void setOutTxWords(const vector<string> &OutTxWords);
            vector<string> getOutTxWords()  const;
            void setOutRxWords(const vector<string> &OutRxWords);
            vector<string> getOutRxWords()  const;
            void setInTxFrames(const vector<string> &InTxFrames);
            vector<string> getInTxFrames()  const;
            void setInRxFrames(const vector<string> &InRxFrames);
            vector<string> getInRxFrames()  const;
            void setOutTxFrames(const vector<string> &OutTxFrames);
            vector<string> getOutTxFrames()  const;
            void setOutRxFrames(const vector<string> &OutRxFrames);
            vector<string> getOutRxFrames()  const;
            void setInTxErrors(const vector<UI32> &InTxErrors);
            vector<UI32> getInTxErrors()  const;
            void setInRxErrors(const vector<UI32> &InRxErrors);
            vector<UI32> getInRxErrors()  const;
            void setOutTxErrors(const vector<UI32> &OutTxErrors);
            vector<UI32> getOutTxErrors()  const;
            void setOutRxErrors(const vector<UI32> &OutRxErrors);
            vector<UI32> getOutRxErrors()  const;
            void setInputTuples(const vector<string> &inputTuples);
            vector<string> getInputTuples()  const;
            void setOutputTuples(const vector<string> &outputTuples);
            vector<string> getOutputTuples()  const;
            void setfullHop(const vector<UI32> &fullHop);
            vector<UI32> getfullHop()  const;

// Now the data members

        private:
        protected:
        public:
            SI32  m_FID;
            SI32  m_destDomainId;
            SI32  m_sourcePort;
            SI32  m_destPort;
            string  m_basicStats;
            string  m_extendedStats;
            string  m_reversePath;
            string  m_ErrMsg;
            vector<UI32>  m_HopCount;
            vector<UI32>  m_InPort;
            vector<UI32>  m_OutPort;
            vector<UI32>  m_RBridgeId;
            vector<UI32>  m_Bandwidth;
            vector<UI32>  m_IsReversePath;
            vector<UI32>  m_Cost;
            vector<UI32>  m_ShortIntvl;
            vector<UI32>  m_LongIntvl;
            vector<UI32>  m_InTxShortBps;
            vector<UI32>  m_InRxShortBps;
            vector<UI32>  m_OutTxShortBps;
            vector<UI32>  m_OutRxShortBps;
            vector<UI32>  m_InTxLongBps;
            vector<UI32>  m_InRxLongBps;
            vector<UI32>  m_OutTxLongBps;
            vector<UI32>  m_OutRxLongBps;
            vector<UI32>  m_InTxShortTxCrdz;
            vector<UI32>  m_InRxShortTxCrdz;
            vector<UI32>  m_OutTxShortTxCrdz;
            vector<UI32>  m_OutRxShortTxCrdz;
            vector<UI32>  m_InTxLongTxCrdz;
            vector<UI32>  m_InRxLongTxCrdz;
            vector<UI32>  m_OutTxLongTxCrdz;
            vector<UI32>  m_OutRxLongTxCrdz;
            vector<UI32>  m_InTxShortFps;
            vector<UI32>  m_InRxShortFps;
            vector<UI32>  m_OutTxShortFps;
            vector<UI32>  m_OutRxShortFps;
            vector<UI32>  m_InTxLongFps;
            vector<UI32>  m_InRxLongFps;
            vector<UI32>  m_OutTxLongFps;
            vector<UI32>  m_OutRxLongFps;
            vector<string>  m_InTxWords;
            vector<string>  m_InRxWords;
            vector<string>  m_OutTxWords;
            vector<string>  m_OutRxWords;
            vector<string>  m_InTxFrames;
            vector<string>  m_InRxFrames;
            vector<string>  m_OutTxFrames;
            vector<string>  m_OutRxFrames;
            vector<UI32>  m_InTxErrors;
            vector<UI32>  m_InRxErrors;
            vector<UI32>  m_OutTxErrors;
            vector<UI32>  m_OutRxErrors;
            vector<string>  m_InputTuples;
            vector<string>  m_OutputTuples;
            vector<UI32>  m_fullHop;
    };
}
#endif                                            //FCPATHUTILGETPATHINFOMESSAGE_H
