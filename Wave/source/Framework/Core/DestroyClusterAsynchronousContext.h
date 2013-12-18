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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DESTROYCLUSTERASYNCHRONOUSCONTEXT_H
#define DESTROYCLUSTERASYNCHRONOUSCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"

namespace WaveNs
{

class DestroyClusterAsynchronousContext : public PrismAsynchronousContext
{
    private :
    protected :
    public :
                                                      DestroyClusterAsynchronousContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual                                      ~DestroyClusterAsynchronousContext ();

                FrameworkObjectManagerFailoverReason  getReasonForDestroyingCluster     () const;
                void                                  setReasonForDestroyingCluster     (const FrameworkObjectManagerFailoverReason &reasonForDestroyingCluster = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

                void                                  setOriginalRequester              (const PrismServiceId &prismServiceId);
                PrismServiceId                        getOriginalRequester              () const;

                void                                  setIsRebootRequired               (const bool &isRebootRequired);
                bool                                  getIsRebootRequired               () const;

                bool                                  getIsPreparingForAddNode          () const;
                void                                  setPreparingForAddNode            (const bool &isPreparingForAddNode);
    // Now the data members

    private :
        FrameworkObjectManagerFailoverReason m_reasonForDestroyingCluster;
        PrismServiceId                       m_originalRequester;
        bool                                 m_isRebootRequired;
        bool                                 m_isPreparingForAddNode;
    protected :
    public :
};

}

#endif // DESTROYCLUSTERASYNCHRONOUSCONTEXT_H
