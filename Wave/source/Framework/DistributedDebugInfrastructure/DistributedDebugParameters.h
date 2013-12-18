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

#ifndef DISTRIBUTEDDEBUGPARAMETERS_H
#define DISTRIBUTEDDEBUGPARAMETERS_H

#include <vector>

#include "Framework/Types/Types.h"

using namespace std;

namespace WaveNs
{

class DistributedDebugParameters
{

   public:
                                        DistributedDebugParameters      ();
                                        DistributedDebugParameters      (const string & inputStringFromScript, const string & nosOrBash, const string & commaDelimitedLocations);
        virtual                        ~DistributedDebugParameters      ();

                void                    setInputStringFromScript        (const string & inputStringFromScript);
        const   string &                getInputStringFromScript        () const;
                void                    setNosOrBash                    (const string & nosOrBash);
        const   string &                getNosOrBash                    () const;
                void                    setCommaDelimitedLocationIds    (const string & commaDelimitedLocationIds);
        const   string &                getCommaDelimitedLocationIds    () const;
                void                    setOutputStringVector           (const vector<string> & outputStringVector);
        const   vector<string> &        getOutputStringVector           () const;
                void                    setLocationsToRunDebugScript    (const vector<LocationId> & locationsToRunDebugScript);
        const   vector<LocationId> &    getLocationsToRunDebugScript    () const;

   private:

                string                  m_inputStringFromScript;
                string                  m_nosOrBash;
                string                  m_commaDelimitedLocationIds;
                vector<LocationId>      m_locationsToRunDebugScript;
                vector<string>          m_outputStringVector;
};

}

#endif // DISTRIBUTEDDEBUGPARAMETERS_H
