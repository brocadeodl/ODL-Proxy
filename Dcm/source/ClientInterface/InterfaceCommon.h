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

#ifndef INTERFACECOMMON_H
#define INTERFACECOMMON_H

typedef enum {
    IF_TYPE_GI,
    IF_TYPE_TE,
    IF_TYPE_FO,
    IF_TYPE_PO,
	IF_TYPE_FI,
	IF_TYPE_VE,
    IF_TYPE_VL,
	IF_TYPE_MA,
    IF_TYPE_PHY, 
    IF_TYPE_INTF_LOOPBACK,
    IF_TYPE_INVALID
}InterfaceType;

#define IFNAME_MAX_STR 64
#define SHORT_TEN_GIGABIT_ETHERNET_TOKEN "te"
#define SHORT_TEN_GIGABIT_ETHERNET_TOKEN_LENGTH 2
#define SHORT_GIGABIT_ETHERNET_TOKEN "gi"
#define SHORT_GIGABIT_ETHERNET_TOKEN_LENGTH 2
#define SHORT_FIBRECHANNEL_TOKEN "fi"
#define SHORT_FIBRECHANNEL_TOKEN_LENGTH 2
#define SHORT_FORTYGIGABIT_ETHERNET_TOKEN "fo"
#define SHORT_FORTYGIGABIT_ETHERNET_TOKEN_LENGTH 2


#define IS_VALID_TE_INTERFACE(arg)\
	    (!strncasecmp((arg), SHORT_TEN_GIGABIT_ETHERNET_TOKEN, SHORT_TEN_GIGABIT_ETHERNET_TOKEN_LENGTH))\

#define IS_VALID_GI_INTERFACE(arg)\
	    (!strncasecmp((arg), SHORT_GIGABIT_ETHERNET_TOKEN, SHORT_GIGABIT_ETHERNET_TOKEN_LENGTH))\
	
#define IS_VALID_FI_INTERFACE(arg)\
        (!strncasecmp((arg), SHORT_FIBRECHANNEL_TOKEN, SHORT_FIBRECHANNEL_TOKEN_LENGTH))\

#define IS_VALID_FO_INTERFACE(arg)\
		(!strncasecmp((arg), SHORT_FORTYGIGABIT_ETHERNET_TOKEN, SHORT_FORTYGIGABIT_ETHERNET_TOKEN_LENGTH))\

#define IS_VALID_PHYSICAL_INTERFACE(arg)\
	    ((IS_VALID_TE_INTERFACE((arg)) ) || (IS_VALID_GI_INTERFACE((arg))) ||(IS_VALID_FI_INTERFACE((arg))) || (IS_VALID_FO_INTERFACE((arg))))\
			   
#if 0

#define GENERATE_SHORT_IF_NAME_FOR_PLUGIN(_ifType, _ifId, _shortName)\
    do\
    {\
        string _twoTupleIfName;\
        switch(_ifType)\
        {\
        case IF_TYPE_GI:\
            _shortName = string("gi");\
            NsmUtils::convertThreeTuppleToTwoTupple(_ifId, _twoTupleIfName);\
            _shortName = _shortName + _twoTupleIfName;\
            break;\
        case IF_TYPE_TE:\
            _shortName = string("te");\
            NsmUtils::convertThreeTuppleToTwoTupple(_ifId, _twoTupleIfName);\
            _shortName = _shortName + _twoTupleIfName;\
            break;\
        case IF_TYPE_VL:\
            _shortName = string("Vl") + _ifId;\
            break;\
        case IF_TYPE_PO:\
            _shortName = string("Po") + _ifId;\
            break;\
        default:\
            _shortName = _ifId;\
            break;\
        }\
    }while(0)

#endif

#define GENERATE_SHORT_IF_NAME_FOR_BACK_END(_ifType, _ifId, _shortName, _sz)\
    do\
    {\
        switch(_ifType)\
        {\
        case IF_TYPE_GI:\
	    snprintf(_shortName, _sz, "gi%s", _ifId);\
            break;\
        case IF_TYPE_TE:\
	    snprintf(_shortName, _sz, "te%s", _ifId);	\
            break;\
		case IF_TYPE_FO:\
		snprintf(_shortName, _sz, "fo%s", _ifId);   \
			break;\
        case IF_TYPE_VL:\
	    snprintf(_shortName, _sz, "Vl%s", _ifId);	\
            break;\
        case IF_TYPE_PO:\
	    snprintf(_shortName, _sz, "Po%s", _ifId);	\
            break;\
        case IF_TYPE_FI:\
        snprintf(_shortName, _sz, "fi%s", _ifId);	\
            break;\
        case IF_TYPE_INTF_LOOPBACK:\
        snprintf(_shortName, _sz, "lo%s", _ifId);	\
            break;\
        case IF_TYPE_INVALID:\
        default:\
        snprintf(_shortName, _sz, "%s", _ifId);		\
        	break;\
        }\
    }while(0)


#endif
