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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVECONFIGURATIONSEGMENTWORKER_H
#define WAVECONFIGURATIONSEGMENTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

class ManagementInterfaceMessage;
class WaveConfigurationSegmentCreateContext;
class WaveConfigurationSegmentDeleteContext;

class WaveConfigurationSegmentWorker : public WaveWorker
{
    private:
    protected:
    public: 
                    WaveConfigurationSegmentWorker  (WaveObjectManager *pWaveObjectManager, const string &configurationSegmentName, const bool isConfigExistValidationRequired = true);
                   ~WaveConfigurationSegmentWorker  ();

        void                                        setConfigurationSegmentName     (const string &configurationSegmentName);
        string                                      getConfigurationSegmentName     () const;
    
        virtual     ManagementInterfaceMessage *    createConfig                    (WaveConfigurationSegmentCreateContext *pWaveConfigurationSegmentCreateContext);
        void                        createConfigValidation ();

        ManagementInterfaceMessage *updateConfig ();
        void                        updateConfigValidation ();
        
        virtual ManagementInterfaceMessage *deleteConfig (WaveConfigurationSegmentDeleteContext *pWaveConfigurationSegmentDeleteContext);

        void                        deleteConfigValidation ();

        bool                        getIsConfigExistValidationRequired () const;
        void                        setIsConfigExistValidationRequired (const bool isConfigExistValidationRequired);

        virtual     string			getErrorString(ManagementInterfaceMessage* pMessage);

    // Data Members 
    private:
        string      m_configurationSegmentName;
        bool        m_isConfigExistValidationRequired;
    protected:
    public: 

};

}

#endif
