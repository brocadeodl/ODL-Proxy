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

#ifndef WAVEMANAGEDOBJECTLOADOPERATIONALDATACONTEXT_H
#define WAVEMANAGEDOBJECTLOADOPERATIONALDATACONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class WaveManagedObjectLoadOperationalDataContext : public PrismAsynchronousContext
{
    private :
    protected :
    public :
                        WaveManagedObjectLoadOperationalDataContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
                       ~WaveManagedObjectLoadOperationalDataContext ();

        vector<string>  getOperationalDataFields                    () const;
        void            setOperationalDataFields                    (const vector<string> &operationalDataFields);

        WaveManagedObject   *getPOwnerManagedObject                 () const;
        void                 setPOwnerManagedObject                 (WaveManagedObject *pOwnerManagedObject);

    // Now the data members

    private :
        vector<string>       m_operationalDataFields;
		WaveManagedObject   *m_pOwnerManagedObject;

    protected :
    public :
};

}

#endif // WAVEMANAGEDOBJECTLOADOPERATIONALDATACONTEXT_H
