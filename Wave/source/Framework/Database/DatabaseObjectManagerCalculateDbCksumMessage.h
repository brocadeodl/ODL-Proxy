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

/******************************************************************************************
 *  @file : DatabaseObjectManagerCalculateDbCksumMessage.h                                *
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.                              *
 *   All rights reserved.                                                                 *
 *   Description : declares a message DatabaseObjectManagerCalculateDbCksumMessage.       *
                   The handler calculates the checksum for the local database.            *
 *   Author : Pritee Ghosh                                                                *
 *   Date   : 02/22/2011                                                                  *
 ******************************************************************************************/

#ifndef DATABASEOBJECTMANAGERCALCULATEDBCKSUMMESSAGE_H
#define DATABASEOBJECTMANAGERCALCULATEDBCKSUMMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace std;

namespace WaveNs
{

    class DatabaseObjectManagerCalculateDbCksumMessage : public PrismMessage
    { 

        private :
            virtual void    setupAttributesForSerialization                     ();
        protected :
        public :
                            DatabaseObjectManagerCalculateDbCksumMessage        ();
            virtual         ~DatabaseObjectManagerCalculateDbCksumMessage       (); 

            string          getDbCksumForNode                                   () const;
            void            setDbCksumForNode                                   (const string &checksum);

            bool            getPauseTheService                                  () const;
            void            setPauseTheService                                  (bool pauseTheService);

    // Now the data members
        private :
            string          m_checksum;
            bool            m_pauseTheService;
            
    };

}

#endif // DATABASEOBJECTMANAGERCALCULATEDBCKSUMMESSAGE_H
