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

#ifndef __CONFIG_CONFIG_SW_H__
#define __CONFIG_CONFIG_SW_H__

#ifdef __cplusplus
extern "C"
{
#endif

static inline int  configInit(void)
{
	return (0);
}

static inline int configGet(const char *key, int type, void *value)
{
	if (!key || !value)
		return (-1);

	return (0);
}

static inline int configSet(const char *key, int type, void *value)
{
	if (!key || !value)
		return (-1);

    return (0);
}


#ifdef __cplusplus
}
#endif

#endif
