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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Sajad Bashir Qadri                                           *
 ***************************************************************************/

#ifndef WAVE_SYSTEM_MANAGEMENT_LOGGER_H_
#define WAVE_SYSTEM_MANAGEMENT_LOGGER_H_

#include <string>
#include <map>
#include <vector>

#include "Framework/Types/Types.h"

namespace WaveNs
{

class WaveSystemManagementLogger
{
  public:
                      WaveSystemManagementLogger  (TraceClientId traceClientId);
                      WaveSystemManagementLogger  (TraceClientId traceClientId, string prefixString);

    virtual           ~WaveSystemManagementLogger ();

            void      log     (const TraceLevel traceLevel, const string label, const vector<string>& attributeUserTags);
            void      log     (const TraceLevel traceLevel, const string label, const vector<UI32>& attributeUserTags);
            void      log     (const TraceLevel traceLevel, const string label, const map<string, string> input);

            void      debug   (const string label, const vector<string>& attributeUserTags);
            void      debug   (const string label, const vector<UI32>& attributeUserTags);
            void      debug   (const string label, const map<string, string> input);

            void      debug   (const string stringToTrace);
            void      info    (const string stringToTrace);
            void      error   (const string stringToTrace);

            bool      isDevelEnabled    ();
            bool      isDebugEnabled    ();
            bool      isInfoEnabled     ();

            TraceClientId     getTraceClientId();

  protected:
  private:
            bool      skipGate        (const TraceLevel traceLevel);
            void      traceDirectly   (const TraceLevel traceLevel, const string stringToTrace);

  // Member variable now
  private:
    TraceClientId           m_traceClientId;
    string                  m_prefixString;
};

}

#endif /* WAVE_SYSTEM_MANAGEMENT_LOGGER_H_ */
