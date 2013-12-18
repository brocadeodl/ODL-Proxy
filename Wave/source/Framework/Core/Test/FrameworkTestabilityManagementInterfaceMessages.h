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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FRAMEWORKTESTABILITYMANAGEMENTINTERFACEMESSAGES_H
#define FRAMEWORKTESTABILITYMANAGEMENTINTERFACEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class FrameworkTestabilityManagementInterfaceMessage1 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 FrameworkTestabilityManagementInterfaceMessage1 ();
        virtual ~FrameworkTestabilityManagementInterfaceMessage1 ();

    // now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityManagementInterfaceMessage2 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 FrameworkTestabilityManagementInterfaceMessage2 ();
        virtual ~FrameworkTestabilityManagementInterfaceMessage2 ();

    // now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityManagementInterfaceMessage3 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 FrameworkTestabilityManagementInterfaceMessage3 ();
        virtual ~FrameworkTestabilityManagementInterfaceMessage3 ();

    // now the data members

    private :
    protected :
    public :
};

class FrameworkTestabilityManagementInterfaceMessage4 : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 FrameworkTestabilityManagementInterfaceMessage4 ();
        virtual ~FrameworkTestabilityManagementInterfaceMessage4 ();
#if 0
                void   setupInputBuffer1            ();
                void   setupOutputBuffer1           ();
                bool   validateInputBuffer1         ();
                bool   validateOutputBuffer1        ();
                void   setInputLargeBuffer1         ();
                bool   validateInputLargeBuffer1    ();

                void   setCompletionStatusType      (const UI32 &completionStatusType);
                ResourceId  getRequestedCompletionStatus ();
#endif
                void   setClientName (const string &clientName);
                string getClientName () const;
    // now the data members

    private :
                string m_clientName;
#if 0
                bool   m_isLargInputBuffer1Set;
                UI32   m_completionStatusType;
#endif
    protected :
    public :
};


class FrameworkTestabilityManagementInterfaceMessageLeakMessage : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
                 FrameworkTestabilityManagementInterfaceMessageLeakMessage ();
        virtual ~FrameworkTestabilityManagementInterfaceMessageLeakMessage ();

                                void      setMessageArguments (const vector<PrismServiceId>  &messageServiceIds, const vector<UI32> &messageOperationCodes, const vector<WaveMessageType> &messageTypes, const vector<string> &btStrings);
                                void      getMessageArguments (vector<PrismServiceId>  &messageServiceIds, vector<UI32> &messageOperationCodes, vector<WaveMessageType> &messageTypes, vector<string> &btStrings) const;

                                void      setPrismServiceId (const PrismServiceId prismServiceId);
                      PrismServiceId      getPrismServiceId () const;
    // now the data members

    private :
                PrismServiceId          m_prismServiceId;
                vector<PrismServiceId>  m_messageServiceIds;
                vector<UI32>            m_messageOperationCodes;
                vector<string>          m_btStrings;
                vector<UI32>            m_messageTypes;
                
    protected :
    public :
};

class FrameworkTestabilityManagementInterfaceObjectLeakMessage : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();
    protected :
    public :
                        FrameworkTestabilityManagementInterfaceObjectLeakMessage ();
               virtual ~FrameworkTestabilityManagementInterfaceObjectLeakMessage ();
            
                void     setMessageArguments (const vector<string> &managedObjectClassNames, const vector<string>  &managedObjectNames, const vector<bool> &queryResults, const vector<string> &btStrings);
                void     getMessageArguments (vector<string> &managedObjectClassNames, vector<string>  &managedObjectNames, vector<bool> &queryResults, vector<string> &btStrings) const;
                void     setPrismServiceId (PrismServiceId prismServiceId);
      PrismServiceId     getPrismServiceId () const;
    // now the data members

    private :
                PrismServiceId          m_prismServiceId;
                vector<string>          m_managedObjectClassNames;
                vector<string>          m_managedObjectNames;
                vector<bool>            m_queryResults;
                vector<string>          m_btStrings;

    protected :
    public :
};

class FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();
    protected :
    public :
                 FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage ();
        virtual ~FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage ();

        void     setLeakType            (const UI32 &leakType);
        UI32     getLeakType            () const;
        void     setBtTrackerState      (const bool &btTrackerState);
        bool     getBtTrackerState      () const;

    // now the data members
    private :
                 UI32                    m_leakType;
                 bool                    m_btTrackerState;
                
    protected :
    public :
};

class FrameworkTestabilityManagementInterfaceFileServiceUsageMessage : public ManagementInterfaceMessage
{
    private :
        virtual void setupAttributesForSerialization ();

    protected :
    public :
        FrameworkTestabilityManagementInterfaceFileServiceUsageMessage ();
        virtual        ~FrameworkTestabilityManagementInterfaceFileServiceUsageMessage ();

        bool    getFileServiceUseforFileTransferFlag      () const;
        void    setFileServiceUseforFileTransferFlag      (const bool &bFileServiceUseForFileTransfer);

        // Now the data members
        //
        private :
                bool m_useFileServiceForFileTransfer;
        
        protected :
        public :

};



}

#endif // FRAMEWORKTESTABILITYMANAGEMENTINTERFACEMESSAGES_H
