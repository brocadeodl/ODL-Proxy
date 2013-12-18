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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTASYNCHRONOUSPLUGINCONTEXT_H
#define WAVEMANAGEDOBJECTASYNCHRONOUSPLUGINCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class WaveManagedObjectAsynchronousPluginContext : public PrismAsynchronousContext
{
    private :
    protected :
    public :

                            WaveManagedObjectAsynchronousPluginContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual            ~WaveManagedObjectAsynchronousPluginContext ();

        vector<UI32>        getAttributeUserTags    () const;
        void                setAttributeUserTags    (const vector<UI32> &attributeUserTags);

        vector<string>      getAttributeNames       () const;
        void                setAttributeNames       (const vector<string> &attributeNames);

        vector<string>      getAttributeValues      () const;
        void                setAttributeValues      (const vector<string> &attributeValues);

        vector<Attribute* > getAttributes           () const;
        void                setAttributeVector      (vector<Attribute *> attributes);
        void                addToAttributeVector    (Attribute *attributes);

        void                setManagedObjectName    (const string &managedObjectName);
        string              getManagedObjectName    () const;

        UI32                getGroupCode            () const;
        void                setGroupCode            (UI32 groupCode);
            

        void                setParentManagedObjectNames ( const vector<string> &parentManagedObjectNames);
        void                getParentManagedObjectNames ( vector<string> &parentManagedObjectNames) const;    


    // Now the data members
    private :
        vector<UI32>        m_attributeUserTags;
        vector<string>      m_attributeNames;
        vector<string>      m_attributeValues;
        vector<Attribute *> m_attributes;
        string              m_managedObjectName;
        UI32                m_groupCode;
        vector<UI32>        m_parentGroupIds;
        vector<string>      m_parentManagedObjectNames;    

    protected :
    public :
};

class UpdateNewAttributeInManagedObjectContext : public WaveManagedObjectAsynchronousPluginContext
{
    private :
    protected :
    public :

                            UpdateNewAttributeInManagedObjectContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual            ~UpdateNewAttributeInManagedObjectContext ();
    
        vector<string >     getNewAttributeNames                     () const;
        void                addNewAttributeName                      (const string &attributeName);

    // Now the data members
    private:
       vector<string>        m_newAttributeNames;

    protected :
    public :
};

class IsAssociatedManagedObjectToBeCreatedContext : public WaveManagedObjectAsynchronousPluginContext
{
    private :
    protected :
    public :

                            IsAssociatedManagedObjectToBeCreatedContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual            ~IsAssociatedManagedObjectToBeCreatedContext ();

        PrismMessage       *getMessage                  () const;
        void                setMessage                  (PrismMessage *message);

        void                setAssociatedClassName      (const string &associatedClassName);
        string              getAssociatedClassName      () const;

        string              getAssociatedAttributeName  () const;
        void                setAssociatedAttributeName  (const string &associatedAttributeName);

        string              getAssociatedAttributeValue () const;
        void                setAssociatedAttributeValue (const string &associatedAttributeValues);

        UI32                getAttributeIndex           () const;
        void                setAttributeIndex           (const UI32 attributeIndex);

        bool                getIsAssociatedMOToBeCreated() const;
        void                setIsAssociatedMOToBeCreated(bool isAssociatedMOToBeCreated);

    // Now the data members
    private :
        PrismMessage     *m_pMessage;
        string           m_associatedClassName;
        string           m_associatedAttributeName;
        string           m_associatedAttributeValue;
        UI32             m_attributeIndex;
        bool             m_isAssociatedMOToBeCreated;

    protected :
    public :
};

class GetHardwareConfigurationDetailsContext : public WaveManagedObjectAsynchronousPluginContext
{
    private :
    protected :
    public :

                                      GetHardwareConfigurationDetailsContext  (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
                                      GetHardwareConfigurationDetailsContext  (WaveManagedObject *pOperateOnWaveManagedObject , PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual                      ~GetHardwareConfigurationDetailsContext  ();


        bool                          getIsUpdateBackend                      ();
        void                          setIsUpdateBackend                      (bool isUpdateBackend);

        void                          setClientName                           (const string &clientName);
        void                          getClientName                           (string &clientName) const;
        
        void                          setAttributeNamesGroupCodeMap           (const map <string, UI32> &attributeNamesGroupCodeMap);
        map <string, UI32>            getAttributeNamesGroupCodeMap           () const;

        void                          setGroupCodeBackendNamesMap             (const map <UI32, string> &groupCodeBackendNamesMap); 
        map <UI32, string>            getGroupCodeBackendNamesMap             () const;
        string                        getClientNameForGroup                   ( const UI32 groupCode );       
        

        void                          setGroupCodeAttributeGroupMap           (const map <UI32, vector <string> > &groupCodeAttributeGroupMap);
        map <UI32, vector <string> >  getGroupCodeAttributeGroupMap           () const;
        vector<string>                getAttributeNamesForGroup               ( const UI32 &groupCode );  

        void                          setAttributeNameBackendNameMap          (const map <string, string> &attributeNameBackendNameMap);
        map <string, string>          getAttributeNameBackendNameMap          () const;
        
        void                          setPOperateOnWaveManagedObject          (WaveManagedObject *pWaveManagedObject);
        WaveManagedObject*            getPOperateOnWaveManagedObject          ();

    // Now the data members
    private :
       bool                           m_isUpdateBackend;
       string                         m_clientName;
       map <string, UI32>             m_attributeNamesGroupCodeMap;
       map <UI32, string>             m_groupCodeBackendNamesMap;
       map <UI32, vector <string> >   m_groupCodeAttributeGroupMap;
       map <string, string >          m_attributeNamesBackendNamesMap;
       WaveManagedObject             *m_pOperateOnWaveManagedObject;
    
    protected :
    public :
};

class GetHardwareConfigurationDetailsForPostbootContext : public GetHardwareConfigurationDetailsContext
{
    private :
    protected :
    public :
                            GetHardwareConfigurationDetailsForPostbootContext  (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
                            GetHardwareConfigurationDetailsForPostbootContext  (string managedObjectName, string passName, vector<WaveManagedObject *> *pWaveManagedObjects, PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext); 
                            GetHardwareConfigurationDetailsForPostbootContext  (string managedObjectName, string passName, vector<WaveManagedObject *> *pWaveManagedObjects, const map<UI32, bool> &groupCodeChoiceFlag, map<UI32, map<UI32, vector<UI32> > > &choiceCaseGroup, PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext);
        virtual            ~GetHardwareConfigurationDetailsForPostbootContext  ();

        void                            setPassName                             ( const string &passName );
        string                          getPassName                             ( ) const;

        void                            setWaveManagedObjects                   ( vector<WaveManagedObject *>* pWaveManagedObjects );
        vector<WaveManagedObject *>*    getWaveManagedObjects                   ( );

        void                            setGroupCodeChoiceFlag                  ( const map<UI32, bool> &groupCodeChoiceFlag);
        void                            getGroupCodeChoiceFlag                  ( map<UI32, bool> &groupCodeChoiceFlag) const;        

        void                            setChoiceCaseGroup                      ( const map<UI32, map<UI32, vector<UI32> > > &choiceCaseGroup);
        void                            getChoiceCaseGroup                      ( map<UI32, map<UI32, vector<UI32> > > &choiceCaseGroup) const;
        void                            getCaseGroupsForChoiceGroup             ( const UI32 &choiceGroup, const UI32 &userTag, vector<UI32> &caseGroups ) const;
        bool                            isAChoiceGroup                          ( const UI32 &groupCode );    

        void                            setKeyAttributeNames                    ( const set<string> &keyAttributeNames );    
        bool                            isKeyAttribute                          ( const string &attributeName);

    // Now the data members
    private :
        string                                      m_passName;
        vector<WaveManagedObject *>*                m_pWaveManagedObjects;
        map<UI32, bool>                             m_groupCodeChoiceFlag;
        set<string>                                 m_keyAttributeNames;
        map<UI32, map<UI32, vector<UI32> > >        m_choiceCaseGroup;

    protected :
    public :

};


// API related contexts. For now we have typedef-ed all Contexts. 
// We will flesh it out later.

typedef WaveManagedObjectAsynchronousPluginContext ValidateForCreateAtThisNodeContext; 
typedef WaveManagedObjectAsynchronousPluginContext ValidateForUpdateAtThisNodeContext; 
typedef WaveManagedObjectAsynchronousPluginContext ValidateForDeleteAtThisNodeContext; 
typedef GetHardwareConfigurationDetailsContext     GetHardwareConfigurationDetailsForCreateContext; 
typedef GetHardwareConfigurationDetailsContext     GetHardwareConfigurationDetailsForUpdateContext; 
typedef GetHardwareConfigurationDetailsContext     GetHardwareConfigurationDetailsForDeleteContext; 

}

#endif //WAVEMANAGEDOBJECTASYNCHRONOUSPLUGINCONTEXT_H
