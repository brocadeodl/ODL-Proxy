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
 *   Author : Kumar Ghanta                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "FCPathUtilLocalObjectManager.h"
#include "FCPathUtilTypes.h"
#include "FCPathUtilGetPathInfoMessage.h"

#include "DcmResourceIdEnums.h"

extern "C" {
  #include "ms/pathinfo.h"
}
namespace DcmNs
{

    FCPathUtilLocalObjectManager::FCPathUtilLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        addOperationMap (FCPATHUTILGETPATHINFO, reinterpret_cast<PrismMessageHandler> (&FCPathUtilLocalObjectManager::getPathInfoResults));
    }

    FCPathUtilLocalObjectManager::~FCPathUtilLocalObjectManager ()
    {
    }

    FCPathUtilLocalObjectManager  *FCPathUtilLocalObjectManager::getInstance()
    {
        static FCPathUtilLocalObjectManager *pFCPathUtilLocalObjectManager = new FCPathUtilLocalObjectManager ();

        WaveNs::prismAssert (NULL != pFCPathUtilLocalObjectManager, __FILE__, __LINE__);

        return (pFCPathUtilLocalObjectManager);
    }

    string  FCPathUtilLocalObjectManager::getClassName()
    {
        return ("FCPathUtil");
    }

    PrismServiceId  FCPathUtilLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *FCPathUtilLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case FCPATHUTILGETPATHINFO :
                pPrismMessage = new FCPathUtilGetPathInfoMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *FCPathUtilLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void FCPathUtilLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        trace (TRACE_LEVEL_INFO, "FCPathUtilLocalObjectManager::install()");
        pWaveAsynchronousContextForBootPhases->setCompletionStatus
            (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForBootPhases->callback ();

    }

    void FCPathUtilLocalObjectManager::getPathInfoResults(FCPathUtilGetPathInfoMessage *pFCPathUtilMessage)
    {
        dcm_in_parms_t dcm_in_param = {0};
        dcm_out_parms_t dcm_out_param = {0};
        vector<UI32>  HopCount;
		vector<UI32>  fullHop;
        vector<UI32>  InPort;
        vector<UI32>  OutPort;
        vector<UI32>  RBridgeId;
        vector<UI32>  Bandwidth;
        vector<UI32>  IsReversePath;
        vector<UI32>  Cost;
        vector<UI32>  ShortIntvl;
        vector<UI32>  LongIntvl;
        vector<UI32>  InTxShortBps;
        vector<UI32>  InRxShortBps;
        vector<UI32>  OutTxShortBps;
        vector<UI32>  OutRxShortBps;
        vector<UI32>  InTxLongBps;
        vector<UI32>  InRxLongBps;
        vector<UI32>  OutTxLongBps;
        vector<UI32>  OutRxLongBps;
        vector<UI32>  InTxShortTxCrdz;
        vector<UI32>  InRxShortTxCrdz;
        vector<UI32>  OutTxShortTxCrdz;
        vector<UI32>  OutRxShortTxCrdz;
        vector<UI32>  InTxLongTxCrdz;
        vector<UI32>  InRxLongTxCrdz;
        vector<UI32>  OutTxLongTxCrdz;
        vector<UI32>  OutRxLongTxCrdz;
        vector<UI32>  InTxShortFps;
        vector<UI32>  InRxShortFps;
        vector<UI32>  OutTxShortFps;
        vector<UI32>  OutRxShortFps;
        vector<UI32>  InTxLongFps;
        vector<UI32>  InRxLongFps;
        vector<UI32>  OutTxLongFps;
        vector<UI32>  OutRxLongFps;
        vector<string>  InTxWords;
        vector<string>  InRxWords;
        vector<string>  OutTxWords;
        vector<string>  OutRxWords;
        vector<string>  InTxFrames;
        vector<string>  InRxFrames;
        vector<string>  OutTxFrames;
        vector<string>  OutRxFrames;
        vector<UI32>  InTxErrors;
        vector<UI32>  InRxErrors;
        vector<UI32>  OutTxErrors;
        vector<UI32>  OutRxErrors;
        vector<string>  inputTuples;
        vector<string>  outputTuples;

        if (pFCPathUtilMessage == NULL) {
            // add trace
            trace (TRACE_LEVEL_ERROR, string("FCPathUtilLocalObjectManager") +
                string("::getPathInfoResults: pFCPathUtilMessage NULL"));
            pFCPathUtilMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            reply (pFCPathUtilMessage);
            return;
        }
        dcm_in_param.destDomainId = pFCPathUtilMessage->getDestDomainId();
        dcm_in_param.fabricID = pFCPathUtilMessage->getFID();
        dcm_in_param.sourcePort = pFCPathUtilMessage->getSourcePort();
        dcm_in_param.destPort = pFCPathUtilMessage->getDestPort();
        strcpy(dcm_in_param.basicStats,
            (char *)((pFCPathUtilMessage->getBasicStats()).c_str()));
        strcpy(dcm_in_param.extendedStats,
            (char *)((pFCPathUtilMessage->getExtendedStats()).c_str()));
        strcpy(dcm_in_param.reversePath,
            (char *)((pFCPathUtilMessage->getReversePath()).c_str()));
        dcm_in_param.errMsgSize = 2048;
        dcm_in_param.errMsg = (char *)calloc(dcm_in_param.errMsgSize,
            sizeof (char));
        lsPATHI_hdr_t *pathiP = NULL;
        trace (TRACE_LEVEL_INFO, string("dcm_in_param = ") +
            reinterpret_cast<UI64>(&dcm_in_param) + string("pathiP = ") +
            reinterpret_cast<UI64>(pathiP));
        int ret = 0, hop = 0;
        int size_pathi = 0;

        ret = get_pathinfo_dcm(&dcm_in_param, (char **)&pathiP, &size_pathi);
        if(ret == 0)
        {
            hop = pack_out_pathinfo_dcm(pathiP, size_pathi, &dcm_in_param,
                &dcm_out_param);
        } else {
			trace (TRACE_LEVEL_ERROR, string("Error: ") + dcm_in_param.errMsg);
            trace (TRACE_LEVEL_ERROR,
                string("FCPathUtilLocalObjectManager::getPathInfoResults:") +
                string(" ret = ") + ret);
            pFCPathUtilMessage->setErrMsg(
                const_cast<const char *>(dcm_in_param.errMsg));
	       if(pathiP) {
			   free(pathiP);
			   pathiP = NULL;
		   }
		   if (dcm_in_param.errMsg) {
			   free(dcm_in_param.errMsg);
			   dcm_in_param.errMsg = NULL;
		   }
		   pFCPathUtilMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		   reply (pFCPathUtilMessage);
		   return;
        }
        if(pathiP) {
			free(pathiP);
			pathiP = NULL;
		}
		if (dcm_in_param.errMsg) {
			free(dcm_in_param.errMsg);
			dcm_in_param.errMsg = NULL;
		}

        if(ret >= 0)
        {
            uint num_hops = 0;
			//This Hop count includes the current switch too.
            // unsigned int hop_count = dcm_out_param.num_hops + 1;
            trace (TRACE_LEVEL_INFO,
                string("FCPathUtilLocalObjectManager::getPathInfoResults:") +
                string(" num_hops = ") + dcm_out_param.num_hops);
            if(dcm_out_param.hop_pathinfo == NULL) {
                trace (TRACE_LEVEL_ERROR,
                    string("FCPathUtilLocalObjectManager::getPathInfoResults")+ string(": dcm_out_param.hop_pathinfo NULL"));
            	pFCPathUtilMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
            	reply (pFCPathUtilMessage);
            	return;
            }
            while ((hop > 0) && (hop < PATHI_DEF_HOPS)) {
            trace (TRACE_LEVEL_INFO, string("FCPathUtilLocalObjectManager") +
                string("::getPathInfoResults: hop_count =") + num_hops);
            trace (TRACE_LEVEL_INFO, string("FCPathUtilLocalObjectManager") +
                string("::getPathInfoResults: total HOPs =") + hop);
				fullHop.push_back(num_hops);
            	HopCount.push_back(
            		dcm_out_param.hop_pathinfo[num_hops].hop_count);
            	IsReversePath.push_back(
                 	dcm_out_param.hop_pathinfo[num_hops].is_reverse_path);
                InPort.push_back(
                    dcm_out_param.hop_pathinfo[num_hops].in_port);
                OutPort.push_back(
                    dcm_out_param.hop_pathinfo[num_hops].out_port);
                RBridgeId.push_back(
                    dcm_out_param.hop_pathinfo[num_hops].domain);
                Bandwidth.push_back(
                    dcm_out_param.hop_pathinfo[num_hops].bandwidth);
                Cost.push_back(dcm_out_param.hop_pathinfo[num_hops].cost);
                ShortIntvl.push_back(
                    dcm_out_param.hop_pathinfo[num_hops].short_interval);
                LongIntvl.push_back(
                    dcm_out_param.hop_pathinfo[num_hops].long_interval);
                
                InTxShortBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_s[PATHINFO_DCM_IN_TX]);
                InRxShortBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_s[PATHINFO_DCM_IN_RX]);
                OutTxShortBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_s[PATHINFO_DCM_OUT_TX]);
                OutRxShortBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_s[PATHINFO_DCM_OUT_RX]);
                InTxLongBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_l[PATHINFO_DCM_IN_TX]);
                InRxLongBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_l[PATHINFO_DCM_IN_RX]);
                OutTxLongBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_l[PATHINFO_DCM_OUT_TX]);
                OutRxLongBps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    bps_l[PATHINFO_DCM_OUT_RX]);
                InTxShortTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_s[PATHINFO_DCM_IN_TX]);
                InRxShortTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_s[PATHINFO_DCM_IN_RX]);
                OutTxShortTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_s[PATHINFO_DCM_OUT_TX]);
                OutRxShortTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_s[PATHINFO_DCM_OUT_RX]);
                InTxLongTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_l[PATHINFO_DCM_IN_TX]);
                InRxLongTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_l[PATHINFO_DCM_IN_RX]);
                OutTxLongTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_l[PATHINFO_DCM_OUT_TX]);
                OutRxLongTxCrdz.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    tx_crdz_l[PATHINFO_DCM_OUT_RX]);
                InTxShortFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
                    fps_s[PATHINFO_DCM_IN_TX]);
                InRxShortFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_s[PATHINFO_DCM_IN_RX]);
                OutTxShortFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_s[PATHINFO_DCM_OUT_TX]);
                OutRxShortFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_s[PATHINFO_DCM_OUT_RX]);
                InTxLongFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_l[PATHINFO_DCM_IN_TX]);
                InRxLongFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_l[PATHINFO_DCM_IN_RX]);
                OutTxLongFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_l[PATHINFO_DCM_OUT_TX]);
                OutRxLongFps.push_back(dcm_out_param.hop_pathinfo[num_hops].
					fps_l[PATHINFO_DCM_OUT_RX]);
                InTxWords.push_back(dcm_out_param.hop_pathinfo[num_hops].
					words[PATHINFO_DCM_IN_TX]);
                InRxWords.push_back(dcm_out_param.hop_pathinfo[num_hops].
					words[PATHINFO_DCM_IN_RX]);
                OutTxWords.push_back(dcm_out_param.hop_pathinfo[num_hops].
					words[PATHINFO_DCM_OUT_TX]);
                OutRxWords.push_back(dcm_out_param.hop_pathinfo[num_hops].
					words[PATHINFO_DCM_OUT_RX]);
                InTxFrames.push_back(dcm_out_param.hop_pathinfo[num_hops].
					frames[PATHINFO_DCM_IN_TX]);
                InRxFrames.push_back(dcm_out_param.hop_pathinfo[num_hops].
					frames[PATHINFO_DCM_IN_RX]);
                OutTxFrames.push_back(dcm_out_param.hop_pathinfo[num_hops].
					frames[PATHINFO_DCM_OUT_TX]);
                OutRxFrames.push_back(dcm_out_param.hop_pathinfo[num_hops].
					frames[PATHINFO_DCM_OUT_RX]);
                InTxErrors.push_back(dcm_out_param.hop_pathinfo[num_hops].
					errors[PATHINFO_DCM_IN_TX]);
                InRxErrors.push_back(dcm_out_param.hop_pathinfo[num_hops].
					errors[PATHINFO_DCM_IN_RX]);
                OutTxErrors.push_back(dcm_out_param.hop_pathinfo[num_hops].
					errors[PATHINFO_DCM_OUT_TX]);
                OutRxErrors.push_back(dcm_out_param.hop_pathinfo[num_hops].
					errors[PATHINFO_DCM_OUT_RX]);
				
            	trace (TRACE_LEVEL_INFO,
                string("FCPathUtilLocalObjectManager::getPathInfoResults:") +
                string(" inpuTuples = ") + dcm_out_param.hop_pathinfo[num_hops].input_tuples);
				if (dcm_out_param.hop_pathinfo[num_hops].input_tuples) {
                	inputTuples.push_back(dcm_out_param.hop_pathinfo[num_hops].
						input_tuples);
				}
            	trace (TRACE_LEVEL_INFO,
                string("FCPathUtilLocalObjectManager::getPathInfoResults:") +
                string(" outputTuples = ") + dcm_out_param.hop_pathinfo[num_hops].output_tuples);
				if (dcm_out_param.hop_pathinfo[num_hops].output_tuples) {
                	outputTuples.push_back(dcm_out_param.hop_pathinfo[num_hops].
						output_tuples);
				}
                num_hops++;
				hop--;
            }
            trace (TRACE_LEVEL_INFO,
                string("FCPathUtilLocalObjectManager::getPathInfoResults:") +
                string(" Full HopCount.size = ") + fullHop.size());
            pFCPathUtilMessage->setHopCount(HopCount);
			pFCPathUtilMessage->setfullHop(fullHop);
            pFCPathUtilMessage->setInPort(InPort);
            pFCPathUtilMessage->setOutPort(OutPort);
            pFCPathUtilMessage->setRBridgeId(RBridgeId);
            pFCPathUtilMessage->setBandwidth(Bandwidth);
            pFCPathUtilMessage->setIsReversePath(IsReversePath);
            pFCPathUtilMessage->setCost(Cost);
            pFCPathUtilMessage->setShortIntvl(ShortIntvl);
            pFCPathUtilMessage->setLongIntvl(LongIntvl);
            pFCPathUtilMessage->setInTxShortBps(InTxShortBps);
            pFCPathUtilMessage->setInRxShortBps(InRxShortBps);
            pFCPathUtilMessage->setOutTxShortBps(OutTxShortBps);
            pFCPathUtilMessage->setOutRxShortBps(OutRxShortBps);
            pFCPathUtilMessage->setInTxLongBps(InTxLongBps);
            pFCPathUtilMessage->setInRxLongBps(InRxLongBps);
            pFCPathUtilMessage->setOutTxLongBps(OutTxLongBps);
            pFCPathUtilMessage->setOutRxLongBps(OutRxLongBps);
            pFCPathUtilMessage->setInTxShortTxCrdz(InTxShortTxCrdz);
            pFCPathUtilMessage->setInRxShortTxCrdz(InRxShortTxCrdz);
            pFCPathUtilMessage->setOutTxShortTxCrdz(OutTxShortTxCrdz);
            pFCPathUtilMessage->setOutRxShortTxCrdz(OutRxShortTxCrdz);
            pFCPathUtilMessage->setInTxLongTxCrdz(InTxLongTxCrdz);
            pFCPathUtilMessage->setInRxLongTxCrdz(InRxLongTxCrdz);
            pFCPathUtilMessage->setOutTxLongTxCrdz(OutTxLongTxCrdz);
            pFCPathUtilMessage->setOutRxLongTxCrdz(OutRxLongTxCrdz);
            pFCPathUtilMessage->setInTxShortFps(InTxShortFps);
            pFCPathUtilMessage->setInRxShortFps(InRxShortFps);
            pFCPathUtilMessage->setOutTxShortFps(OutTxShortFps);
            pFCPathUtilMessage->setOutRxShortFps(OutRxShortFps);
            pFCPathUtilMessage->setInTxLongFps(InTxLongFps);
            pFCPathUtilMessage->setInRxLongFps(InRxLongFps);
            pFCPathUtilMessage->setOutTxLongFps(OutTxLongFps);
            pFCPathUtilMessage->setOutRxLongFps(OutRxLongFps);
            pFCPathUtilMessage->setInTxWords(InTxWords);
            pFCPathUtilMessage->setInRxWords(InRxWords);
            pFCPathUtilMessage->setOutTxWords(OutTxWords);
            pFCPathUtilMessage->setOutRxWords(OutRxWords);
            pFCPathUtilMessage->setInTxFrames(InTxFrames);
            pFCPathUtilMessage->setInRxFrames(InRxFrames);
            pFCPathUtilMessage->setOutTxFrames(OutTxFrames);
            pFCPathUtilMessage->setOutRxFrames(OutRxFrames);
            pFCPathUtilMessage->setInTxErrors(InTxErrors);
            pFCPathUtilMessage->setInRxErrors(InRxErrors);
            pFCPathUtilMessage->setOutTxErrors(OutTxErrors);
            pFCPathUtilMessage->setOutRxErrors(OutRxErrors);
            pFCPathUtilMessage->setInputTuples(inputTuples);
            pFCPathUtilMessage->setOutputTuples(outputTuples);
            pFCPathUtilMessage->setErrMsg(
                const_cast<const char *>(dcm_out_param.status_msg));
        }
        else
        {
			trace (TRACE_LEVEL_ERROR, string("Error: ") + dcm_out_param.status_msg);
            trace (TRACE_LEVEL_ERROR,
                string("FCPathUtilLocalObjectManager::getPathInfoResults:") +
                string(" ret = ") + ret);
            pFCPathUtilMessage->setErrMsg(
                const_cast<const char *>(dcm_out_param.status_msg));
        }
        // free(dcm_out_param.status_msg);
        free(dcm_out_param.hop_pathinfo);
        pFCPathUtilMessage->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        reply (pFCPathUtilMessage);

 }
#if 0
    void  FCPathUtilLocalObjectManager::FCPathUtilGetPathInfoMessageHandler( FCPathUtilGetPathInfoMessage *pFCPathUtilGetPathInfoMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FCPathUtilLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FCPathUtilLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FCPathUtilLocalObjectManager::getPathInfoResults),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FCPathUtilLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&FCPathUtilLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFCPathUtilGetPathInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

#endif
}
