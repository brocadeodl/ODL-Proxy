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

#ifndef APPLICATIONSERVICEMESSAGES_H
#define APPLICATIONSERVICEMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

typedef enum
{
    APPLICATION_SERVICE_GENERIC_MESSAGE = 0
} ApplicationServiceOperationCodes;

typedef enum
{
    APPLICATION_SERVICE_GENERIC_MESSAGE_INPUT_BUFFER1 = 0,
    APPLICATION_SERVICE_GENERIC_MESSAGE_OUTPUT_BUFFER1 = 1
} ApplicationServiceMessageBufgferTags;

class ApplicationServiceMessage : public PrismMessage
{
    private :
    protected :
        void setupAttributesForSerialization ();

    public :
              ApplicationServiceMessage ();
              ApplicationServiceMessage (PrismServiceId prismServiceId);
             ~ApplicationServiceMessage ();

        void *getInputBuffer1           (UI32 &bufferLength);
        void *getOutputBuffer1          (UI32 &bufferLength);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // APPLICATIONSERVICEMESSAGES_H
