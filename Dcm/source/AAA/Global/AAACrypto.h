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
 *   Author : Kumar Ghanta                                                     *
 **************************************************************************/
#ifndef AAACRYPTO_H
#define AAACRYPTO_H

#include <string>
using namespace std;

namespace DcmNs
{
class AAACrypto
{
  protected:

  public:
	static string encryptPassword(const string &password);
	static string decryptPassword(const string &encodedPassword);

  /* private base64 functions */
  private:
	static unsigned char *base64_decode(unsigned char *src, int *rlen );
	static unsigned char *base64_encode(unsigned char *src, int srclen);
};
}
#endif	//AAACRYPTO_H
