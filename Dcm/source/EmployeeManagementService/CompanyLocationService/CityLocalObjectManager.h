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

#ifndef CITY_LOCAL_OBJECT_MANAGER_H 
#define CITY_LOCAL_OBJECT_MANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class CityLocalObjectManager : public WaveLocalObjectManager
    {   
        private:
        protected:
        public:
            CityLocalObjectManager();
            virtual ~CityLocalObjectManager();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            static CityLocalObjectManager *getInstance();


        private:
        protected:
        public:

    };

}
#endif
