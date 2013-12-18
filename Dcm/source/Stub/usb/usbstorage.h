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

#ifndef __USB_STORAGE_H__
#define __USB_STORAGE_H__
#ifdef __cplusplus
extern "C"
{
#endif

#define USB_PATH_MAX    128

static inline int usbstorage_init(char* area, char* path)
{
    if (area) {
        if (strcmp(area, "config") != 0)
            return (-1);
    } else {
        return (-1);
    }

    if (path) {
        path[0] = '\0';
        return (0);
    } else {
        return (-1);
    }
}


#ifdef __cplusplus
}
#endif
#endif
