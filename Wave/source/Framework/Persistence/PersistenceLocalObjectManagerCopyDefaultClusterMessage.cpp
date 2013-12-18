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
 *   Description : This is a dummy message created to do copy default to   *
 *                 startup in a cluster.                                   * 
 *   Author : Anil ChannaveeraSetty                                        *
 *   Date : Oct/05/2010                                                    * 
 ***************************************************************************/


#include "Framework/Persistence/PersistenceLocalObjectManagerCopyDefaultClusterMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerTypes.h"
#include "Framework/Persistence/PersistenceLocalObjectManager.h"

namespace WaveNs
{

PersistenceLocalObjectManagerCopyDefaultClusterMessage::PersistenceLocalObjectManagerCopyDefaultClusterMessage()
: PrismMessage (PersistenceLocalObjectManager::getPrismServiceId (),PERSISTENCE_COPY_DEFAULT_CLUSTER) 
{ }

PersistenceLocalObjectManagerCopyDefaultClusterMessage::~PersistenceLocalObjectManagerCopyDefaultClusterMessage()
{ }

}

