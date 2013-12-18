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

#ifndef GIOT_GIOT_H
#define GIOT_GIOT_H

#include <giot/ioss.h>  /* ioss_t */

#ifdef __cplusplus
extern "C" 
{
#endif

/* opaque data type manipulated as reference at application level */
typedef struct giot giot_t;

/*
 * the following block is the collection of all that are declared
 * in this header file gathered here for readability purose.
 * the same follows with proper commenting as per coding style requirements.
 *
 * common traits:
 * - all functions return completion code (0 - OK, otherwise - otherwise)
 * - minimize usage of globals - ideally none.
 */

static inline int giot_create(giot_t **handle)
{
    return (0);
}

static inline int giot_free(giot_t **handle)
{
    return (0);
}

static inline int giot_register(giot_t *handle, ioss_t *ioss)
{
    return (0);
}

static inline int giot_unregister(giot_t *handle, ioss_t *ioss)
{
    return (0);
}

#ifdef __cplusplus
}
#endif

#endif  /* GIOT_GIOT_H */

