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

#ifndef FCOE_CLIENT_PLUGIN_CONTROLMESSAGE_H
#define FCOE_CLIENT_PLUGIN_CONTROLMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{   

    class FcoeClientPluginControlMessage : public DcmManagementInterfaceMessage
    {   
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientPluginControlMessage ();
            FcoeClientPluginControlMessage (const UI32 &oper_bmp, const UI32 &plugin_status);
            virtual    ~FcoeClientPluginControlMessage ();
            virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure) {
                return ;
            }

			void setOper_bmp(const UI32 &oper_bmp);
			UI32 getOper_bmp()  const;
            void setPlugin_status(const UI32 &plugin_status);
            UI32 getPlugin_status()  const;

// Now the data members

        private:
        protected:
        public:
			UI32  m_oper_bmp;
            UI32  m_plugin_status;
    };
}
#endif                                            //FCOE_CLIENT_PLUGIN_CONTROLMESSAGE_H


