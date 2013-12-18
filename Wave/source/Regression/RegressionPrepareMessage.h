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
 *   Copyright (C) 2006 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef REGRESSIONPREPAREMESSAGE_H
#define REGRESSIONPREPAREMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class RegressionPrepareMessage : public PrismMessage
{
    private :
    protected :
    public :
                        RegressionPrepareMessage        (PrismServiceId id);
                        RegressionPrepareMessage        ();
                       ~RegressionPrepareMessage        ();
        void            setupAttributesForSerialization ();
        void            addInputString                  (const string &inputString);
        vector<string> &getInputStrings                 ();

    // Now the data members

    private :
        vector<string> m_inputStrings;

    protected :
    public :
};

}

#endif // REGRESSIONPREPAREMESSAGE_H
