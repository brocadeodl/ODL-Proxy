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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DETACHFROMCLUSTERASYNCHRONOUSCONTEXT_H
#define DETACHFROMCLUSTERASYNCHRONOUSCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "FrameworkObjectManagerTypes.h"

namespace WaveNs
{

class DetachFromClusterAsynchronousContext : public PrismAsynchronousContext
{
    private :
    protected :
    public :
                                                      DetachFromClusterAsynchronousContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual                                      ~DetachFromClusterAsynchronousContext ();

                FrameworkObjectManagerFailoverReason  getReasonForDetachingFromCluster     () const;
                void                                  setReasonForDetachingFromCluster     (const FrameworkObjectManagerFailoverReason &reasonForDetachingFromCluster = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

    // Now the data members

    private:
        FrameworkObjectManagerFailoverReason m_reasonForDetachingFromCluster;

    protected :
    public :
};

}

#endif // DETACHFROMCLUSTERASYNCHRONOUSCONTEXT_H
