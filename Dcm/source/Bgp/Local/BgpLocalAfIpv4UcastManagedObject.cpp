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




#include "Bgp/Local/BgpLocalAfIpv4UcastManagedObject.h"
#include "Bgp/Local/BgpLocalTypes.h"
#include "Bgp/Local/BgpLocalObjectManager.h"
#include "brocade-bgp.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4UcastManagedObject::BgpLocalAfIpv4UcastManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4UcastManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase(this),
    BgpLocalRedistributeCommonManagedObjectBase(this),
    BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase(this),
    BgpLocalMaxiumPathCommonManagedObjectBase(this),
    BgpLocalMultiPathCommonManagedObjectBase(this),
    BgpLocalAfBasicAttributesCommonManagedObjectBase(this),
    BgpLocalAfCommonAttributesCommonManagedObjectBase(this),
    BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase(this),
    m_presenceFlag(false),
    m_nextHopRecursionFlag(false)
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setupAttributesForPersistence();
    BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeBool (&m_presenceFlag, true, "false", "presenceFlag",bgp_ipv4_unicast));
    addPersistableAttribute (new AttributeBool (&m_nextHopRecursionFlag, true, "false", "nextHopRecursionFlag",bgp_next_hop_recursion));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase::setupAttributesForCreate();
    BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase::setupAttributesForCreate();
    BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeBool (&m_presenceFlag, true, "false", "presenceFlag",bgp_ipv4_unicast));
    addPersistableAttributeForCreate (new AttributeBool (&m_nextHopRecursionFlag, true, "false", "nextHopRecursionFlag",bgp_next_hop_recursion));

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::setupKeys()
{

    vector<string> keyName;
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
BgpLocalAfIpv4UcastManagedObject::BgpLocalAfIpv4UcastManagedObject(BgpLocalObjectManager *pBgpLocalObjectManager,
                                                                                                                const bool &presenceFlag,
                                                                                                                const bool &nextHopRecursionFlag)
    :PrismElement  (pBgpLocalObjectManager ),
    PrismPersistableObject(BgpLocalAfIpv4UcastManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pBgpLocalObjectManager),
    DcmManagedObject (pBgpLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pBgpLocalObjectManager),
    CommonManagedObjectBase(this),
    BgpLocalAfIpv4CommonAttributesCommonManagedObjectBase(this),
    BgpLocalRedistributeCommonManagedObjectBase(this),
    BgpLocalAfIpv4UnicastAndVrfCommonManagedObjectBase(this),
    BgpLocalMaxiumPathCommonManagedObjectBase(this),
    BgpLocalMultiPathCommonManagedObjectBase(this),
    BgpLocalAfBasicAttributesCommonManagedObjectBase(this),
    BgpLocalAfCommonAttributesCommonManagedObjectBase(this),
    BgpLocalAfIpv4UcastAndMcastAttributesCommonManagedObjectBase(this),
    m_presenceFlag(presenceFlag),
    m_nextHopRecursionFlag(nextHopRecursionFlag)
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add any additional handling needed for ManagedObject constructor
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string BgpLocalAfIpv4UcastManagedObject::getClassName()
{
    return ("BgpLocalAfIpv4UcastManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~BgpLocalAfIpv4UcastManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
BgpLocalAfIpv4UcastManagedObject::~BgpLocalAfIpv4UcastManagedObject()
{

    /**@uBreezeStart:Please put all the code below this comment Sections
     * Instructions:Add the Destructor handling for the ManagedObject
     * @uBreezeClose
     */




    /**@uBreezeEnd:Please put all the code above this comment Section
     * Instructions:Add the Destructor handling for the ManagedObject
     * @uBreezeClose
     */

}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::setpresenceFlag:This function is used to set the value of member m_presenceFlag
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::setpresenceFlag (const bool &presenceFlag)
{
    m_presenceFlag = presenceFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::getpresenceFlag:This function is used to get the value of member m_presenceFlag
 * @uBreezeClose
 */
bool BgpLocalAfIpv4UcastManagedObject::getpresenceFlag ()
{
    return (m_presenceFlag);
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::setnextHopRecursionFlag:This function is used to set the value of member m_nextHopRecursionFlag
 * @uBreezeClose
 */
void BgpLocalAfIpv4UcastManagedObject::setnextHopRecursionFlag (const bool &nextHopRecursionFlag)
{
    m_nextHopRecursionFlag = nextHopRecursionFlag;
}


/**@uBreezeFunction: Description of the function
 * Brief:BgpLocalAfIpv4UcastManagedObject::getnextHopRecursionFlag:This function is used to get the value of member m_nextHopRecursionFlag
 * @uBreezeClose
 */
bool BgpLocalAfIpv4UcastManagedObject::getnextHopRecursionFlag ()
{
    return (m_nextHopRecursionFlag);
}


string BgpLocalAfIpv4UcastManagedObject::customConfigurationDisplay (WaveCustomCliDisplayConfigurationContext *waveCustomCliDisplayConfigurationContext)
{
    string displayString="";

    if(!getclientToClientReflectionFlag ())
    {
        displayString = "  no client-to-client-reflection \n";
    }

    return displayString;
}

}

