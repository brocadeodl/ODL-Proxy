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
 *   Author :  Vipool Prajapati                                            *
 ***************************************************************************/

#ifndef VCSNODEWWNMESSAGES_H
#define VCSNODEWWNMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"
#include "Framework/Types/WorldWideName.h"
#include "Framework/Types/BitMap.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetSwitchWWNMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeSetSwitchWWNMessage();
                        VcsNodeSetSwitchWWNMessage (const UI32 &rBridgeId, const WorldWideName &newWWN);
        virtual        ~VcsNodeSetSwitchWWNMessage ();

                UI32            getRBridgeId                            () const;
                WorldWideName    getNewWWN                     () const;

    // Now the data members

    private :
        UI32          m_rBridgeId;
        WorldWideName m_newWWN;

    protected :
    public :
};

class VcsNodeGetSwitchWWNMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetSwitchWWNMessage ();
        virtual        ~VcsNodeGetSwitchWWNMessage ();

                WorldWideName    getSwitchWWN                     () const;
                void             setSwitchWWN                     (const WorldWideName &wwn);

    // Now the data members

    private :
        WorldWideName m_switchWWN;

    protected :
    public :
};

class VcsNodeGetSwitchWWNFromRBridgeIdMessage : public ManagementInterfaceMessage 
{
    private :
                void            setupAttributesForSerialization         ();

    protected :
    public :
                                VcsNodeGetSwitchWWNFromRBridgeIdMessage ();
        virtual                ~VcsNodeGetSwitchWWNFromRBridgeIdMessage ();

                WorldWideName   getSwitchWWN                            () const;
                void            setSwitchWWN                            (const WorldWideName &switchWWN);
                UI32            getRBridgeId                            () const;
                void            setRBridgeId                            (const UI32 rBridgeId);

    // Now the data members
    private :
                WorldWideName   m_switchWWN;
                UI32            m_rBridgeId;
            
    protected :
    public :
};

class VcsGetClusterRbridgeIdFromWWNMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                                VcsGetClusterRbridgeIdFromWWNMessage ();
                                VcsGetClusterRbridgeIdFromWWNMessage (const WorldWideName &wwn);
        virtual                ~VcsGetClusterRbridgeIdFromWWNMessage ();

                BitMap          getRbridgeIdBitMap                   () const;
                void            setRbridgeIdBitMap                  (BitMap &rbridgeIdBitMap);

                WorldWideName   getInputWWN                         () const;
                void            setInputWWN                         (const WorldWideName &wwn);

    // Now the data members

    private :
        WorldWideName    m_inputWWN;
        BitMap           m_rbridgeIdBitMap;

    protected :
    public :

};

}

#endif // VCSNODEWWNMESSAGES_H
