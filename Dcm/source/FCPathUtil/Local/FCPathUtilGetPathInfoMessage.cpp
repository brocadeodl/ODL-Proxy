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
 *   Author : Kumar Ghanta                                                 *
 **************************************************************************/

#include "FCPathUtil/Local/FCPathUtilLocalObjectManager.h"
#include "FCPathUtil/Local/FCPathUtilGetPathInfoMessage.h"
#include "FCPathUtil/Local/FCPathUtilTypes.h"

namespace DcmNs
{

    FCPathUtilGetPathInfoMessage::FCPathUtilGetPathInfoMessage ()
        : PrismMessage (FCPathUtilLocalObjectManager::getPrismServiceId (), FCPATHUTILGETPATHINFO)
    {
    }

    FCPathUtilGetPathInfoMessage::FCPathUtilGetPathInfoMessage (const SI32 &FID,const SI32 &destDomainId,const SI32 &sourcePort,const SI32 &destPort,const string &basicStats,const string &extendedStats,const string &reversePath,const string &ErrMsg,const vector<UI32> &HopCount,const vector<UI32> &InPort,const vector<UI32> &OutPort,const vector<UI32> &RBridgeId,const vector<UI32> &Bandwidth,const vector<UI32> &IsReversePath,const vector<UI32> &Cost,const vector<UI32> &ShortIntvl,const vector<UI32> &LongIntvl,const vector<UI32> &InTxShortBps,const vector<UI32> &InRxShortBps,const vector<UI32> &OutTxShortBps,const vector<UI32> &OutRxShortBps,const vector<UI32> &InTxLongBps,const vector<UI32> &InRxLongBps,const vector<UI32> &OutTxLongBps,const vector<UI32> &OutRxLongBps,const vector<UI32> &InTxShortTxCrdz,const vector<UI32> &InRxShortTxCrdz,const vector<UI32> &OutTxShortTxCrdz,const vector<UI32> &OutRxShortTxCrdz,const vector<UI32> &InTxLongTxCrdz,const vector<UI32> &InRxLongTxCrdz,const vector<UI32> &OutTxLongTxCrdz,const vector<UI32> &OutRxLongTxCrdz,const vector<UI32> &InTxShortFps,const vector<UI32> &InRxShortFps,const vector<UI32> &OutTxShortFps,const vector<UI32> &OutRxShortFps,const vector<UI32> &InTxLongFps,const vector<UI32> &InRxLongFps,const vector<UI32> &OutTxLongFps,const vector<UI32> &OutRxLongFps,const vector<string> &InTxWords,const vector<string> &InRxWords,const vector<string> &OutTxWords,const vector<string> &OutRxWords,const vector<string> &InTxFrames,const vector<string> &InRxFrames,const vector<string> &OutTxFrames,const vector<string> &OutRxFrames,const vector<UI32> &InTxErrors,const vector<UI32> &InRxErrors,const vector<UI32> &OutTxErrors,const vector<UI32> &OutRxErrors, const vector<string> &inputTuples, const vector<string> &outputTuples, const vector<UI32> &fullHop)
        : PrismMessage (FCPathUtilLocalObjectManager::getPrismServiceId (), FCPATHUTILGETPATHINFO),
        m_FID    (FID),
        m_destDomainId    (destDomainId),
        m_sourcePort    (sourcePort),
        m_destPort    (destPort),
        m_basicStats    (basicStats),
        m_extendedStats    (extendedStats),
        m_reversePath    (reversePath),
        m_ErrMsg    (ErrMsg),
        m_HopCount    (HopCount),
        m_InPort    (InPort),
        m_OutPort    (OutPort),
        m_RBridgeId    (RBridgeId),
        m_Bandwidth    (Bandwidth),
        m_IsReversePath    (IsReversePath),
        m_Cost    (Cost),
        m_ShortIntvl    (ShortIntvl),
        m_LongIntvl    (LongIntvl),
        m_InTxShortBps    (InTxShortBps),
        m_InRxShortBps    (InRxShortBps),
        m_OutTxShortBps    (OutTxShortBps),
        m_OutRxShortBps    (OutRxShortBps),
        m_InTxLongBps    (InTxLongBps),
        m_InRxLongBps    (InRxLongBps),
        m_OutTxLongBps    (OutTxLongBps),
        m_OutRxLongBps    (OutRxLongBps),
        m_InTxShortTxCrdz    (InTxShortTxCrdz),
        m_InRxShortTxCrdz    (InRxShortTxCrdz),
        m_OutTxShortTxCrdz    (OutTxShortTxCrdz),
        m_OutRxShortTxCrdz    (OutRxShortTxCrdz),
        m_InTxLongTxCrdz    (InTxLongTxCrdz),
        m_InRxLongTxCrdz    (InRxLongTxCrdz),
        m_OutTxLongTxCrdz    (OutTxLongTxCrdz),
        m_OutRxLongTxCrdz    (OutRxLongTxCrdz),
        m_InTxShortFps    (InTxShortFps),
        m_InRxShortFps    (InRxShortFps),
        m_OutTxShortFps    (OutTxShortFps),
        m_OutRxShortFps    (OutRxShortFps),
        m_InTxLongFps    (InTxLongFps),
        m_InRxLongFps    (InRxLongFps),
        m_OutTxLongFps    (OutTxLongFps),
        m_OutRxLongFps    (OutRxLongFps),
        m_InTxWords    (InTxWords),
        m_InRxWords    (InRxWords),
        m_OutTxWords    (OutTxWords),
        m_OutRxWords    (OutRxWords),
        m_InTxFrames    (InTxFrames),
        m_InRxFrames    (InRxFrames),
        m_OutTxFrames    (OutTxFrames),
        m_OutRxFrames    (OutRxFrames),
        m_InTxErrors    (InTxErrors),
        m_InRxErrors    (InRxErrors),
        m_OutTxErrors    (OutTxErrors),
        m_OutRxErrors    (OutRxErrors),
        m_InputTuples    (inputTuples),
        m_OutputTuples    (outputTuples),
		m_fullHop		(fullHop)
    {
    }

    FCPathUtilGetPathInfoMessage::~FCPathUtilGetPathInfoMessage ()
    {
    }

    void  FCPathUtilGetPathInfoMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_FID,"FID"));
        addSerializableAttribute (new AttributeSI32(&m_destDomainId,"destDomainId"));
        addSerializableAttribute (new AttributeSI32(&m_sourcePort,"sourcePort"));
        addSerializableAttribute (new AttributeSI32(&m_destPort,"destPort"));
        addSerializableAttribute (new AttributeString(&m_basicStats,"basicStats"));
        addSerializableAttribute (new AttributeString(&m_extendedStats,"extendedStats"));
        addSerializableAttribute (new AttributeString(&m_reversePath,"reversePath"));
        addSerializableAttribute (new AttributeString(&m_ErrMsg,"ErrMsg"));
        addSerializableAttribute (new AttributeUI32Vector(&m_HopCount,"HopCount"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InPort,"InPort"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutPort,"OutPort"));
        addSerializableAttribute (new AttributeUI32Vector(&m_RBridgeId,"RBridgeId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_Bandwidth,"Bandwidth"));
        addSerializableAttribute (new AttributeUI32Vector(&m_IsReversePath,"IsReversePath"));
        addSerializableAttribute (new AttributeUI32Vector(&m_Cost,"Cost"));
        addSerializableAttribute (new AttributeUI32Vector(&m_ShortIntvl,"ShortIntvl"));
        addSerializableAttribute (new AttributeUI32Vector(&m_LongIntvl,"LongIntvl"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxShortBps,"InTxShortBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxShortBps,"InRxShortBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxShortBps,"OutTxShortBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxShortBps,"OutRxShortBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxLongBps,"InTxLongBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxLongBps,"InRxLongBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxLongBps,"OutTxLongBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxLongBps,"OutRxLongBps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxShortTxCrdz,"InTxShortTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxShortTxCrdz,"InRxShortTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxShortTxCrdz,"OutTxShortTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxShortTxCrdz,"OutRxShortTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxLongTxCrdz,"InTxLongTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxLongTxCrdz,"InRxLongTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxLongTxCrdz,"OutTxLongTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxLongTxCrdz,"OutRxLongTxCrdz"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxShortFps,"InTxShortFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxShortFps,"InRxShortFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxShortFps,"OutTxShortFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxShortFps,"OutRxShortFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxLongFps,"InTxLongFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxLongFps,"InRxLongFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxLongFps,"OutTxLongFps"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxLongFps,"OutRxLongFps"));
        addSerializableAttribute (new AttributeStringVector(&m_InTxWords,"InTxWords"));
        addSerializableAttribute (new AttributeStringVector(&m_InRxWords,"InRxWords"));
        addSerializableAttribute (new AttributeStringVector(&m_OutTxWords,"OutTxWords"));
        addSerializableAttribute (new AttributeStringVector(&m_OutRxWords,"OutRxWords"));
        addSerializableAttribute (new AttributeStringVector(&m_InTxFrames,"InTxFrames"));
        addSerializableAttribute (new AttributeStringVector(&m_InRxFrames,"InRxFrames"));
        addSerializableAttribute (new AttributeStringVector(&m_OutTxFrames,"OutTxFrames"));
        addSerializableAttribute (new AttributeStringVector(&m_OutRxFrames,"OutRxFrames"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InTxErrors,"InTxErrors"));
        addSerializableAttribute (new AttributeUI32Vector(&m_InRxErrors,"InRxErrors"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutTxErrors,"OutTxErrors"));
        addSerializableAttribute (new AttributeUI32Vector(&m_OutRxErrors,"OutRxErrors"));
        addSerializableAttribute (new AttributeStringVector(&m_InputTuples,"InputTuples"));
        addSerializableAttribute (new AttributeStringVector(&m_OutputTuples,"outputTuples"));
        addSerializableAttribute (new AttributeUI32Vector(&m_fullHop,"fullHop"));
    }

    void  FCPathUtilGetPathInfoMessage::setFID(const SI32 &FID)
    {
        m_FID  =  FID;
    }

    SI32  FCPathUtilGetPathInfoMessage::getFID() const
    {
        return (m_FID);
    }

    void  FCPathUtilGetPathInfoMessage::setDestDomainId(const SI32 &destDomainId)
    {
        m_destDomainId  =  destDomainId;
    }

    SI32  FCPathUtilGetPathInfoMessage::getDestDomainId() const
    {
        return (m_destDomainId);
    }

    void  FCPathUtilGetPathInfoMessage::setSourcePort(const SI32 &sourcePort)
    {
        m_sourcePort  =  sourcePort;
    }

    SI32  FCPathUtilGetPathInfoMessage::getSourcePort() const
    {
        return (m_sourcePort);
    }

    void  FCPathUtilGetPathInfoMessage::setDestPort(const SI32 &destPort)
    {
        m_destPort  =  destPort;
    }

    SI32  FCPathUtilGetPathInfoMessage::getDestPort() const
    {
        return (m_destPort);
    }

    void  FCPathUtilGetPathInfoMessage::setBasicStats(const string &basicStats)
    {
        m_basicStats  =  basicStats;
    }

    string  FCPathUtilGetPathInfoMessage::getBasicStats() const
    {
        return (m_basicStats);
    }

    void  FCPathUtilGetPathInfoMessage::setExtendedStats(const string &extendedStats)
    {
        m_extendedStats  =  extendedStats;
    }

    string  FCPathUtilGetPathInfoMessage::getExtendedStats() const
    {
        return (m_extendedStats);
    }

    void  FCPathUtilGetPathInfoMessage::setReversePath(const string &reversePath)
    {
        m_reversePath  =  reversePath;
    }

    string  FCPathUtilGetPathInfoMessage::getReversePath() const
    {
        return (m_reversePath);
    }

    void  FCPathUtilGetPathInfoMessage::setErrMsg(const string &ErrMsg)
    {
        m_ErrMsg  =  ErrMsg;
    }

    string  FCPathUtilGetPathInfoMessage::getErrMsg() const
    {
        return (m_ErrMsg);
    }

    void  FCPathUtilGetPathInfoMessage::setHopCount(const vector<UI32> &HopCount)
    {
        m_HopCount  =  HopCount;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getHopCount() const
    {
        return (m_HopCount);
    }

    void  FCPathUtilGetPathInfoMessage::setInPort(const vector<UI32> &InPort)
    {
        m_InPort  =  InPort;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInPort() const
    {
        return (m_InPort);
    }

    void  FCPathUtilGetPathInfoMessage::setOutPort(const vector<UI32> &OutPort)
    {
        m_OutPort  =  OutPort;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutPort() const
    {
        return (m_OutPort);
    }

    void  FCPathUtilGetPathInfoMessage::setRBridgeId(const vector<UI32> &RBridgeId)
    {
        m_RBridgeId  =  RBridgeId;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getRBridgeId() const
    {
        return (m_RBridgeId);
    }

    void  FCPathUtilGetPathInfoMessage::setBandwidth(const vector<UI32> &Bandwidth)
    {
        m_Bandwidth  =  Bandwidth;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getBandwidth() const
    {
        return (m_Bandwidth);
    }

    void  FCPathUtilGetPathInfoMessage::setIsReversePath(const vector<UI32> &IsReversePath)
    {
        m_IsReversePath  =  IsReversePath;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getIsReversePath() const
    {
        return (m_IsReversePath);
    }

    void  FCPathUtilGetPathInfoMessage::setCost(const vector<UI32> &Cost)
    {
        m_Cost  =  Cost;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getCost() const
    {
        return (m_Cost);
    }

    void  FCPathUtilGetPathInfoMessage::setShortIntvl(const vector<UI32> &ShortIntvl)
    {
        m_ShortIntvl  =  ShortIntvl;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getShortIntvl() const
    {
        return (m_ShortIntvl);
    }

    void  FCPathUtilGetPathInfoMessage::setLongIntvl(const vector<UI32> &LongIntvl)
    {
        m_LongIntvl  =  LongIntvl;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getLongIntvl() const
    {
        return (m_LongIntvl);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxShortBps(const vector<UI32> &InTxShortBps)
    {
        m_InTxShortBps  =  InTxShortBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxShortBps() const
    {
        return (m_InTxShortBps);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxShortBps(const vector<UI32> &InRxShortBps)
    {
        m_InRxShortBps  =  InRxShortBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxShortBps() const
    {
        return (m_InRxShortBps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxShortBps(const vector<UI32> &OutTxShortBps)
    {
        m_OutTxShortBps  =  OutTxShortBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxShortBps() const
    {
        return (m_OutTxShortBps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxShortBps(const vector<UI32> &OutRxShortBps)
    {
        m_OutRxShortBps  =  OutRxShortBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxShortBps() const
    {
        return (m_OutRxShortBps);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxLongBps(const vector<UI32> &InTxLongBps)
    {
        m_InTxLongBps  =  InTxLongBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxLongBps() const
    {
        return (m_InTxLongBps);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxLongBps(const vector<UI32> &InRxLongBps)
    {
        m_InRxLongBps  =  InRxLongBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxLongBps() const
    {
        return (m_InRxLongBps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxLongBps(const vector<UI32> &OutTxLongBps)
    {
        m_OutTxLongBps  =  OutTxLongBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxLongBps() const
    {
        return (m_OutTxLongBps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxLongBps(const vector<UI32> &OutRxLongBps)
    {
        m_OutRxLongBps  =  OutRxLongBps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxLongBps() const
    {
        return (m_OutRxLongBps);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxShortTxCrdz(const vector<UI32> &InTxShortTxCrdz)
    {
        m_InTxShortTxCrdz  =  InTxShortTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxShortTxCrdz() const
    {
        return (m_InTxShortTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxShortTxCrdz(const vector<UI32> &InRxShortTxCrdz)
    {
        m_InRxShortTxCrdz  =  InRxShortTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxShortTxCrdz() const
    {
        return (m_InRxShortTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxShortTxCrdz(const vector<UI32> &OutTxShortTxCrdz)
    {
        m_OutTxShortTxCrdz  =  OutTxShortTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxShortTxCrdz() const
    {
        return (m_OutTxShortTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxShortTxCrdz(const vector<UI32> &OutRxShortTxCrdz)
    {
        m_OutRxShortTxCrdz  =  OutRxShortTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxShortTxCrdz() const
    {
        return (m_OutRxShortTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxLongTxCrdz(const vector<UI32> &InTxLongTxCrdz)
    {
        m_InTxLongTxCrdz  =  InTxLongTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxLongTxCrdz() const
    {
        return (m_InTxLongTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxLongTxCrdz(const vector<UI32> &InRxLongTxCrdz)
    {
        m_InRxLongTxCrdz  =  InRxLongTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxLongTxCrdz() const
    {
        return (m_InRxLongTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxLongTxCrdz(const vector<UI32> &OutTxLongTxCrdz)
    {
        m_OutTxLongTxCrdz  =  OutTxLongTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxLongTxCrdz() const
    {
        return (m_OutTxLongTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxLongTxCrdz(const vector<UI32> &OutRxLongTxCrdz)
    {
        m_OutRxLongTxCrdz  =  OutRxLongTxCrdz;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxLongTxCrdz() const
    {
        return (m_OutRxLongTxCrdz);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxShortFps(const vector<UI32> &InTxShortFps)
    {
        m_InTxShortFps  =  InTxShortFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxShortFps() const
    {
        return (m_InTxShortFps);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxShortFps(const vector<UI32> &InRxShortFps)
    {
        m_InRxShortFps  =  InRxShortFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxShortFps() const
    {
        return (m_InRxShortFps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxShortFps(const vector<UI32> &OutTxShortFps)
    {
        m_OutTxShortFps  =  OutTxShortFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxShortFps() const
    {
        return (m_OutTxShortFps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxShortFps(const vector<UI32> &OutRxShortFps)
    {
        m_OutRxShortFps  =  OutRxShortFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxShortFps() const
    {
        return (m_OutRxShortFps);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxLongFps(const vector<UI32> &InTxLongFps)
    {
        m_InTxLongFps  =  InTxLongFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxLongFps() const
    {
        return (m_InTxLongFps);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxLongFps(const vector<UI32> &InRxLongFps)
    {
        m_InRxLongFps  =  InRxLongFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxLongFps() const
    {
        return (m_InRxLongFps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxLongFps(const vector<UI32> &OutTxLongFps)
    {
        m_OutTxLongFps  =  OutTxLongFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxLongFps() const
    {
        return (m_OutTxLongFps);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxLongFps(const vector<UI32> &OutRxLongFps)
    {
        m_OutRxLongFps  =  OutRxLongFps;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxLongFps() const
    {
        return (m_OutRxLongFps);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxWords(const vector<string> &InTxWords)
    {
        m_InTxWords  =  InTxWords;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getInTxWords() const
    {
        return (m_InTxWords);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxWords(const vector<string> &InRxWords)
    {
        m_InRxWords  =  InRxWords;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getInRxWords() const
    {
        return (m_InRxWords);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxWords(const vector<string> &OutTxWords)
    {
        m_OutTxWords  =  OutTxWords;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getOutTxWords() const
    {
        return (m_OutTxWords);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxWords(const vector<string> &OutRxWords)
    {
        m_OutRxWords  =  OutRxWords;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getOutRxWords() const
    {
        return (m_OutRxWords);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxFrames(const vector<string> &InTxFrames)
    {
        m_InTxFrames  =  InTxFrames;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getInTxFrames() const
    {
        return (m_InTxFrames);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxFrames(const vector<string> &InRxFrames)
    {
        m_InRxFrames  =  InRxFrames;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getInRxFrames() const
    {
        return (m_InRxFrames);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxFrames(const vector<string> &OutTxFrames)
    {
        m_OutTxFrames  =  OutTxFrames;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getOutTxFrames() const
    {
        return (m_OutTxFrames);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxFrames(const vector<string> &OutRxFrames)
    {
        m_OutRxFrames  =  OutRxFrames;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getOutRxFrames() const
    {
        return (m_OutRxFrames);
    }

    void  FCPathUtilGetPathInfoMessage::setInTxErrors(const vector<UI32> &InTxErrors)
    {
        m_InTxErrors  =  InTxErrors;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInTxErrors() const
    {
        return (m_InTxErrors);
    }

    void  FCPathUtilGetPathInfoMessage::setInRxErrors(const vector<UI32> &InRxErrors)
    {
        m_InRxErrors  =  InRxErrors;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getInRxErrors() const
    {
        return (m_InRxErrors);
    }

    void  FCPathUtilGetPathInfoMessage::setOutTxErrors(const vector<UI32> &OutTxErrors)
    {
        m_OutTxErrors  =  OutTxErrors;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutTxErrors() const
    {
        return (m_OutTxErrors);
    }

    void  FCPathUtilGetPathInfoMessage::setOutRxErrors(const vector<UI32> &OutRxErrors)
    {
        m_OutRxErrors  =  OutRxErrors;
    }

    vector<UI32>  FCPathUtilGetPathInfoMessage::getOutRxErrors() const
    {
        return (m_OutRxErrors);
    }

    void  FCPathUtilGetPathInfoMessage::setInputTuples(const vector<string> &inputTuples)
    {
        m_InputTuples  =  inputTuples;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getInputTuples() const
    {
        return (m_InputTuples);
    }

    void  FCPathUtilGetPathInfoMessage::setOutputTuples(const vector<string> &outputTuples)
    {
        m_OutputTuples  =  outputTuples;
    }

    vector<string>  FCPathUtilGetPathInfoMessage::getOutputTuples() const
    {
        return (m_OutputTuples);
    }
    
	void  FCPathUtilGetPathInfoMessage::setfullHop(const vector<UI32> &fullHop)
    {
        m_fullHop=  fullHop;
    }

	vector<UI32>  FCPathUtilGetPathInfoMessage::getfullHop() const
    {
        return (m_fullHop);
    }

}
