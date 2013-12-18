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

#ifndef PRISMNATIVETEST1SERVICEMESSAGES_H
#define PRISMNATIVETEST1SERVICEMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

typedef enum
{
    WAVE_NATIVE_TEST1_SERVICE_MESSAGE1
} PrismNativeTest1ServiceMessageOpCodes;

class PrismNativeTest1ServiceMessage1 : public PrismMessage
{
    private :
    protected :
    public :
       PrismNativeTest1ServiceMessage1 ();
      ~PrismNativeTest1ServiceMessage1 ();

      // Now the data members

      private :
      protected :
      public :
};

}

#endif // PRISMNATIVETEST1SERVICEMESSAGES_H
