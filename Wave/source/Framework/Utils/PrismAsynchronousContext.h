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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#ifndef PRISMASYNCHRONOUSCONTEXT_H
#define PRISMASYNCHRONOUSCONTEXT_H

#include "Framework/ObjectModel/PrismElement.h"

namespace WaveNs
{

class PrismAsynchronousContext;
class WaveManagedObject;

typedef void (PrismElement::* PrismAsynchronousCallback) (PrismAsynchronousContext *);

class PrismAsynchronousContext
{
    private :
                void                      garbageCollect     ();
    protected :
    public :
                                          PrismAsynchronousContext              (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
                                          PrismAsynchronousContext              (const PrismAsynchronousContext &prismAsynchronousContext);
        virtual                          ~PrismAsynchronousContext              ();
                PrismAsynchronousContext &operator =                            (const PrismAsynchronousContext &prismAsynchronousContext);
                void                      setCompletionStatus                   (const ResourceId &status);
                ResourceId                getCompletionStatus                   ();
                void                     *getPCallerContext                     ();

                void                      callback                              ();

                void                      addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &managedObjectsForGarbageCollection);
                void                      addManagedObjectForGarbageCollection  (WaveManagedObject *pWaveManagedObjectForGarbageCollection);

    // Now the data members

    private :
        PrismElement              *      m_pCaller;
        PrismAsynchronousCallback        m_pCallback;
        void                      *const m_pCallerContext;
        ResourceId                       m_completionStatus;
        vector<WaveManagedObject *>      m_managedObjectsForGarbageCollection;

    protected :
    public :
};

}

#endif //PRISMASYNCHRONOUSCONTEXT_H
