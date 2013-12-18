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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/




#include "Bgp/Local/BgpLocalMultiPathCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalMultiPathCommonManagedObjectBase::BgpLocalMultiPathCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_multiPathEbgp(false),
    m_multiPathIbgp(false),
    m_multiPathMultiAs(false)
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add any additional handling needed for CommonManagedObjectBase constructor
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add any additional handling needed for CommonManagedObjectBase constructor
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalMultiPathCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeBool (&m_multiPathEbgp, true, "false", "multiPathEbgp",bgp_multipath_ebgp));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_multiPathIbgp, true, "false", "multiPathIbgp",bgp_multipath_ibgp));
    addPersistableAttributeForCommonBase (new AttributeBool (&m_multiPathMultiAs, true, "false", "multiPathMultiAs",bgp_multi_as));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalMultiPathCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_multiPathEbgp, true, "false", "multiPathEbgp",bgp_multipath_ebgp));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_multiPathIbgp, true, "false", "multiPathIbgp",bgp_multipath_ibgp));
    addPersistableAttributeForCommonBaseCreate (new AttributeBool (&m_multiPathMultiAs, true, "false", "multiPathMultiAs",bgp_multi_as));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalMultiPathCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalMultiPathCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalMultiPathCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalMultiPathCommonManagedObjectBase::~BgpLocalMultiPathCommonManagedObjectBase()
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add the Destructor handling for the CommonManagedObjectBase
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add the Destructor handling for the CommonManagedObjectBase
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::setmultiPathEbgp:This function is used to set the value of member m_multiPathEbgp
 * @uBreezeClose
 */
void BgpLocalMultiPathCommonManagedObjectBase::setmultiPathEbgp (const bool &multiPathEbgp)
{
    m_multiPathEbgp = multiPathEbgp;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::getmultiPathEbgp:This function is used to get the value of member m_multiPathEbgp
 * @uBreezeClose
 */
bool BgpLocalMultiPathCommonManagedObjectBase::getmultiPathEbgp ()
{
    return (m_multiPathEbgp);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::setmultiPathIbgp:This function is used to set the value of member m_multiPathIbgp
 * @uBreezeClose
 */
void BgpLocalMultiPathCommonManagedObjectBase::setmultiPathIbgp (const bool &multiPathIbgp)
{
    m_multiPathIbgp = multiPathIbgp;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::getmultiPathIbgp:This function is used to get the value of member m_multiPathIbgp
 * @uBreezeClose
 */
bool BgpLocalMultiPathCommonManagedObjectBase::getmultiPathIbgp ()
{
    return (m_multiPathIbgp);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::setmultiPathMultiAs:This function is used to set the value of member m_multiPathMultiAs
 * @uBreezeClose
 */
void BgpLocalMultiPathCommonManagedObjectBase::setmultiPathMultiAs (const bool &multiPathMultiAs)
{
    m_multiPathMultiAs = multiPathMultiAs;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalMultiPathCommonManagedObjectBase::getmultiPathMultiAs:This function is used to get the value of member m_multiPathMultiAs
 * @uBreezeClose
 */
bool BgpLocalMultiPathCommonManagedObjectBase::getmultiPathMultiAs ()
{
    return (m_multiPathMultiAs);
}



}

