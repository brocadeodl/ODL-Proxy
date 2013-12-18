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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef RMONCLIENTCREATECOLLECTIONHISTORYMESSAGE_H
#define RMONCLIENTCREATECOLLECTIONHISTORYMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RmonClientCreateCollectionHistoryMessage : public DcmManagementInterfaceMessage 
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonClientCreateCollectionHistoryMessage ();
            RmonClientCreateCollectionHistoryMessage (const string &ifName,const SI32 &historyIndex,const SI32 &buckets,
				const SI32 &interval,const string &owner);
            virtual    ~RmonClientCreateCollectionHistoryMessage ();
			void setIfName(const string &ifName);
            string getIfName()  const;
            void setHistoryIndex(const SI32 &historyIndex);
            SI32 getHistoryIndex()  const;
            void setBuckets(const SI32 &buckets);
            SI32 getBuckets()  const;
            void setInterval(const SI32 &interval);
            SI32 getInterval()  const;
            void setOwner(const string &owner);
            string getOwner()  const;
			virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);


// Now the data members

        private:
        protected:
        public:
			string  m_ifName;
            SI32  m_historyIndex;
            SI32  m_buckets;
            SI32  m_interval;
            string  m_owner;
    };
}
#endif                                            //RMONCLIENTCREATECOLLECTIONHISTORYMESSAGE_H
