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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef VAL_INTERFACE_H
#define VAL_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
             char name[64];
             char ip_address[64];
    unsigned int  mtu;
} val_interface;

int query_interface (const char *p_name, val_interface *p_val_interface);

#ifdef __cplusplus
}
#endif

#endif // VAL_INTERFACE_H
