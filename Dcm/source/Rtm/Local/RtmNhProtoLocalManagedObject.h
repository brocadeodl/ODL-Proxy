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
 *   Author : hzhu                                                     *
 **************************************************************************/

#ifndef RTMNHPROTOLOCALMANAGEDOBJECT_H
#define RTMNHPROTOLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class RtmLocalObjectManager;
    class RtmNhProtoLocalManagedObject : virtual public DcmLocalManagedObject
    {
        private:
        protected:
            virtual void setupAttributesForPersistence();
            virtual void setupAttributesForCreate();
			virtual void setupKeys();
        public:
            RtmNhProtoLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager);
            virtual ~RtmNhProtoLocalManagedObject ();
            static string getClassName();
            void setNhProto(const UI32 &nhProto);
            UI32 getNhProto()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_nhProto;
    };
}
#endif                                            //RTMCONFIGLOCALMANAGEDOBJECT_H
