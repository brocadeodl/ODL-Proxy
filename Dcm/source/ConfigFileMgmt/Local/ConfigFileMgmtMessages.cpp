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

#include "ConfigFileMgmt/Local/ConfigFileMgmtLocalObjectManager.h"
#include "ConfigFileMgmt/Local/ConfigFileMgmtLocalTypes.h"
#include "ConfigFileMgmt/Local/ConfigFileMgmtMessages.h"

namespace DcmNs
{

    //////////////////////////////////////////////////////////////
    // ConfigFileMgmtCopyToStartupAllowedMessage Implementation //
    //////////////////////////////////////////////////////////////
    ConfigFileMgmtCopyToStartupAllowedMessage::ConfigFileMgmtCopyToStartupAllowedMessage ()
        : ManagementInterfaceMessage (ConfigFileMgmtLocalObjectManager::getClassName (), COPY_TO_STARTUP_ALLOWED)
    {
    }

    ConfigFileMgmtCopyToStartupAllowedMessage::~ConfigFileMgmtCopyToStartupAllowedMessage ()
    {
    }

    void  ConfigFileMgmtCopyToStartupAllowedMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool (&m_isAllowed, "isAllowed"));
    }

    void  ConfigFileMgmtCopyToStartupAllowedMessage::setResult(const int &result)
    {
        m_result  =  result;
    }

    int  ConfigFileMgmtCopyToStartupAllowedMessage::getResult() const
    {
        return (m_result);
    }
    
    void  ConfigFileMgmtCopyToStartupAllowedMessage::setIsAllowed(const bool isAllowed)
    {
        m_isAllowed  =  isAllowed;
    }

    bool  ConfigFileMgmtCopyToStartupAllowedMessage::getIsAllowed() const
    {
        return (m_isAllowed);
    }

    //////////////////////////////////////////////////////////////
    // ConfigFileMgmtCopyToStartupConfigMessage Implementation  //
    //////////////////////////////////////////////////////////////
    ConfigFileMgmtCopyToStartupConfigMessage::ConfigFileMgmtCopyToStartupConfigMessage ()
        : ManagementInterfaceMessage (ConfigFileMgmtLocalObjectManager::getClassName (), COPY_TO_STARTUP_CONFIG)
    {
    }

    ConfigFileMgmtCopyToStartupConfigMessage::ConfigFileMgmtCopyToStartupConfigMessage(const string &path)
        : ManagementInterfaceMessage (ConfigFileMgmtLocalObjectManager::getClassName (), COPY_TO_STARTUP_CONFIG), m_path (path)
    {
    }

    ConfigFileMgmtCopyToStartupConfigMessage::~ConfigFileMgmtCopyToStartupConfigMessage ()
    {
    }

    void  ConfigFileMgmtCopyToStartupConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_path, "path"));
    }

    void  ConfigFileMgmtCopyToStartupConfigMessage::setResult(const int &result)
    {
        m_result  =  result;
    }

    int  ConfigFileMgmtCopyToStartupConfigMessage::getResult() const
    {
        return (m_result);
    }

    //////////////////////////////////////////////////////////////
    // ConfigFileMgmtCopyRunningToStartupMessage Implementation //
    //////////////////////////////////////////////////////////////
    ConfigFileMgmtCopyRunningToStartupMessage::ConfigFileMgmtCopyRunningToStartupMessage () : ManagementInterfaceMessage (ConfigFileMgmtLocalObjectManager::getClassName (), COPY_RUNNING_TO_STARTUP)
    {
    }

    ConfigFileMgmtCopyRunningToStartupMessage::~ConfigFileMgmtCopyRunningToStartupMessage ()
    {
    }

    void  ConfigFileMgmtCopyRunningToStartupMessage::setResult(const int &result)
    {
        m_result  =  result;
    }

    int  ConfigFileMgmtCopyRunningToStartupMessage::getResult() const
    {
        return (m_result);
    }

    void  ConfigFileMgmtCopyRunningToStartupMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_result, "result"));
    }

    //////////////////////////////////////////////////////////////
    // ConfigFileMgmtCopyDefaultToStartupMessage Implementation //
    //////////////////////////////////////////////////////////////
    ConfigFileMgmtCopyDefaultToStartupMessage::ConfigFileMgmtCopyDefaultToStartupMessage () : ManagementInterfaceMessage (ConfigFileMgmtLocalObjectManager::getClassName (), COPY_DEFAULT_TO_STARTUP)
    {
    }

    ConfigFileMgmtCopyDefaultToStartupMessage::~ConfigFileMgmtCopyDefaultToStartupMessage ()
    {
    }

    void  ConfigFileMgmtCopyDefaultToStartupMessage::setResult(const int &result)
    {
        m_result  =  result;
    }

    int  ConfigFileMgmtCopyDefaultToStartupMessage::getResult() const
    {
        return (m_result);
    }

    void  ConfigFileMgmtCopyDefaultToStartupMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_result, "result"));
    }

}

