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
 **************************************************************************/

#ifndef CONFIGFILEMGMTMESSAGE_H
#define CONFIGFILEMGMTMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class ConfigFileMgmtCopyToStartupAllowedMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ConfigFileMgmtCopyToStartupAllowedMessage();
            virtual    ~ConfigFileMgmtCopyToStartupAllowedMessage();
            void setResult(const int &result);
            int  getResult()  const;
            void setIsAllowed(const bool isAllowed);
            bool getIsAllowed() const;

// Now the data members
        private:
        protected:
        public:
            int     m_result;
            bool    m_isAllowed;
    };    
    
    class ConfigFileMgmtCopyToStartupConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ConfigFileMgmtCopyToStartupConfigMessage();
            ConfigFileMgmtCopyToStartupConfigMessage(const string &path);
            virtual    ~ConfigFileMgmtCopyToStartupConfigMessage();
            void setResult(const int &result);
            int  getResult()  const;

// Now the data members
        private:
        protected:
        public:
            string  m_path;
            int     m_result;
    };

    class ConfigFileMgmtCopyRunningToStartupMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ConfigFileMgmtCopyRunningToStartupMessage();
            virtual    ~ConfigFileMgmtCopyRunningToStartupMessage();
            void setResult(const int &result);
            int  getResult()  const;

// Now the data members
        private:
        protected:
        public:
            int     m_result;
    };

    class ConfigFileMgmtCopyDefaultToStartupMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ConfigFileMgmtCopyDefaultToStartupMessage();
            virtual    ~ConfigFileMgmtCopyDefaultToStartupMessage();
            void setResult(const int &result);
            int  getResult()  const;

// Now the data members
        private:
        protected:
        public:
            int     m_result;
    };
}

#endif // CONFIGFILEMGMTMESSAGE_H
