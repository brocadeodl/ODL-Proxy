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
 *   Author : Vadiraj C S
 ***************************************************************************/


#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<ftw.h>
#include<string>
#include "Framework/Trace/TraceMessages.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{
	class ZeroizeForFIPSSecureDelete
	{
		private:
			ZeroizeForFIPSSecureDelete();
	
		public:
            static const ssize_t BLOCK_SIZE = 4096;
			static void cleanDeleteDir(const string &dirName);
			static void cleanDeleteFile(const char *fpath, const struct stat *psb,bool deleteOnClear = true);
			static int removeFileCallBack(const char *fpath, const struct stat *sb, int typeflag);
			static void secureClearFile(const string &filePath,bool deleteOnClear);
	};
	
}


