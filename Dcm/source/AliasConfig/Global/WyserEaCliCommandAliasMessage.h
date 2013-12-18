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
*   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : cgangwar                                                     *
***************************************************************************/

#ifndef WYSEREACLICOMMANDALIASMESSAGE_H
#define WYSEREACLICOMMANDALIASMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
 
using namespace WaveNs;
 
namespace DcmNs
{
    class WyserEaCliCommandAliasMessage : public PrismMessage
    {
        private:
        protected:
             virtual     void    setupAttributesForSerialization    ();
        public:
                                 WyserEaCliCommandAliasMessage        (const UI32 &opcode);
                                 WyserEaCliCommandAliasMessage        (const UI32 &opcode, const UI32 &cmdcode,const string &username, const string &aliasname, const string &aliasexpansion, bool &startTimer,const UI32 &operationmode, const UI32 &aliasTypeEnumValue);

             virtual            ~WyserEaCliCommandAliasMessage        ();
                        void    setCmdCode                          (const UI32 &cmdcode);
                        UI32    getCmdCode                          () const;
                        void    setusername                         (const string &username);
                        string  getusername                         ()  const;
                        void    setaliasname                        (const string &aliasname);
                        string  getaliasname                        ()  const;
                        void    setaliasexpansion                   (const string &aliasexpansion);
                        string  getaliasexpansion                   ()  const;
                        void    setStartTimer                       (bool &startTimer);
                        bool    getStartTimer                       ();
                        UI32    getOperationMode                    ();    
                        UI32    getAliasType                        () const;

        private:
        protected:
        public:
                        UI32   m_cmdcode; 
                        string m_username;
                        string m_aliasname;
                        string m_aliasexpansion;
                        bool   m_startTimer;        
                        UI32   m_operationMode;  
                        UI32   m_aliasType;
       };
}    


#endif
