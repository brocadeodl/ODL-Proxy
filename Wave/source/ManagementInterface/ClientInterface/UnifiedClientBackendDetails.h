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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : cgangwar                                                     *
 ***************************************************************************/

#ifndef UNIFIEDCLIENTBACKENDDETAILS_H
#define UNIFIEDCLIENTBACKENDDETAILS_H

#include "Framework/Types/WaveCValue.h"

namespace WaveNs
{
    class UnifiedClientBackendDetails
    {
        private:
        protected:
        public:
                                UnifiedClientBackendDetails      ();
            virtual             ~UnifiedClientBackendDetails     ();
                        void    setBackendNotifyValue (UI32 &backendNotifyValue);
                        UI32    getBackendNotifyValue();
                        void    setClientName(string &clientName);
                        string  getClientName();

       private:
           string      m_clientName;              
           UI32        m_backendNotifyValue;
       protected:
       public:
    };        
}
#endif