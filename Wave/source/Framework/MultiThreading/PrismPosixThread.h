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
#ifndef PRISMPOSIXTHREAD_H
#define PRISMPOSIXTHREAD_H

#include <iostream>
#include <pthread.h>
#include "Framework/MultiThreading/WaveThreadStatus.h"
#include "Framework/MultiThreading/PrismPosixThreadContext.h"
#include "WaveResourceIdEnums.h"
#include "Framework/Types/Types.h"

namespace WaveNs
{

class PrismPosixThread;

typedef PrismPosixThread *(*PrismPosixThreadStartMethodType) (PrismPosixThread *);
typedef void *(*PosixThreadStartMethodType) (void *);
typedef pthread_t PrismThreadId;

class PrismPosixThread
{
    public :
                                         PrismPosixThread  (const UI32 &stackSize = 0);
                                         PrismPosixThread  (const PrismPosixThread &prismPosixThread);
        virtual                         ~PrismPosixThread  ();
                PrismPosixThread        &operator =        (const PrismPosixThread &prismPosixThread);
        virtual WaveThreadStatus        run               ();
                WaveThreadStatus        joinCallingThread ();
        const   PrismPosixThreadContext *getThreadContext  ();
                PrismThreadId            getId             () const;

    protected :
        virtual WaveThreadStatus        start               ();
        virtual PrismPosixThreadContext *createThreadContext ();
        virtual void                     stopCallingThread   ();

    private :
        static PrismPosixThread *pthreadStartMethod (PrismPosixThread *pPrismPoixThread);

    private:
        pthread_t                m_pThreadId;
        PrismPosixThreadContext *m_pThreadContext;
        UI32                     m_stackSize;
};

}

#endif
