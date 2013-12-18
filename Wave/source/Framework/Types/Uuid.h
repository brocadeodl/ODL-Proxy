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

#ifndef UUID_H
#define UUID_H

#include "Framework/Types/Types.h"
#include "uuid/uuid.h"
#include <string>

namespace WaveNs
{

class Uuid
{

    private:
    protected:
    public:
                Uuid             ();
                Uuid             (const string &uuidInStringFormat);
                Uuid             (const Uuid &uuid);
               ~Uuid             ();

        string  toString         ()                                     const;
        bool    fromString       (const string &uuidInStringFormat);

        bool    operator ==      (const Uuid &uuid)                     const;
        bool    operator !=      (const Uuid &uuid)                     const;
        Uuid    &operator =      (const Uuid &uuid);

        void    getUuid          (uuid_t out)                           const;
        string  getUuidString    ()                                     const;
        UI64    getUuidHash      ()                                     const;

        void    clear            ();

    // Now the data members
    private:
        uuid_t      m_uuidInBinary;

    protected:
    public:
};

}

#endif //UUID_H
