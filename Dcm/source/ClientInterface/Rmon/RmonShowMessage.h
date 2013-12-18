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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef RMONSHOWMESSAGE_H
#define RMONSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{
    class RmonShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            RmonShowMessage ();
            RmonShowMessage (const SI32 &index, const bool &showAll, const bool &showBrief,const UI32 &cmdCode, 
                             const UI32 &mappedId, const SI32 & startIndex, const SI32 & endIndex);
            virtual    ~RmonShowMessage ();

            void setIndex (const SI32 &index);
            SI32 getIndex ()  const;

			void setShowAll (const bool &showAll);
            bool getShowAll ()  const;

            void setShowBrief (const bool &showBrief);
            bool getShowBrief ()  const;

            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode ()  const;

            void setMappedId (const UI32 &mappedId);
            UI32 getMappedId ()  const;

			const virtual void *getCStructureForInputs();
			virtual void loadOutputsFromCStructure(const void *pOutputCStructure);
			void getOutputBufferRef(UI32 bufferNum, UI32 & size, void *& pBuffer);

			//Pagination methods
			SI32 getEndIndex() const;
			void setEndIndex(SI32 endIndex);

			SI32 getStartIndex() const;
			void setStartIndex(SI32 startIndex);

			SI32 getEndSampleIndex() const;
			void setEndSampleIndex(SI32 endSampleIndex);

			SI32 getStartSampleIndex() const;
			void setStartSampleIndex(SI32 startSampleIndex);
        // Now the data members
    private:
    protected:
    public:
        SI32 m_index;
        bool m_showAll;
        bool m_showBrief;
        UI32 m_cmdCode;
        UI32 m_mappedId;
        //Pagination attributes
		SI32 m_startIndex;
		SI32 m_endIndex;
		SI32 m_startSampleIndex;
		SI32 m_endSampleIndex;
    };
}
#endif                                            //RMONSHOWMESSAGE_H
