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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
//#include "WaveResourceIdEnums.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "AAACrypto.h"
extern "C" {
	#include "security/public.h"
}

using namespace WaveNs;

namespace DcmNs
{

/* Encoded length */
#define ENCODED_LEN(size)   (2 * size)

/* Define Password Key and Initialization Vector */
#define PasswordKey "dsR4sOa7wPa0drabcDe9nmuTCiwArGzy"
#define PasswordIV "vceL6aKdp9asDWSfgEf08JcvSabfGnfs"

/* Extract the base64 encoded data and perform base64 decoding.
 * It allocated the memory.
 */
unsigned char * AAACrypto::base64_decode(unsigned char *src, int *rlen )
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
		FREE(decode_buf);
		return (NULL);
	}
	rc = EVP_DecodeFinal(&evp_ctx, decode_buf + decodeLen, &padlen);
	if (rc < 0) {
		trace(TRACE_LEVEL_ERROR, "base64_decode DecodeFinal failed");
		FREE(decode_buf);
		return (NULL);
	}
	*rlen = decodeLen + padlen;

	return (decode_buf);
}

/* Prepares the data to transmit on the wire by doing base64 encoding.
 * It allocated the memory.
 */
unsigned char *AAACrypto::base64_encode(unsigned char *src, int srclen)
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
	encode_buf[enclen + padlen] = '\0';

	return (encode_buf);
}

/*
 * This function encrypts the password and prepare it for transmission
 * by doing base64 encoding.
 */
string AAACrypto::encryptPassword(const string &password)
{
	unsigned char *encryptedBuf = NULL, *finalBuf = NULL;
	int enclen = 0, padlen = 0, allocSz = 0;
	EVP_CIPHER_CTX ctx;
	int rc = 0;
	string encryptedPassword = "";
	int passwdlen = password.length();

	if (passwdlen == 0) {
		trace(TRACE_LEVEL_ERROR, "Zero length password input");
		return (encryptedPassword);
	}

	/* Initializes the cipher context ctx. */
	EVP_CIPHER_CTX_init(&ctx);

	/* Sets up the cipher context "ctx" for encryption.
	 * Third parameter is Engine, where we can provide our own
	 * cipher/digest implementations, which we are not doing anyway.
	 * All the remaining parameters are self explanatory except the
	 * third parameter.
	 */
	rc = EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL,
							(const unsigned char*) PasswordKey,
							(const unsigned char*) PasswordIV);
	if (!rc) {
		trace(TRACE_LEVEL_ERROR, "encryptPassword EncryptInit failed");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return (encryptedPassword);
	}

	allocSz = passwdlen + EVP_CIPHER_CTX_block_size(&ctx);
	encryptedBuf = (unsigned char *)calloc(sizeof(unsigned char *), allocSz);
	if (!encryptedBuf) {
		trace(TRACE_LEVEL_ERROR,
				"encryptPassword Memory allocation error");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return (encryptedPassword);
	}

	rc = EVP_EncryptUpdate(&ctx, encryptedBuf, &enclen, (unsigned char*)password.c_str(), passwdlen);
	/* Encrypts the blocks of Data. */
	if (rc) {
		/* As padding is enabled, encrypts the `final' data, that is any
	 	 * data that remains in a partial block.
	 	 */
		rc = EVP_EncryptFinal_ex(&ctx, (encryptedBuf + enclen), &padlen);
		if (rc) {
			enclen += padlen;
			/*
	 	 	 * As the binary data can't be sent through wire, we should encode
	 	 	 * it in base64 format. No need to worry about the final encoded
	 	 	 * buffer size here as it is a C string and ends with '\0'.
	 	 	 */
			finalBuf = base64_encode(encryptedBuf, enclen);
		} else {
			trace(TRACE_LEVEL_ERROR, "encryptPassword EncryptFinal failed");
		}
	} else {
		trace(TRACE_LEVEL_ERROR, "encryptPassword EncryptUpdate failed");
	}

	FREE(encryptedBuf);

	/* Cleanup the cipher context. */
	EVP_CIPHER_CTX_cleanup(&ctx);

	if (finalBuf)
	{
		encryptedPassword = (char *)finalBuf;
		FREE(finalBuf);
	}
	else
	{
		trace(TRACE_LEVEL_ERROR, "encryptPassword base64_encode failed");
	}

	return (encryptedPassword);
}

/*
 * This function does the base64 decoding and decrypt the password.
 */
string AAACrypto::decryptPassword(const string &encodedPassword)
{
	unsigned char *passwd = NULL, *decodedBuf = NULL;
	int declen, padlen = 0;
	EVP_CIPHER_CTX ctx;
	int enclen = 0, rc = 0;
	int len = encodedPassword.length();
	string decryptedPassword = "";

	if (len == 0) {
		trace(TRACE_LEVEL_ERROR, "Zero length encoded password input");
		return (decryptedPassword);
	}

	/*
	 * base64 data arrived here. We should decode it  and get encrypted
	 * data.
	 */
	decodedBuf = base64_decode((unsigned char*)encodedPassword.c_str(), &enclen);
	if (!decodedBuf) {
		trace(TRACE_LEVEL_ERROR, "decryptPassword base64_decode failed");
		return (decryptedPassword);
	}
	/* Initializes the cipher context ctx. */
	memset(&ctx, 0, sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(&ctx);

	/*
	 * Plain text length is  always equal or lesser than cipher text.
	 * Cipher text length is always equal or lesser than cipher text.
	 * So we are safe to allocate the length.
	 */
	passwd = (unsigned char *)calloc(sizeof(unsigned char *), len);
	if (!passwd) {
		trace(TRACE_LEVEL_ERROR, "decryptPassword memory allocation failed");
		EVP_CIPHER_CTX_cleanup(&ctx);
		FREE(decodedBuf);
		return (decryptedPassword);
	}

	/*
	 * Sets up the cipher context "ctx" for decryption.
	 * Third parameter is Engine, where we can provide our own
	 * cipher/digest implementations, which we are not doing anyway.
	 * All the remaining parameters are self explanatory except the
	 * third parameter.
	 */
	rc = EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL,
							(const unsigned char*) PasswordKey,
							(const unsigned char*) PasswordIV);
	if (rc) {
		rc = EVP_DecryptUpdate(&ctx, passwd, &declen, decodedBuf, enclen);
		if (rc) {
			rc = EVP_DecryptFinal_ex(&ctx, passwd + declen, &padlen);
			if (rc) {
				passwd[declen + padlen] = '\0';
			} else {
				trace(TRACE_LEVEL_ERROR,"decryptPassword DecryptFinal failed");
	 		}
		} else {
			trace(TRACE_LEVEL_ERROR,"decryptPassword DecryptUpdate failed");
		}
	} else {
		trace(TRACE_LEVEL_ERROR, "decryptPassword DecryptInit failed");
	}

	/* Cleanup the cipher context. */
	EVP_CIPHER_CTX_cleanup(&ctx);

	/* Freeing the decoded buf. */
	FREE(decodedBuf);

	/* Return the decoded string */
	decryptedPassword = (char *)passwd;
	FREE(passwd);

	return (decryptedPassword);
}

}
