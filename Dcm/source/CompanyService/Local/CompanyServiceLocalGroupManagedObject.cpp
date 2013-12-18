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




#include "CompanyService/Local/CompanyServiceLocalGroupManagedObject.h"
#include "CompanyService/Local/CompanyServiceLocalTypes.h"
#include "CompanyService/Local/CompanyServiceLocalObjectManager.h"
#include "CompanyService.h"
#include "Framework/Attributes/AttributeManagedObjectComposition.cpp"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"


using namespace WaveNs;


namespace DcmNs{


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject:Constructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalGroupManagedObject::CompanyServiceLocalGroupManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalGroupManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    CommonManagedObjectBase(this),
    CompanyServiceLocalgroupbaseCommonManagedObjectBase(this),
    m_groupname("")
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
 * Brief:CompanyServiceLocalGroupManagedObject::setupAttributesForPersistence: Add all the attributes for Persistence
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject:: setupAttributesForPersistence ()
{
    DcmLocalManagedObject::setupAttributesForPersistence();
    CompanyServiceLocalgroupbaseCommonManagedObjectBase::setupAttributesForPersistence();
    addPersistableAttribute (new AttributeManagedObjectVectorComposition <CompanyServiceLocalSubgroupManagedObject> (&m_subgroup, "subgroup", getClassName(),getObjectId(),CompanyServiceLocalSubgroupManagedObject::getClassName(),companyservice_subgroup));
    addPersistableAttribute (new AttributeString (&m_groupname, true, "", "groupname",companyservice_groupname));
    addPersistableAttribute (new AttributeManagedObjectComposition <CompanyServiceLocalGroupmanagerManagedObject> (&m_groupmanager, "groupmanager",CompanyServiceLocalGroupmanagerManagedObject::getClassName(), true,companyservice_groupmanager));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::setupAttributesForCreate: Add all attributes which represnts a column in the DB
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject:: setupAttributesForCreate ()
{
    DcmLocalManagedObject::setupAttributesForCreate();
    CompanyServiceLocalgroupbaseCommonManagedObjectBase::setupAttributesForCreate();
    addPersistableAttributeForCreate (new AttributeManagedObjectVectorComposition <CompanyServiceLocalSubgroupManagedObject> (&m_subgroup, "subgroup", getClassName(),getObjectId(),CompanyServiceLocalSubgroupManagedObject::getClassName(),companyservice_subgroup));
    addPersistableAttributeForCreate (new AttributeString (&m_groupname, true, "", "groupname",companyservice_groupname));
    addPersistableAttributeForCreate (new AttributeManagedObjectComposition <CompanyServiceLocalGroupmanagerManagedObject> (&m_groupmanager, "groupmanager",CompanyServiceLocalGroupmanagerManagedObject::getClassName(), true,companyservice_groupmanager));

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject:setupKeys For ManagedObject
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::setupKeys()
{

    vector<string> keyName;
    keyName.push_back ("groupname");
    PrismPersistableObject::setUserDefinedKeyCombination(keyName);

}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject:Constructor For ManagedObject with all paramters
 * @uBreezeClose
 */
CompanyServiceLocalGroupManagedObject::CompanyServiceLocalGroupManagedObject(CompanyServiceLocalObjectManager *pCompanyServiceLocalObjectManager,
                                                                                                                const vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > &subgroup,
                                                                                                                const string &groupname,
                                                                                                                const WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> &groupmanager)
    :PrismElement  (pCompanyServiceLocalObjectManager ),
    PrismPersistableObject(CompanyServiceLocalGroupManagedObject::getClassName(),     DcmLocalManagedObject::getClassName ()),
    WaveManagedObject (pCompanyServiceLocalObjectManager),
    DcmManagedObject (pCompanyServiceLocalObjectManager),
    WaveLocalManagedObjectBase (this),
    DcmLocalManagedObjectBase (this),
    DcmLocalManagedObject(pCompanyServiceLocalObjectManager),
    CommonManagedObjectBase(this),
    CompanyServiceLocalgroupbaseCommonManagedObjectBase(this),
    m_subgroup(subgroup),
    m_groupname(groupname),
    m_groupmanager(groupmanager)
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
 * Brief:CompanyServiceLocalGroupManagedObject::getClassName(): Returns the class name used by ManagedObject
 * @uBreezeClose
 */
string CompanyServiceLocalGroupManagedObject::getClassName()
{
    return ("CompanyServiceLocalGroupManagedObject");
}


/**@uBreezeFunction: Description of the function
 * Brief:~CompanyServiceLocalGroupManagedObject:Destructor For ManagedObject
 * @uBreezeClose
 */
CompanyServiceLocalGroupManagedObject::~CompanyServiceLocalGroupManagedObject()
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
 * Brief:CompanyServiceLocalGroupManagedObject::setsubgroup:This function is used to set the value of member m_subgroup
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::setsubgroup (const vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > &subgroup)
{
    m_subgroup = subgroup;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getsubgroup:This function is used to get the value of member m_subgroup
 * @uBreezeClose
 */
vector<WaveManagedObjectPointer<CompanyServiceLocalSubgroupManagedObject> > CompanyServiceLocalGroupManagedObject::getsubgroup ()
{
    return (m_subgroup);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::setgroupname:This function is used to set the value of member m_groupname
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::setgroupname (const string &groupname)
{
    m_groupname = groupname;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getgroupname:This function is used to get the value of member m_groupname
 * @uBreezeClose
 */
string CompanyServiceLocalGroupManagedObject::getgroupname ()
{
    return (m_groupname);
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::setgroupmanager:This function is used to set the value of member m_groupmanager
 * @uBreezeClose
 */
void CompanyServiceLocalGroupManagedObject::setgroupmanager (const WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> &groupmanager)
{
    m_groupmanager = groupmanager;
}


/**@uBreezeFunction: Description of the function
 * Brief:CompanyServiceLocalGroupManagedObject::getgroupmanager:This function is used to get the value of member m_groupmanager
 * @uBreezeClose
 */
WaveManagedObjectPointer<CompanyServiceLocalGroupmanagerManagedObject> CompanyServiceLocalGroupManagedObject::getgroupmanager ()
{
    return (m_groupmanager);
}



}

