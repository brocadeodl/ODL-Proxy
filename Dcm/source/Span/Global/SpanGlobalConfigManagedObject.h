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
 *   Author : Ojasvita                                                     *
 **************************************************************************/


#ifndef SPANLOCALCONFIGLOCALMANAGEDOBJECT_H
#define SPANLOCALCONFIGLOCALMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

	    class SpanGlobalObjectManager;
		class SpanGlobalConfigManagedObject : virtual public DcmManagedObject
		{
		     private:
		     protected:
		        virtual void setupAttributesForPersistence();
		        virtual void setupAttributesForCreate();
			 public:
				SpanGlobalConfigManagedObject (SpanGlobalObjectManager *pSpanGlobalObjectManager);
				SpanGlobalConfigManagedObject (SpanGlobalObjectManager *pSpanGlobalObjectManager,const UI32 &session_id,const string &source_ifName,const string &dest_ifName,const UI32 &direction,const string &description, const UI32 &default_source_enum, const UI32 &default_destination_enum, const UI32 & source_tengiga_enum, const UI32 &dest_tengiga_enum,const UI32 &dstVlanID);
				virtual    ~SpanGlobalConfigManagedObject ();
				static string getClassName();
				void setSession_id(const UI32 &session_id);
				UI32 getSession_id()  const;
				void setSource_ifName(const string &source_ifName);
				string getSource_ifName()  const;
				void setDest_ifName(const string &dest_ifName);
				string getDest_ifName()  const;
				void setDirection(const UI32 &direction);
				UI32 getDirection()  const;
				void setDescription(const string &description);
				string getDescription()  const;
				void setSource(const UI32 &source);
				UI32 getSource()  const;
				void setDestination(const UI32 &destination);
				UI32 getDestination()  const;
				void setSource_tengiga(const UI32 &source_tengiga);
				UI32 getSource_tengiga()  const;
				void setDest_tengiga(const UI32 &destination_tengiga);
				UI32 getDest_tengiga()  const;
				void setEnumleaves_default();
				void unsetEnumleaves_default();

				void setDestVlanId(const UI32 &vlanId);
				UI32 getDestVlanId()  const;


// Now the data members
			
			private:
	        protected:
	        public:
	            UI32  m_session_id;
 	            string  m_source_ifName;
	            string  m_dest_ifName;
	            UI32  m_direction;
	            string  m_description;
	            UI32   m_source_enum;
	            UI32   m_destination_enum;
	            UI32   m_source_tengiga_enum;
	            UI32   m_dest_tengiga_enum;

	            UI32  m_destVlanId;
		};
}

#endif                                            //SPANLOCALCONFIGLOCALMANAGEDOBJECT_H
				
																								  
