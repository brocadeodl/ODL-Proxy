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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef YANGCONTAINER_H
#define YANGCONTAINER_H

#include "Modeling/YANG/ObjectModel/YangDataElement.h"
#include "Modeling/YANG/ObjectModel/YangLeaf.h"

namespace WaveNs
{

class YangContainer : public YangDataElement
{
    private :
                bool         getConfigurationStringForLeafWithHideInSubMode (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, string &configurationString);

    protected :
    public :
                             YangContainer                         ();
                             YangContainer                         (const string &name);
        virtual             ~YangContainer                         ();

        static  string       getYangName                           ();

        static  YangElement *createInstance                        ();

        virtual void         processChildElement                   (YangElement *pYangElement);

        virtual bool         meetsCriteriaToBeShownInConfiguration () const;

        virtual void         displayConfiguration2                 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData);
        virtual void         displayConfiguration3                 (YangDisplayConfigurationContext *pYangDisplayConfigurationContext, vector<YangElement *> &selectedChildElementsForData);

        virtual void         collectHideInSubModeLeafForSelf       ();
        virtual void         propagateCompactSyntaxOneLevelForSelf ();

    // Now the data members

    private :
        bool                m_isPresenceContainer;
        YangLeaf           *m_pYangLeafWithHideInSubMode;

    protected :
    public :
};

}

#endif // YANGCONTAINER_H
