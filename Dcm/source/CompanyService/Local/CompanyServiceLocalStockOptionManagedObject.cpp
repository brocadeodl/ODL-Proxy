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
 *   Author : apurva                                                     *
 **************************************************************************/




#include "CompanyService/Local/CompanyServiceLocalStockOptionManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalStockOptionManagedObject::CompanyServiceLocalStockOptionManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalStockOptionManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_stockOptionPresent (true) 
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
 * Brief:CompanyServiceLocalStockOptionManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence in DB
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeBool (&m_stockOptionPresent, "stockOptionPresent",companyservice_stockOption));
    addPersistableAttribute (new AttributeBool (&m_isStockOption, "isStockOption",companyservice_isStockOption));
    addPersistableAttribute (new AttributeUI32 (&m_stockshare, "stockshare",companyservice_stockshare));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeBool (&m_stockOptionPresent, "stockOptionPresent",companyservice_stockOption));
    addPersistableAttributeForCreate (new AttributeBool (&m_isStockOption, "isStockOption",companyservice_isStockOption));
    addPersistableAttributeForCreate (new AttributeUI32 (&m_stockshare, "stockshare",companyservice_stockshare));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::setupKeys()
{

    vector<string> keyName;
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceLocalStockOptionManagedObject::CompanyServiceLocalStockOptionManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const bool &stockOptionPresent,
                                                                                                                const bool &isStockOption,
                                                                                                                const UI32 &stockshare)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalStockOptionManagedObject::getClassName(),     DcmManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    m_stockOptionPresent(stockOptionPresent),
    m_isStockOption(isStockOption),
    m_stockshare(stockshare)
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
 * Brief:CompanyServiceLocalStockOptionManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalStockOptionManagedObject::getClassName()
{
    return ("CompanyServiceLocalStockOptionManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalStockOptionManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalStockOptionManagedObject::~CompanyServiceLocalStockOptionManagedObject()
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
 * Brief:CompanyServiceLocalStockOptionManagedObject::setstockOptionPresent:This function is used to set the value of member m_stockOptionPresent
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::setstockOptionPresent (const bool &stockOptionPresent)
{
    m_stockOptionPresent = stockOptionPresent;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getstockOptionPresent:This function is used to get the value of member m_stockOptionPresent
 * @uBreezeClose
 */
bool CompanyServiceLocalStockOptionManagedObject::getstockOptionPresent ()
{
    return (m_stockOptionPresent);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::setisStockOption:This function is used to set the value of member m_isStockOption
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::setisStockOption (const bool &isStockOption)
{
    m_isStockOption = isStockOption;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getisStockOption:This function is used to get the value of member m_isStockOption
 * @uBreezeClose
 */
bool CompanyServiceLocalStockOptionManagedObject::getisStockOption ()
{
    return (m_isStockOption);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::setstockshare:This function is used to set the value of member m_stockshare
 * @uBreezeClose
 */
void CompanyServiceLocalStockOptionManagedObject::setstockshare (const UI32 &stockshare)
{
    m_stockshare = stockshare;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalStockOptionManagedObject::getstockshare:This function is used to get the value of member m_stockshare
 * @uBreezeClose
 */
UI32 CompanyServiceLocalStockOptionManagedObject::getstockshare ()
{
    return (m_stockshare);
}



}

