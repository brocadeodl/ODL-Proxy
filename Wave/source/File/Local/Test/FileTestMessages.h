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
 *   Author : Mandar Datar                                                 *
 ***************************************************************************/

#ifndef COPYFILEWAVECLIENTMESSAGE_H
#define COPYFILEWAVECLIENTMESSAGE_H

#include <vector>
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace std;

namespace WaveNs
{

class CopyFileWaveClientMessage : public ManagementInterfaceMessage
{
    private :
            void            setupAttributesForSerialization             ();

    protected :
    public :
                            CopyFileWaveClientMessage                   (const string &sourceFileName, const string &destinationFileName, UI32 &copyFileFlag, const vector<string> &vectorOfDestinationIpAddresses);
                            CopyFileWaveClientMessage                   ();
    virtual                ~CopyFileWaveClientMessage                   ();

            string          getSourceFileName                           () const;
            void            setSourceFileName                           (const string &sourceFileName);
            string          getDestinationFileName                      () const;
            void            setDestinationFileName                      (const string &destinationFileName);
            UI32            getCopyFileFlag                             () const;
            void            setCopyFileFlag                             (const UI32 &copyFileFlag);
            vector<string>  getVectorOfDestinationIpAddressesAndPorts   () const;
            void            setVectorOfDestinationIpAddressesAndPorts   (const vector<string> &vectorOfDestinationIpAddressesAndPorts);


    // Now the data members

    private :
        string              m_sourceFileName;
        string              m_destinationFileName;
        UI32                m_copyFileFlag;
        vector<string>      m_vectorOfDestinationIpAddressesAndPorts;


    protected :
    public :
};

class CopyFileToHaPeerWaveClientMessage : public ManagementInterfaceMessage
{
    private :
            void            setupAttributesForSerialization             ();

    protected :
    public :
                            CopyFileToHaPeerWaveClientMessage           (const string &sourceFileName, const string &destinationFileName);
                            CopyFileToHaPeerWaveClientMessage           ();
    virtual                ~CopyFileToHaPeerWaveClientMessage           ();

            string          getSourceFileName                           () const;
            void            setSourceFileName                           (const string &sourceFileName);
            string          getDestinationFileName                      () const;
            void            setDestinationFileName                      (const string &destinationFileName);

    // Now the data members

    private :
        string              m_sourceFileName;
        string              m_destinationFileName;

    protected :
    public :
};

}

#endif // COPYFILEWAVECLIENTMESSAGE_H
