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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou	                                                   *
 ***************************************************************************/

#ifndef IPV4ADDRESSUC_H
#define IPV4ADDRESSUC_H

#include "Framework/Types/IpV4Address.h"
#include "Framework/Types/Types.h"

namespace WaveNs
{

class IpV4AddressUC 
{
    private :
    protected :
    public :
                                      IpV4AddressUC              ();
                                      IpV4AddressUC              (const IpV4Address &ipV4Address);
                                      IpV4AddressUC              (const IpV4Address &ipV4Address, const bool &isUserConfigured);
                                      IpV4AddressUC              (const IpV4AddressUC &ipV4AddressUC);
        virtual                      ~IpV4AddressUC              ();
                                                                  
        virtual   UI32                getAddressFamily           ();
                                                                  
                  void                setIpV4AddressValue        (const IpV4Address &pIpV4Address);
                  IpV4Address         getIpV4AddressValue        () const;
                                                                  
                  void                setIsUserConfigured        (const bool &isUserConfigured);
                  bool                getIsUserConfigured        () const;
                                                                  
                  string              toString                   ();
        void                          fromString                 (const string &pIpV4AddressUCInStringFormat);
                                                                  
        virtual   void                getPlainString             (string &valueString) const;
        virtual   ResourceId          loadFromPlainString        (const string &pIpV4AddressUCInPlainStringFormat);
                                      
                  bool                operator == (const IpV4AddressUC &pIpV4AddressUC)  const ;
                  bool                operator != (const IpV4AddressUC &pIpV4AddressUC)  const ;
                  IpV4AddressUC       &operator  = (const IpV4AddressUC &pIpV4AddressUC);

    // Now the data members

    private :
        IpV4Address    m_pIpV4Address;
        bool           m_pIsUserConfigured;
};

}

#endif // IPV4ADDRESSUC_H
