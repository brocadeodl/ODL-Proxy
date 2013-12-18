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
 *   Author : Murugesan												       *
 **************************************************************************/

#include <string.h>
#include <openssl/evp.h>
#include "SnmpAuthCrypto.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/TraceUtils.h"

using namespace WaveNs;

namespace DcmNs
{
/* Encoded length */
#define ENCODED_LEN(size)   (2 * size)

/* Prepares the data to transmit on the wire by doing base64 encoding.
 * It allocated the memory.
 */
unsigned char *SnmpAuthCrypto::base64_encode(unsigned char *src, int srclen,int *len)
{
	int enclen, padlen;
	unsigned char *encode_buf;
	EVP_ENCODE_CTX EVP_ctx;

	if (!src) {
		trace(TRACE_LEVEL_ERROR, "base64_encode Input error");
		return (NULL);
	}
	/* Encoded length is always less than the double size of the
	 * original string.
	 */
	encode_buf = (unsigned char *) calloc(sizeof(unsigned char *),
						ENCODED_LEN(srclen));
	if (!encode_buf) {
		trace(TRACE_LEVEL_ERROR, "base64_encode memory allocation failed");
		return NULL;
	}

    EVP_EncodeInit(&EVP_ctx);
    EVP_EncodeUpdate(&EVP_ctx, encode_buf, &enclen, src, srclen);
    EVP_EncodeFinal(&EVP_ctx, encode_buf + enclen , &padlen);
	*len = enclen + padlen;
	encode_buf[*len] = '\0';

	return (encode_buf);
}

/* Extract the base64 encoded data and perform base64 decoding.
 * It allocated the memory.
 */
unsigned char *SnmpAuthCrypto::base64_decode(unsigned char *src, int *rlen )
{
	int rc, padlen, decodeLen;
	unsigned char *decode_buf;
	EVP_ENCODE_CTX evp_ctx;

	if (!src || !rlen) {
		trace(TRACE_LEVEL_ERROR, "base64_decode Input error");
		return (NULL);
	}
	/* Final decrypted string length is less than encrypted and
	 * string.
	 */
	int srclen = strlen((char *)src);
	decode_buf = (unsigned char *)calloc(sizeof(unsigned char *), srclen);
	if (!decode_buf) {
		trace(TRACE_LEVEL_ERROR, "base64_decode Memory allocation failed");
		return (NULL);
	}

	EVP_DecodeInit(&evp_ctx);
	rc = EVP_DecodeUpdate(&evp_ctx, decode_buf, &decodeLen,
			src, srclen);
	if (rc < 0) {
		trace(TRACE_LEVEL_ERROR, "base64_decode Decode Update failed");
		free(decode_buf);
		return (NULL);
	}
	rc = EVP_DecodeFinal(&evp_ctx, decode_buf + decodeLen, &padlen);
	if (rc < 0) {
		trace(TRACE_LEVEL_ERROR, "base64_decode DecodeFinal failed");
		free(decode_buf);
		return (NULL);
	}
	*rlen = decodeLen + padlen;

	return (decode_buf);
}
}
