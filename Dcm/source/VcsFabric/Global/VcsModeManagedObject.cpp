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

#include "VcsFabric/Global/VcsModeManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "vcs.h"

namespace DcmNs {

VcsModeManagedObject::VcsModeManagedObject (WaveObjectManager *pWaveObjectManager)
    : PrismElement              (pWaveObjectManager),
      PrismPersistableObject    (VcsModeManagedObject::getClassName (), DcmManagedObject::getClassName ()),
      WaveManagedObject         (pWaveObjectManager),
      DcmManagedObject          (pWaveObjectManager),
      m_vcsMode                 (false),
      m_vcsClusterMode          (false)
{
    setEmptyNeededOnPersistentBoot (false);
    setEmptyNeededOnPersistentBootWithDefault (false);

    m_swbdNumber = DcmToolKit::getSwBDModel ();
}

VcsModeManagedObject::~VcsModeManagedObject ()
{
}

string VcsModeManagedObject::getClassName ()
{
    return ("VcsModeManagedObject");
}

void VcsModeManagedObject::setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence ();

    addPersistableAttribute (new AttributeBool (&m_vcsMode, "vcsMode"));
    addPersistableAttribute (new AttributeBool (&m_vcsClusterMode, "vcsClusterMode"));
    addPersistableAttribute (new AttributeUI32 (&m_swbdNumber, "swbdNumber"));
    setUserTagForAttribute ("vcsMode", vcs_vcs_mode);
    setUserTagForAttribute ("vcsClusterMode", vcs_vcs_cluster_mode);
    setUserTagForAttribute ("swbdNumber", vcs_swbd_number);
}

void VcsModeManagedObject::setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate ();

    addPersistableAttributeForCreate (new AttributeBool (&m_vcsMode, "vcsMode"));
    addPersistableAttributeForCreate (new AttributeBool (&m_vcsClusterMode, "vcsClusterMode"));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_swbdNumber, "swbdNumber"));
}

bool VcsModeManagedObject::getVcsMode () const
{
    return (m_vcsMode);
}

void VcsModeManagedObject::setVcsMode (bool vcsMode)
{
    m_vcsMode = vcsMode;
}

bool VcsModeManagedObject::getVcsClusterMode () const
{
    return (m_vcsClusterMode);
}

void VcsModeManagedObject::setVcsClusterMode (bool vcsClusterMode)
{
    m_vcsClusterMode = vcsClusterMode;
}

}
