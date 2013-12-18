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
 *   Author : Navin
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/DceDbg/DceDbgMsg.h"
#include "ClientInterface/DceDbg/DceDbgMsgDef.h"

namespace DcmNs
{

    DceDbgMsg::DceDbgMsg ()
        : DcmManagementInterfaceMessage (DCE_DBG_MSG)
    {
    }

    DceDbgMsg::DceDbgMsg (const string &dbgArg, const UI32 &dbgCliCode)
        : DcmManagementInterfaceMessage (DCE_DBG_MSG),
        m_dbgArg    (dbgArg),
        m_dbgCliCode   (dbgCliCode),
        m_cookieFlag (0)
    {
    }

    DceDbgMsg::~DceDbgMsg ()
    {
    }

    void  DceDbgMsg::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_dbgCliCode,"dbgCliCode"));
        addSerializableAttribute (new AttributeString(&m_dbgArg,"dbgArg"));
        addSerializableAttribute (new AttributeUI8(&m_cookieFlag, "cookieFlag"));
    }

    void  DceDbgMsg::setDbgArg(const string &dbgArg)
    {
        m_dbgArg  =  dbgArg;
    }

    string  DceDbgMsg::getDbgArg() const
    {
        return (m_dbgArg);
    }
    
    void  DceDbgMsg::setDbgCliCode(const UI32 &dbgCliCode)
    {
        m_dbgCliCode  =  dbgCliCode;
    }

    UI32  DceDbgMsg::getDbgCliCode() const
    {
        return (m_dbgCliCode);
    }

    void  DceDbgMsg::setCookieFlag(const UI8 &cookieFlag)
    {
        m_cookieFlag  =  cookieFlag;
    }

    UI8  DceDbgMsg::getCookieFlag() const
    {
        return (m_cookieFlag);
    }

    void DceDbgMsg::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
							UI32 &size, void *&pBuffer)
    {
        pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *DceDbgMsg::getCStructureForInputs ()
    {
        void *cookie_data = NULL;
        char *mycookie = NULL;
		UI32 size = 0;
		dce_dbg_in_msg_t *pInput = 
			(dce_dbg_in_msg_t *) calloc (1, sizeof(dce_dbg_in_msg_t));

		if (pInput == NULL) {
			return (NULL);
		}

		cookie_data = findBuffer(COOKIE_REQ_TAG, size);

		if (size > 0 && cookie_data) {
            pInput->cookie_data.size = size;
            mycookie = (char *)calloc (1, size);
			
			if (NULL == mycookie) {
				return (NULL);
			}
            memcpy(mycookie, cookie_data, size);
            pInput->cookie_data.data = mycookie;
        }
        pInput->cookie_flag = m_cookieFlag;

        strncpy (pInput->dbg_argv, m_dbgArg.c_str (), 200);
        pInput->dbg_cli_code    = m_dbgCliCode;

        return (pInput);
    }

    void DceDbgMsg::loadOutputsFromCStructure (const void 
						*pOutputCStructure)
    {
        dce_dbg_out_msg_t *msg = ((dce_dbg_out_msg_t *)pOutputCStructure);

        if (msg == 0) {
          return;
        }

        removeAllBuffers();

		/* Add Data Buffer */

        addBuffer (DCE_DBG_MSG, msg->out_buf_len, pOutputCStructure, false);

		/* Add Cookie buffer only if cookie_flag is set by backend */
        if (msg->cookie_flag) {
            addBuffer(COOKIE_RESP_TAG, msg->cookie_data.size,
                            (char *)(msg->cookie_data.data), false);
        }

        setCookieFlag(msg->cookie_flag);
    }
}
