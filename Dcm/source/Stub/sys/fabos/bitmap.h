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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Anand Subramanian                                            *
 ***************************************************************************/

#ifndef __BITMAP_H__
#define __BITMAP_H__ 

#ifdef __cplusplus
extern "C"
{
#endif

#define BM_NBBY 8                   /* # bits per byte */
#define BM_NBPF (sizeof (unsigned long) * BM_NBBY)  /* # bits per field */

#define BM_SetBitN(n, p)    ((p)[(n)/BM_NBPF] |= \
                     ((unsigned)(1 << ((n) % BM_NBPF))))

/* check if bit 'n' in the given byte array pointed by 'p' is set */
#define BM_isBitNSet(n, p)  ((p)[(n)/BM_NBPF] & \
                         ((unsigned)(1 << ((n) % BM_NBPF))))
                 
#define BM_ARRAY_SIZE(num_bits) (((num_bits) % BM_NBPF) ? \
                                (((num_bits) / BM_NBPF) + 1) : \
                                ((num_bits) / BM_NBPF))
#define SIZEOF_BM(num_bits) (sizeof (u_long) *\
                                    BM_ARRAY_SIZE(num_bits))

#define BM_Clear(x, num_bits) memset((void *)x, 0,\
                            SIZEOF_BM(num_bits))


/* Typedefs for Domain and Port Bitmaps */
typedef unsigned long domainBitMap_t[256 / BM_NBPF];

#ifdef __cplusplus
}
#endif

#endif // __BITMAP_H__ 
