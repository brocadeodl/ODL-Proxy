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
 *   Author : tgajendr                                                     *
 **************************************************************************/

#ifndef LICENSESERVICESHOWMESSAGE_H
#define LICENSESERVICESHOWMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class LicenseServiceShowMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            LicenseServiceShowMessage ();
            LicenseServiceShowMessage (const vector<string> &license,
                const vector<string> &expirydatestr,
                const vector<string> &featureName,
                const vector<string> &capacity,
                const vector<string> &consumed,
                const vector<string> &validState
                );
            virtual    ~LicenseServiceShowMessage ();
            void setLicense(const vector<string> &license);
            vector<string> getLicense()  const;
            void setExpirydatestr(const vector<string> &expirydatestr);
	        vector<string> getExpirydatestr()  const;
    	    void setFeatureName(const vector<string> &featureName);
	        vector<string> getFeatureName()  const;	  
    	    void setCapacity(const vector<string> &capacity);
	        vector<string> getCapacity()  const;	  
    	    void setConsumed(const vector<string> &consumed);
	        vector<string> getConsumed()  const;	  
            void setValidState(const vector<string> &validState);
            vector<string> getValidState()  const;    

// Now the data members

        private:
        protected:
        public:
            vector<string>  m_license;
	        vector<string>  m_expirydatestr;
	        vector<string>  m_featureName;
	        vector<string>  m_capacity;
	        vector<string>  m_consumed;
            vector<string>  m_validState;
    };
}
#endif                                            //LICENSESERVICESHOWMESSAGE_H
