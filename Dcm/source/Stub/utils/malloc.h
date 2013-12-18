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
 *    Copyright (c) 1996-2004 by Brocade Communications Systems, Inc.
 *    All rights reserved.
 *
 * malloc related definitions.
 */

#ifndef _FABOS_MALLOC_H_
#define	_FABOS_MALLOC_H_

#include <stdlib.h>
#include <string.h>

#define mallocType(type, i)             (type *)malloc(sizeof (type)*(i))
#define callocType(type, i)             (type *)calloc(i, sizeof (type))
#define reallocType(ptr, type, i)       (type *)realloc(ptr, sizeof (type)*(i))
#define zeroType(ptr, type, i)          memset(ptr, 0, sizeof (type)*(i))
#define memCheck(ptr)                   { if (ptr == NULL)  exit (-1); }
#define Free(ptr)                       { free(ptr); (ptr) = NULL; }

#endif	/* _FABOS_MALLOC_H_ */
