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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWSWITCHRECEIVERTHREAD_H
#define OPENFLOWSWITCHRECEIVERTHREAD_H

#include "Framework/MultiThreading/PrismPosixThread.h"

using namespace WaveNs;

namespace OpenFlowNs
{

class OpenFlowSwitchReceiverThread : public PrismPosixThread
{
    private :
    protected :
    public :
                                  OpenFlowSwitchReceiverThread ();
        virtual                  ~OpenFlowSwitchReceiverThread ();

        virtual WaveThreadStatus  start                        ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // OPENFLOWSWITCHRECEIVERTHREAD_H
