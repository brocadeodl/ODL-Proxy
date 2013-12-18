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
 * URLEncode.cpp
 *
 *  Created on: Jun 26, 2012
 *      Author: rmadapur
 */

#include "discover/utils/URLEncode.h"

namespace DcmNs {

    URLEncode::URLEncode() {
        // TODO Auto-generated constructor stub

    }

    URLEncode::~URLEncode() {
        // TODO Auto-generated destructor stub
    }

    string URLEncode::mac_conver_format(const string &text) {
        vector<string> tokens;
        char sep = ':';
        size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens[0] + tokens[1] + "." + tokens[2] + tokens[3] + "."
                + tokens[4] + tokens[5];
    }

    string URLEncode::dbencode(const string &c) {
        string escaped = "";
        int max = c.length();
        for (int i = 0; i < max; i++) {
            if (c[i] == '\'') {
                escaped.append("'");
            }
            escaped.append(&c[i], 1);

        }

        return escaped;
    }

    string URLEncode::urlencode(const string &c) {

        string escaped = "";
        int max = c.length();
        for (int i = 0; i < max; i++) {
            if ((48 <= c[i] && c[i] <= 57) || //0-9
                    (65 <= c[i] && c[i] <= 90) || //abc...xyz
                    (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
                    (c[i] == '-' || c[i] == '_' || c[i] == '.' )) {
                escaped.append(&c[i], 1);
            }
            else if (c[i] == ' ') {
                escaped.append("+", 1);
            }
            else {
                escaped.append("@");
                escaped.append(char2hex(c[i])); //converts char 255 to string "ff"
            }
        }
        return escaped;
    }

    string URLEncode::char2hex(char dec) {
        char dig1 = (dec & 0xF0) >> 4;
        char dig2 = (dec & 0x0F);
        if (0 <= dig1 && dig1 <= 9)
            dig1 += 48; //0,48inascii
        if (10 <= dig1 && dig1 <= 15)
            dig1 += 97 - 10; //a,97inascii
        if (0 <= dig2 && dig2 <= 9)
            dig2 += 48;
        if (10 <= dig2 && dig2 <= 15)
            dig2 += 97 - 10;

        string r;
        r.append(&dig1, 1);
        r.append(&dig2, 1);
        return r;
    }

} /* namespace DcmNs */
