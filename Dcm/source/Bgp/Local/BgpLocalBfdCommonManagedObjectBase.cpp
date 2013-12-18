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



#if 0
#include "Bgp/Local/BgpLocalBfdCommonManagedObjectBase.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase:Constructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalBfdCommonManagedObjectBase::BgpLocalBfdCommonManagedObjectBase(PrismPersistableObject *pPrismPersistableObject)
    :CommonManagedObjectBase(pPrismPersistableObject),
    m_bfdHoldInterval(0),
    m_bfdMintx(0),
    m_bfdMinrx(0),
    m_bfdMultiplier(0)
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
 * Brief:BgpLocalBfdCommonManagedObjectBase::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void BgpLocalBfdCommonManagedObjectBase:: setupAttributesForPersistence ()
{
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_bfdHoldInterval, 0, "bfdHoldInterval",bgp_bfd_holdover_interval));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_bfdMintx, 0, "bfdMintx",bgp_bfd_min_tx));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_bfdMinrx, 0, "bfdMinrx",bgp_bfd_min_rx));
    addPersistableAttributeForCommonBase (new AttributeUI32 (&m_bfdMultiplier, 0, "bfdMultiplier",bgp_bfd_multiplier));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalBfdCommonManagedObjectBase:: setupAttributesForCreate ()
{
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_bfdHoldInterval, 0, "bfdHoldInterval",bgp_bfd_holdover_interval));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_bfdMintx, 0, "bfdMintx",bgp_bfd_min_tx));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_bfdMinrx, 0, "bfdMinrx",bgp_bfd_min_rx));
    addPersistableAttributeForCommonBaseCreate (new AttributeUI32 (&m_bfdMultiplier, 0, "bfdMultiplier",bgp_bfd_multiplier));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalBfdCommonManagedObjectBase::getClassName()
{
    return ("BgpLocalBfdCommonManagedObjectBase");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalBfdCommonManagedObjectBase:Destructor For CommonManagedObjectBase
 * @uBreezeClose
 */
BgpLocalBfdCommonManagedObjectBase::~BgpLocalBfdCommonManagedObjectBase()
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
 * Brief:BgpLocalBfdCommonManagedObjectBase::setbfdHoldInterval:This function is used to set the value of member m_bfdHoldInterval
 * @uBreezeClose
 */
void BgpLocalBfdCommonManagedObjectBase::setbfdHoldInterval (const UI32 &bfdHoldInterval)
{
    m_bfdHoldInterval = bfdHoldInterval;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::getbfdHoldInterval:This function is used to get the value of member m_bfdHoldInterval
 * @uBreezeClose
 */
UI32 BgpLocalBfdCommonManagedObjectBase::getbfdHoldInterval ()
{
    return (m_bfdHoldInterval);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::setbfdMintx:This function is used to set the value of member m_bfdMintx
 * @uBreezeClose
 */
void BgpLocalBfdCommonManagedObjectBase::setbfdMintx (const UI32 &bfdMintx)
{
    m_bfdMintx = bfdMintx;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::getbfdMintx:This function is used to get the value of member m_bfdMintx
 * @uBreezeClose
 */
UI32 BgpLocalBfdCommonManagedObjectBase::getbfdMintx ()
{
    return (m_bfdMintx);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::setbfdMinrx:This function is used to set the value of member m_bfdMinrx
 * @uBreezeClose
 */
void BgpLocalBfdCommonManagedObjectBase::setbfdMinrx (const UI32 &bfdMinrx)
{
    m_bfdMinrx = bfdMinrx;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::getbfdMinrx:This function is used to get the value of member m_bfdMinrx
 * @uBreezeClose
 */
UI32 BgpLocalBfdCommonManagedObjectBase::getbfdMinrx ()
{
    return (m_bfdMinrx);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::setbfdMultiplier:This function is used to set the value of member m_bfdMultiplier
 * @uBreezeClose
 */
void BgpLocalBfdCommonManagedObjectBase::setbfdMultiplier (const UI32 &bfdMultiplier)
{
    m_bfdMultiplier = bfdMultiplier;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalBfdCommonManagedObjectBase::getbfdMultiplier:This function is used to get the value of member m_bfdMultiplier
 * @uBreezeClose
 */
UI32 BgpLocalBfdCommonManagedObjectBase::getbfdMultiplier ()
{
    return (m_bfdMultiplier);
}



}
#endif
