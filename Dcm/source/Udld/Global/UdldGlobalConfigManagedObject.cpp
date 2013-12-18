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
 *   Author : statpatt                                                     *
 **************************************************************************/




#include "Udld/Global/UdldGlobalConfigManagedObject.h"
#include "Udld/Global/UdldGlobalTypes.h"
#include "Udld/Global/UdldGlobalObjectManager.h"
#include "brocade-udld.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
UdldGlobalConfigManagedObject::UdldGlobalConfigManagedObject(UdldGlobalObjectManager *pUdldGlobalObjectManager)
    :PrismElement  (pUdldGlobalObjectManager ),
    PrismPersistableObject(UdldGlobalConfigManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pUdldGlobalObjectManager),
    DcmManagedObject (pUdldGlobalObjectManager),
    m_UdldPresent(true),
    m_Hello(5),
    m_Multiplier(5),
    m_Shutdown(false)
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
 * Brief:UdldGlobalConfigManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject:: setupAttributesForPersistence ()
{
    DcmManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeBool (&m_UdldPresent, "UdldPresent",udld_udld));
    addPersistableAttribute (new AttributeUI32 (&m_Hello, 5, "Hello",udld_hello));
    addPersistableAttribute (new AttributeUI32 (&m_Multiplier, 5, "Multiplier",udld_multiplier));
    addPersistableAttribute (new AttributeBool (&m_Shutdown, "Shutdown",udld_shutdown));

}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject:: setupAttributesForCreate ()
{
    DcmManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeBool (&m_UdldPresent, "UdldPresent",udld_udld));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_Hello, 5, "Hello",udld_hello));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_Multiplier, 5, "Multiplier",udld_multiplier));
    addPersistableAttributeForCreate (new AttributeBool (&m_Shutdown, "Shutdown",udld_shutdown));

}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setupKeys()
{

    vector<string> keyName;
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
UdldGlobalConfigManagedObject::UdldGlobalConfigManagedObject(UdldGlobalObjectManager *pUdldGlobalObjectManager,
                                                                                                                const bool &UdldPresent,
                                                                                                                const UI32 &Hello,
                                                                                                                const UI32 &Multiplier,
                                                                                                                const bool &Shutdown)
    :PrismElement  (pUdldGlobalObjectManager ),
    PrismPersistableObject(UdldGlobalConfigManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pUdldGlobalObjectManager),
    DcmManagedObject (pUdldGlobalObjectManager),
    m_UdldPresent(UdldPresent),
    m_Hello(Hello),
    m_Multiplier(Multiplier),
    m_Shutdown(Shutdown)
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
 * Brief:UdldGlobalConfigManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string UdldGlobalConfigManagedObject::getClassName()
{
    return ("UdldGlobalConfigManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~UdldGlobalConfigManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
UdldGlobalConfigManagedObject::~UdldGlobalConfigManagedObject()
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
 * Brief:UdldGlobalConfigManagedObject::setUdldPresent:This function is used to set the value of member m_UdldPresent
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setUdldPresent (const bool &UdldPresent)
{
    m_UdldPresent = UdldPresent;
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::getUdldPresent:This function is used to get the value of member m_UdldPresent
 * @uBreezeClose
 */
bool UdldGlobalConfigManagedObject::getUdldPresent ()
{
    return (m_UdldPresent);
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::setHello:This function is used to set the value of member m_Hello
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setHello (const UI32 &Hello)
{
    m_Hello = Hello;
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::getHello:This function is used to get the value of member m_Hello
 * @uBreezeClose
 */
UI32 UdldGlobalConfigManagedObject::getHello ()
{
    return (m_Hello);
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::setMultiplier:This function is used to set the value of member m_Multiplier
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setMultiplier (const UI32 &Multiplier)
{
    m_Multiplier = Multiplier;
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::getMultiplier:This function is used to get the value of member m_Multiplier
 * @uBreezeClose
 */
UI32 UdldGlobalConfigManagedObject::getMultiplier ()
{
    return (m_Multiplier);
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::setShutdown:This function is used to set the value of member m_Shutdown
 * @uBreezeClose
 */
void UdldGlobalConfigManagedObject::setShutdown (const bool &Shutdown)
{
    m_Shutdown = Shutdown;
}


/**@uBreezeFunction: Description of the function
 * Brief:UdldGlobalConfigManagedObject::getShutdown:This function is used to get the value of member m_Shutdown
 * @uBreezeClose
 */
bool UdldGlobalConfigManagedObject::getShutdown ()
{
    return (m_Shutdown);
}



}

