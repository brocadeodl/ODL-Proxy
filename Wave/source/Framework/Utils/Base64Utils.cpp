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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Utils/Base64Utils.h"

#include <string.h>

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

namespace WaveNs
{

string Base64Utils::encode (const string& input)
{
    BIO     *pBmem;
    BIO     *pB64;
    BUF_MEM *pBuffer;
    string   output;
    char    *pOutputBuffer = NULL;
    int      temp		   = 0;

    pB64  = BIO_new  (BIO_f_base64 ());
    pBmem = BIO_new  (BIO_s_mem ());
    pB64  = BIO_push (pB64, pBmem);

    BIO_write       (pB64, input.c_str (), input.length ());
    temp = BIO_flush       (pB64);

    if (0 != temp)
    {
    	// Nothing to do.
    }

    BIO_get_mem_ptr (pB64, &pBuffer);

    pOutputBuffer = (char *) malloc (pBuffer->length);

    memcpy (pOutputBuffer, pBuffer->data, pBuffer->length - 1);
    pOutputBuffer[pBuffer->length - 1] = 0;

    output = pOutputBuffer;

    BIO_free_all (pB64);
    free         (pOutputBuffer);

    return (output);
}

string Base64Utils::decode (const string& input)
{
    BIO    *pB64;
    BIO    *pBmem;
    char   *pOutputBuffer = (char *) malloc (input.length ());
    string  output;

    memset(pOutputBuffer, 0, input.length ());

    pB64 = BIO_new (BIO_f_base64 ());

    BIO_set_flags (pB64, BIO_FLAGS_BASE64_NO_NL);

    pBmem = BIO_new_mem_buf ((void *) input.c_str (), input.length ());
    pBmem = BIO_push        (pB64, pBmem);

    BIO_read (pBmem, pOutputBuffer, input.length ());

    output = pOutputBuffer;

    BIO_free_all (pBmem);
    free (pOutputBuffer);

    return output;
}

}
