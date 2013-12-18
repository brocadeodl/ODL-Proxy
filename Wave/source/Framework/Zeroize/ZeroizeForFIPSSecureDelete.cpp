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


#include "ZeroizeForFIPSSecureDelete.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{
    //static char s_zeroBuf[1024];
   	//static ssize_t BLOCK_SIZE = 4096;
   	ZeroizeForFIPSSecureDelete::ZeroizeForFIPSSecureDelete()
   	{
   	}
   	
   	
void ZeroizeForFIPSSecureDelete::cleanDeleteDir(const string &m_dirName)
{
   		
    vector<string> commandOutput;
    ftw(m_dirName.c_str(),ZeroizeForFIPSSecureDelete::removeFileCallBack,20);
    string lDirDelCmd = string("rm -rf ") + m_dirName;
    FrameworkToolKit::systemCommandOutput (lDirDelCmd, commandOutput);
   	
}
   	
void ZeroizeForFIPSSecureDelete::cleanDeleteFile(const char *fpath, const struct stat *psb, bool deleteOnClear)
{
        int fd ;
        struct stat sb; 
        int n_blocks = 0;
        ssize_t remaining_size,f_size;
        char *s_zeroBuf;
        fd = open(fpath, O_WRONLY);
        if(fd < 0)
        {
            tracePrintf (TRACE_LEVEL_INFO,"ZeroizeForFIPSSecureDelete::cleanDeleteFile: cannot open file %s skipping cleanup\n",fpath);
            return ;
        }
        if( NULL == psb)
        {
            if(0 != fstat(fd,(struct stat*)&sb))
            {
                tracePrintf (TRACE_LEVEL_INFO,"ZeroizeForFIPSSecureDelete::cleanDeleteFile: cannot stat file %s skipping cleanup\n",fpath);
				close (fd);
                return ;
            }
            f_size = sb.st_size;

        }
        else 
        {
            f_size = psb->st_size;
        }
   
        n_blocks = f_size/ZeroizeForFIPSSecureDelete::BLOCK_SIZE;
   
        if(n_blocks > 0)
        {		
            /**
             * Write One block at a time 
             */
            s_zeroBuf = (char*)malloc(sizeof(char) * ZeroizeForFIPSSecureDelete::BLOCK_SIZE + 1);

            if(s_zeroBuf == NULL) 
            {
                prismAssert(false,__FILE__,__LINE__);
            }

            memset(s_zeroBuf,0,ZeroizeForFIPSSecureDelete::BLOCK_SIZE);

            for(int i = 0; i < n_blocks; i++)
            {
                if(write(fd,s_zeroBuf,ZeroizeForFIPSSecureDelete::BLOCK_SIZE) < 0 )
                {
                    tracePrintf (TRACE_LEVEL_DEBUG,"ZeroizeForFIPSSecureDelete::cleanDeleteFile: writing zero to file  %s \n failed",fpath);
                    break;
                }
            }
            remaining_size = n_blocks * ZeroizeForFIPSSecureDelete::BLOCK_SIZE - f_size;
        
        }
        else
        {
            s_zeroBuf = (char*)malloc(sizeof(char) * f_size);

            if(s_zeroBuf == NULL) 
                prismAssert(false,__FILE__,__LINE__);

            memset(s_zeroBuf,0,f_size);
            remaining_size = f_size;
        }
        /**
         * Fill in the remaining size.
         */
        if(write(fd,s_zeroBuf,remaining_size) < 0)
        {
            tracePrintf (TRACE_LEVEL_DEBUG,"ZeroizeForFIPSSecureDelete::cleanDeleteFile: writing zero to file  %s \n failed",fpath);
        }	
   
        close(fd);
        free(s_zeroBuf);	

        if(deleteOnClear)
        {	
            if(unlink(fpath) != 0)
            {
                tracePrintf (TRACE_LEVEL_INFO,"ZeroizeForFIPSSecureDelete::cleanDeleteFile: skipped deleting file %s\n",fpath);
            }
        }
        return ;
    }
   	
int ZeroizeForFIPSSecureDelete::removeFileCallBack(const char *fpath, const struct stat *sb, int typeflag)
{
    if(typeflag != FTW_F)
    {
        return 0;
    }

    ZeroizeForFIPSSecureDelete::cleanDeleteFile(fpath,sb);
    return 0;
}


void ZeroizeForFIPSSecureDelete::secureClearFile(const string &filePath,bool deleteOnClear)
{
        cleanDeleteFile(filePath.c_str(),NULL,deleteOnClear);
}
   	

}
