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

/*
 * URLEncode.h
 *
 *  Created on: Jun 26, 2012
 *      Author: rmadapur
 */

#ifndef URLENCODE_H_
#define URLENCODE_H_
#include <string>
#include <vector>

using namespace std;
namespace DcmNs {

    class URLEncode {
        public:
            URLEncode();
            virtual ~URLEncode();
            static string urlencode(const string &c);
            static string dbencode(const string &c);
            static string mac_conver_format(const string &text);
        protected:
            static string char2hex( char dec );
    };

} /* namespace DcmNs */
#endif /* URLENCODE_H_ */
