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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Leifang Hu                                   *
 ***************************************************************************/

#ifndef HASYNCWORKERSEQUENCERCONTEXT_H
#define HASYNCWORKERSEQUENCERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"

using namespace std;

namespace WaveNs
{

class StartHaSyncDumpContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                StartHaSyncDumpContext   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                StartHaSyncDumpContext   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                ~StartHaSyncDumpContext   ();
    // Now the data members

    private : 
        vector<PrismServiceId> m_prismServiceIdsVector;
        vector<PrismServiceId> m_prismServiceIdsToCommunicate;
        vector<void *>         m_validationDetailsVector;
        vector<UI32>           m_validationDetailsSizesVector;

        map<string, void *>    m_validationResultsVector;
        map<string, UI32>      m_validationResultsSizesVector;

        PrismMessage *         m_standbyMessage;
        UI32                   m_haVersion;
        bool                   m_isDbSyncRequired;
        bool                   m_isDbResumeRequired;
        UI32                   m_contextInfo;
        string                 m_haPeerIp;
        SI32                   m_haPeerPort;
        

    protected :
    public :
        vector<PrismServiceId> &getPrismServiceIdsToCommunicate ();
        vector<PrismServiceId> &getPrismServiceIdsVector        ();

        void                    addValidationDetailsForService  (const PrismServiceId &prismServiceId, void *pValidationDetials, const UI32 size);
        vector<void *>         &getValidationDetailsVector      ();
        vector<UI32>           &getValidationDetailsSizesVector ();
        void                    addValidationResultsForService (const PrismServiceId &prismServiceId, void *const &pValidationResults, const UI32 &validationResultsSize);

        void                    getValidationResultsForService  (const PrismServiceId &prismServiceId, void *&pValidationResults, UI32 &size);
        void                    setStandbyMessage               (PrismMessage *standbyMessage);
        PrismMessage *          getStandbyMessage               ();
        void                    setHaVersion                    (const UI32 &haVersion);
        UI32                    getHaVersion                    ();
        void                    setIsDbSyncRequired             (const bool &isDbSyncRequired);
        bool                    getIsDbSyncRequired             ();
        void                    setIsDbResumeRequired           (const bool &isDbResumeRequired);
        bool                    getIsDbResumeRequired           ();
        UI32                    getContextInfo                  ();
        void                    setContextInfo                  (const UI32 &contextInfo);
        string                  getHaPeerIp                     () const;
        void                    setHaPeerIp                     (const string& haPeerIp);
        SI32                    getHaPeerPort                   () const;
        void                    setHaPeerPort                   (const SI32& port);    
};

class ReceiveHaSyncDumpContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                ReceiveHaSyncDumpContext   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                ReceiveHaSyncDumpContext   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                ~ReceiveHaSyncDumpContext   ();
        void                    setIsDbSyncRequired             (const bool &isDbSyncRequired);
        bool                    getIsDbSyncRequired             ();
        UI32                    getContextInfo                  ();
        void                    setContextInfo                  (const UI32 &contextInfo);
        string                  getHaPeerIp                     () const;
        void                    setHaPeerIp                     (const string& haPeerIp);
        SI32                    getHaPeerPort                   () const;
        void                    setHaPeerPort                   (const SI32& port);    
    // Now the data members

    private :
        bool                   m_isDbSyncRequired;
        UI32                   m_contextInfo;
        string                 m_haPeerIp;
        SI32                   m_haPeerPort;

    protected :
    public :
};


}

#endif // PRISMFRAMEWORKOBJECTMANAGERHASYNCWORKERSEQUENCERCONTEXT_H 
