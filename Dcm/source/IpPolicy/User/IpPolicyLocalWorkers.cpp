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
 *   Author : suw                                                     *
 **************************************************************************/
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "Utils/Capabilities.h"
#include <WyserEaGateway/GenericClientMessage.h>
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "IpPolicy/Framework/IpPolicyLocalObjectManager.h"
#include "IpPolicy/User/IpPolicyLocalWorkers.h"
#include "IpPolicy/Framework/IpPolicyLocalMessages.h"
#include "IpPolicy/Framework/PbrVrfNexthopMessage.h"
#include "IpPolicy/Framework/IpPolicyLocalManagedObjects.h"
#include "IpPolicy/Framework/PbrVrfManagedObject.h"
#include "IpPolicy/Framework/PbrNexthopAddrManagedObject.h"
#include "IpPolicy/Framework/PbrGlobalNexthopManagedObject.h"
#include "IpPolicy/Framework/IpPolicyDeletePBRMessage.h"
#include "IpPolicy/Framework/IpPolicyDeleteGlobalPBRMessage.h"
#include "IpPolicy/Framework/PBRShowMessage.h"
#include "IpPolicy/Framework/IpAsPathAccessListLocalManagedObject.h"
#include "IpPolicy/Framework/IpCommunityStdAccessListLocalManagedObject.h"
#include "IpPolicy/Framework/IpCommunityExtAccessListLocalManagedObject.h"
#include "IpPolicy/Framework/FilterChangeUpdateLocalMessage.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "IpPolicy/Framework/FilterChangeUpdateLocalManagedObject.h"
#include "IpPolicy/Framework/IpPolicyLocalTypes.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "IpPolicy/Framework/IpPolicyGetPBRObjectMessage.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/L3PostBootUtil.h"
#include "ssmPbr_dcmd_public.h"
#include "ClientInterface/Ssm/SsmPbrShowClientMessage.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmCStatus.h"

using namespace std;

#define SET_MO_ATTRIBUTE(_attr_name) \
do { \
    if (msg->get##_attr_name##Flag()) {\
        mo->set##_attr_name(msg->get##_attr_name());\
    } \
} while (0)

#define UNSET_MO_ATTRIBUTE(_attr_name) \
do { \
    if (msg->get##_attr_name##Flag()) {\
        mo->unset##_attr_name();\
    } \
} while (0)

#define UNSET_MO_ATTRIBUTE_SELECTIVE(_attr_name) \
do { \
    if (msg->get##_attr_name##Flag()) {\
        mo->unset##_attr_name##_selective(msg->get##_attr_name());\
    } \
} while (0)

#define SET_UNSET_MO_ATTRIBUTE(_attr_name) \
do { \
    if (msg->get##_attr_name##Flag()) {\
        mo->set##_attr_name(msg->get##_attr_name());\
    } \
    else { \
        mo->unset##_attr_name();\
    } \
} while (0)


namespace DcmNs
{

    static string toString(PolicyRouteType type)
    {

        switch (type) {
        case internal:
            return "internal";
        case type_1:
            return "type-1";
        case type_2:
            return "type-2";
        case level_1:
            return "level-1";
        case level_2:
            return "level-2";

        default:
            return "unknown";
        };
    }

    static string toString(PolicyMetricAction type)
    {
        switch (type) {
        case add:
            return "add";
        case subtract:
            return "subtract";
        case assign:
            return "assign";
        case none:
        	return "none";

        default:
            return "unknown";

        }
        return "unknown";
    }

    static string toString(PolicyAction action) {
        switch (action) {
        case permit:
            return "permit";

        case deny:
            return "deny";

        default:
            return "";
        }

        return "";
    }

    template<typename VAL>
    static string toString (VAL val)
    {
        stringstream ss;
        ss << val;
        return ss.str();
    }

    string getTwoTouple (string ifName)
    {
        string twoTupleName = "";

        if (NsmUtils::isValidThreeTuple(ifName) &&
            (NsmUtils::getLocationId(ifName) == FrameworkToolKit::getThisLocationId())) {
                NsmUtils::getTwoTupleIfName(ifName, twoTupleName);
        } else {
            twoTupleName = ifName;
        }

        return twoTupleName;
    }

    static void parseCommunityList(const string& communtiyListStr, vector<string>& args){
    	size_t indexStr = 0;
    	size_t tmpIndex = string::npos;
		while(indexStr != string::npos)
		{
			tmpIndex = communtiyListStr.find_first_of(" ", indexStr);
			if(indexStr != tmpIndex)
			{
				string tmpStr = communtiyListStr.substr(indexStr, tmpIndex - indexStr);
				args.push_back(tmpStr);
			}
			indexStr = communtiyListStr.find_first_not_of(" ", tmpIndex);
		}
    }

    static void reSizeClientMsgArgs(vector<string>& args) {
		if(args.size() > GENERIC_WYSEREA_ARGS_MAX) {
			size_t diffsize = args.size() - GENERIC_WYSEREA_ARGS_MAX;
			for(size_t idx = 0; idx < diffsize; idx++) {
				args.erase(args.end());
			}
		}
    }

    static ResourceId updateRouteMapMessage (IpPolicyRouteMapMessage *msg, vector<string>& args, int& op_code, 
            const RouteMapLocalManagedObject* mo)
    {
        int total_interfaces = 0;

        switch(op_code) {
            case SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF:
                if (msg->getMatch_intf_ethFlag()) {
                    vector<string> eths = msg->getMatch_intf_eth();
                    if(!eths.size()) { 
                        eths = mo->getMatch_intf_eth();
                        if(!eths.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }        
                    
                    total_interfaces += eths.size();
                    
                    for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                        args.push_back("te");
                        args.push_back(getTwoTouple(toString(*it)));
                    }
                }    
                
                if (msg->getMatch_intf_giFlag()) {
                    vector<string> eths = msg->getMatch_intf_gi();
                    if(!eths.size()) { 
                        eths = mo->getMatch_intf_gi();
                        if(!eths.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }        

                    total_interfaces += eths.size();

                    for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                        args.push_back("gi");
                        args.push_back(getTwoTouple(toString(*it)));
                    }
                }

                if (msg->getMatch_intf_foFlag()) {
                    vector<string> eths = msg->getMatch_intf_fo();
                    if(!eths.size()) { 
                        eths = mo->getMatch_intf_fo();
                        if(!eths.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }        

                    total_interfaces += eths.size();

                    for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                        args.push_back("fo");
                        args.push_back(getTwoTouple(toString(*it)));
                    }
                }

                if (msg->getMatch_intf_vlanFlag()) {
                    vector<UI32> vlans = msg->getMatch_intf_vlan();
                    if(!vlans.size()) {
                        vlans = mo->getMatch_intf_vlan();
                        if(!vlans.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }    
                    
                    total_interfaces += vlans.size();

                    for (vector<UI32>::iterator it = vlans.begin(); it != vlans.end(); ++it) {
                        args.push_back("vlan");
                        args.push_back(toString(*it));
                    }
                }

                if (msg->getMatch_intf_poFlag()) {
                    vector<UI32> pos = msg->getMatch_intf_po();
                    if(!pos.size()) {
                        pos = mo->getMatch_intf_po();
                        if(!pos.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }    

                    total_interfaces += pos.size();
                    
                    for (vector<UI32>::iterator it = pos.begin(); it != pos.end(); ++it) {
                        args.push_back("po");
                        args.push_back(toString(*it));
                    }
                }

                if (msg->getMatch_intf_veFlag()) {
                    vector<UI32> ves = msg->getMatch_intf_ve();
                    if(!ves.size()) {
                    	ves = mo->getMatch_intf_ve();
                        if(!ves.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }

                    total_interfaces += ves.size();

                    for (vector<UI32>::iterator it = ves.begin(); it != ves.end(); ++it) {
                        args.push_back("ve");
                        args.push_back(toString(*it));
                    }
                }

                if (msg->getMatch_intf_loopbackFlag()) {
                    vector<UI32> loopbacks = msg->getMatch_intf_loopback();
                    if(!loopbacks.size()) {
                    	loopbacks = mo->getMatch_intf_loopback();
                        if(!loopbacks.size())
                            return WRC_GENERIC_BACKEND_ERR;
                    }

                    total_interfaces += loopbacks.size();

                    for (vector<UI32>::iterator it = loopbacks.begin(); it != loopbacks.end(); ++it) {
                        args.push_back("loopback");
                        args.push_back(toString(*it));
                    }
                }

                break;

            default:
                break;
        }       

        if ( total_interfaces > IP_POLICY_MATCH_MAX_INTERFACES )
        {
            msg->set_backend_err(string("%%Error: Maximum allowed match interfaces is ") + IP_POLICY_MATCH_MAX_INTERFACES);
            return WRC_GENERIC_BACKEND_ERR;
        }

        return WAVE_MESSAGE_SUCCESS; 
    }

    static ResourceId parseRouteMap (IpPolicyRouteMapMessage *msg, vector<string>& args, int& op_code, bool set_cmd,
            const RouteMapLocalManagedObject* mo)
    {
    	bool intf = false, te = true, gi = true, fo = true, vlan = true, po = true, ve =true, loopback=true;
        int total_interfaces = 0;
		int dscpPrecedenceVal;
		string dscpPrecedenceStr;
		std::stringstream ss;

        if (msg->getRoute_continueFlag()) {
        	op_code = SSM_CLIENT_SET_ROUTEMAP_CONTINUE;
        	if (set_cmd)
        		args.push_back(toString(msg->getRoute_continue()));
        }
        else if ( msg->getRoute_continue() == 100 && msg->getRoute_continueFlag() == false ) {
        	op_code = SSM_CLIENT_SET_ROUTEMAP_CONTINUE;
        	if (set_cmd)
        		args.push_back(toString(0));
        }

        if (msg->getMatch_addr_prefixFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_ADDR;
            if (set_cmd)
                args.push_back(msg->getMatch_addr_prefix());
        }

        if (msg->getMatch_addr_aclFlag()) {
		  	op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_IPACL;
            if (set_cmd) {
                args.push_back(msg->getMatch_addr_acl());
				trace(TRACE_LEVEL_INFO, string(" inside set "));
				trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::parseRoute: msg->getMatch_addr_acl() =") + msg->getMatch_addr_acl());
            } else {
			  	args.push_back("no");
				trace(TRACE_LEVEL_INFO, string(" inside unset "));
				//args.push_back(msg->getMatch_addr_acl());
			}
        }

        if (msg->getMatch_nh_prefixFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_NH;
            if (set_cmd)
                args.push_back(msg->getMatch_nh_prefix());
        }

        if (msg->getMatch_nullZeroFlag()) {
		  	op_code = SSM_CLIENT_SET_ROUTEMAP_SET_INTERFACE;
            if(set_cmd) {
                //args.push_back(msg->getMatch_nullZero());
			    args.push_back("null0");
				trace(TRACE_LEVEL_INFO, string(" inside set "));
				trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::parseRoute: getMatch_nullZero() =") + msg->getMatch_nullZero());
			} else {
			  	args.push_back("no");
				trace(TRACE_LEVEL_INFO, string(" inside unset "));
				//args.push_back(msg->getMatch_nullZero());
			}
       }
/*
        if (msg->getMatch_precedenceFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_PRECEDENCE;
            if(set_cmd) {
                precedenceVal = msg->getMatch_precedence();
                ss  << precedenceVal;
                precedenceStr = ss.str();
                args.push_back(precedenceStr);
            }
        }
		*/

        if (msg->getMatch_dscpPrecedenceFlag()) {
		    op_code = SSM_CLIENT_SET_ROUTEMAP_SET_DSCP;
		  	dscpPrecedenceVal = msg->getMatch_dscpPrecedence();
			ss  << dscpPrecedenceVal;
			dscpPrecedenceStr = ss.str();
            if(set_cmd) {
                args.push_back(dscpPrecedenceStr);
				trace(TRACE_LEVEL_INFO, string(" inside set "));
				trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::parseRoute: dscpPrecedenceStr =") + dscpPrecedenceStr);
            } else {
			  	args.push_back("no");
				trace(TRACE_LEVEL_INFO, string(" inside unset "));
				//args.push_back(dscpPrecedenceStr);
			}
        }

        if (msg->getMatch_next_hop_addressFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_NEXT_HOP_FILTER;
            if (set_cmd)
                args.push_back(toString<UI32>(msg->getMatch_next_hop_address()));
        }

        if (msg->getMatch_rs_prefixFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_RS_PREFIX_LIST;
            if (set_cmd) {
                string ip_prefix = msg->getMatch_rs_prefix();
            	size_t indexStr = 0;
            	size_t tmpIndex = string::npos;
				while(indexStr != string::npos)
				{
					tmpIndex = ip_prefix.find_first_of(" ", indexStr);
					if(indexStr != tmpIndex)
					{
						string tmpAsPathStr = ip_prefix.substr(indexStr, tmpIndex - indexStr);
						args.push_back(tmpAsPathStr);
					}
					indexStr = ip_prefix.find_first_not_of(" ", tmpIndex);
				}
            }
        }

        if (msg->getMatch_intf_ethFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<string> eths = msg->getMatch_intf_eth();
                for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                    args.push_back("te");
                    args.push_back(getTwoTouple(toString(*it)));
                }
                intf = true;
                te = false;

                total_interfaces += eths.size();
            }
        }

        if (msg->getMatch_intf_giFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<string> eths = msg->getMatch_intf_gi();
                for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                    args.push_back("gi");
                    args.push_back(getTwoTouple(toString(*it)));
                }
                intf = true;
                gi = false;

                total_interfaces += eths.size();
            }
        }

        if (msg->getMatch_intf_foFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<string> eths = msg->getMatch_intf_fo();
                for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                    args.push_back("fo");
                    args.push_back(getTwoTouple(toString(*it)));
                }
                intf = true;
                fo = false;

                total_interfaces += eths.size();
            }
        }

        if (msg->getMatch_intf_vlanFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<UI32> vlans = msg->getMatch_intf_vlan();
                for (vector<UI32>::iterator it = vlans.begin(); it != vlans.end(); ++it) {
                    args.push_back("vlan");
                    args.push_back(toString(*it));
                }
                intf = true;
                vlan = false;

                total_interfaces += vlans.size();
            }
        }

        if (msg->getMatch_intf_poFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<UI32> pos = msg->getMatch_intf_po();
                for (vector<UI32>::iterator it = pos.begin(); it != pos.end(); ++it) {
                    args.push_back("po");
                    args.push_back(toString(*it));
                }
                intf = true;
                po = false;

                total_interfaces += pos.size();
            }
        }

        if (msg->getMatch_intf_veFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<UI32> ves = msg->getMatch_intf_ve();
                for (vector<UI32>::iterator it = ves.begin(); it != ves.end(); ++it) {
                    args.push_back("ve");
                    args.push_back(toString(*it));
                }
                intf = true;
                ve = false;

                total_interfaces += ves.size();
            }
        }

        if (msg->getMatch_intf_loopbackFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF;
            if (set_cmd) {
                vector<UI32> loopbacks = msg->getMatch_intf_loopback();
                for (vector<UI32>::iterator it = loopbacks.begin(); it != loopbacks.end(); ++it) {
                    args.push_back("loopback");
                    args.push_back(toString(*it));
                }
                intf = true;
                loopback = false;

                total_interfaces += loopbacks.size();
            }
        }

        if (intf) {
        	// if match interface changes, include what is already saved
        	if (te) {
                vector<string> eths = mo->getMatch_intf_eth();
                for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                    args.push_back("te");
                    args.push_back(getTwoTouple(toString(*it)));
                }

                total_interfaces += eths.size();
        	}

        	if (gi) {
                vector<string> eths = mo->getMatch_intf_gi();
                for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                    args.push_back("gi");
                    args.push_back(getTwoTouple(toString(*it)));
                }

                total_interfaces += eths.size();
        	}

        	if (fo) {
                vector<string> eths = mo->getMatch_intf_fo();
                for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
                    args.push_back("fo");
                    args.push_back(getTwoTouple(toString(*it)));
                }

                total_interfaces += eths.size();
        	}

        	if (po) {
                vector<UI32> pos = mo->getMatch_intf_po();
                for (vector<UI32>::iterator it = pos.begin(); it != pos.end(); ++it) {
                    args.push_back("po");
                    args.push_back(toString(*it));
                }

                total_interfaces += pos.size();
        	}

        	if (ve) {
                vector<UI32> ves = mo->getMatch_intf_ve();
                for (vector<UI32>::iterator it = ves.begin(); it != ves.end(); ++it) {
                    args.push_back("ve");
                    args.push_back(toString(*it));
                }

                total_interfaces += ves.size();
        	}

        	if (loopback) {
                vector<UI32> loopbacks = mo->getMatch_intf_loopback();
                for (vector<UI32>::iterator it = loopbacks.begin(); it != loopbacks.end(); ++it) {
                    args.push_back("loopback");
                    args.push_back(toString(*it));
                }

                total_interfaces += loopbacks.size();
        	}

        	if (vlan) {
				vector<UI32> vlans = mo->getMatch_intf_vlan();
				for (vector<UI32>::iterator it = vlans.begin(); it != vlans.end(); ++it) {
					args.push_back("vlan");
					args.push_back(toString(*it));
				}

                total_interfaces += vlans.size();
        	}
        }

        /*Maximum allowed match interfaces is */
        if ( total_interfaces > IP_POLICY_MATCH_MAX_INTERFACES )
        {
            msg->set_backend_err(string("%%Error: Maximum allowed match interfaces is ") + IP_POLICY_MATCH_MAX_INTERFACES);
            return WRC_GENERIC_BACKEND_ERR;
        }

        if (msg->getMatch_metricFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_METRIC;
            if (set_cmd) {
                args.push_back(toString(msg->getMatch_metric()));
            }
        }

        if (msg->getMatch_route_typeFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TYPE;
            if (set_cmd) {
                args.push_back(toString(msg->getMatch_route_type()));
            }
        }

        if (msg->getMatch_protocol_bgpFlag()) {
        	op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_BGP_PROTOCOL_ROUTE_TYPE;
        	if (set_cmd) {
        		args.push_back("bgp");
        	}
        }

        if (msg->getMatch_bgp_route_typeFlag()) {
        	op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_BGP_PROTOCOL_ROUTE_TYPE;
        	if (set_cmd) {
        		if(msg->getMatch_bgp_route_type() == bgp_proto_external) {
        			args.push_back("external");
        		} else if(msg->getMatch_bgp_route_type() == bgp_proto_static_network) {
        			args.push_back("static-network");
        		} else if (mo->getMatch_bgp_route_type() == bgp_proto_internal) {
					args.push_back("internal");
				}
        	}
        }

        if (msg->getMatch_route_tagFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TAG;
            if (set_cmd) {
                vector<UI32> tags = msg->getMatch_route_tag();
                for (vector<UI32>::iterator it = tags.begin(); it != tags.end(); ++it) {
                    args.push_back(toString(*it));
                }
            }
        }

        if (msg->getMatch_route_aspathFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_ASPATH;
            if (set_cmd) {
                string asPathStr = msg->getMatch_route_aspath();
                set<string> matchAsPathList;
                stringstream asPathTmpStr;
            	size_t indexStr = 0;
            	size_t tmpIndex = string::npos;
				while(indexStr != string::npos)
				{
					tmpIndex = asPathStr.find_first_of(" ", indexStr);
					if(indexStr != tmpIndex)
					{
						string tmpAsPathStr = asPathStr.substr(indexStr, tmpIndex - indexStr);
						matchAsPathList.insert(tmpAsPathStr);
					}
					indexStr = asPathStr.find_first_not_of(" ", tmpIndex);
				}
				set<string>::iterator it;
				for ( it = matchAsPathList.begin(); it != matchAsPathList.end(); it++ )
				{
					args.push_back(*it);
					asPathTmpStr << *it;
					asPathTmpStr << " ";
				}
				msg->setMatch_route_aspath(asPathTmpStr.str());
            }
        }

        if (msg->getSet_distanceFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_DISTANCE;
            if (set_cmd)
                args.push_back(toString<UI32>(msg->getSet_distance()));
        }

        if (msg->getSet_metricFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_METRIC;
            if (set_cmd) {
                if (!msg->getSet_metric_actionFlag()) {
                    if (mo && mo->getSet_metric_action() != assign)
                        args.push_back(toString(mo->getSet_metric_action()));
                }
                else if (msg->getSet_metric_action() !=  assign) {
                    args.push_back(toString(msg->getSet_metric_action()));
                }

                if (msg->getSet_metric_action() !=  none) {
                	args.push_back(toString(msg->getSet_metric()));
                }
            }
        }
        else if (msg->getSet_metric_actionFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_METRIC;
            if (set_cmd) {
                if (msg->getSet_metric_action() !=  assign)
                    args.push_back(toString(msg->getSet_metric_action()));

                if (mo) {
                	if (msg->getSet_metric_action() !=  none) {
                		args.push_back(toString(mo->getSet_metric()));
                	}
                }
                else // fake number, should not happen
                    args.push_back("0");
            }
        }

        if (msg->getSet_route_typeFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TYPE;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_type()));
        }

        if (msg->getSet_route_tagFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TAG;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_tag()));
        }

        if (msg->getSet_route_local_preferenceFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_LOCAL_PREFERENCE;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_local_preference()));
        }

        if (msg->getSet_route_weightFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_WEIGHT;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_weight()));
        }

        if( msg->getSet_route_dampening_half_lifeFlag() || msg->getSet_route_dampening_reuseFlag()
        		|| msg->getSet_route_dampening_suppressFlag() || msg->getSet_route_dampening_max_suppressFlag() )
        {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_DAMPENING;

            if (msg->getSet_route_dampening_half_lifeFlag()) {
                if (set_cmd)
                    args.push_back(toString(msg->getSet_route_dampening_half_life()));
            }
            else if(mo->getSet_route_dampening_half_life())
            {
                if (set_cmd)
                    args.push_back(toString(mo->getSet_route_dampening_half_life()));
            }

            if (msg->getSet_route_dampening_reuseFlag()) {
            	if (set_cmd)
                    args.push_back(toString(msg->getSet_route_dampening_reuse()));
            }
            else if(mo->getSet_route_dampening_reuse())
            {
                if (set_cmd)
                    args.push_back(toString(mo->getSet_route_dampening_reuse()));
            }

            if (msg->getSet_route_dampening_suppressFlag()) {
                if (set_cmd)
                    args.push_back(toString(msg->getSet_route_dampening_suppress()));
            }
            else if(mo->getSet_route_dampening_suppress())
            {
                if (set_cmd)
                    args.push_back(toString(mo->getSet_route_dampening_suppress()));
            }

            if (msg->getSet_route_dampening_max_suppressFlag()) {
            	if (set_cmd)
                    args.push_back(toString(msg->getSet_route_dampening_max_suppress()));
            }
            else if(mo->getSet_route_dampening_max_suppress())
            {
                if (set_cmd)
                    args.push_back(toString(mo->getSet_route_dampening_max_suppress()));
            }
        }

        if (msg->getSet_route_aspath_prependFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ASPATH_PREPEND;
            if (set_cmd) {
            	parseCommunityList(msg->getSet_route_aspath_prepend(), args);
            }
        }

        if (msg->getSet_route_aspath_tagFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ASPATH_TAG;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_aspath_tag()));
        }

        if (msg->getMatch_route_community_listFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_MATCH_COMMUNITY_LIST;
            if (set_cmd) {
            	parseCommunityList(msg->getMatch_route_community_list(), args);
            	if(args[(args.size()-1)].compare("exact-match") == 0) {
            		args.erase(args.end());
            		args.push_back("1");
            	} else {
            		args.push_back("0");
            	}
            }
        }

        if (msg->getSet_route_origin_igpFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ORIGIN;
            if (set_cmd)
                args.push_back("igp");
        }

        if (msg->getSet_route_origin_incompleteFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ORIGIN;
            if (set_cmd)
                args.push_back("incomplete");
        }

        if (msg->getSet_next_hop_peer_addressFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH_PEER_ADDR;
            if (set_cmd)
                args.push_back("peer-address");
        }

        if (msg->getSet_route_metric_type_externalFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE;
            if (set_cmd)
                args.push_back("external");
        }

        if (msg->getSet_route_metric_type_internalFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE;
            if (set_cmd)
                args.push_back("internal");
        }

        if (msg->getSet_route_metric_type_type_1Flag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE;
            if (set_cmd)
                args.push_back("type_1");
        }

        if (msg->getSet_route_metric_type_type_2Flag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE;
            if (set_cmd)
                args.push_back("type_2");
        }

        if (msg->getSet_route_community_acl_deleteFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_ACCESS_LIST;
            if (set_cmd) {
            	 args.push_back(toString(msg->getSet_route_community_access_list()));
            }
        }

        if (msg->getSet_route_community_exprFlag()) {
        	op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_EXPR;
        	if (set_cmd) {
        		if(!(msg->getSet_route_community_expr().empty()))
        		{
        			vector<string> communityTmpList;
        			bool local_as=false;
        			bool no_export=false;
        			bool none=false;
        			bool additive=false;
        			bool no_advertise = false;

        			set<string> communityList;
        			stringstream tmpCommStr;

        			tokenize(msg->getSet_route_community_expr(),communityTmpList,' ');

        			for(size_t indexTmp=0; indexTmp < communityTmpList.size(); indexTmp++) {

        				if ((strncmp(communityTmpList[indexTmp].c_str(), "local-as", strlen("local-as"))== 0)) {
        					local_as = true;
        				} else if ((strncmp(communityTmpList[indexTmp].c_str(), "none", strlen("none"))== 0)) {
        					none = true;
        				} else if ((strncmp(communityTmpList[indexTmp].c_str(), "additive", strlen("additive"))== 0)) {
        					additive = true;
        				} else if ((strncmp(communityTmpList[indexTmp].c_str(), "no-export", strlen("no-export"))== 0)) {
        					no_export = true;
        				} else if ((strncmp(communityTmpList[indexTmp].c_str(), "no-advertise", strlen("no-advertise"))== 0)) {
        					no_advertise = true;
        				} else {
        					string strDecimal = communityTmpList[indexTmp];
        					size_t found = strDecimal.find(":");

        					if (found == string::npos)
        					{
        						unsigned long decimalNum = strtoul(strDecimal.c_str(), NULL, 0);
        						stringstream decimlaAANN;

        						if(decimalNum > 65535)
        						{
        							int quotVal = (decimalNum / 65536);
        							int remVal = (decimalNum % 65536);
        							decimlaAANN << quotVal;
        							decimlaAANN << ":";
        							decimlaAANN << remVal;
        						} else {
        							decimlaAANN << "0";
        							decimlaAANN << ":";
        							decimlaAANN << strDecimal;
        						}

        						if ( decimlaAANN.str().compare("65535:65281") == 0 )
        						{
        							no_export = true;
        						}
        						else if ( decimlaAANN.str().compare("65535:65282") == 0 )
        						{
        							no_advertise = true;
        						}
        						else if ( decimlaAANN.str().compare("65535:65283") == 0 )
        						{
        							local_as = true;
        						}
        						else
        						{
        							communityList.insert(decimlaAANN.str());
        						}
        					} else {

        						if( communityTmpList[indexTmp].compare("65535:65281") == 0 )
        						{
        							no_export = true;
        						}
        						else if( communityTmpList[indexTmp].compare("65535:65282") == 0 )
        						{
        							no_advertise = true;
        						}
        						else if( communityTmpList[indexTmp].compare("65535:65283") == 0 )
        						{
        							local_as = true;
        						}
        						else
        						{
        							communityList.insert(communityTmpList[indexTmp]);
        						}
        					}
        				}
        			}

        			if(!communityList.empty()) {
        				stringstream sizeStr;
        				size_t argSize = communityList.size();
        				if( argSize > GENERIC_WYSEREA_ARGS_MAX ) {
        					argSize = 50;
        				}
        				sizeStr << (argSize);
        				args.push_back(sizeStr.str());
        				set<string>::iterator it;
        				for (it=communityList.begin(); it!=communityList.end(); it++) {
        					args.push_back(*it);
        					tmpCommStr << *it;
        					tmpCommStr << " ";
        				}
        			} else {
        				args.push_back("0");
        			}

        			if(local_as) {
        				args.push_back("local-as");
        				tmpCommStr << "local-as";
        				tmpCommStr << " ";
        			}

        			if(none) {
        				args.push_back("none");
        				tmpCommStr << "none";
        				tmpCommStr << " ";
        			}

        			if(no_export) {
        				args.push_back("no-export");
        				tmpCommStr << "no-export";
        				tmpCommStr << " ";
        			}

        			if(no_advertise) {
        				args.push_back("no-advertise");
        				tmpCommStr << "no-advertise";
        				tmpCommStr << " ";
        			}

        			if(additive) {
        				args.push_back("additive");
        				tmpCommStr << "additive";
        			}

        			msg->setSet_route_community_expr(tmpCommStr.str());
        		} else {
        			args.push_back("0");
        		}
        	}
        }

        if (msg->getSet_route_extcommunity_exprFlag()) {
        	op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_EXTCOMMUNITY_EXPR;
        	if (set_cmd) {
        		if(!(msg->getSet_route_extcommunity_expr().empty()))
        		{
        			vector<string> communityTmpList;
        			bool additive = false;
        			set<string> communityList;
        			stringstream tmpCommStr;

        			tokenize(msg->getSet_route_extcommunity_expr(),communityTmpList,' ');

        			for(size_t indexTmp=0; indexTmp < communityTmpList.size(); indexTmp++) {
        				if ((strncmp(communityTmpList[indexTmp].c_str(), "additive", strlen("additive"))== 0)) {
        					additive = true;
        				} else {
        					communityList.insert(communityTmpList[indexTmp]);
        				}
        			}

        			if(!communityList.empty()) {
        				stringstream sizeStr;
        				size_t argSize = communityList.size();
        				if( argSize > GENERIC_WYSEREA_ARGS_MAX ) {
        					argSize = GENERIC_WYSEREA_ARGS_MAX;
        				}
        				sizeStr << (argSize);
        				args.push_back(sizeStr.str());
        				set<string>::iterator it;
        				for (it=communityList.begin(); it!=communityList.end(); it++) {
        					args.push_back(*it);
        					tmpCommStr << *it;
        					tmpCommStr << " ";
        				}
        			} else {
        				args.push_back("0");
        			}

        			if(additive) {
        				args.push_back("additive");
        				tmpCommStr << "additive";
        				tmpCommStr << " ";
        			}

        			msg->setSet_route_extcommunity_expr(tmpCommStr.str());
        		} else {
        			args.push_back("0");
        		}

        	}
        }

        if (msg->getSet_route_extcommunity_soo_exprFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_EXTCOMMUNITY_SOO_EXPR;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_extcommunity_soo_expr()));
        }

        if (msg->getSet_route_automatic_tagFlag()) {
            op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_AUTOMATIC_TAG;
            if (set_cmd)
                args.push_back(toString(msg->getSet_route_automatic_tag()));
        }

        return WAVE_MESSAGE_SUCCESS; 
    }

    IpPolicyLocalWorker::IpPolicyLocalWorker ( IpPolicyLocalObjectManager *pIpPolicyLocalObjectManager)
        : WaveWorker  (pIpPolicyLocalObjectManager)
    {
        RouteMapLocalManagedObject    RouteMapLocalManagedObject    (pIpPolicyLocalObjectManager);
		PbrVrfManagedObject			  PbrVrfManagedObject			(pIpPolicyLocalObjectManager);
		PbrNexthopAddrManagedObject   PbrNexthopAddrManagedObject   (pIpPolicyLocalObjectManager);
		PbrGlobalNexthopManagedObject PbrGlobalNexthopManagedObject (pIpPolicyLocalObjectManager);
        IpPrefixLocalManagedObject    IpPrefixLocalManagedObject    (pIpPolicyLocalObjectManager);
        RouteMapLocalManagedObject.setupOrm ();
        addManagedClass (RouteMapLocalManagedObject::getClassName (), this);
		PbrVrfManagedObject.setupOrm ();
		addManagedClass (PbrVrfManagedObject::getClassName (), this);
		PbrNexthopAddrManagedObject.setupOrm ();
		addManagedClass (PbrNexthopAddrManagedObject::getClassName (), this);
		PbrGlobalNexthopManagedObject.setupOrm ();
		addManagedClass (PbrGlobalNexthopManagedObject::getClassName (), this);
        IpPrefixLocalManagedObject.setupOrm ();
        addManagedClass (IpPrefixLocalManagedObject::getClassName (), this);
        addOperationMap (IPPOLICYROUTEMAP, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::IpPolicyRouteMapMessageHandler));
        addOperationMap (IPPOLICYIPPREFIX, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::IpPolicyIpPrefixMessageHandler));
        IpAsPathAccessListLocalManagedObject  IpAsPathAccessListLocalManagedObject  (pIpPolicyLocalObjectManager);
        IpAsPathAccessListLocalManagedObject.setupOrm ();
        addManagedClass (IpAsPathAccessListLocalManagedObject::getClassName (), this);
        addOperationMap (IPPOLICYIPASPATHACCESSLIST, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::IpPolicyAsPathAccessListLocalMessageHandler));
        IpCommunityStdAccessListLocalManagedObject IpCommunityStdAccessListLocalManagedObject (pIpPolicyLocalObjectManager);
        IpCommunityStdAccessListLocalManagedObject.setupOrm ();
        addManagedClass (IpCommunityStdAccessListLocalManagedObject::getClassName (), this);
        addOperationMap (IPPOLICYIPCOMMUNITYSTDACCESSLIST, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::IpCommunityAccessListLocalMessageHandler));
        IpCommunityExtAccessListLocalManagedObject IpCommunityExtAccessListLocalManagedObject (pIpPolicyLocalObjectManager);
        IpCommunityExtAccessListLocalManagedObject.setupOrm ();
        addManagedClass (IpCommunityExtAccessListLocalManagedObject::getClassName (), this);
        addOperationMap (IPPOLICYIPCOMMUNITYEXTACCESSLIST, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::IpCommunityAccessListLocalMessageHandler));
        FilterChangeUpdateLocalManagedObject FilterChangeUpdateLocalManagedObject (pIpPolicyLocalObjectManager);
        FilterChangeUpdateLocalManagedObject.setupOrm ();
        addManagedClass (FilterChangeUpdateLocalManagedObject::getClassName (), this);
        addOperationMap (IPPOLICYFILTERCHANGEUPDATEDELAY, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::FilterChangeUpdateLocalMessageHandler));
		addOperationMap (IPPOLICYPBRVRF, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::PbrVrfNexthopMessageHandler));
        addOperationMap (IPPOLICYGETPBR, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::IpPolicyGetRouteMapMessageHandler));
		addOperationMap (PBR_SHOW_ROUTEMAP, reinterpret_cast<PrismMessageHandler> (&IpPolicyLocalWorker::PBRShowMessageHandler));
    }

    IpPolicyLocalWorker::~IpPolicyLocalWorker ()
    {
    }

    PrismMessage  *IpPolicyLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case IPPOLICYROUTEMAP :
                pPrismMessage = new IpPolicyRouteMapMessage ();
                break;
            case IPPOLICYIPPREFIX :
                pPrismMessage = new IpPolicyIpPrefixMessage ();
                break;
            case IPPOLICYIPASPATHACCESSLIST :
                pPrismMessage = new IpPolicyAsPathAccessListLocalMessage ();
                break;
            case IPPOLICYIPCOMMUNITYSTDACCESSLIST :
                pPrismMessage = new IpCommunityAccessListLocalMessage (IPPOLICYIPCOMMUNITYSTDACCESSLIST);
                break;
            case IPPOLICYIPCOMMUNITYEXTACCESSLIST :
                pPrismMessage = new IpCommunityAccessListLocalMessage (IPPOLICYIPCOMMUNITYEXTACCESSLIST);
                break;
            case IPPOLICYFILTERCHANGEUPDATEDELAY :
                pPrismMessage = new FilterChangeUpdateLocalMessage ();
                break;
			case IPPOLICYPBRVRF :
				pPrismMessage = new PbrVrfNexthopMessage ();
				break;
            case IPPOLICYGETPBR :
                pPrismMessage = new IpPolicyGetPBRObjectMessage ();
                break;
            case PBR_SHOW_ROUTEMAP :
                pPrismMessage = new PBRShowMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IpPolicyLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((RouteMapLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RouteMapLocalManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((PbrVrfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PbrVrfManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((PbrNexthopAddrManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PbrNexthopAddrManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((PbrGlobalNexthopManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new PbrGlobalNexthopManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
		}
        else if ((IpPrefixLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpPrefixLocalManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IpAsPathAccessListLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpAsPathAccessListLocalManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IpCommunityStdAccessListLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpCommunityStdAccessListLocalManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((IpCommunityExtAccessListLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IpCommunityExtAccessListLocalManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((FilterChangeUpdateLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new FilterChangeUpdateLocalManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IpPolicyLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  IpPolicyLocalWorker::IpPolicyAsPathAccessListLocalMessageHandler( IpPolicyAsPathAccessListLocalMessage *pIpPolicyCreateAsPathMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::validateAsPathAccessList),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::forwardAsPathAccessList),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::writeAsPathAccessListMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIpPolicyCreateAsPathMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IpPolicyLocalWorker::validateAsPathAccessList(PrismSynchronousLinearSequencerContext *seq)
    {
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

    	IpPolicyAsPathAccessListLocalMessage *msg =
                static_cast<IpPolicyAsPathAccessListLocalMessage*> (seq->getPPrismMessage ());

        if (msg->opCode() == IPPOLICY_MO_CREATE) {
            //TODO validation here
        }
        else if (msg->opCode() == IPPOLICY_MO_SET) {
            //TODO validation here
        }

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::validateAsPathAccessList called, status= ") + status);

    	return status;
    }

    ResourceId IpPolicyLocalWorker::forwardAsPathAccessList(PrismSynchronousLinearSequencerContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<string> args;
        int loopCount = 1;
    	objectIds.clear();

        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
    	IpPolicyAsPathAccessListLocalMessage *msg =
                static_cast<IpPolicyAsPathAccessListLocalMessage*> (seq->getPPrismMessage ());
        int op_code = 0;

        IpAsPathAccessListLocalManagedObject *mo = 0;
        vector<WaveManagedObject *> *rlts = 0;

        if( msg->opCode() == IPPOLICY_MO_DEL )
        {
        	WaveManagedObjectSynchronousQueryContext
        	            ctx(IpAsPathAccessListLocalManagedObject::getClassName());
            ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));
            if(msg->getInstanceFlag()) {
            	ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
            } else {

            	if(msg->getActionFlag()) {
            		ctx.addAndAttribute(new AttributeEnum(((UI32)msg->getAction()), "action"));
            	}

            	if(msg->getRegExprFlag()) {
            		ctx.addAndAttribute(new AttributeString(msg->getRegExpr(), "regExpr"));
            	}
            }

            rlts= querySynchronouslyLocalManagedObjectsForLocationId(
            		FrameworkToolKit::getThisLocationId(), &ctx);

            if (rlts && rlts->size() > 0) {
                loopCount = rlts->size();
                mo =  dynamic_cast<IpAsPathAccessListLocalManagedObject*>(rlts->back());
                rlts->pop_back();
            }
            else
            {
                status = WRC_GENERIC_BACKEND_ERR;
                loopCount = 0;
            }
        }

        while((status == WAVE_MESSAGE_SUCCESS) && (loopCount--))
        {
            args.clear();
            args.push_back(msg->getName());

            if (msg->opCode() == IPPOLICY_MO_CREATE) {
            	if(msg->getInstance() == 0) {
            		args.push_back(toString(0));
            		op_code = SSM_CLIENT_ADD_ASPATHACESSLIST_WITHOUT_SEQ;
            	} else {
                    op_code = SSM_CLIENT_ADD_ASPATHACESSLIST;
                	args.push_back(toString(msg->getInstance()));
            	}
                args.push_back(toString(msg->getAction()));
                args.push_back(msg->getRegExpr());
            }
            else if (msg->opCode() == IPPOLICY_MO_DEL) {
                op_code = SSM_CLIENT_DEL_ASPATHACESSLIST;
                objectIds.push_back(mo->getObjectId());
            	args.push_back(toString(mo->getInstance()));
                args.push_back(toString(mo->getAction()));
                args.push_back(mo->getRegExpr());
            }
            else if (msg->opCode() == IPPOLICY_MO_SET) {
                // unsupported from wysereaconfiguration
                prismAssert(false, __FILE__, __LINE__);

            }
            else if (msg->opCode() == IPPOLICY_MO_UNSET) {
                // unsupported from wysereaconfiguration
                prismAssert(false, __FILE__, __LINE__);

            }

	        // To set the max args as 50, since the limitation of the GenericClientMessage_.
	        reSizeClientMsgArgs(args);

			for(size_t tmpIndx = 0; tmpIndx < args.size(); tmpIndx++) {
			  trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::forwardAsPathAccessList ARG VALUES ## ==> ") + args[tmpIndx]);
			}

			trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::forwardAsPathAccessList - opcode ==> ") + op_code);

            GenericClientMessage_ client_msg(args, op_code);
            status = sendSynchronouslyToWaveClient ("ssm", &client_msg);

            if(status == WAVE_MESSAGE_SUCCESS)
            {
                if ( (status = client_msg.getCompletionStatus()) == WAVE_MESSAGE_SUCCESS ) {
                    status = client_msg.getClientStatus();
                }

                UI32 size = 0;
                void* data = 0;
                client_msg.getDuplicateInterfaceOutputBuffer(0, size, data);
                generic_wyser_msg_reply_t* reply_msg = (generic_wyser_msg_reply_t*)data;

                if (reply_msg and reply_msg->num_records) {
                    size = 0, data = 0;
                    client_msg.getDuplicateInterfaceOutputBuffer(1, size, data);

                    if( ( op_code == SSM_CLIENT_ADD_ASPATHACESSLIST || op_code == SSM_CLIENT_ADD_ASPATHACESSLIST_WITHOUT_SEQ )
                    		&& ( !(msg->getInstance()) ) )
                    {
                        char *ptr = (char*)data;

                        trace(TRACE_LEVEL_INFO, string("as path back end response:") + string((char*)data) + "==END");

                        UI32 seqSize = 0;
                        if((ptr = strstr((char*)data, "Sequence number")))
                        {
                            ptr += 16;

                            while(*ptr++ != '\n')
                                seqSize++;

                            if(seqSize+16 == size-1)
                            {
                                *(--ptr) = '\0';
                                ptr -= seqSize;

                                msg->setInstance(atoi(ptr));
                            }
                            else
                            {
                                msg->set_backend_err(string("%%Error: ")+string((char*)++ptr, size-seqSize));
                                status = WRC_GENERIC_BACKEND_ERR;
                            }
                        }
                        else
                        {
                            msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                            status = WRC_GENERIC_BACKEND_ERR;
                        }
                    }
                    else
                    {
                        msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                        status = WRC_GENERIC_BACKEND_ERR;
                    }
                }
            }

            if( msg->opCode() == IPPOLICY_MO_DEL )
            {
                if (mo)
                {
                    seq->addManagedObjectForGarbageCollection(mo);
                    mo = 0;
                }

                if (rlts && rlts->size() > 0) {
                    mo =  dynamic_cast<IpAsPathAccessListLocalManagedObject*>(rlts->back());
                    rlts->pop_back();
                }
            }
        }//while


        if( msg->opCode() == IPPOLICY_MO_DEL )
        {
            if (rlts) {
                seq->addManagedObjectsForGarbageCollection(*rlts);
                delete rlts;
            }
            if (mo)
                seq->addManagedObjectForGarbageCollection(mo);
        }

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::forwardAsPathAccessList called, status= ") + status);

    	return status;
    }

    ResourceId IpPolicyLocalWorker::writeAsPathAccessListMO(PrismSynchronousLinearSequencerContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        IpAsPathAccessListLocalManagedObject *mo = 0;

        IpPolicyAsPathAccessListLocalMessage *msg =
                static_cast<IpPolicyAsPathAccessListLocalMessage*> (seq->getPPrismMessage ());

        vector<WaveManagedObject *> *rlts = 0;


        if(msg->opCode() != IPPOLICY_MO_DEL) {
            WaveManagedObjectSynchronousQueryContext
                ctx(IpAsPathAccessListLocalManagedObject::getClassName());
            ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));
        	ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
        	ctx.addSelectField("objectId");

            rlts = querySynchronouslyLocalManagedObjectsForLocationId(
            		FrameworkToolKit::getThisLocationId(), &ctx);

            if (rlts && rlts->size() > 0) {
                mo =  dynamic_cast<IpAsPathAccessListLocalManagedObject*>(rlts->back());
                rlts->pop_back();
            }
        }

        if (msg->opCode() == IPPOLICY_MO_CREATE) {
            if (mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("IP AS Path AccessList create MO ") + msg->getName()
                        + string(" already exists"));
            }
            else {
                mo = new IpAsPathAccessListLocalManagedObject(
                        static_cast<IpPolicyLocalObjectManager*>(getPWaveObjectManager()));
                mo->setName(msg->getName());
                mo->setInstance(msg->getInstance());
                mo->setAction(msg->getAction());
                mo->setRegExpr(msg->getRegExpr());
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_DEL) {

        	for(size_t idx = 0; idx < objectIds.size(); idx++) {
        		trace(TRACE_LEVEL_DEBUG, string("AS Path AccessList MO delete - objectdIds ==> ") + objectIds[idx].toString());
        	}

            if (objectIds.empty()) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("AS Path AccessList delete MO ") + msg->getName()
                        + string(" does not exist"));
            } else {
            	for(size_t index=0; index < objectIds.size(); index++)
            	{
            		if (objectIds[index] == ObjectId::NullObjectId) {
            			trace(TRACE_LEVEL_ERROR,"As Path AccessList id is null");
            		} else {
                		deleteWaveManagedObject(objectIds[index]);
            		}
            	}
            	objectIds.clear();
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_SET) {
            if (!mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("As Path AccessList set attributes for MO ") + msg->getName()
                        + string(" does not exist"));
            }
            else {
                updateWaveManagedObject(mo);
                mo->setAction(msg->getAction());
                mo->setRegExpr(msg->getRegExpr());
            }
        }
        else {
            trace(TRACE_LEVEL_ERROR, string("As Path AccessList opcode not found MO ") + msg->getName());
            status = WAVE_MESSAGE_ERROR;
        }

        if (rlts) {
            seq->addManagedObjectsForGarbageCollection(*rlts);
            delete rlts;
        }
        if (mo)
            seq->addManagedObjectForGarbageCollection(mo);

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::writeAsPathAccessListMO called, status= ") + status);

    	return status;
    }

    void IpPolicyLocalWorker::IpPolicyGetRouteMapMessageHandler(IpPolicyGetPBRObjectMessage *pIpPolicyGetPBRObjectMessage)
    {
        const string routeMapName           = pIpPolicyGetPBRObjectMessage->getPBRRouteMapName();
        RouteMapLocalManagedObject *routeMapMO  = NULL;
        int status                          = WAVE_MESSAGE_ERROR;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RouteMapLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString (routeMapName, "name"));
        vector<WaveManagedObject *> *pResults = querySynchronouslyLocalManagedObjectsForLocationId( FrameworkToolKit::getThisLocationId(), &syncQueryCtxt );

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                trace(TRACE_LEVEL_INFO, string("Found Existing RouteMap"));
                routeMapMO = dynamic_cast<RouteMapLocalManagedObject *>((*pResults)[0]);
                pIpPolicyGetPBRObjectMessage->setPBRRouteMapObjId(routeMapMO->getObjectId ());
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                trace(TRACE_LEVEL_INFO, string("Two or more object Exists"));
                status = WAVE_MESSAGE_ERROR;
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        } else {
            status = WAVE_MESSAGE_ERROR;
            trace(TRACE_LEVEL_INFO, string("Query Policy Object Failed or More than one RouteMapLocalManagedObject MO"));
        }

        pIpPolicyGetPBRObjectMessage->setCompletionStatus (status);
        reply(pIpPolicyGetPBRObjectMessage);

    }

    void  IpPolicyLocalWorker::PbrVrfNexthopMessageHandler(PbrVrfNexthopMessage *pPbrVrfNexthopMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::validatePbrVrfNexthop),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::forwardPbrVrfNexthop),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::writePbrVrfNexthopMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pPbrVrfNexthopMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IpPolicyLocalWorker::validatePbrVrfNexthop(PrismSynchronousLinearSequencerContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        PbrVrfNexthopMessage *msg =
                 static_cast<PbrVrfNexthopMessage *> (seq->getPPrismMessage ());

        RouteMapLocalManagedObject *pRouteMapLocalManagedObject = NULL;
        vector<WaveManagedObject *> *rltsParent = 0;
        size_t MOCount = 0;
        UI32 existingNextHops = 0;

        tracePrintf (TRACE_LEVEL_DEBUG, "Inside validatePbrVrfNexthop msg->getName() = %s", msg->getName().c_str());
        tracePrintf (TRACE_LEVEL_DEBUG, "Inside validatePbrVrfNexthop msg->getInstance() = %d", msg->getInstance());
        tracePrintf (TRACE_LEVEL_DEBUG, "Inside validatePbrVrfNexthop msg->opCode() = %d", msg->opCode());
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
                    RouteMapLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
		syncQueryCtxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
		vector<string> selectFields;
		syncQueryCtxt.addSelectFieldForObjectId("objectId");
		syncQueryCtxt.addSelectField("pbrVrfMO");
		syncQueryCtxt.addSelectField("pbrNexthopMO");
		syncQueryCtxt.addSelectField("pbrGlobalNexthopMO");

		rltsParent = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);
		MOCount = rltsParent->size();

		tracePrintf (TRACE_LEVEL_DEBUG, "Inside validatePbrVrfNexthop MOCount = %d", MOCount);
		if(MOCount == 1)
		{
			tracePrintf (TRACE_LEVEL_DEBUG, "Inside  MOCount == 1");
			pRouteMapLocalManagedObject = dynamic_cast<RouteMapLocalManagedObject *>((*rltsParent)[0]);

			if (pRouteMapLocalManagedObject == NULL) {
				trace(TRACE_LEVEL_INFO,
					string(__FUNCTION__) + string(": pRouteMapLocalManagedObject not found !!"));
				return WAVE_MESSAGE_ERROR;
			}

			existingNextHops = pRouteMapLocalManagedObject->getPbrNexthopConf().size() +
					pRouteMapLocalManagedObject->getPbrVrfConf().size() +
					pRouteMapLocalManagedObject->getPbrGlobalNexthopConf().size();

			if(msg->opCode() == PBR_NEXTHOP_MO_CREATE)
			{
				tracePrintf (TRACE_LEVEL_DEBUG, "INside PBR_NEXTHOP_MO_CREATE");
				tracePrintf (TRACE_LEVEL_DEBUG, "size() = %d", pRouteMapLocalManagedObject->getPbrNexthopConf().size());
				if (pRouteMapLocalManagedObject->getPbrNexthopConf().size() >= 3) {
					tracePrintf (TRACE_LEVEL_DEBUG, "INside PBR_NEXTHOP_MO_CREATE and greater than 3");
					return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MORE_THAN_3_NH;
				}

				if (existingNextHops >= 8) {
					return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MORE_THAN_8_NEXT_HOP;
				}

			}

			if(msg->opCode() == PBR_VRF_NEXTHOP_MO_CREATE)
			{
				tracePrintf (TRACE_LEVEL_DEBUG, "INside PBR_VRF_NEXTHOP_MO_CREATE");
				tracePrintf (TRACE_LEVEL_DEBUG, "size() = %d", pRouteMapLocalManagedObject->getPbrVrfConf().size());
				if (pRouteMapLocalManagedObject->getPbrVrfConf().size() >= 3) {
					tracePrintf (TRACE_LEVEL_DEBUG, "INside PBR_NEXTHOP_MO_CREATE and greater than 3");
					return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MORE_THAN_3_NH;
				}

				if (existingNextHops >= 8) {
					return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MORE_THAN_8_NEXT_HOP;
				}
			}

			if(msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_CREATE)
			{
				tracePrintf (TRACE_LEVEL_DEBUG, "INside PBR_GLOBAL_NEXTHOP_MO_CREATE");
				tracePrintf (TRACE_LEVEL_DEBUG, "size() = %d", pRouteMapLocalManagedObject->getPbrGlobalNexthopConf().size());
				if (pRouteMapLocalManagedObject->getPbrGlobalNexthopConf().size() >= 3) {
					tracePrintf (TRACE_LEVEL_DEBUG, "INside PBR_NEXTHOP_MO_CREATE and greater than 3");
					return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MORE_THAN_3_NH;
				}

				if (existingNextHops >= 8) {
					return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MORE_THAN_8_NEXT_HOP;
				}
			}

		}

        trace(TRACE_LEVEL_INFO,
                 string("IpPolicyLocalWorker::validatePbrVrfNexthop called, status= ") + status);

        return status;
    }

    ResourceId IpPolicyLocalWorker::forwardPbrVrfNexthop(PrismSynchronousLinearSequencerContext *seq)
    {
        trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::Entering forwardPbrVrfNexthop"));

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<string> args;

        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
        PbrVrfNexthopMessage *msg =
                static_cast<PbrVrfNexthopMessage *> (seq->getPPrismMessage ());

        int op_code = -1;

        args.push_back(msg->getName());
        args.push_back(toString(msg->getAction()));
        args.push_back(toString(msg->getInstance()));

        if((msg->opCode() == PBR_VRF_NEXTHOP_MO_CREATE) || (msg->opCode() == PBR_VRF_NEXTHOP_MO_DEL))
        {
			op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_VRF_NH;
			
            if (msg->opCode() == PBR_VRF_NEXTHOP_MO_CREATE)
            {
				args.push_back(msg->getVrf());
				args.push_back(msg->getSet_next_hop_address().toString());
            } 
			else if (msg->opCode() == PBR_VRF_NEXTHOP_MO_DEL)
			{
				args.push_back("no");
				args.push_back(msg->getVrf());
				args.push_back(msg->getSet_next_hop_address().toString());
			}
        }
		
		if((msg->opCode() == PBR_NEXTHOP_MO_CREATE) || (msg->opCode() == PBR_NEXTHOP_MO_DEL))
		{
			op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH;
			
			if (msg->opCode() == PBR_NEXTHOP_MO_CREATE)
            {				
				args.push_back(msg->getSet_next_hop_address().toString());
            } 
			else if (msg->opCode() == PBR_NEXTHOP_MO_DEL)
			{
				args.push_back("no");
				args.push_back(msg->getSet_next_hop_address().toString());
			}
		}
		
		if((msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_CREATE) || (msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_DEL))
		{
			op_code = SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_GLOBAL_NH;
			
			if (msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_CREATE)
            {				
				args.push_back(msg->getSet_next_hop_address().toString());
            } 
			else if (msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_DEL)
			{
				args.push_back("no");				
				args.push_back(msg->getSet_next_hop_address().toString());
			}
		}
		
        trace(TRACE_LEVEL_INFO, string("forwardPbrVrfNexthop - OPCODE: ") + op_code);

        for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
            trace(TRACE_LEVEL_INFO, string("forwardPbrVrfNexthop - ARG. VALUE: ") + args[tmpIdx]);
        }

        GenericClientMessage_ client_msg(args, op_code);
        status = sendSynchronouslyToWaveClient ("ssm", &client_msg);

        if(status == WAVE_MESSAGE_SUCCESS)
        {
            if ((status = client_msg.getCompletionStatus()) == WAVE_MESSAGE_SUCCESS)
            {
                status = client_msg.getClientStatus();
                trace(TRACE_LEVEL_DEBUG, string("forwardPbrVrfNexthop debug_1_ status- : ") + status);
            }

            if(status != WAVE_MESSAGE_SUCCESS)
            {
                trace(TRACE_LEVEL_DEBUG, string("forwardPbrVrfNexthop debug_2_ status- : ") + status);
                UI32 size = 0;
                void* data = 0;
                client_msg.getDuplicateInterfaceOutputBuffer(1, size, data);
                msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                status = WRC_GENERIC_BACKEND_ERR;
            }
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
        }

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::forwardPbrVrfNexthop called, status= ") + status);

		//status = WAVE_MESSAGE_SUCCESS;
        return status;
    }

    ResourceId IpPolicyLocalWorker::writePbrVrfNexthopMO(PrismSynchronousLinearSequencerContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        PbrVrfNexthopMessage *msg =
                static_cast<PbrVrfNexthopMessage *> (seq->getPPrismMessage ());
        if((msg->opCode() == PBR_NEXTHOP_MO_CREATE) || (msg->opCode() == PBR_NEXTHOP_MO_DEL))
            status = writePbrNexthopMO(seq);
        else if((msg->opCode() == PBR_VRF_NEXTHOP_MO_CREATE) || (msg->opCode() == PBR_VRF_NEXTHOP_MO_DEL))
            status = writePbrVrfMO(seq);
		else if((msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_CREATE) || (msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_DEL))
			status = writePbrGlobalNexthopMO(seq);
		else
			status = WAVE_MESSAGE_ERROR;
        return status;
    }

    ResourceId IpPolicyLocalWorker::writePbrNexthopMO(PrismSynchronousLinearSequencerContext *seq)
    {
        vector<WaveManagedObject *> *rltsParent = 0;
        UI32 MOCount = 0;

        PbrVrfNexthopMessage *msg =
                static_cast<PbrVrfNexthopMessage *> (seq->getPPrismMessage ());

        RouteMapLocalManagedObject *pRouteMapLocalManagedObject = NULL;
        PbrNexthopAddrManagedObject *mo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            RouteMapLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
        syncQueryCtxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
        vector<string> selectFields;
        syncQueryCtxt.addSelectFieldForObjectId("objectId");

    	rltsParent = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);
        MOCount = rltsParent->size();
		UI32 numberOfResults = 0;
        trace(TRACE_LEVEL_INFO, string("MOCount= ") + MOCount);

        if(MOCount == 1)
        {
            pRouteMapLocalManagedObject = dynamic_cast<RouteMapLocalManagedObject *>((*rltsParent)[0]);

            if (pRouteMapLocalManagedObject == NULL) {
                trace(TRACE_LEVEL_INFO,
                    string(__FUNCTION__) + string(": pRouteMapLocalManagedObject not found !!"));
                return WAVE_MESSAGE_ERROR;
            }

            if(msg->opCode() == PBR_NEXTHOP_MO_DEL)
            {
                trace(TRACE_LEVEL_INFO,
                  "IpPolicyLocalWorkers::MOStep: Deletion");

                WaveManagedObjectSynchronousQueryContext queryCtxtForChild(PbrNexthopAddrManagedObject::getClassName());
                queryCtxtForChild.addSelectFieldForObjectId("objectId");
                queryCtxtForChild.addSelectFieldForObjectId("ownerManagedObjectId");
                queryCtxtForChild.addAndAttribute (new AttributeIpV4Address (msg->getSet_next_hop_address(), "next_hop_address"));
    			queryCtxtForChild.addAndAttribute (new AttributeObjectId (pRouteMapLocalManagedObject->getObjectId(), "ownerManagedObjectId"));

    			vector<WaveManagedObject *> *pResultsChildMO = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &queryCtxtForChild);

                if (NULL != pResultsChildMO) {
                    numberOfResults = pResultsChildMO->size ();
                    if (0 == numberOfResults) {
                    } else if (1 == numberOfResults) {
                        PbrNexthopAddrManagedObject *pbrNexthopMO = dynamic_cast<PbrNexthopAddrManagedObject *>((*pResultsChildMO)[0]);
                        deleteFromComposition(RouteMapLocalManagedObject::getClassName(), PbrNexthopAddrManagedObject::getClassName(),
                            "pbrNexthopMO", pbrNexthopMO->getOwnerManagedObjectId(), pbrNexthopMO->getObjectId());
    				} else {
    	    			trace(TRACE_LEVEL_ERROR, string ("More than one PbrNexthopAddrManagedObject MO"));
                    }
                    seq->addManagedObjectsForGarbageCollection(*pResultsChildMO);
                    delete pResultsChildMO;
                }
            }
            if(msg->opCode() == PBR_NEXTHOP_MO_CREATE)
            {
                trace(TRACE_LEVEL_INFO,
                  "IpPolicyLocalWorkers::MOStep: Creation");
				UI32 highestIndex = 0;
				WaveManagedObjectSynchronousQueryContext synchronousQueryContext (PbrNexthopAddrManagedObject::getClassName ());
				synchronousQueryContext.addAndAttribute  (new AttributeUI32 ((UI32)0, "index"));
				synchronousQueryContext.addOrderField ("index", false);
				synchronousQueryContext.addLimitForQueryResults (1);

				tracePrintf(TRACE_LEVEL_INFO, "Before querySynchronously");				
				vector<WaveManagedObject *>   *pResultsNextHop = querySynchronously (&synchronousQueryContext);
				tracePrintf(TRACE_LEVEL_INFO, "After querySynchronously");

				prismAssert (NULL != pResultsNextHop, __FILE__, __LINE__);

				UI32 countNextHop = pResultsNextHop->size ();

				if (0 == countNextHop)
				{
				  	tracePrintf(TRACE_LEVEL_INFO, "countNextHop = 0");
					highestIndex = 0;
				} 
				else if (countNextHop > 0)				  	
				{
				  	tracePrintf(TRACE_LEVEL_INFO, "countNextHop > 0");
					PbrNexthopAddrManagedObject *lastMo = dynamic_cast<PbrNexthopAddrManagedObject *> ((*pResultsNextHop)[0]);
					if(NULL != lastMo)
					  	tracePrintf(TRACE_LEVEL_INFO, "NULL != lastMo");
					else
					  	tracePrintf(TRACE_LEVEL_INFO, "NULL ==  lastMo");
					prismAssert( NULL != lastMo, __FILE__, __LINE__);

					highestIndex = lastMo->getIndex();					
				}

                mo = new PbrNexthopAddrManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
                prismAssert (NULL != mo, __FILE__, __LINE__);
                mo->setSet_next_hop_address(msg->getSet_next_hop_address());
                mo->setOwnerManagedObjectId(pRouteMapLocalManagedObject->getObjectId());
				mo->setIndex(highestIndex+1);
                addToComposition(RouteMapLocalManagedObject::getClassName(), PbrNexthopAddrManagedObject::getClassName(),"pbrNexthopMO", pRouteMapLocalManagedObject->getObjectId(), mo->getObjectId());
                if(pResultsNextHop)
                {
                	seq->addManagedObjectsForGarbageCollection(*pResultsNextHop);
                    delete pResultsNextHop;
                }
//    			seq->addManagedObjectForGarbageCollection(mo);

    		}

			if (pRouteMapLocalManagedObject)
			  	seq->addManagedObjectForGarbageCollection(pRouteMapLocalManagedObject);

//    		seq->addManagedObjectsForGarbageCollection(*rltsParent);

//    		if (rltsParent) {
//    			delete rltsParent;
//    		}
    	}
    	else
    	{
    		trace(TRACE_LEVEL_ERROR, string("More than one RouteMapLocalManagedObject and count is ") + MOCount);
        }

        return WAVE_MESSAGE_SUCCESS;
    }


    ResourceId IpPolicyLocalWorker::writePbrVrfMO(PrismSynchronousLinearSequencerContext *seq)
    {
        vector<WaveManagedObject *> *rltsParent = 0;
		UI32 MOCount = 0;

        PbrVrfNexthopMessage *msg =
                static_cast<PbrVrfNexthopMessage *> (seq->getPPrismMessage ());

		RouteMapLocalManagedObject *pRouteMapLocalManagedObject = NULL;
		PbrVrfManagedObject *mo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            RouteMapLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
		syncQueryCtxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
		syncQueryCtxt.addSelectFieldForObjectId("objectId");

		rltsParent = querySynchronouslyLocalManagedObjectsForLocationId( FrameworkToolKit::getThisLocationId(), &syncQueryCtxt );
		MOCount = rltsParent->size();
		UI32 numberOfResults = 0;
		trace(TRACE_LEVEL_INFO, string("MOCount= ") + MOCount);

		if(MOCount == 1)
		{
			pRouteMapLocalManagedObject = dynamic_cast<RouteMapLocalManagedObject *>((*rltsParent)[0]);

		    if (pRouteMapLocalManagedObject == NULL) {
        		trace(TRACE_LEVEL_INFO,
              		string(__FUNCTION__) + string(": pRouteMapLocalManagedObject not found !!"));
		        return WAVE_MESSAGE_ERROR;
    		}
			
			if(msg->opCode() == PBR_VRF_NEXTHOP_MO_DEL)
			{
		        trace(TRACE_LEVEL_INFO,
        	      "IpPolicyLocalWorkers::MOStep: Deletion");
				WaveManagedObjectSynchronousQueryContext queryCtxtForChild(PbrVrfManagedObject::getClassName());
		        queryCtxtForChild.addSelectFieldForObjectId("objectId");
        		queryCtxtForChild.addSelectFieldForObjectId("ownerManagedObjectId");
				queryCtxtForChild.addAndAttribute (new AttributeString (msg->getVrf(), "vrf_name"));
				queryCtxtForChild.addAndAttribute (new AttributeIpV4Address (msg->getSet_next_hop_address(), "next_hop_address"));

				vector<WaveManagedObject *> *pResultsChildMO = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &queryCtxtForChild);

		        if (NULL != pResultsChildMO) {
    		        numberOfResults = pResultsChildMO->size ();
        		    if (0 == numberOfResults) {
		            } else if (1 == numberOfResults) {
    		            PbrVrfManagedObject *pbrVrfMO = dynamic_cast<PbrVrfManagedObject *>((*pResultsChildMO)[0]);
        		        deleteFromComposition(RouteMapLocalManagedObject::getClassName(), PbrVrfManagedObject::getClassName(),
            		        "pbrVrfMO", pbrVrfMO->getOwnerManagedObjectId(), pbrVrfMO->getObjectId());
            		}
		            seq->addManagedObjectsForGarbageCollection(*pResultsChildMO);
    		        delete pResultsChildMO;
        		}
			}
			if(msg->opCode() == PBR_VRF_NEXTHOP_MO_CREATE)
			{
                trace(TRACE_LEVEL_INFO,
                  "IpPolicyLocalWorkers::MOStep: Creation");

                UI32 highestIndex = 0;
                WaveManagedObjectSynchronousQueryContext synchronousQueryContext (PbrVrfManagedObject::getClassName ());
                synchronousQueryContext.addAndAttribute  (new AttributeUI32 ((UI32)0, "index"));
                synchronousQueryContext.addOrderField ("index", false);
                synchronousQueryContext.addLimitForQueryResults (1);

                vector<WaveManagedObject *>   *pResultsVrfNextHop = querySynchronously (&synchronousQueryContext);
                prismAssert (NULL != pResultsVrfNextHop, __FILE__, __LINE__);

                UI32 countVrfNextHop = pResultsVrfNextHop->size ();

                if (0 == countVrfNextHop)
                {
                    highestIndex = 0;
                }
                else if (countVrfNextHop > 0)
                {
                    PbrVrfManagedObject *lastMo = dynamic_cast<PbrVrfManagedObject *> ((*pResultsVrfNextHop)[0]);
                    prismAssert( NULL != lastMo, __FILE__, __LINE__);

                    highestIndex = lastMo->getIndex();
                }

				mo = new PbrVrfManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
				trace (TRACE_LEVEL_INFO, string("Debug_1"));
		        prismAssert (NULL != mo, __FILE__, __LINE__); 
				trace (TRACE_LEVEL_INFO, string("Debug_2"));
				trace (TRACE_LEVEL_INFO, string("msg->getVrf()= ") + msg->getVrf());
				mo->setVrf(msg->getVrf());
				mo->setSet_next_hop_address(msg->getSet_next_hop_address());
				mo->setOwnerManagedObjectId(pRouteMapLocalManagedObject->getObjectId());
				mo->setIndex(highestIndex+1);
		        addToComposition(RouteMapLocalManagedObject::getClassName(), PbrVrfManagedObject::getClassName(),"pbrVrfMO", pRouteMapLocalManagedObject->getObjectId(), mo->getObjectId());

				trace (TRACE_LEVEL_INFO, string("mo->getVrfNameMO()= ") + mo->getVrfNameMO());
				trace (TRACE_LEVEL_INFO, string("mo->getSet_next_hop_address()= ") + mo->getSet_next_hop_address().toString());
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsVrfNextHop);
			}
		}

        if ( NULL != rltsParent )
        {
    		seq->addManagedObjectsForGarbageCollection(*rltsParent);
	        delete rltsParent;
		}

		return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId IpPolicyLocalWorker::writePbrGlobalNexthopMO(PrismSynchronousLinearSequencerContext *seq)
    {
        vector<WaveManagedObject *> *rltsParent = 0;
        UI32 MOCount = 0;

        PbrVrfNexthopMessage *msg =
                static_cast<PbrVrfNexthopMessage *> (seq->getPPrismMessage ());

        RouteMapLocalManagedObject *pRouteMapLocalManagedObject = NULL;
        PbrGlobalNexthopManagedObject *mo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            RouteMapLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
        syncQueryCtxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
        vector<string> selectFields;
        syncQueryCtxt.addSelectFieldForObjectId("objectId");

        rltsParent = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &syncQueryCtxt);
        MOCount = rltsParent->size();
        UI32 numberOfResults = 0;
        trace(TRACE_LEVEL_INFO, string("MOCount= ") + MOCount);

        if(MOCount == 1)
        {
            pRouteMapLocalManagedObject = dynamic_cast<RouteMapLocalManagedObject *>((*rltsParent)[0]);

            if (pRouteMapLocalManagedObject == NULL) {
                trace(TRACE_LEVEL_INFO,
                    string(__FUNCTION__) + string(": pRouteMapLocalManagedObject not found !!"));
                return WAVE_MESSAGE_ERROR;
            }

            if(msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_DEL)
            {
                trace(TRACE_LEVEL_INFO,
                  "IpPolicyLocalWorkers::MOStep: Deletion");

                WaveManagedObjectSynchronousQueryContext queryCtxtForChild(PbrGlobalNexthopManagedObject::getClassName());
                queryCtxtForChild.addSelectFieldForObjectId("objectId");
                queryCtxtForChild.addSelectFieldForObjectId("ownerManagedObjectId");
                queryCtxtForChild.addAndAttribute (new AttributeIpV4Address (msg->getSet_next_hop_address(), "next_hop_address"));

                vector<WaveManagedObject *> *pResultsChildMO = querySynchronouslyLocalManagedObjectsForLocationId(FrameworkToolKit::getThisLocationId(), &queryCtxtForChild);

                if (NULL != pResultsChildMO) {
                    numberOfResults = pResultsChildMO->size ();
                    if (0 == numberOfResults) {
                    } else if (1 == numberOfResults) {
                        PbrGlobalNexthopManagedObject *pbrGlobalNexthopMO = dynamic_cast<PbrGlobalNexthopManagedObject *>((*pResultsChildMO)[0]);
                        deleteFromComposition(RouteMapLocalManagedObject::getClassName(), PbrGlobalNexthopManagedObject::getClassName(),
                            "pbrGlobalNexthopMO", pbrGlobalNexthopMO->getOwnerManagedObjectId(), pbrGlobalNexthopMO->getObjectId());
                    }
                    seq->addManagedObjectsForGarbageCollection(*pResultsChildMO);
                    delete pResultsChildMO;
                }
            }
            if(msg->opCode() == PBR_GLOBAL_NEXTHOP_MO_CREATE)
            {
                trace(TRACE_LEVEL_INFO,
                  "IpPolicyLocalWorkers::MOStep: Creation");

                UI32 highestIndex = 0;
                WaveManagedObjectSynchronousQueryContext synchronousQueryContext (PbrGlobalNexthopManagedObject::getClassName ());
                synchronousQueryContext.addAndAttribute  (new AttributeUI32 ((UI32)0, "index"));
                synchronousQueryContext.addOrderField ("index", false);
                synchronousQueryContext.addLimitForQueryResults (1);

                vector<WaveManagedObject *>   *pResultsGlobalNextHop = querySynchronously (&synchronousQueryContext);
                prismAssert (NULL != pResultsGlobalNextHop, __FILE__, __LINE__);

                UI32 countGlobalNextHop = pResultsGlobalNextHop->size ();

                if (0 == countGlobalNextHop)
                {
                    highestIndex = 0;
                }
                else if (countGlobalNextHop > 0)
                {
                    PbrNexthopAddrManagedObject *lastMo = dynamic_cast<PbrNexthopAddrManagedObject *> ((*pResultsGlobalNextHop)[0]);
                    prismAssert( NULL != lastMo, __FILE__, __LINE__);

                    highestIndex = lastMo->getIndex();
                }
                mo = new PbrGlobalNexthopManagedObject(dynamic_cast<IpPolicyLocalObjectManager *>(getPWaveObjectManager()));
                prismAssert (NULL != mo, __FILE__, __LINE__);
                mo->setSet_next_hop_address(msg->getSet_next_hop_address());
                mo->setOwnerManagedObjectId(pRouteMapLocalManagedObject->getObjectId());
				mo->setIndex(highestIndex+1);
                addToComposition(RouteMapLocalManagedObject::getClassName(), PbrGlobalNexthopManagedObject::getClassName(),"pbrGlobalNexthopMO", pRouteMapLocalManagedObject->getObjectId(), mo->getObjectId());
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultsGlobalNextHop);

            }
            if (pRouteMapLocalManagedObject)
                seq->addManagedObjectForGarbageCollection(pRouteMapLocalManagedObject);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    void  IpPolicyLocalWorker::FilterChangeUpdateLocalMessageHandler( FilterChangeUpdateLocalMessage *pFilterChangeUpdateLocalMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::validateFilterChangeUpdateDelay),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::forwardFilterChangeUpdateDelay),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::writeFilterChangeUpdateDelayMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pFilterChangeUpdateLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IpPolicyLocalWorker::validateFilterChangeUpdateDelay(PrismSynchronousLinearSequencerContext *seq)
    {
     	ResourceId status = WAVE_MESSAGE_SUCCESS;

     	FilterChangeUpdateLocalMessage *msg =
                 static_cast<FilterChangeUpdateLocalMessage*> (seq->getPPrismMessage ());

         if (msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_CREATE) {
             //TODO validation here
         }
         else if (msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_DEL) {
             //TODO validation here
         }
         else if (msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_CLEAR) {
             //TODO validation here
         }

         trace(TRACE_LEVEL_INFO,
                 string("IpPolicyLocalWorker::validateFilterChangeUpdateDelay called, status= ") + status);

     	return status;
    }

    ResourceId IpPolicyLocalWorker::forwardFilterChangeUpdateDelay(PrismSynchronousLinearSequencerContext *seq)
    {
    	trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::Entering forwardFilterChangeUpdateDelay"));

    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	vector<string> args;

    	// Check for Layer3 license before sending to backend
    	LAYER_3_LICENSE_CHECK();
    	FilterChangeUpdateLocalMessage *msg =
    			static_cast<FilterChangeUpdateLocalMessage*> (seq->getPPrismMessage ());

    	int op_code = -1;

    	if (msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_CREATE)
    	{
    		op_code = SSM_CLIENT_ADD_FILTERCHANGEUPDATEDELAY;
    		args.push_back(toString(msg->getFilterUpdateDelay()));
    	}
    	else if (msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_DEL)
    	{
    		op_code = SSM_CLIENT_DEL_FILTERCHANGEUPDATEDELAY;
    		args.push_back("no");
    	}
    	else if (msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_CLEAR) {
    		op_code = SSM_CLIENT_CLEAR_FILTERCHANGEUPDATEDELAY;
    		args.push_back("clear");
    	}

    	trace(TRACE_LEVEL_INFO, string("FILTERCHANGEUPDATE - OPCODE: ") +op_code);

    	for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
    		trace(TRACE_LEVEL_DEBUG, string("FILTERCHANGEUPDATE - ARG. VALUE: ") + args[tmpIdx]);
    	}

    	GenericClientMessage_ client_msg(args, op_code);
    	status = sendSynchronouslyToWaveClient ("ssm", &client_msg);

    	if(status == WAVE_MESSAGE_SUCCESS)
    	{
    		if ((status = client_msg.getCompletionStatus()) == WAVE_MESSAGE_SUCCESS)
    		{
    			status = client_msg.getClientStatus();
    		}

			if(status != WAVE_MESSAGE_SUCCESS)
			{
	    		UI32 size = 0;
	    		void* data = 0;
    			client_msg.getDuplicateInterfaceOutputBuffer(1, size, data);
				msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
				status = WRC_GENERIC_BACKEND_ERR;
			}
    	}
    	else
    	{
    		status = WAVE_MESSAGE_ERROR;
    	}

		trace(TRACE_LEVEL_INFO,
				string("IpPolicyLocalWorker::forwardFilterChangeUpdateDelay called, status= ") + status);

		return status;
    }

    ResourceId IpPolicyLocalWorker::writeFilterChangeUpdateDelayMO(PrismSynchronousLinearSequencerContext *seq)
    {
    	ResourceId status = WAVE_MESSAGE_SUCCESS;
    	FilterChangeUpdateLocalManagedObject *mo = 0;
    	vector<WaveManagedObject *> *rlts = 0;

    	FilterChangeUpdateLocalMessage *msg =
    			static_cast<FilterChangeUpdateLocalMessage*> (seq->getPPrismMessage ());

    	WaveManagedObjectSynchronousQueryContext ctx(FilterChangeUpdateLocalManagedObject::getClassName());

    	if( msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_DEL ||
    			msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_CREATE )
    	{
    		rlts = querySynchronouslyLocalManagedObjectsForLocationId(
    				FrameworkToolKit::getThisLocationId(), &ctx);

    		if(rlts)
    		{
    			for ( size_t idx=0; idx < rlts->size(); idx++ )
    			{
    				mo =  dynamic_cast<FilterChangeUpdateLocalManagedObject*>((*rlts)[idx]);
    				ObjectId objectId = mo->getObjectId();
    				if (objectId == ObjectId::NullObjectId) {
    					trace(TRACE_LEVEL_ERROR,"FilterChangeUpdateLocalManagedObject id is null");
    				} else {
    					//to delete the existing delay values
    					deleteWaveManagedObject(objectId);
    				}
    			}
    			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);
    		}
    	}

    	if(msg->opCode() == IPPOLICY_FILTERCHANGEUPDATE_CREATE)
    	{
    		mo = new FilterChangeUpdateLocalManagedObject(
    				static_cast<IpPolicyLocalObjectManager*>(getPWaveObjectManager()));
    		mo->setFilterUpdateDelay(msg->getFilterUpdateDelay());
    		if (mo) {
    			seq->addManagedObjectForGarbageCollection(mo);
    		}
    	}

    	trace(TRACE_LEVEL_INFO,
    			string("IpPolicyLocalWorker::writeFilterChangeUpdateDelayMO called, status= ") + status);

    	return status;
    }

    void  IpPolicyLocalWorker::IpCommunityAccessListLocalMessageHandler( IpCommunityAccessListLocalMessage *pIpPolicyCreateCommunityMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::validateIpCommunityAccessList),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::forwardIpCommunityAccessList),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::writeIpCommunityAccessListMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIpPolicyCreateCommunityMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IpPolicyLocalWorker::validateIpCommunityAccessList(PrismSynchronousLinearSequencerContext *seq)
    {
    	ResourceId status = WAVE_MESSAGE_SUCCESS;

    	IpCommunityAccessListLocalMessage *msg =
    			static_cast<IpCommunityAccessListLocalMessage*> (seq->getPPrismMessage ());


    	if ( msg->opCode() == IPPOLICY_MO_CREATE && msg->getInstance() ) {

    		// check the same mo exist in other community type

    		string moClassName = "";
    		UI32 count = 0;

    		if( msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE )
    		{
    			moClassName = IpCommunityStdAccessListLocalManagedObject::getClassName();
    		}
    		else if( msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE )
    		{
    			moClassName = IpCommunityExtAccessListLocalManagedObject::getClassName();
    		}

    		WaveManagedObjectSynchronousQueryContext ctx(moClassName);
    		ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));
    		ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
    		ctx.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
    		status = querySynchronouslyForCount(&ctx, count);

    		if ( WAVE_MESSAGE_SUCCESS == status )
    		{
    			if ( 0 != count )
    			{
    	    		if( msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE )
    	    		{
    	    			return WRC_IPPOLICY_DCM_ERR_IPCOMMUNITYACCESSLIST_ALREADY_EXISTS_STD;
    	    		}
    	    		else if( msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE )
    	    		{
    	    			return WRC_IPPOLICY_DCM_ERR_IPCOMMUNITYACCESSLIST_ALREADY_EXISTS_EXTD;
    	    		}
    			}
    		}
    	}
    	else if (msg->opCode() == IPPOLICY_MO_SET) {
    		//TODO validation here
    	}

    	trace(TRACE_LEVEL_INFO,
    			string("IpPolicyLocalWorker::validateCommunityAccessList called, status= ") + status);

    	return status;
    }

     ResourceId IpPolicyLocalWorker::forwardIpCommunityAccessList(PrismSynchronousLinearSequencerContext *seq)
     {
         trace(TRACE_LEVEL_INFO, string("IpPolicyLocalWorker::Entering forwardIpCommunityAccessList"));

         ResourceId status = WAVE_MESSAGE_SUCCESS;
         vector<string> args;
         int loopCount = 1;
     	 objectIds.clear();

         // Check for Layer3 license before sending to backend
         LAYER_3_LICENSE_CHECK();
     	 IpCommunityAccessListLocalMessage *msg =
                 static_cast<IpCommunityAccessListLocalMessage*> (seq->getPPrismMessage ());
         int op_code = 0;

         IpCommunityStdAccessListLocalManagedObject *moStd = 0;
         IpCommunityExtAccessListLocalManagedObject *moExt = 0;
         vector<WaveManagedObject *> *rlts = 0;

         if( msg->opCode() == IPPOLICY_MO_DEL )
         {
			string moClassName = "";
			if(msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE) {
				moClassName = IpCommunityStdAccessListLocalManagedObject::getClassName();
			} else if (msg->getIp_community_acl_type() ==  IPCOMMUNITY_EXT_ACL_TYPE) {
				moClassName = IpCommunityExtAccessListLocalManagedObject::getClassName();
			}

			WaveManagedObjectSynchronousQueryContext ctx(moClassName);
			ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));

			if( msg->getInstanceFlag() && msg->getInstance() > 0 )
			{
				ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
			}
			else
			{
				if(msg->getActionFlag())
				{
					ctx.addAndAttribute(new AttributeEnum(msg->getAction(), "action"));
				}

				if( msg->getCommunityExpr().length() > 0 )
				{
					vector<string> communityTmpList;
					bool local_as=false;
					bool no_export=false;
					bool internet=false;
					bool no_advertise = false;
					set<string> communityList;
					stringstream tmpCommStr;

					tokenize(msg->getCommunityExpr(),communityTmpList,' ');

					for(size_t indexTmp=0; indexTmp < communityTmpList.size(); indexTmp++)
					{
						if ((strncmp(communityTmpList[indexTmp].c_str(), "local-as", strlen("local-as"))== 0)) {
							local_as = true;
						} else if ((strncmp(communityTmpList[indexTmp].c_str(), "internet", strlen("internet"))== 0)) {
							internet = true;
						} else if ((strncmp(communityTmpList[indexTmp].c_str(), "no-export", strlen("no-export"))== 0)) {
							no_export = true;
						} else if ((strncmp(communityTmpList[indexTmp].c_str(), "no-advertise", strlen("no-advertise"))== 0)) {
							no_advertise = true;
						} else {
							string strDecimal = communityTmpList[indexTmp];
							size_t found = strDecimal.find(":");
							if (found == string::npos) {
								unsigned long decimalNum = strtoul(strDecimal.c_str(), NULL, 0);
								stringstream decimlaAANN;
								if(decimalNum > 65535) {
									int quotVal = (decimalNum / 65536);
									int remVal = (decimalNum % 65536);
									decimlaAANN << quotVal;
									decimlaAANN << ":";
									decimlaAANN << remVal;
								} else {
									decimlaAANN << "0";
									decimlaAANN << ":";
									decimlaAANN << strDecimal;
								}
								communityList.insert(decimlaAANN.str());
							} else {
								communityList.insert(communityTmpList[indexTmp]);
							}
						}
					}

					if(!communityList.empty()) {
						stringstream sizeStr;
						size_t argSize = communityList.size();
						if( argSize > GENERIC_WYSEREA_ARGS_MAX ) {
							argSize = GENERIC_WYSEREA_ARGS_MAX;
						}
						sizeStr << (argSize);
						set<string>::iterator it;
						for (it=communityList.begin(); it!=communityList.end(); it++) {
							tmpCommStr << *it;
							tmpCommStr << " ";
						}
					}

					if(local_as) {
						tmpCommStr << "local-as";
						tmpCommStr << " ";
					}

					if(internet) {
						tmpCommStr << "internet";
						tmpCommStr << " ";
					}

					if(no_export) {
						tmpCommStr << "no-export";
						tmpCommStr << " ";
					}

					if(no_advertise) {
						tmpCommStr << "no-advertise";
					}
					StringUC communityStringData(tmpCommStr.str(), true);
					ctx.addAndAttribute(new AttributeStringUC(communityStringData,false, "communityExpr"));
				}

				if( msg->getCommunityRegExpr().length() > 0 )
				{
					StringUC communityRegExprStringData(msg->getCommunityRegExpr(), true);
					ctx.addAndAttribute(new AttributeStringUC(communityRegExprStringData,false, "regExpr"));
				}
			}

            rlts = querySynchronouslyLocalManagedObjectsForLocationId(
            		FrameworkToolKit::getThisLocationId(), &ctx);

			if (rlts && rlts->size() > 0)
			{
				loopCount = rlts->size();
				if(msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE) {
					moStd =  dynamic_cast<IpCommunityStdAccessListLocalManagedObject*>(rlts->back());
				} else if (msg->getIp_community_acl_type() ==  IPCOMMUNITY_EXT_ACL_TYPE) {
					moExt =  dynamic_cast<IpCommunityExtAccessListLocalManagedObject*>(rlts->back());
				}
				rlts->pop_back();
			}
			else
			{
				status = WRC_GENERIC_BACKEND_ERR;
				loopCount = 0;
			}
         }

         while((status == WAVE_MESSAGE_SUCCESS) && (loopCount--))
         {
             args.clear();
             args.push_back(msg->getName());

             if (msg->opCode() == IPPOLICY_MO_CREATE && msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE)
             {
            	 if(msg->getUserEnteredSeqid()){
            		 op_code = SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST;
            		 args.push_back(toString(msg->getInstance()));
            	 } else {
            		 op_code = SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST_WITHOUT_SEQ;
            		 args.push_back(toString(0));
            	 }
            	 args.push_back(toString(msg->getAction()));

            	 if(!(msg->getCommunityExpr().empty()))
            	 {
            		 vector<string> communityTmpList;
            		 bool local_as=false;
            		 bool no_export=false;
            		 bool internet=false;
            		 bool no_advertise = false;
            		 set<string> communityList;
            		 stringstream tmpCommStr;

            		 tokenize(msg->getCommunityExpr(),communityTmpList,' ');

            		 for(size_t indexTmp=0; indexTmp < communityTmpList.size(); indexTmp++)
            		 {
            			 if ((strncmp(communityTmpList[indexTmp].c_str(), "local-as", strlen("local-as"))== 0)) {
            				 local_as = true;
            			 } else if ((strncmp(communityTmpList[indexTmp].c_str(), "internet", strlen("internet"))== 0)) {
            				 internet = true;
            			 } else if ((strncmp(communityTmpList[indexTmp].c_str(), "no-export", strlen("no-export"))== 0)) {
            				 no_export = true;
            			 } else if ((strncmp(communityTmpList[indexTmp].c_str(), "no-advertise", strlen("no-advertise"))== 0)) {
            				 no_advertise = true;
            			 } else {
            				 string strDecimal = communityTmpList[indexTmp];
            				 size_t found = strDecimal.find(":");
            				 if (found == string::npos) {
            					 unsigned long decimalNum = strtoul(strDecimal.c_str(), NULL, 0);
            					 stringstream decimlaAANN;
            					 if(decimalNum > 65535) {
            						 int quotVal = (decimalNum / 65536);
            						 int remVal = (decimalNum % 65536);
            						 decimlaAANN << quotVal;
            						 decimlaAANN << ":";
            						 decimlaAANN << remVal;
            					 } else {
            						 decimlaAANN << "0";
            						 decimlaAANN << ":";
            						 decimlaAANN << strDecimal;
            					 }
            					 communityList.insert(decimlaAANN.str());
            				 } else {
            					 communityList.insert(communityTmpList[indexTmp]);
            				 }
            			 }
            		 }

            		 if(!communityList.empty()) {
            			 stringstream sizeStr;
         				 size_t argSize = communityList.size();
         				 if( argSize > GENERIC_WYSEREA_ARGS_MAX ) {
         					argSize = GENERIC_WYSEREA_ARGS_MAX;
         				 }
            			 sizeStr << (argSize);
            			 args.push_back(sizeStr.str());
            			 set<string>::iterator it;
            			 for (it=communityList.begin(); it!=communityList.end(); it++) {
            				 args.push_back(*it);
            				 tmpCommStr << *it;
            				 tmpCommStr << " ";
            			 }
            		 } else {
            			 args.push_back("0");
            		 }

            		 if(local_as) {
            			 args.push_back("local-as");
            			 tmpCommStr << "local-as";
            			 tmpCommStr << " ";
            		 }

            		 if(internet) {
            			 args.push_back("internet");
            			 tmpCommStr << "internet";
            			 tmpCommStr << " ";
            		 }

            		 if(no_export) {
            			 args.push_back("no-export");
            			 tmpCommStr << "no-export";
            			 tmpCommStr << " ";
            		 }

            		 if(no_advertise) {
            			 args.push_back("no-advertise");
            			 tmpCommStr << "no-advertise";
            		 }

            		 msg->setCommunityExpr(tmpCommStr.str());

            	 } else {
            		 args.push_back("0");
            	 }
             }
             else if (msg->opCode() == IPPOLICY_MO_CREATE && msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE) {
                 op_code = SSM_CLIENT_ADD_IPCOMMUNITYEXTACESSLIST;
             	 args.push_back(toString(msg->getInstance()));
                 args.push_back(toString(msg->getAction()));
                 args.push_back(msg->getCommunityRegExpr());
             }
             else if (msg->opCode() == IPPOLICY_MO_DEL) {
            	 if( msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE ){
                     op_code = SSM_CLIENT_DEL_IPCOMMUNITYSTDACESSLIST;
                     objectIds.push_back(moStd->getObjectId());
                 	 args.push_back(toString(moStd->getInstance()));
                     args.push_back(toString(moStd->getAction()));
            	 } else if( msg->getIp_community_acl_type() ==  IPCOMMUNITY_EXT_ACL_TYPE){
                     op_code = SSM_CLIENT_DEL_IPCOMMUNITYEXTACESSLIST;
                     objectIds.push_back(moExt->getObjectId());
                 	 args.push_back(toString(moExt->getInstance()));
                     args.push_back(toString(moExt->getAction()));
            	 }
             }
             else if (msg->opCode() == IPPOLICY_MO_SET) {
                 // unsupported from wysereaconfiguration
                 prismAssert(false, __FILE__, __LINE__);

             }
             else if (msg->opCode() == IPPOLICY_MO_UNSET) {
                 // unsupported from wysereaconfiguration
                 prismAssert(false, __FILE__, __LINE__);

             }

	        // To set the max args as 50, since the limitation of the GenericClientMessage_.
	        reSizeClientMsgArgs(args);

 			trace(TRACE_LEVEL_INFO, string("IP COMMUNITY - OPCODE: ") +op_code);

 	        for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
 	        	trace(TRACE_LEVEL_INFO, string("IP COMMUNITY - ARG. VALUE: ") + args[tmpIdx]);
 	        }

             GenericClientMessage_ client_msg(args, op_code);
             status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
             if(status == WAVE_MESSAGE_SUCCESS) {
                 if ((status = client_msg.getCompletionStatus())
                         == WAVE_MESSAGE_SUCCESS) {
                     status = client_msg.getClientStatus();
                 }

                 UI32 size = 0;
                 void* data = 0;
                 client_msg.getDuplicateInterfaceOutputBuffer(0, size, data);
                 generic_wyser_msg_reply_t* reply_msg = (generic_wyser_msg_reply_t*)data;

                 if (reply_msg and reply_msg->num_records) {
                     size = 0, data = 0;
                     client_msg.getDuplicateInterfaceOutputBuffer(1, size, data);

                     if((op_code == SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST ||
 	        				op_code == SSM_CLIENT_ADD_IPCOMMUNITYEXTACESSLIST || SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST_WITHOUT_SEQ ) && (!(msg->getInstance()) ))
                     {
                    	 trace(TRACE_LEVEL_INFO, string("IP COMMUNITY - back end response:") +string((char*)data)+"==END");

                         char *ptr = (char*)data;
                         UI32 seqSize = 0;
                         if((ptr = strstr((char*)data, "Sequence number")))
                         {
                             ptr += 16;

                             while(*ptr++ != '\n')
                                 seqSize++;

                             if(seqSize+16 == size-1)
                             {
                                 *(--ptr) = '\0';
                                 ptr -= seqSize;

                                 msg->setInstance(atoi(ptr));
                             }
                             else
                             {
                                 msg->set_backend_err(string("%%Error: ")+string((char*)++ptr, size-seqSize));
                                 status = WRC_GENERIC_BACKEND_ERR;
                             }
                         }
                         else
                         {
                             msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                             status = WRC_GENERIC_BACKEND_ERR;
                         }
                     }
                     else
                     {
                         msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                         status = WRC_GENERIC_BACKEND_ERR;
                     }
                 }
             }

             if( msg->opCode() == IPPOLICY_MO_DEL )
             {
                 if (moStd)
                 {
                     seq->addManagedObjectForGarbageCollection(moStd);
                     moStd = 0;
                 }
                 else if (moExt)
                 {
                     seq->addManagedObjectForGarbageCollection(moExt);
                     moExt = 0;
                 }

                 if (rlts && rlts->size() > 0) {
                	 if(msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE) {
                         moStd =  dynamic_cast<IpCommunityStdAccessListLocalManagedObject*>(rlts->back());
                	 }
                	 else if(msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE) {
                         moExt =  dynamic_cast<IpCommunityExtAccessListLocalManagedObject*>(rlts->back());
                	 }
                     rlts->pop_back();
                 }
             }
         }//while


         if( msg->opCode() == IPPOLICY_MO_DEL )
         {
             if (rlts) {
                 seq->addManagedObjectsForGarbageCollection(*rlts);
                 delete rlts;
             }

             if (moStd)
                 seq->addManagedObjectForGarbageCollection(moStd);

             if (moExt)
                 seq->addManagedObjectForGarbageCollection(moExt);
         }

         trace(TRACE_LEVEL_INFO,
                 string("IpPolicyLocalWorker::forwardIpCommunityAccessList called, status= ") + status);

     	return status;
     }

     ResourceId IpPolicyLocalWorker::writeIpCommunityAccessListMO(PrismSynchronousLinearSequencerContext *seq)
     {
         ResourceId status = WAVE_MESSAGE_SUCCESS;
         IpCommunityStdAccessListLocalManagedObject *moStd = 0;
         IpCommunityExtAccessListLocalManagedObject *moExt = 0;

         IpCommunityAccessListLocalMessage *msg =
                 static_cast<IpCommunityAccessListLocalMessage*> (seq->getPPrismMessage ());

         vector<WaveManagedObject *> *rlts = 0;

         if(msg->opCode() != IPPOLICY_MO_DEL) {
        	 string moClassName = "";
        	 if(msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE) {
        		 moClassName = IpCommunityStdAccessListLocalManagedObject::getClassName();
        	 } else if(msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE) {
        		 moClassName = IpCommunityExtAccessListLocalManagedObject::getClassName();
        	 }

             WaveManagedObjectSynchronousQueryContext ctx(moClassName);
			 ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));
			 ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
			 ctx.addSelectField("objectId");

			 rlts= querySynchronouslyLocalManagedObjectsForLocationId(
					 FrameworkToolKit::getThisLocationId(), &ctx);

			 if (rlts && rlts->size() > 0) {
				 if(msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE) {
					 moStd =  dynamic_cast<IpCommunityStdAccessListLocalManagedObject*>(rlts->back());
				 } else if(msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE) {
					 moExt =  dynamic_cast<IpCommunityExtAccessListLocalManagedObject*>(rlts->back());
				 }
				 rlts->pop_back();
			 }
         }

         if (msg->opCode() == IPPOLICY_MO_CREATE) {
             if (moStd || moExt) {
                 status = WAVE_MESSAGE_ERROR;
                 trace(TRACE_LEVEL_ERROR, string("IP Community AccessList create MO ") + msg->getName()
                         + string(" already exists"));
             }
             else {
            	 if(msg->getIp_community_acl_type() == IPCOMMUNITY_STD_ACL_TYPE) {
                     moStd = new IpCommunityStdAccessListLocalManagedObject(
                             static_cast<IpPolicyLocalObjectManager*>(getPWaveObjectManager()));
                     moStd->setName(msg->getName());
                     moStd->setInstance(msg->getInstance());
                     moStd->setAction(msg->getAction());
                     if(!msg->getCommunityExpr().empty())
                     {
                         moStd->setCommunityExpr(msg->getCommunityExpr());
                     }
            	 } else if(msg->getIp_community_acl_type() == IPCOMMUNITY_EXT_ACL_TYPE) {
                     moExt = new IpCommunityExtAccessListLocalManagedObject(
                             static_cast<IpPolicyLocalObjectManager*>(getPWaveObjectManager()));
                     moExt->setName(msg->getName());
                     moExt->setInstance(msg->getInstance());
                     moExt->setAction(msg->getAction());
                     moExt->setCommunityRegExpr(msg->getCommunityRegExpr());
            	 }
             }
         }
         else if (msg->opCode() == IPPOLICY_MO_DEL) {

         	for(size_t idx = 0; idx < objectIds.size(); idx++) {
         		trace(TRACE_LEVEL_DEBUG, string("Ip Community AccessList MO delete - objectdIds ==> ") + objectIds[idx].toString());
         	}

             if (objectIds.empty()) {
                 status = WAVE_MESSAGE_ERROR;
                 trace(TRACE_LEVEL_ERROR, string("Ip Community AccessList delete MO ") + msg->getName()
                         + string(" does not exist"));
             } else {
             	for(size_t index=0; index < objectIds.size(); index++)
             	{
             		if (objectIds[index] == ObjectId::NullObjectId) {
             			trace(TRACE_LEVEL_ERROR,"Ip Community AccessList id is null");
             		} else {
                 		deleteWaveManagedObject(objectIds[index]);
             		}
             	}
             	objectIds.clear();
             }
         }
         else if (msg->opCode() == IPPOLICY_MO_SET) {
             if (!moStd || !moExt) {
                 status = WAVE_MESSAGE_ERROR;
                 trace(TRACE_LEVEL_ERROR, string("Ip Community AccessList set attributes for MO ") + msg->getName()
                         + string(" does not exist"));
             }
             else {
            	 if(moStd){
            		 updateWaveManagedObject(moStd);
            		 moStd->setAction(msg->getAction());
            	 }
            	 else if(moExt){
            		 updateWaveManagedObject(moExt);
            		 moExt->setAction(msg->getAction());
            	 }
             }
         }
         else {
             trace(TRACE_LEVEL_ERROR, string("Ip Community AccessList opcode not found MO ") + msg->getName());
             status = WAVE_MESSAGE_ERROR;
         }

         if (rlts) {
             seq->addManagedObjectsForGarbageCollection(*rlts);
             delete rlts;
         }

         if (moStd)
             seq->addManagedObjectForGarbageCollection(moStd);
         else if (moExt)
             seq->addManagedObjectForGarbageCollection(moExt);

         trace(TRACE_LEVEL_INFO,
                 string("IpPolicyLocalWorker::writeCommunityAccessListMO called, status= ") + status);

     	return status;
     }

    void  IpPolicyLocalWorker::IpPolicyIpPrefixMessageHandler( IpPolicyIpPrefixMessage *pIpPolicyCreateIpPrefixMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::validateIpPrefix),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::forwardIpPrefix),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::writeIpPrefixMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIpPolicyCreateIpPrefixMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  IpPolicyLocalWorker::IpPolicyRouteMapMessageHandler( IpPolicyRouteMapMessage *pIpPolicyCreateRouteMapMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::validateRouteMap),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::forwardRouteMap),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::writeRouteMapMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pIpPolicyCreateRouteMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId
    IpPolicyLocalWorker::validateRouteMap(DceSynchronousLinearSeqContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS; // WAVE_MESSAGE_ERROR
        IpPolicyRouteMapMessage *msg =
                static_cast<IpPolicyRouteMapMessage*> (seq->getPPrismMessage ());
        tracePrintf(TRACE_LEVEL_DEBUG, "Debo msg->opCode() = %d", msg->opCode());

		if( msg->getMatch_addr_aclFlag() && !Capabilities::isCapabilityPresent(ROUTE_MAP_MATCH_ACL))
		{
				WaveNs::trace(TRACE_LEVEL_INFO, "Applying ACL match criteria on Route-Map is not supported on this platform");
				return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_MATCH_ACL_PLATFORM_LT ;
		}

        if (msg->opCode() == IPPOLICY_MO_CREATE) {
    		WaveManagedObjectSynchronousQueryContext q_ctxt(RouteMapLocalManagedObject::getClassName());
    		q_ctxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
    		q_ctxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
            vector<WaveManagedObject *> *pResultsPMO = querySynchronouslyLocalManagedObjectsForLocationId(
            		FrameworkToolKit::getThisLocationId(), &q_ctxt);
            RouteMapLocalManagedObject* mo = 0;
    		if (NULL != pResultsPMO && pResultsPMO->size() == 1 )
    		{
    			mo = dynamic_cast<RouteMapLocalManagedObject*>((*pResultsPMO)[0]);
    			seq->setPWaveManagedObject(mo);
    		}
    		if (pResultsPMO) {
    			seq->addManagedObjectsForGarbageCollection(*pResultsPMO);
  				delete pResultsPMO;
    		}
        }
        else if (msg->opCode() == IPPOLICY_MO_SET) {
            if(msg->getSet_route_aspath_prependFlag() && (!msg->getSet_route_aspath_prepend().empty())) {
            	vector<string> tmpList;
            	parseCommunityList(msg->getSet_route_aspath_prepend(),tmpList);
            	if(tmpList.size() > 20) {
            		status = WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_IP_ASPATH_LIMIT;
            	}
            }

        } else if (msg->opCode() == IPPOLICY_MO_DEL) {
        	tracePrintf(TRACE_LEVEL_DEBUG, "Debo INside IPPOLICY_MO_DEL msg->getName() = %s", msg->getName().c_str());
        	tracePrintf(TRACE_LEVEL_DEBUG, "Debo INside IPPOLICY_MO_DEL msg->getInstance() = %d", msg->getInstance());
        	WaveManagedObjectSynchronousQueryContext q_ctxt(RouteMapLocalManagedObject::getClassName());
			q_ctxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
			if (msg->getInstance() > 0)
				q_ctxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
			q_ctxt.addSelectField("objectId");

			vector<WaveManagedObject *> *pResultsPMO = querySynchronouslyLocalManagedObjectsForLocationId(
							FrameworkToolKit::getThisLocationId(), &q_ctxt);

			 RouteMapLocalManagedObject* mo = 0;

			 if (pResultsPMO == NULL) {
				 tracePrintf(TRACE_LEVEL_DEBUG, "Debo INside IPPOLICY_MO_DEL, pResultsPMO is NULL");
			 } else {
				 tracePrintf(TRACE_LEVEL_DEBUG, "Debo INside IPPOLICY_MO_DEL, pResultsPMO is NOT NULL, size = %d", pResultsPMO->size());
			 }
			 if (NULL != pResultsPMO && pResultsPMO->size() == 1 && (msg->getInstance() == 0))
			{
				 tracePrintf(TRACE_LEVEL_DEBUG, "Debo INside IPPOLICY_MO_DEL, Inside if");
				mo = dynamic_cast<RouteMapLocalManagedObject*>(pResultsPMO->back());
                
                if (NULL == mo)
                {
                    // FIXME : Sagar : Please handle this scenario.
                }
                else
                {
				    //ObjectId objectId = mo->getObjectId();
				}

				UI32 routeMapCount = 0;
				WaveManagedObjectSynchronousQueryContext pSyncQueryCtx(NsmLocalPBRManagedObject::getClassName());
				pSyncQueryCtx.addAndAttribute(new AttributeString(msg->getName(), "routeMapName"));
				/////////////////////////pSyncQueryCtx.addAndAttribute(new AttributeObjectIdAssociation(objectId, "interfaceRouteMap",NsmLocalPBRManagedObject::getClassName()));
				//pSyncQueryCtx.setFilterByConnectedNodes(true);
				ResourceId statusCount = querySynchronouslyForCount(&pSyncQueryCtx, routeMapCount);
				tracePrintf(TRACE_LEVEL_DEBUG, "Debo routeMapCount = %d", routeMapCount);

				if ( WAVE_MESSAGE_SUCCESS == statusCount )
				{
					tracePrintf(TRACE_LEVEL_DEBUG, "Debo, Inside WAVE_MESSAGE_SUCCESS");
					if ( routeMapCount > 0 )
					{
						tracePrintf(TRACE_LEVEL_DEBUG, "Debo, Inside routeMapCount > 0");
						trace (TRACE_LEVEL_ERROR, msg->getName() + string (" ROUTEMAP applied on an interface."));
						return WRC_IPPOLICY_DCM_ERR_ROUTE_MAP_APPLIED_ON_INTERFACE;
					}
				}
				else
				{
					tracePrintf(TRACE_LEVEL_DEBUG, "Debo, Inside WAVE_MESSAGE_ERROR");
					trace (TRACE_LEVEL_DEBUG, msg->getName() + string (" ROUTEMAP applied not applied on any interface."));
				}

			}

        } else if(msg->opCode() == IPPOLICY_MO_UNSET) {

        	UI8 ifType = msg->getIfTypeToDeleteAllIntfs();

        	if(ifType != IF_TYPE_INVALID )
        	{
        		WaveManagedObjectSynchronousQueryContext q_ctxt(RouteMapLocalManagedObject::getClassName());
        		q_ctxt.addAndAttribute(new AttributeString(msg->getName(), "name"));
        		q_ctxt.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));

        		q_ctxt.addSelectField("objectId");

        		if(ifType == IF_TYPE_TE) {
        			q_ctxt.addSelectField("match_intf_eth");
				} else if(ifType == IF_TYPE_GI) {
					q_ctxt.addSelectField("match_intf_gi");
				} else if(ifType == IF_TYPE_FO) {
					q_ctxt.addSelectField("match_intf_fo");
				} else if(ifType == IF_TYPE_PO) {
					q_ctxt.addSelectField("match_intf_po");
				} else if(ifType == IF_TYPE_VL) {
					q_ctxt.addSelectField("match_intf_vlan");
				} else if(ifType == IF_TYPE_VE) {
					q_ctxt.addSelectField("match_intf_ve");
				} else if(ifType == IF_TYPE_INTF_LOOPBACK) {
					q_ctxt.addSelectField("match_intf_loopback");
				}

                vector<WaveManagedObject *> *pResultsPMO = querySynchronouslyLocalManagedObjectsForLocationId(
                		FrameworkToolKit::getThisLocationId(), &q_ctxt);

        		RouteMapLocalManagedObject* mo = 0;

        		if (NULL != pResultsPMO && pResultsPMO->size() == 1 )
        		{
        			mo = dynamic_cast<RouteMapLocalManagedObject*>(pResultsPMO->back());

        			seq->setPWaveManagedObject(mo);
        			pResultsPMO->pop_back();

        			if(ifType == IF_TYPE_TE) {
        				mo->addAttributeToBeUpdated("match_intf_eth");
        				vector<string> intfList(mo->getMatch_intf_eth());
        				msg->setMatch_intf_eth(intfList);
        			} else if(ifType == IF_TYPE_GI) {
        				mo->addAttributeToBeUpdated("match_intf_gi");
        				vector<string> intfList(mo->getMatch_intf_gi());
        				msg->setMatch_intf_gi(intfList);
        			} else if(ifType == IF_TYPE_FO) {
        				mo->addAttributeToBeUpdated("match_intf_fo");
        				vector<string> intfList(mo->getMatch_intf_fo());
        				msg->setMatch_intf_fo(intfList);
        			} else if(ifType == IF_TYPE_PO) {
        				mo->addAttributeToBeUpdated("match_intf_po");
        				vector<UI32> poVlanIntfList(mo->getMatch_intf_po());
        				msg->setMatch_intf_po(poVlanIntfList);
        			} else if(ifType == IF_TYPE_VL) {
        				mo->addAttributeToBeUpdated("match_intf_vlan");
        				vector<UI32> poVlanIntfList(mo->getMatch_intf_vlan());
        				msg->setMatch_intf_vlan(poVlanIntfList);
        			} else if(ifType == IF_TYPE_VE) {
        				mo->addAttributeToBeUpdated("match_intf_ve");
        				vector<UI32> veIntfList(mo->getMatch_intf_ve());
        				msg->setMatch_intf_ve(veIntfList);
    				} else if(ifType == IF_TYPE_INTF_LOOPBACK) {
    					mo->addAttributeToBeUpdated("match_intf_loopback");
        				vector<UI32> loopbackIntfList(mo->getMatch_intf_loopback());
        				msg->setMatch_intf_loopback(loopbackIntfList);
    				}
        		}

        		if (pResultsPMO) {
        			seq->addManagedObjectsForGarbageCollection(*pResultsPMO);
	  				delete pResultsPMO;
        		}
        	}
        }

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::validateRouteMap called, status= ") + status);
        return status;
    }

    ResourceId
    IpPolicyLocalWorker::forwardRouteMap(DceSynchronousLinearSeqContext *seq)
    {
        vector<string> args;
        ResourceId status = WAVE_MESSAGE_SUCCESS; // WAVE_MESSAGE_ERROR
        int loopCount = 1;
        int op_code = 0;
    	RouteMapLocalManagedObject *mo = 0;
    	vector<WaveManagedObject *> *rlts = NULL;

    	objectIds.clear();

        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

        IpPolicyRouteMapMessage *msg =
                static_cast<IpPolicyRouteMapMessage*> (seq->getPPrismMessage ());

        if(msg->opCode() == IPPOLICY_MO_UNSET &&
        		msg->getIfTypeToDeleteAllIntfs() != IF_TYPE_INVALID)
        {
    		mo = dynamic_cast<RouteMapLocalManagedObject*>(seq->getPWaveManagedObject());
    	}
        else if ( msg->opCode() != IPPOLICY_MO_CREATE )
        {
			WaveManagedObjectSynchronousQueryContext ctx(RouteMapLocalManagedObject::getClassName());
			ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));

			if( (msg->opCode() == IPPOLICY_MO_DEL) && ( !(msg->getInstanceFlag()) ) )
			{
				ctx.addSelectField("action");
				ctx.addSelectField("instance");
				ctx.addSelectField("objectId");
			}

			if(msg->getInstanceFlag())
			{
				ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
			}

			if(msg->getActionFlag())
			{
				ctx.addAndAttribute(new AttributeEnum(msg->getAction(), "action"));
			}

			rlts = querySynchronouslyLocalManagedObjectsForLocationId(
					FrameworkToolKit::getThisLocationId(), &ctx);

			if (rlts && rlts->size() > 0)
			{
				loopCount = rlts->size();
				mo =  dynamic_cast<RouteMapLocalManagedObject*>(rlts->back());
				seq->setPWaveManagedObject(mo);
				rlts->pop_back();
			}
			else
			{
				status = WRC_GENERIC_BACKEND_ERR;
				loopCount = 0;
			}
    	}

        while((status == WAVE_MESSAGE_SUCCESS) && (loopCount--))
        {
            args.clear();
            args.push_back(msg->getName());

            if( (msg->opCode() == IPPOLICY_MO_DEL) && ( !(msg->getInstanceFlag()) ) )
            {
            	op_code = SSM_CLIENT_DEL_ROUTEMAP;
                args.push_back(toString(mo->getAction()));
                args.push_back(toString(mo->getInstance()));
            	objectIds.push_back(mo->getObjectId());
            }
            else
            {
                args.push_back(toString(msg->getAction()));
                args.push_back(toString(msg->getInstance()));

                if (msg->opCode() == IPPOLICY_MO_CREATE) {
                    op_code = SSM_CLIENT_ADD_ROUTEMAP;
                }
                else if (msg->opCode() == IPPOLICY_MO_DEL) {
                    op_code = SSM_CLIENT_DEL_ROUTEMAP;
                    objectIds.push_back(mo->getObjectId());
                }
                else if (msg->opCode() == IPPOLICY_MO_SET) {
                    if((status = parseRouteMap(msg, args, op_code, true, mo)) != WAVE_MESSAGE_SUCCESS)
                    {
                    	if(rlts) {
                    		seq->addManagedObjectsForGarbageCollection(*rlts);
                    		delete rlts;
                    	}

                        if (mo)
                        {
                            seq->addManagedObjectForGarbageCollection(mo);
                        }

                        return status;
                    }
                }
                else if (msg->opCode() == IPPOLICY_MO_UNSET) {
                    args.push_back("no");
                    parseRouteMap(msg, args, op_code, false, mo);
                    if ((status = updateRouteMapMessage(msg, args, op_code, mo)) != WAVE_MESSAGE_SUCCESS)
                    {
                    	if(rlts) {
                    		seq->addManagedObjectsForGarbageCollection(*rlts);
                    		delete rlts;
                    	}

                        if (mo)
                        {
                            seq->addManagedObjectForGarbageCollection(mo);
                        }

                        return status;
                    }
                }
            }

            // To set the max args as 50, since the limitation of the GenericClientMessage_.
            reSizeClientMsgArgs(args);

            trace(TRACE_LEVEL_INFO, string("RouteMap - OPCODE: ") +op_code);

            for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
            	trace(TRACE_LEVEL_INFO, string("RouteMap - ARG. VALUE: ") + args[tmpIdx]);
            }

            GenericClientMessage_ client_msg(args, op_code);
            status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
            if(status == WAVE_MESSAGE_SUCCESS) {
                if ((status = client_msg.getCompletionStatus())
                        == WAVE_MESSAGE_SUCCESS) {
                    status = client_msg.getClientStatus();
                }
            	UI32 size = 0;
            	void* data = 0;
            	client_msg.getDuplicateInterfaceOutputBuffer(0, size, data);
            	generic_wyser_msg_reply_t* reply_msg = (generic_wyser_msg_reply_t*)data;

            	if (reply_msg and reply_msg->num_records) {
            		size = 0, data = 0;
            		client_msg.getDuplicateInterfaceOutputBuffer(1, size, data);
            		msg->set_backend_err(string("%%")+string((char*)data, size));
            		status = WRC_GENERIC_BACKEND_ERR;
            	}
            }

            if( (msg->opCode() == IPPOLICY_MO_DEL))
            {
                if (mo)
                {
                    seq->addManagedObjectForGarbageCollection(mo);
                    mo = 0;
                }

                if ( (!msg->getInstanceFlag()) )
                {
                if (rlts && rlts->size() > 0) {
                    mo =  dynamic_cast<RouteMapLocalManagedObject*>(rlts->back());
                    rlts->pop_back();
                    }
                }
            }
        }

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::forwardRouteMap called, status= ") + status);

        if (mo)
        {
            seq->addManagedObjectForGarbageCollection(mo);
        }

        if(rlts) {
        	seq->addManagedObjectsForGarbageCollection(*rlts);
        	delete rlts;
        }
        return status;
    }

	void IpPolicyLocalWorker::deleteFromPBRMO(string routeMapName, DceSynchronousLinearSeqContext *seq) {
		ResourceId  status = WAVE_MESSAGE_ERROR;
		trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : routeMapName = ") + routeMapName);
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmLocalPBRManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&routeMapName,"routeMapName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		NsmLocalPBRManagedObject *nsmPBRMO = NULL;
		if (NULL != pResults) {
			trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_1 "));
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_2 "));
				status = WAVE_MESSAGE_ERROR;
			} else {
				trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_3 "));
				for(UI32 i=0; i < pResults->size(); i++) {
					trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_4 "));
					nsmPBRMO = NULL;
					nsmPBRMO = dynamic_cast<NsmLocalPBRManagedObject *> ((*pResults)[i]);
					//trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : nsmPBRMO->name = ") +
					//      nsmPBRMO->getRouteMapName());
					if(nsmPBRMO != NULL) {
						ObjectId intfManagedObjectObjectId = nsmPBRMO->getOwnerManagedObjectId();
						InterfaceType ifTYpe = NsmUtils::getIfType(intfManagedObjectObjectId);
						trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : ifTYpe= ") + ifTYpe);
						//trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : intfManagedObjectObjectId = ") +
						//      intfManagedObjectObjectId.toString());
						WaveManagedObjectSynchronousQueryContext syncQueryCtxtCHild(IntfManagedObject::getClassName());
						syncQueryCtxtCHild.addAndAttribute (new AttributeUI64 (intfManagedObjectObjectId.getInstanceId(),
								"objectidinstanceid"));
						vector<WaveManagedObject *> *pResultsCHild = querySynchronously (&syncQueryCtxtCHild);
						IntfManagedObject *intfMO = NULL;
						trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_5 "));
						if(NULL != pResultsCHild) {
							trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_6 "));
							intfMO = dynamic_cast<IntfManagedObject *> ((*pResultsCHild)[0]);
							if(intfMO != NULL) {
								trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_7 "));
								string ifName = intfMO->getIfName();
								trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : intfMO->getIfName() ") + intfMO->getIfName());
								if(ifTYpe == IF_TYPE_PO) {
									trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_9 "));
									IpPolicyDeleteGlobalPBRMessage *globalPBRDelete = new IpPolicyDeleteGlobalPBRMessage(ifName, ifTYpe);
									status = sendSynchronously (globalPBRDelete);
									trace(TRACE_LEVEL_INFO, string("After sendSynchronously() PO "));
								} else {
									trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_10 "));
									IpPolicyDeletePBRMessage *localPBRDelete = new IpPolicyDeletePBRMessage(ifName, ifTYpe);
									status = sendSynchronously (localPBRDelete);
									trace(TRACE_LEVEL_INFO, string("After sendSynchronously() VE/PHY "));
								}

							} else if (intfMO == NULL)
								trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : inside debug_8 "));
							//seq->addManagedObjectForGarbageCollection(intfMO);
							//delete pResultsCHild;
						}
						//seq->addManagedObjectForGarbageCollection(nsmPBRMO);
					} else
						trace(TRACE_LEVEL_INFO, string("Inside deleteFromPBRMO : nsmPBRMO is null "));
				}
			}
			//delete pResults;
		}
		trace(TRACE_LEVEL_INFO, string("Before coming out from deletePBRMO() "));

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            // FIXME : Sagar : Please handle this status otherwise please remove the variable.
        }
	}


    ResourceId
    IpPolicyLocalWorker::writeRouteMapMO(DceSynchronousLinearSeqContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        IpPolicyRouteMapMessage *msg =
                static_cast<IpPolicyRouteMapMessage*> (seq->getPPrismMessage ());
        RouteMapLocalManagedObject *mo = 0;

        mo = dynamic_cast<RouteMapLocalManagedObject*>(seq->getPWaveManagedObject());

        if (msg->opCode() == IPPOLICY_MO_CREATE) {
            if (mo) {
                trace(TRACE_LEVEL_INFO, string("Routemap create MO ") + msg->getName()
                        + string(" already exists with same name and instance"));
                mo->setAction(msg->getAction());
                updateWaveManagedObject(mo);
            }
            else {
                mo = new RouteMapLocalManagedObject(
                        static_cast<IpPolicyLocalObjectManager*>(getPWaveObjectManager()));
                mo->setName(msg->getName());
                mo->setInstance(msg->getInstance());
                mo->setAction(msg->getAction());
				trace(TRACE_LEVEL_INFO, string("Routemap create MO ") + msg->getName() + string("msg->getInstance() ") + msg->getInstance() + string("msg->getAction() ") + msg->getAction());
            	seq->addManagedObjectForGarbageCollection(mo);
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_DEL)
        {
            if (objectIds.empty())
            {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("Routemap delete MO ") + msg->getName()
                        + string(" does not exist"));
            }
            else
            {
        		UI32 count = 0;
        		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RouteMapLocalManagedObject::getClassName());
        		syncQueryCtxt.addAndAttribute(new AttributeString(msg->getName(),"name"));
        		querySynchronouslyForCount (&syncQueryCtxt, count);
        		tracePrintf(TRACE_LEVEL_DEBUG, "Count of stanzas Route Map has : %d ", count);

        		//here objectIds have entries corresponding to each stanza for a RM
        		//PBR should be removed if no other stanza exists for RM

        		if(objectIds.size() == count)
        		{
        			deleteFromPBRMO(msg->getName(), seq);
        			trace(TRACE_LEVEL_ERROR, string("After calling deleteFromPBRMO() "));
        		}

            	for(size_t index=0; index < objectIds.size(); index++)
            	{
            		if (objectIds[index] == ObjectId::NullObjectId)
            		{
                        trace(TRACE_LEVEL_ERROR, string("Policy id is null. Routemap delete MO ") + msg->getName()
                                + string(" does not exist"));
            		}
            		else
            		{
            			trace(TRACE_LEVEL_ERROR, string("Deleting a RM stanza"));
            			deleteWaveManagedObject(objectIds[index]);
            		}
            	}
            	objectIds.clear();

            }
        }
        else if (msg->opCode() == IPPOLICY_MO_SET) {
            if (!mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("Routemap set attributes for MO ") + msg->getName()
                        + string(" does not exist"));
            }
            else {
                updateWaveManagedObject(mo);
                SET_MO_ATTRIBUTE(Action);
                SET_MO_ATTRIBUTE(Match_intf_eth);
                SET_MO_ATTRIBUTE(Match_intf_gi);
                SET_MO_ATTRIBUTE(Match_intf_fo);
                SET_MO_ATTRIBUTE(Match_intf_vlan);
                SET_MO_ATTRIBUTE(Match_intf_po);
                SET_MO_ATTRIBUTE(Match_intf_ve);
                SET_MO_ATTRIBUTE(Match_intf_loopback);
                SET_MO_ATTRIBUTE(Match_addr_prefix);
                SET_MO_ATTRIBUTE(Match_nh_prefix);
                SET_MO_ATTRIBUTE(Match_addr_acl);
//                SET_MO_ATTRIBUTE(Match_precedence);
                SET_MO_ATTRIBUTE(Match_dscpPrecedence);
                SET_MO_ATTRIBUTE(Match_nullZero);
                SET_MO_ATTRIBUTE(Match_next_hop_address);
                SET_MO_ATTRIBUTE(Match_metric);
                SET_MO_ATTRIBUTE(Match_route_type);
                SET_MO_ATTRIBUTE(Match_route_tag);
                SET_MO_ATTRIBUTE(Match_rs_prefix);
                SET_MO_ATTRIBUTE(Match_route_aspath);
                SET_MO_ATTRIBUTE(Match_protocol_bgp);
                SET_MO_ATTRIBUTE(Match_bgp_route_type);
                SET_MO_ATTRIBUTE(Set_distance);
                SET_MO_ATTRIBUTE(Set_next_hop_peer_address);
				SET_MO_ATTRIBUTE(Set_route_metric_type_external);
				SET_MO_ATTRIBUTE(Set_route_metric_type_internal);
				SET_MO_ATTRIBUTE(Set_route_metric_type_type_1);
				SET_MO_ATTRIBUTE(Set_route_metric_type_type_2);
                SET_MO_ATTRIBUTE(Set_metric_action);
                SET_MO_ATTRIBUTE(Set_metric);
                SET_MO_ATTRIBUTE(Set_route_type);
                SET_MO_ATTRIBUTE(Set_route_tag);
                SET_MO_ATTRIBUTE(Set_route_local_preference);
                SET_MO_ATTRIBUTE(Set_route_weight);
                SET_MO_ATTRIBUTE(Set_route_aspath_prepend);
                SET_MO_ATTRIBUTE(Set_route_aspath_tag);
                SET_MO_ATTRIBUTE(Match_route_community_list);
                SET_MO_ATTRIBUTE(Set_route_origin_igp);
                SET_MO_ATTRIBUTE(Set_route_origin_incomplete);
                SET_MO_ATTRIBUTE(Set_route_automatic_tag);
                SET_MO_ATTRIBUTE(Set_route_dampening);
                SET_MO_ATTRIBUTE(Set_route_dampening_half_life);
                SET_MO_ATTRIBUTE(Set_route_dampening_reuse);
                SET_MO_ATTRIBUTE(Set_route_dampening_suppress);
                SET_MO_ATTRIBUTE(Set_route_dampening_max_suppress);
                SET_MO_ATTRIBUTE(Set_route_community_access_list);
                SET_MO_ATTRIBUTE(Set_route_community_expr);
                SET_MO_ATTRIBUTE(Set_route_extcommunity_expr);
                SET_MO_ATTRIBUTE(Set_route_extcommunity_soo_expr);

                if( msg->getRoute_continue() == 100 && msg->getRoute_continueFlag() == false) {
                	mo->setRoute_continue_no_val(true);
                } else {
                	if(msg->getRoute_continueFlag()) {
                		mo->setRoute_continue_no_val(true);
                		SET_MO_ATTRIBUTE(Route_continue);
                	}
                }
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_UNSET) {
            if (!mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("Routemap unset attributes for MO ") + msg->getName()
                        + string(" does not exist"));
            }
            else {
                updateWaveManagedObject(mo);
                //UNSET_MO_ATTRIBUTE(Match_intf_eth);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_eth);
                //UNSET_MO_ATTRIBUTE(Match_intf_gi);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_gi);
                //UNSET_MO_ATTRIBUTE(Match_intf_fo);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_fo);
                //UNSET_MO_ATTRIBUTE(Match_intf_vlan);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_vlan);
                //UNSET_MO_ATTRIBUTE(Match_intf_po);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_po);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_ve);
                UNSET_MO_ATTRIBUTE_SELECTIVE(Match_intf_loopback);
                UNSET_MO_ATTRIBUTE(Match_addr_prefix);
                UNSET_MO_ATTRIBUTE(Match_nh_prefix);
                UNSET_MO_ATTRIBUTE(Match_addr_acl);
//                UNSET_MO_ATTRIBUTE(Match_precedence);
                UNSET_MO_ATTRIBUTE(Match_dscpPrecedence);
                UNSET_MO_ATTRIBUTE(Match_nullZero);
                UNSET_MO_ATTRIBUTE(Match_next_hop_address);
                UNSET_MO_ATTRIBUTE(Match_metric);
                UNSET_MO_ATTRIBUTE(Match_route_type);
                UNSET_MO_ATTRIBUTE(Match_route_tag);
                UNSET_MO_ATTRIBUTE(Match_rs_prefix);
                UNSET_MO_ATTRIBUTE(Match_route_aspath);
                UNSET_MO_ATTRIBUTE(Match_protocol_bgp);
                UNSET_MO_ATTRIBUTE(Match_bgp_route_type);
                UNSET_MO_ATTRIBUTE(Set_distance);
                UNSET_MO_ATTRIBUTE(Set_next_hop_peer_address);
				UNSET_MO_ATTRIBUTE(Set_route_metric_type_external);
                UNSET_MO_ATTRIBUTE(Set_route_metric_type_internal);
                UNSET_MO_ATTRIBUTE(Set_route_metric_type_type_1);
                UNSET_MO_ATTRIBUTE(Set_route_metric_type_type_2);
                UNSET_MO_ATTRIBUTE(Set_metric_action);
                UNSET_MO_ATTRIBUTE(Set_metric);
                UNSET_MO_ATTRIBUTE(Set_route_type);
                UNSET_MO_ATTRIBUTE(Set_route_tag);
                UNSET_MO_ATTRIBUTE(Set_route_local_preference);
                UNSET_MO_ATTRIBUTE(Set_route_weight);
                UNSET_MO_ATTRIBUTE(Set_route_aspath_prepend);
                UNSET_MO_ATTRIBUTE(Set_route_aspath_tag);
                UNSET_MO_ATTRIBUTE(Match_route_community_list);
                UNSET_MO_ATTRIBUTE(Set_route_origin_igp);
                UNSET_MO_ATTRIBUTE(Set_route_origin_incomplete);
                UNSET_MO_ATTRIBUTE(Set_route_automatic_tag);
                UNSET_MO_ATTRIBUTE(Set_route_dampening);
                UNSET_MO_ATTRIBUTE(Set_route_dampening_half_life);
                UNSET_MO_ATTRIBUTE(Set_route_dampening_reuse);
                UNSET_MO_ATTRIBUTE(Set_route_dampening_suppress);
                UNSET_MO_ATTRIBUTE(Set_route_dampening_max_suppress);
                UNSET_MO_ATTRIBUTE(Set_route_community_access_list);
                if(mo->getRoute_continue_no_val()) {
                	mo->unsetRoute_continue_no_val();
                	mo->unsetRoute_continue();
                }
                UNSET_MO_ATTRIBUTE(Route_continue);
                UNSET_MO_ATTRIBUTE(Set_route_community_expr);
                UNSET_MO_ATTRIBUTE(Set_route_extcommunity_expr);
                UNSET_MO_ATTRIBUTE(Set_route_extcommunity_soo_expr);
            }
        }
        else {
            trace(TRACE_LEVEL_ERROR, string("Routemap opcode not found MO ") + msg->getName());
            status = WAVE_MESSAGE_ERROR;
        }

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::writeRouteMapMO called, status= ") + status);

        return status;
    }

    ResourceId
    IpPolicyLocalWorker::validateIpPrefix(PrismSynchronousLinearSequencerContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS; // WAVE_MESSAGE_ERROR
        IpPolicyIpPrefixMessage *msg =
                static_cast<IpPolicyIpPrefixMessage*> (seq->getPPrismMessage ());

        if (msg->opCode() == IPPOLICY_MO_CREATE) {
            //TODO validation here
        }
        else if (msg->opCode() == IPPOLICY_MO_SET) {
            //TODO validation here
        }


        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::validateIpPrefix called, status= ") + status);

        return status;
    }

    ResourceId
    IpPolicyLocalWorker::forwardIpPrefix(PrismSynchronousLinearSequencerContext *seq)
    {
        vector<string> args;
        ResourceId status = WAVE_MESSAGE_SUCCESS; // WAVE_MESSAGE_ERROR
        int loopCount = 1;
    	objectIds.clear();

        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
        IpPolicyIpPrefixMessage *msg =
                static_cast<IpPolicyIpPrefixMessage*> (seq->getPPrismMessage ());
        int op_code = 0;
        
        IpPrefixLocalManagedObject *mo = 0;
        WaveManagedObjectSynchronousQueryContext
            ctx(IpPrefixLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *rlts = 0;    

        if( (msg->opCode() == IPPOLICY_MO_DEL) && !(msg->getInstance()))
        {
            //Delete All instances of the ip-prefix
            ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));
            ctx.addSelectField("instance");
            ctx.addSelectField("objectId");

			rlts = querySynchronouslyLocalManagedObjectsForLocationId(
					FrameworkToolKit::getThisLocationId(), &ctx);

            if (rlts && rlts->size() > 0) {
                loopCount = rlts->size();
                mo =  dynamic_cast<IpPrefixLocalManagedObject*>(rlts->back());
                objectIds.push_back(mo->getObjectId());
                rlts->pop_back();
            }
            else
            {
                status = WRC_GENERIC_BACKEND_ERR; 
                loopCount = 0;
            }    
        }

        while((status == WAVE_MESSAGE_SUCCESS) && (loopCount--))
        {
            args.clear();
            args.push_back(msg->getName());
            args.push_back("seq");
            if( (msg->opCode() == IPPOLICY_MO_DEL) && !(msg->getInstance())) {
            	objectIds.push_back(mo->getObjectId());
                args.push_back(toString(mo->getInstance()));
            }
            else {
                args.push_back(toString(msg->getInstance()));
            }

            args.push_back(toString(msg->getAction()));

            if (msg->opCode() == IPPOLICY_MO_CREATE) {
                op_code = SSM_CLIENT_ADD_IPPREFIX;
                args.push_back(msg->getIp_prefix().toString());

                if (msg->getLeFlag()) {
                    args.push_back("le");
                    args.push_back(toString(msg->getLe()));
                }

                if (msg->getGeFlag()) {
                    args.push_back("ge");
                    args.push_back(toString(msg->getGe()));
                }
            }
            else if (msg->opCode() == IPPOLICY_MO_DEL) {
                op_code = SSM_CLIENT_DEL_IPPREFIX;
            }
            else if (msg->opCode() == IPPOLICY_MO_SET) {
                // unsupported from wysereaconfiguration
                prismAssert(false, __FILE__, __LINE__);

            }
            else if (msg->opCode() == IPPOLICY_MO_UNSET) {
                // unsupported from wysereaconfiguration
                prismAssert(false, __FILE__, __LINE__);

            }

            trace(TRACE_LEVEL_INFO, string("IP PREFIX - OPCODE: ") +op_code);

            for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
            	trace(TRACE_LEVEL_INFO, string("IP PREFIX-LIST - ARG. VALUE: ") + args[tmpIdx]);
            }

            GenericClientMessage_ client_msg(args, op_code);
            status = sendSynchronouslyToWaveClient ("ssm", &client_msg);
            if(status == WAVE_MESSAGE_SUCCESS) {
                if ((status = client_msg.getCompletionStatus())
                        == WAVE_MESSAGE_SUCCESS) {
                    status = client_msg.getClientStatus();
                }

                UI32 size = 0;
                void* data = 0;
                client_msg.getDuplicateInterfaceOutputBuffer(0, size, data);
                generic_wyser_msg_reply_t* reply_msg = (generic_wyser_msg_reply_t*)data;

                if (reply_msg ) {
                	trace(TRACE_LEVEL_INFO, string("From Back-end: reply_msg->num_records = > ")+reply_msg->num_records);
                }

                if (reply_msg and reply_msg->num_records)
                {
                    size = 0, data = 0;
                    client_msg.getDuplicateInterfaceOutputBuffer(1, size, data);

                    if((op_code == SSM_CLIENT_ADD_IPPREFIX) && (!(msg->getInstance()) ))
                    {

                    	trace(TRACE_LEVEL_INFO, string("IP PREFIX - back end response:") +string((char*)data)+"==END");

                        char *ptr = (char*)data;
                        UI32 seqSize = 0;

                        if((ptr = strstr((char*)data, "Sequence number")))
                        {
                            ptr += 16;

                            while(*ptr++ != '\n')
                                seqSize++;

                            if(seqSize+16 == size-1)
                            {
                                *(--ptr) = '\0';
                                ptr -= seqSize;

                                msg->setInstance(atoi(ptr));
                            }
                            else
                            {
                                msg->set_backend_err(string("%%Error: ")+string((char*)++ptr, size-seqSize));
                                status = WRC_GENERIC_BACKEND_ERR;
                            }
                        }
                        else
                        {
                            msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                            status = WRC_GENERIC_BACKEND_ERR;
                        }
                    }
                    else
                    {
                        msg->set_backend_err(string("%%Error: ")+string((char*)data, size));
                        status = WRC_GENERIC_BACKEND_ERR;
                    }
                }
            }

            if( (msg->opCode() == IPPOLICY_MO_DEL) && !(msg->getInstance()))
            {
                if (mo)
                {
                    seq->addManagedObjectForGarbageCollection(mo);
                    mo = 0;
                }    

                if (rlts && rlts->size() > 0) {
                    mo =  dynamic_cast<IpPrefixLocalManagedObject*>(rlts->back());
                    rlts->pop_back();
                }
            }
        }//while


        if( (msg->opCode() == IPPOLICY_MO_DEL) && !(msg->getInstance()))
        {
            if (rlts) {
                seq->addManagedObjectsForGarbageCollection(*rlts);
                delete rlts;
            }
            if (mo)
                seq->addManagedObjectForGarbageCollection(mo);
        }        

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::forwardIpPrefix called, status= ") + status);
        return status;
    }

    ResourceId
    IpPolicyLocalWorker::writeIpPrefixMO(PrismSynchronousLinearSequencerContext *seq)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        IpPolicyIpPrefixMessage *msg =
                static_cast<IpPolicyIpPrefixMessage*> (seq->getPPrismMessage ());
        IpPrefixLocalManagedObject *mo = 0;

        WaveManagedObjectSynchronousQueryContext
            ctx(IpPrefixLocalManagedObject::getClassName());
        ctx.addAndAttribute(new AttributeString(msg->getName(), "name"));

        vector<WaveManagedObject *> *rlts = 0;

        if( !((msg->opCode() == IPPOLICY_MO_DEL) && !(msg->getInstance())) )
        {
            ctx.addAndAttribute(new AttributeUI16(msg->getInstance(), "instance"));
            if(msg->opCode() == IPPOLICY_MO_DEL) {
            	ctx.addSelectField("objectId");
            }

			rlts = querySynchronouslyLocalManagedObjectsForLocationId(
					FrameworkToolKit::getThisLocationId(), &ctx);

            if (rlts && rlts->size() > 0) {
                mo =  dynamic_cast<IpPrefixLocalManagedObject*>(rlts->back());
                if(msg->opCode() == IPPOLICY_MO_DEL && msg->getInstance()) {
                	objectIds.push_back(mo->getObjectId());
                }
                rlts->pop_back();
            }
        }

        if (msg->opCode() == IPPOLICY_MO_CREATE) {
            if (mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("IP prefix create MO ") + msg->getName()
                        + string(" already exists"));
            }
            else {
                mo = new IpPrefixLocalManagedObject(
                        static_cast<IpPolicyLocalObjectManager*>(getPWaveObjectManager()));
                mo->setName(msg->getName());
                mo->setInstance(msg->getInstance());
                SET_MO_ATTRIBUTE(Action);
                SET_MO_ATTRIBUTE(Ip_prefix);
                SET_UNSET_MO_ATTRIBUTE(Ge);
                SET_UNSET_MO_ATTRIBUTE(Le);
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_DEL) {
            if (objectIds.empty()) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("IP prefix delete MO ") + msg->getName()
                        + string(" does not exist"));
            } else {
            	for(size_t index=0; index < objectIds.size(); index++)
            	{
            		if (objectIds[index] == ObjectId::NullObjectId) {
            			trace(TRACE_LEVEL_ERROR,"Policy id is null");
            		} else {
                		deleteWaveManagedObject(objectIds[index]);
            		}
            	}
            	objectIds.clear();
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_SET) {
            if (!mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("IP prefix set attributes for MO ") + msg->getName()
                        + string(" does not exist"));
            }
            else {
                updateWaveManagedObject(mo);
                SET_MO_ATTRIBUTE(Action);
                SET_MO_ATTRIBUTE(Ip_prefix);
                SET_UNSET_MO_ATTRIBUTE(Ge);
                SET_UNSET_MO_ATTRIBUTE(Le);
            }
        }
        else if (msg->opCode() == IPPOLICY_MO_UNSET) {
            if (!mo) {
                status = WAVE_MESSAGE_ERROR;
                trace(TRACE_LEVEL_ERROR, string("IP prefix unset attributes for MO ") + msg->getName()
                        + string(" does not exist"));
            }
            else {
                updateWaveManagedObject(mo);
                UNSET_MO_ATTRIBUTE(Ge);
                UNSET_MO_ATTRIBUTE(Le);
            }
        }
        else {
            trace(TRACE_LEVEL_ERROR, string("Ip prefix opcode not found MO ") + msg->getName());
            status = WAVE_MESSAGE_ERROR;
        }

        if (rlts) {
            seq->addManagedObjectsForGarbageCollection(*rlts);
            delete rlts;
        }
        if (mo)
            seq->addManagedObjectForGarbageCollection(mo);

        trace(TRACE_LEVEL_INFO,
                string("IpPolicyLocalWorker::writeIpPrefixMO called, status= ") + status);

        return status;
    }


    ResourceId IpPolicyLocalWorker::restoreRouteMapMO(int slot_id)
    {
        RouteMapLocalManagedObject* mo = 0;

        WaveManagedObjectSynchronousQueryContext q_ctxt(RouteMapLocalManagedObject::getClassName());

        vector<WaveManagedObject *> *rlts= querySynchronouslyLocalManagedObjectsForLocationId(
        		FrameworkToolKit::getThisLocationId(), &q_ctxt);

		bool send_match_interface = (slot_id == -1);

        if (rlts && !rlts->empty()) {
            vector<string> args;

            for (size_t i = 0; i<rlts->size(); ++i) {
                mo = dynamic_cast<RouteMapLocalManagedObject*>((*rlts)[i]);
				args.clear();

			    GenericClientMessage_* client_msg;
                if (slot_id == -1) {
					args.push_back(mo->getName());
					args.push_back(toString(mo->getAction()));
					args.push_back(toString(mo->getInstance()));
					client_msg =
							new GenericClientMessage_(args,SSM_CLIENT_ADD_ROUTEMAP);
					sendSynchronouslyToWaveClient("ssm", client_msg);
					delete client_msg;
                }

                args.clear();
                args.push_back(mo->getName());
                args.push_back(toString(mo->getAction()));
                args.push_back(toString(mo->getInstance()));

                if (slot_id >= 0) {
                	stringstream slot_str;
                	slot_str << slot_id;
					vector<string> eths = mo->getMatch_intf_eth();
					if (!eths.empty()) {
						for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
							string twoTupleIfName = getTwoTouple(toString(*it));
							args.push_back("te");
                            args.push_back(twoTupleIfName);

							// test remoge later
							trace (TRACE_LEVEL_INFO, "IpPolicyLocalWorker:: name is " + *it + "; slot is " + slot_str.str()); 
							if (twoTupleIfName.compare(0, slot_str.str().size(), slot_str.str()) == 0) {
							  	trace (TRACE_LEVEL_INFO, "IpPolicyLocalWorker::  compare 1 true"); 
								send_match_interface = true;
							}
						}
					}
					eths = mo->getMatch_intf_gi();
					if (!eths.empty()) {
						for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
							string twoTupleIfName = getTwoTouple(toString(*it));
							args.push_back("gi");
                            args.push_back(twoTupleIfName);
                            if (twoTupleIfName.compare(0, slot_str.str().size(), slot_str.str()) == 0)
								send_match_interface = true;
						}
					}
					eths = mo->getMatch_intf_fo();
					if (!eths.empty()) {
						for (vector<string>::iterator it = eths.begin(); it != eths.end(); ++it) {
							string twoTupleIfName = getTwoTouple(toString(*it));
							args.push_back("fo");
                            args.push_back(twoTupleIfName);
                            if (twoTupleIfName.compare(0, slot_str.str().size(), slot_str.str()) == 0)
								send_match_interface = true;
						}
					}
                }

                if (send_match_interface) {
					vector<UI32> vlans = mo->getMatch_intf_vlan();
					if (!vlans.empty()) {
						for (vector<UI32>::iterator it = vlans.begin(); it != vlans.end(); ++it) {
							args.push_back("vlan");
							args.push_back(toString(*it));
						}
					}

					vector<UI32> pos = mo->getMatch_intf_po();
					if (!pos.empty()) {
						for (vector<UI32>::iterator it = pos.begin(); it != pos.end(); ++it) {
							args.push_back("po");
							args.push_back(toString(*it));
						}
					}

					vector<UI32> ves = mo->getMatch_intf_ve();
					if (!ves.empty()) {
						for (vector<UI32>::iterator it = ves.begin(); it != ves.end(); ++it) {
							args.push_back("ve");
							args.push_back(toString(*it));
						}
					}

					vector<UI32> loopbacks = mo->getMatch_intf_loopback();
					if (!loopbacks.empty()) {
						for (vector<UI32>::iterator it = loopbacks.begin(); it != loopbacks.end(); ++it) {
							args.push_back("loopback");
							args.push_back(toString(*it));
						}
					}

					if (args.size()>3) {
						args.push_back("replay");
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_INTF);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}
                }

                if (slot_id == -1) {
					if (mo->m_match_addr_prefix.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(mo->m_match_addr_prefix.getStringValue());
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_ADDR);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_match_nh_prefix.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(mo->m_match_nh_prefix.getStringValue());
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_NH);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

                    if (mo->m_match_next_hop_address.getIsUserConfigured()) {
                        args.clear();
                        args.push_back(mo->getName());
                        args.push_back(toString(mo->getAction()));
                        args.push_back(toString(mo->getInstance()));
                        args.push_back(toString(mo->m_match_next_hop_address.getUI32Value()));
                        client_msg = new GenericClientMessage_(args,
                                SSM_CLIENT_SET_ROUTEMAP_MATCH_NEXT_HOP_FILTER);
                        sendSynchronouslyToWaveClient("ssm", client_msg);
                        delete client_msg;
                    }

					if (mo->m_match_metric.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString<UI32>(
								mo->m_match_metric.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_METRIC);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_match_route_type.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->getMatch_route_type()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TYPE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_match_protocol_bgp.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if(mo->m_match_protocol_bgp.getBoolValue()){
							args.push_back("bgp");
						}
						if (mo->m_match_bgp_route_type.getIsUserConfigured()) {
							if(mo->getMatch_bgp_route_type() == bgp_proto_external) {
								args.push_back("external");
							} else if (mo->getMatch_bgp_route_type() == bgp_proto_static_network) {
								args.push_back("static-network");
							} else if (mo->getMatch_bgp_route_type() == bgp_proto_internal) {
								args.push_back("internal");
							}
						}
						client_msg = new GenericClientMessage_(args,
							SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TYPE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					vector<UI32> tags = mo->getMatch_route_tag();
					if (!tags.empty())  {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						for (vector<UI32>::iterator it = tags.begin();
								it != tags.end(); ++it) {
							args.push_back(toString(*it));
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_TAG);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					string asPathStr = mo->getMatch_route_aspath();
					if (!asPathStr.empty())  {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
		            	size_t indexStr = 0;
		            	size_t tmpIndex = string::npos;
						while(indexStr != string::npos)
						{
							tmpIndex = asPathStr.find_first_of(" ", indexStr);
							if(indexStr != tmpIndex)
							{
								string tmpAsPathStr = asPathStr.substr(indexStr, tmpIndex - indexStr);
								args.push_back(tmpAsPathStr);
							}
							indexStr = asPathStr.find_first_not_of(" ", tmpIndex);
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_ROUTE_ASPATH);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_distance.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->m_set_distance.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_DISTANCE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if ( mo->getPbrNexthopConf().size() > 0 )
					{
				        WaveManagedObjectSynchronousQueryContext qPbrNexthopCtxt(PbrNexthopAddrManagedObject::getClassName());
				        qPbrNexthopCtxt.addAndAttribute (new AttributeObjectId (mo->getObjectId(), "ownerManagedObjectId"));
				        qPbrNexthopCtxt.addOrderField("index", true);

				        vector<WaveManagedObject *> *pbrNexthopMos = querySynchronously (&qPbrNexthopCtxt);

				        if (NULL != pbrNexthopMos)
				        {
							for(size_t idxNextHop = 0; idxNextHop < pbrNexthopMos->size(); idxNextHop ++ )
							{
								args.clear();
								args.push_back(mo->getName());
								args.push_back(toString(mo->getAction()));
								args.push_back(toString(mo->getInstance()));
								PbrNexthopAddrManagedObject *pbrNexthopMO = dynamic_cast<PbrNexthopAddrManagedObject *>((*pbrNexthopMos)[idxNextHop]);
								args.push_back(pbrNexthopMO->getSet_next_hop_address().toString());
							client_msg = new GenericClientMessage_(args,
									SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH);
							sendSynchronouslyToWaveClient("ssm", client_msg);
							delete client_msg;
						}

							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pbrNexthopMos);
				        }
					}

					if (mo->m_set_metric.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if (mo->getSet_metric_action() != assign)
							args.push_back(toString((PolicyMetricAction)
									mo->m_set_metric_action.getUI32Value()));
						if (mo->getSet_metric_action() != none) {
							args.push_back(toString(mo->m_set_metric.getUI32Value()));
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_METRIC);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_type.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString((PolicyRouteType)
								mo->m_set_route_type.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TYPE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_tag.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->m_set_route_tag.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_TAG);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_local_preference.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->m_set_route_local_preference.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_LOCAL_PREFERENCE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_weight.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->m_set_route_weight.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_WEIGHT);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_route_continue.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->m_route_continue.getUI32Value()));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_CONTINUE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					} else if (mo->m_route_continue_no_val.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(0));
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_CONTINUE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if(mo->m_set_route_aspath_prepend.getIsUserConfigured()) {
						asPathStr = mo->getSet_route_aspath_prepend();
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
		            	size_t indexStr = 0;
		            	size_t tmpIndex = string::npos;
						while(indexStr != string::npos)
						{
							tmpIndex = asPathStr.find_first_of(" ", indexStr);
							if(indexStr != tmpIndex)
							{
								string tmpAsPathStr = asPathStr.substr(indexStr, tmpIndex - indexStr);
								args.push_back(tmpAsPathStr);
							}
							indexStr = asPathStr.find_first_not_of(" ", tmpIndex);
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ASPATH_PREPEND);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_aspath_tag.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if(mo->m_set_route_aspath_tag.getBoolValue()){
							args.push_back("1");
						} else {
							args.push_back("0");
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ASPATH_TAG);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					string communityListStr = mo->getMatch_route_community_list();
					if (!communityListStr.empty())  {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						parseCommunityList(communityListStr, args);
		            	if(args[(args.size()-1)].compare("exact-match") == 0) {
		            		args.erase(args.end());
		            		args.push_back("1");
		            	} else {
		            		args.push_back("0");
		            	}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_COMMUNITY_LIST);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_origin_igp.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if(mo->m_set_route_origin_igp.getBoolValue()){
							args.push_back("igp");
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ORIGIN);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_origin_incomplete.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if(mo->m_set_route_origin_incomplete.getBoolValue()){
							args.push_back("incomplete");
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_ORIGIN);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_next_hop_peer_address.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back("peer-address");
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH_PEER_ADDR);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

                    if (mo->m_set_route_metric_type_external.getIsUserConfigured()) {
                        args.clear();
                        args.push_back(mo->getName());
                        args.push_back(toString(mo->getAction()));
                        args.push_back(toString(mo->getInstance()));
                        args.push_back("external");
                        client_msg = new GenericClientMessage_(args,
                                SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE);
                        sendSynchronouslyToWaveClient("ssm", client_msg);
                        delete client_msg;
                    }

                    if (mo->m_set_route_metric_type_internal.getIsUserConfigured()) {
                        args.clear();
                        args.push_back(mo->getName());
                        args.push_back(toString(mo->getAction()));
                        args.push_back(toString(mo->getInstance()));
                        args.push_back("internal");
                        client_msg = new GenericClientMessage_(args,
                                SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE);
                        sendSynchronouslyToWaveClient("ssm", client_msg);
                        delete client_msg;
                    }
                    
                    if (mo->m_set_route_metric_type_type_1.getIsUserConfigured()) {
                        args.clear();
                        args.push_back(mo->getName());
                        args.push_back(toString(mo->getAction()));
                        args.push_back(toString(mo->getInstance()));
                        args.push_back("type_1");
                        client_msg = new GenericClientMessage_(args,
                                SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE);
                        sendSynchronouslyToWaveClient("ssm", client_msg);
                        delete client_msg;
                    }
                    
                    if (mo->m_set_route_metric_type_type_2.getIsUserConfigured()) {
                        args.clear();
                        args.push_back(mo->getName());
                        args.push_back(toString(mo->getAction()));
                        args.push_back(toString(mo->getInstance()));
                        args.push_back("type_2");
                        client_msg = new GenericClientMessage_(args,
                                SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_METRIC_TYPE);
                        sendSynchronouslyToWaveClient("ssm", client_msg);
                        delete client_msg;
                    }                    

					if (mo->m_match_rs_prefix.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						string ip_prefix = mo->m_match_rs_prefix.getStringValue();

		            	size_t indexStr = 0;
		            	size_t tmpIndex = string::npos;
						while(indexStr != string::npos)
						{
							tmpIndex = ip_prefix.find_first_of(" ", indexStr);
							if(indexStr != tmpIndex)
							{
								string tmpAsPathStr = ip_prefix.substr(indexStr, tmpIndex - indexStr);
								args.push_back(tmpAsPathStr);
							}
							indexStr = ip_prefix.find_first_not_of(" ", tmpIndex);
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_RS_PREFIX_LIST);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

			        if (mo->m_match_protocol_bgp.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back("bgp");
						if(mo->m_match_bgp_route_type.getIsUserConfigured()){
							if(mo->getMatch_bgp_route_type() == bgp_proto_external) {
			        			args.push_back("external");
							} else if(mo->getMatch_bgp_route_type() == bgp_proto_static_network) {
								args.push_back("static-network");
							} else if(mo->getMatch_bgp_route_type() == bgp_proto_internal) {
								args.push_back("internal");
							}
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_MATCH_BGP_PROTOCOL_ROUTE_TYPE);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
			        }
					
					if (mo->m_set_route_automatic_tag.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if(mo->m_set_route_automatic_tag.getBoolValue()){
							args.push_back("1");
						} else {
							args.push_back("0");
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_AUTOMATIC_TAG);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_community_acl_delete.getIsUserConfigured()) {
							args.clear();
							args.push_back(mo->getName());
							args.push_back(toString(mo->getAction()));
							args.push_back(toString(mo->getInstance()));
							args.push_back(mo->m_set_route_community_access_list.getStringValue());
							client_msg = new GenericClientMessage_(args,
									SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_ACCESS_LIST);
							sendSynchronouslyToWaveClient("ssm", client_msg);
							delete client_msg;
					}

					if (mo->m_set_route_community_expr.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));

						if(!(mo->getSet_route_community_expr().empty()))
						{
							vector<string> communityList;

							tokenize(mo->getSet_route_community_expr(),communityList,' ');

							size_t commNumSize = communityList.size();

							for(size_t idx = 0; idx < communityList.size(); idx++)
							{
								if (strncmp(communityList[idx].c_str(), "local-as", strlen("local-as"))== 0) {
									commNumSize--;
								} else if (strncmp(communityList[idx].c_str(), "none", strlen("none"))== 0) {
									commNumSize--;
								} else if (strncmp(communityList[idx].c_str(), "additive", strlen("additive"))== 0) {
									commNumSize--;
								}
								else if (strncmp(communityList[idx].c_str(), "no-export", strlen("no-export"))== 0) {
									commNumSize--;
								} else if (strncmp(communityList[idx].c_str(), "no-advertise", strlen("no-advertise"))== 0) {
									commNumSize--;
								}
							}

							stringstream sizeStr;
	         				sizeStr << commNumSize;
							args.push_back(sizeStr.str());

							if(!communityList.empty()) {
								for(size_t idx = 0; idx < communityList.size(); idx++) {
									args.push_back(communityList[idx]);
								}
							}
						} else {
							args.push_back("0");
						}

			            trace(TRACE_LEVEL_INFO, string("POST BOOT: RouteMap - OPCODE: ") +SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_EXPR);

			            for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
			            	trace(TRACE_LEVEL_INFO, string("POST BOOT: RouteMap - ARG. VALUE: ") + args[tmpIdx]);
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_COMMUNITY_EXPR);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_extcommunity_expr.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));

						if(!(mo->getSet_route_extcommunity_expr().empty()))
						{
							vector<string> communityList;
							bool  additive = false;

							tokenize(mo->getSet_route_extcommunity_expr(),communityList,' ');

							size_t commNumSize = communityList.size();

							for(size_t idx = 0; idx < communityList.size(); idx++)
							{
								if (strncmp(communityList[idx].c_str(), "additive", strlen("additive"))== 0) {
									additive = true;
									commNumSize--;
								}
							}

							stringstream sizeStr;
	         				 if( commNumSize > GENERIC_WYSEREA_ARGS_MAX ) {
	         					commNumSize = GENERIC_WYSEREA_ARGS_MAX;
	         				 }
							sizeStr << commNumSize;
							args.push_back(sizeStr.str());

							if(!communityList.empty()) {
								for(size_t idx = 0; idx < communityList.size(); idx++) {
									args.push_back(communityList[idx]);
								}
							}

							if(additive) {
								args.push_back("additive");
							}

						} else {
							args.push_back("0");
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_EXTCOMMUNITY_EXPR);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_extcommunity_soo_expr.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(toString(mo->getInstance()));

						if(!(mo->getSet_route_extcommunity_soo_expr().empty()))
						{
							args.push_back(toString(mo->getSet_route_extcommunity_soo_expr()));
						}

						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_EXTCOMMUNITY_SOO_EXPR);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_set_route_dampening.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						if(mo->m_set_route_dampening.getBoolValue()){
							args.push_back("1");
						} else {
							args.push_back("0");
						}
						if (mo->m_set_route_dampening_half_life.getIsUserConfigured()) {
							args.push_back(toString(mo->m_set_route_dampening_half_life.getUI32Value()));
						}
						if (mo->m_set_route_dampening_reuse.getIsUserConfigured()) {
							args.push_back(toString(mo->m_set_route_dampening_reuse.getUI32Value()));
						}
						if (mo->m_set_route_dampening_suppress.getIsUserConfigured()) {
							args.push_back(toString(mo->m_set_route_dampening_suppress.getUI32Value()));
						}
						if (mo->m_set_route_dampening_max_suppress.getIsUserConfigured()) {
							args.push_back(toString(mo->m_set_route_dampening_max_suppress.getUI32Value()));
						}
						client_msg = new GenericClientMessage_(args,
								SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_DAMPENING);
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_match_addr_acl.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(mo->getMatch_addr_acl());
						client_msg = new GenericClientMessage_(args, SSM_CLIENT_SET_ROUTEMAP_MATCH_IPACL);
						for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
							trace(TRACE_LEVEL_INFO, string("m_match_addr_acl - ARG. VALUE: ") + args[tmpIdx]);
						}
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_dscp_precedence.getIsUserConfigured()) {
						args.clear();
						DscpPrecedence dscpPrecedence = mo->getMatch_dscpPrecedence();
						std::stringstream ss;
						ss << dscpPrecedence;
						string dscpPrecedenceStr = ss.str();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back(dscpPrecedenceStr);
						client_msg = new GenericClientMessage_(args, SSM_CLIENT_SET_ROUTEMAP_SET_DSCP);
						for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
							trace(TRACE_LEVEL_INFO, string("m_dscp_precedence - ARG. VALUE: ") + args[tmpIdx]);
						}
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->m_match_nullZero.getIsUserConfigured()) {
						args.clear();
						args.push_back(mo->getName());
						args.push_back(toString(mo->getAction()));
						args.push_back(toString(mo->getInstance()));
						args.push_back("null0");
						client_msg = new GenericClientMessage_(args, SSM_CLIENT_SET_ROUTEMAP_SET_INTERFACE);
						for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
							trace(TRACE_LEVEL_INFO, string("m_match_nullZero - ARG. VALUE: ") + args[tmpIdx]);
						}
						sendSynchronouslyToWaveClient("ssm", client_msg);
						delete client_msg;
					}

					if (mo->getPbrVrfConf().size() > 0) {
						vector<WaveManagedObjectPointer<PbrVrfManagedObject> > m_pbrVrfMO = mo->getPbrVrfConf();
						for(UI32 indx=0; indx < m_pbrVrfMO.size(); indx++) {
							args.clear();
							string vrfName = (m_pbrVrfMO[indx])->getVrfNameMO();
							IpV4Address nexthopAddr = (m_pbrVrfMO[indx])->getSet_next_hop_address();
							args.push_back(mo->getName());
							args.push_back(toString(mo->getAction()));
							args.push_back(toString(mo->getInstance()));
							args.push_back(vrfName);
							args.push_back(nexthopAddr.toString());
							client_msg = new GenericClientMessage_ (args, SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_VRF_NH);
							for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
								trace(TRACE_LEVEL_INFO, string("getPbrVrfConf - ARG. VALUE: ") + args[tmpIdx]);
							}
							sendSynchronouslyToWaveClient("ssm", client_msg);
							delete client_msg;
						}
					}

					if (mo->getPbrNexthopConf().size() > 0) {
						vector<WaveManagedObjectPointer<PbrNexthopAddrManagedObject> > m_pbrNexthopAddrMO
							= mo->getPbrNexthopConf();
						for(UI32 indx=0; indx < m_pbrNexthopAddrMO.size(); indx++) {
							args.clear();
							IpV4Address nexthopAddr = (m_pbrNexthopAddrMO[indx])->getSet_next_hop_address();
							args.push_back(mo->getName());
							args.push_back(toString(mo->getAction()));
							args.push_back(toString(mo->getInstance()));
							args.push_back(nexthopAddr.toString());
							client_msg = new GenericClientMessage_ (args, SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_NH);
							for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
								trace(TRACE_LEVEL_INFO, string("getPbrNexthopConf - ARG. VALUE: ") + args[tmpIdx]);
							}
							sendSynchronouslyToWaveClient("ssm", client_msg);
							delete client_msg;
						}
					}

					if (mo->getPbrGlobalNexthopConf().size() > 0) {
						vector<WaveManagedObjectPointer<PbrGlobalNexthopManagedObject> > m_pbrGlobalNexthopAddrMO
							= mo->getPbrGlobalNexthopConf();
						for(UI32 indx=0; indx < m_pbrGlobalNexthopAddrMO.size(); indx++) {
							args.clear();
							IpV4Address nexthopAddr = (m_pbrGlobalNexthopAddrMO[indx])->getSet_next_hop_address();
							args.push_back(mo->getName());
							args.push_back(toString(mo->getAction()));
							args.push_back(toString(mo->getInstance()));
							args.push_back(nexthopAddr.toString());
							client_msg = new GenericClientMessage_ (args, SSM_CLIENT_SET_ROUTEMAP_SET_ROUTE_GLOBAL_NH);
							for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
								trace(TRACE_LEVEL_INFO, string("getPbrGlobalNexthopConf - ARG. VALUE: ") + args[tmpIdx]);
							}
							sendSynchronouslyToWaveClient("ssm", client_msg);
							delete client_msg;
						}
					}
                }
            }
        }

        if (rlts)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId IpPolicyLocalWorker::ConfigReplayPbrGlobalIntf(const UI32 ifType)
    {
        string className                                        = "";
        ResourceId status                                       = WAVE_MESSAGE_ERROR;
        vector<WaveManagedObject *> *pResults                   = NULL;

        tracePrintf(TRACE_LEVEL_INFO, "ConfigReplay of Routemap Global Interface Association: ..." + ifType);

        if (ifType == IF_TYPE_PO) {
            className = PoIntfManagedObject::getClassName();
        } else if (ifType == IF_TYPE_VL) {
            className = NsmIntfSviManagedObject::getClassName();
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);

        if (ifType == IF_TYPE_VL) {
            syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId());
        }

        pResults = querySynchronously(&syncQueryCtxt);

        if (NULL != pResults) {
            UI32 numberOfResults                                            = pResults->size ();

            tracePrintf(TRACE_LEVEL_INFO, "Number of Vlan/Po interfaces: " + numberOfResults);

            for (UI32 i = 0; i < numberOfResults; i++) {
                GenericClientMessage_* client_msg   = NULL;

                if(((*pResults)[i]) == NULL) {
                    tracePrintf(TRACE_LEVEL_INFO, "****VlanMOs/PortChannel MOs ith element is NULL in IpPolicyLocalObjectManager***");
                    continue;
                }
                client_msg = ReplayPbrGlobalIntf(ifType, ((*pResults)[i]));

				if(client_msg != NULL)
				{
                	status = sendSynchronouslyToWaveClient ("ssm",client_msg);
	                if (status == WAVE_MESSAGE_SUCCESS) {
    	                status = client_msg->getClientStatus();
        	            tracePrintf(TRACE_LEVEL_INFO, "ConfigReplayPbrGlobalIntf: Success %lld",  status);
            	    } else {
                	    tracePrintf(TRACE_LEVEL_INFO,  "ConfigReplayPbrGlobalIntf: Failed: %lld",  client_msg->getClientStatus());
	                }
    	            delete client_msg;
        	        client_msg = NULL;
				}
            }

            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return WAVE_MESSAGE_SUCCESS;
    }

	GenericClientMessage_* IpPolicyLocalWorker::ReplayPbrGlobalIntf(const UI32 ifType, WaveManagedObject *pWaveManagedObject)
	{
		PoIntfManagedObject *pPoIntfManagedObject           = NULL;
		NsmIntfSviManagedObject *pNsmIntfSviManagedObject   = NULL;
		string routeMapName;
		int op_code = 0;
		vector<string> args;
		string ifName = "";
		char buffer[25];
		GenericClientMessage_* client_msg           = NULL;

		if (ifType == IF_TYPE_PO) {
			pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>(pWaveManagedObject);
			if (pPoIntfManagedObject) {
				if ((pPoIntfManagedObject->getPBR().operator->()) != NULL) {
					op_code = SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE;
					pPoIntfManagedObject->getPBRNameFromInterface (routeMapName);
					sprintf(buffer, "po%d", pPoIntfManagedObject->getId());
					ifName = buffer;
					args.clear();
					args.push_back(routeMapName);
					args.push_back(ifName);
					for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
						trace(TRACE_LEVEL_INFO, string("ReplayPbrGlobalIntf PO - ARG. VALUE: ") + args[tmpIdx]);
					}
					client_msg = new GenericClientMessage_(args, op_code);
				}
			}
		} else if (ifType == IF_TYPE_VL) {
			pNsmIntfSviManagedObject = dynamic_cast<NsmIntfSviManagedObject *>(pWaveManagedObject);
			if (pNsmIntfSviManagedObject) {
				if ((pNsmIntfSviManagedObject->getPBR().operator->()) != NULL) {
					op_code = SSM_CLIENT_SET_ROUTEMAP_APPLY_INTERFACE;
					pNsmIntfSviManagedObject->getPBRNameFromInterface (routeMapName);
					sprintf(buffer, "ve0.%d", pNsmIntfSviManagedObject->getIfId());
					ifName = buffer;
					args.clear();
					args.push_back(routeMapName);
					args.push_back(ifName);
					for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
						trace(TRACE_LEVEL_INFO, string("ReplayPbrGlobalIntf VL - ARG. VALUE: ") + args[tmpIdx]);
					}
					client_msg = new GenericClientMessage_(args, op_code);
				}
			}
		}
		return client_msg;
	}

	ResourceId IpPolicyLocalWorker::restorePBRMO(SI32 slot_id)
	{

		trace(TRACE_LEVEL_INFO, "IpPolicyLocalWorker :: Replaying Ip Acl Interface Association:Binding...." + slot_id);
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		if (slot_id >= 0) {
			vector<PhyIntfLocalManagedObject*> phyMOs;
			NsmUtils::SlotGetPhyIntfMOs(phyMOs, slot_id);
			UI32 size              = phyMOs.size();
			UI32 i                     = 0;

			for (i = 0; i < size; i++) {

				GenericClientMessage_* client_msg = NULL;

				if (phyMOs[i] == NULL) {
					trace(TRACE_LEVEL_INFO, "*** phyMOs ith element is NULL");
					continue;
				}
				trace(TRACE_LEVEL_INFO, "IpPolicyLocalWorker :: restorePBRMO : debug_1");
				if(phyMOs[i]->getPortrole() == false) {
					trace(TRACE_LEVEL_INFO, "IpPolicyLocalWorker :: restorePBRMO : debug_2");
					client_msg = L3PostBootUtil::ConfigReplayPBRPhyIntf(phyMOs[i]);
					if (client_msg) {
						trace(TRACE_LEVEL_INFO, "IpPolicyLocalWorker :: restorePBRMO : debug_3");
						status = WAVE_MESSAGE_ERROR;
						status = sendSynchronouslyToWaveClient ("ssm", client_msg);
						if (status == WAVE_MESSAGE_SUCCESS) {
							trace(TRACE_LEVEL_INFO, "Phy Interface Ip Acl configuration successful ");
						} else {
							trace(TRACE_LEVEL_INFO, string("Phy Inerface Ip Acl Interface configuration failed - ") + status);
						}

						delete client_msg;
						client_msg = NULL;
					} else {
						//trace(TRACE_LEVEL_INFO, "IpPolicyLocalWorker::restorePBRMO SsmIpAclIntfAssocBulkMessage is NULL ");
					}
				}
			}

			for (i = 0; i < size; i++) {
				//trace(TRACE_LEVEL_INFO, "*** IpPolicyLocalWorker:: Deleting phyMOs " + i);
				if(phyMOs[i])
					delete phyMOs[i];
			}
		}
		return status;
    }

    ResourceId IpPolicyLocalWorker::restoreIpPrefixMO()
    {
        IpPrefixLocalManagedObject* mo = 0;

        WaveManagedObjectSynchronousQueryContext q_ctxt(IpPrefixLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *rlts= querySynchronouslyLocalManagedObjectsForLocationId(
        		FrameworkToolKit::getThisLocationId(), &q_ctxt);

        if (rlts && !rlts->empty()) {
            vector<string> args;

            for (size_t i = 0; i<rlts->size(); ++i) {
                mo = dynamic_cast<IpPrefixLocalManagedObject*>((*rlts)[i]);
				args.clear();

                args.push_back(mo->getName());
                args.push_back("seq");
                args.push_back(toString(mo->getInstance()));
                args.push_back(toString(mo->getAction()));
                args.push_back(mo->getIp_prefix().toString());

                if (mo->m_le.getIsUserConfigured()) {
                    args.push_back("le");
                    args.push_back(toString(mo->getLe()));
                }

                if (mo->m_ge.getIsUserConfigured()) {
                    args.push_back("ge");
                    args.push_back(toString(mo->getGe()));
                }

                GenericClientMessage_* client_msg = new GenericClientMessage_(args,
                        SSM_CLIENT_ADD_IPPREFIX);
                sendSynchronouslyToWaveClient("ssm", client_msg);
                delete client_msg;
            }
        }


        if (rlts)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId IpPolicyLocalWorker::restoreAsPathAccessListMO()
    {
    	IpAsPathAccessListLocalManagedObject* mo = 0;

        WaveManagedObjectSynchronousQueryContext q_ctxt(IpAsPathAccessListLocalManagedObject::getClassName());

        vector<WaveManagedObject *> *rlts= querySynchronouslyLocalManagedObjectsForLocationId(
        		FrameworkToolKit::getThisLocationId(), &q_ctxt);

        if (rlts && !rlts->empty()) {
            vector<string> args;

            for (size_t i = 0; i<rlts->size(); ++i) {
                mo = dynamic_cast<IpAsPathAccessListLocalManagedObject*>((*rlts)[i]);
				args.clear();

                args.push_back(mo->getName());
                args.push_back(toString(mo->getInstance()));
                args.push_back(toString(mo->getAction()));
                args.push_back(mo->getRegExpr());

    			for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
    				trace(TRACE_LEVEL_INFO, string("PostBoot: => IP ASPATH - ARG. VALUE: ") + args[tmpIdx]);
    			}

                GenericClientMessage_* client_msg = new GenericClientMessage_(args,
                		SSM_CLIENT_ADD_ASPATHACESSLIST);
                sendSynchronouslyToWaveClient("ssm", client_msg);
                delete client_msg;
            }
        }

        if (rlts)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId IpPolicyLocalWorker::restoreIpCommunityStdAccessListMO()
    {
    	IpCommunityStdAccessListLocalManagedObject* mo = 0;

    	WaveManagedObjectSynchronousQueryContext q_ctxt(IpCommunityStdAccessListLocalManagedObject::getClassName());
    	vector<WaveManagedObject *> *rlts= querySynchronouslyLocalManagedObjectsForLocationId(
    			FrameworkToolKit::getThisLocationId(), &q_ctxt);

    	if (rlts && !rlts->empty()) {
    		vector<string> args;

    		for (size_t i = 0; i<rlts->size(); ++i) {
    			mo = dynamic_cast<IpCommunityStdAccessListLocalManagedObject*>((*rlts)[i]);
    			args.clear();

    			args.push_back(mo->getName());
    			args.push_back(toString(mo->getInstance()));
    			args.push_back(toString(mo->getAction()));

    			if(!(mo->getCommunityExpr().empty()))
    			{
    				vector<string> communityList;

    				tokenize(mo->getCommunityExpr(),communityList,' ');

    				size_t commNumSize = communityList.size();

    				for(size_t idx = 0; idx < communityList.size(); idx++)
    				{
    					if (strncmp(communityList[idx].c_str(), "local-as", strlen("local-as"))== 0) {
    						commNumSize--;
    					} else if (strncmp(communityList[idx].c_str(), "internet", strlen("internet"))== 0) {
    						commNumSize--;
    					} else if (strncmp(communityList[idx].c_str(), "no-export", strlen("no-export"))== 0) {
    						commNumSize--;
    					} else if (strncmp(communityList[idx].c_str(), "no-advertise", strlen("no-advertise"))== 0) {
    						commNumSize--;
    					}
    				}

    				stringstream sizeStr;

    				sizeStr << commNumSize;
    				args.push_back(sizeStr.str());

    				if(!communityList.empty()) {
    					for(size_t idx = 0; idx < communityList.size(); idx++) {
    						args.push_back(communityList[idx]);
    					}
    				}
    			} else {
    				args.push_back("0");
    			}

    			for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
    				trace(TRACE_LEVEL_INFO, string("PostBoot: => IP COMMUNITY STD - ARG. VALUE: ") + args[tmpIdx]);
    			}

    			GenericClientMessage_* client_msg = new GenericClientMessage_(args,
    					SSM_CLIENT_ADD_IPCOMMUNITYSTDACESSLIST);
    			sendSynchronouslyToWaveClient("ssm", client_msg);
    			if(client_msg) {
    				delete client_msg;
    			}
    		}
    	}

    	if (rlts)
    		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);

    	return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId IpPolicyLocalWorker::restoreIpCommunityExtAccessListMO()
    {
    	IpCommunityExtAccessListLocalManagedObject* mo = 0;

        WaveManagedObjectSynchronousQueryContext q_ctxt(IpCommunityExtAccessListLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *rlts= querySynchronouslyLocalManagedObjectsForLocationId(
        		FrameworkToolKit::getThisLocationId(), &q_ctxt);

        if (rlts && !rlts->empty()) {
            vector<string> args;

            for (size_t i = 0; i<rlts->size(); ++i) {
                mo = dynamic_cast<IpCommunityExtAccessListLocalManagedObject*>((*rlts)[i]);
				args.clear();

                args.push_back(mo->getName());
                args.push_back(toString(mo->getInstance()));
                args.push_back(toString(mo->getAction()));
                args.push_back(mo->getCommunityRegExpr());

     	        for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
     	        	trace(TRACE_LEVEL_INFO, string("PostBoot: => IP COMMUNITY EXT - ARG. VALUE: ") + args[tmpIdx]);
     	        }

                GenericClientMessage_* client_msg = new GenericClientMessage_(args,
                		SSM_CLIENT_ADD_IPCOMMUNITYEXTACESSLIST);
                sendSynchronouslyToWaveClient("ssm", client_msg);
                if(client_msg) {
                	delete client_msg;
                }
            }
        }

        if (rlts)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId IpPolicyLocalWorker::restoreFilterChangeUpdateDelayMO() {
    	FilterChangeUpdateLocalManagedObject* mo = 0;

        WaveManagedObjectSynchronousQueryContext q_ctxt(FilterChangeUpdateLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *rlts= querySynchronouslyLocalManagedObjectsForLocationId(
        		FrameworkToolKit::getThisLocationId(), &q_ctxt);

        if (rlts && !rlts->empty()) {
            vector<string> args;

            for (size_t i = 0; i<rlts->size(); ++i) {
                mo = dynamic_cast<FilterChangeUpdateLocalManagedObject*>((*rlts)[i]);
				args.clear();
				args.push_back(toString(mo->getFilterUpdateDelay()));

     	        for(size_t tmpIdx=0; tmpIdx < args.size(); tmpIdx++) {
     	        	trace(TRACE_LEVEL_INFO, string("PostBoot: => FilterChangeUpdateLocalManagedObject - ARG. VALUE: ") + args[tmpIdx]);
     	        }

                GenericClientMessage_* client_msg = new GenericClientMessage_(args,
                		SSM_CLIENT_ADD_FILTERCHANGEUPDATEDELAY);
                sendSynchronouslyToWaveClient("ssm", client_msg);
                if(client_msg) {
                	delete client_msg;
                }
            }
        }

        if (rlts)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(rlts);

        return WAVE_MESSAGE_SUCCESS;
    }

    void  IpPolicyLocalWorker::PBRShowMessageHandler(PBRShowMessage *pPBRShowMessage)
    {
        if (pPBRShowMessage->getIsGwRequest())
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                    reinterpret_cast<PrismLinearSequencerStep> (&IpPolicyLocalWorker::sendShowRouteMapCommandToCluster),
                    reinterpret_cast<PrismLinearSequencerStep> (&IpPolicyLocalWorker::prismLinearSequencerSucceededStep),
                    reinterpret_cast<PrismLinearSequencerStep> (&IpPolicyLocalWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext
                    (pPBRShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        }
        else
        {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IpPolicyLocalWorker::sendShowToSsmDaemon),
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IpPolicyLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&IpPolicyLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };
            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pPBRShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();
        }
    }



    void IpPolicyLocalWorker::sendShowRouteMapCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster enter"));

        PBRShowMessage                      *msg_p      = NULL;
        PBRShowMessage                      *loc_msg_p  = NULL;
        WaveSendToClusterContext            *send_ctx_p = NULL;
        UI32                                locationId  = 0;
        SI32                                mappedId    = 0;
        vector<UI32>                        locations;
        bool                                sendToLocal = false;

        msg_p = dynamic_cast<PBRShowMessage *>(seq_ctx_p->getPPrismMessage ());
        loc_msg_p = new PBRShowMessage();

        loc_msg_p->setOpCode(msg_p->getOpCode());
        loc_msg_p->setRouteMapName(msg_p->getRouteMapName());
        loc_msg_p->setIfName(msg_p->getIfName());
        loc_msg_p->setRbridgeId(msg_p->getRbridgeId());
        loc_msg_p->setIfType(msg_p->getIfType());
        loc_msg_p->setIsGwRequest(false);
        loc_msg_p->setLastStanza(msg_p->getLastStanza());

        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- loc_msg_p->setOpCode -> ")+msg_p->getOpCode());
        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- msg_p->getRouteMapName() -->") + msg_p->getRouteMapName());
        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- msg_p->getIfName() ->")+ msg_p->getIfName());
        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- msg_p->getIfType() -> ")+ msg_p->getIfType());
        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- msg_p->getRbridgeId() -> ") + msg_p->getRbridgeId());

        /* Check Validity of mapped id before sending any msg */
        if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
                || (msg_p->getIfType() == IF_TYPE_FO))
        {
            if (NsmUtils::isValidTwoTuple(msg_p->getIfName())) {
                sendToLocal = true;
                tracePrintf(TRACE_LEVEL_DEBUG, "debug_debo Inside sendToLOcal");
            }
            else if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))
            {
                /* Send to the targeted LocationId pointed to by the mappedId. */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getIfName());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
                if (!locationId)
                {
                	tracePrintf(TRACE_LEVEL_DEBUG, "debug_debo !locationId");
                    //TODO: need to set the proper status error  resource id.
                    msg_p->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                    //TODO: need to set the proper status  error resource id.
                    seq_ctx_p->executeNextStep (WAVE_MESSAGE_SUCCESS);
                    delete loc_msg_p;
                    return;
                }
                else {
                    trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- locationId -> ")+ locationId);
                    locations.push_back(locationId);
                }
            }
        }

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast <PrismAsynchronousCallback>(
                &IpPolicyLocalWorker::RouteMapLocalShowSendToClusterCallback), seq_ctx_p);

        if (!send_ctx_p)
            return;

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        UI32 opCode = msg_p->getOpCode();

        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- msg_p->getIfName() -> ")+ msg_p->getIfName());
        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- msg_p->getIfType() -> ")+ msg_p->getIfType());

        if (0 != msg_p->getIfName().length())
        {
            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
                    || (msg_p->getIfType() == IF_TYPE_FO))
            {
                if (!sendToLocal) {
                    send_ctx_p->setLocationsToSendToForPhase1(locations);
                    tracePrintf(TRACE_LEVEL_DEBUG, "debug_debo debug_006");
                }
            }
            else if (msg_p->getIfType () == IF_TYPE_PO)
            {
                NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getIfName(),send_ctx_p);
            }
        }

        trace(TRACE_LEVEL_INFO, string("sendShowRouteMapCommandToCluster -- opCode -> ")+ opCode);

        if(msg_p->getIfType () == IF_TYPE_VE || opCode == SSMPBR_DCM_SHOW_STATUS_BY_ROUTE_MAP_E)
        {
            if((!strcmp(msg_p->getRbridgeId().c_str(),"all") == 0) && (!msg_p->getRbridgeId().empty()) )
            {
                UI32 rbridgeID = 0;
                stringstream tmpStream(msg_p->getRbridgeId());
                tmpStream >> rbridgeID;
                locationId = DcmToolKit::getLocationIdFromRbridgeId(rbridgeID);
                tracePrintf(TRACE_LEVEL_DEBUG, "debug_debo locationId = %d", locationId);
                if (!locationId)
                {
                	tracePrintf(TRACE_LEVEL_DEBUG, "debug-debo debug_001");
                    msg_p->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
                    seq_ctx_p->executeNextStep (WAVE_MESSAGE_SUCCESS);
                    delete loc_msg_p;
                    return;
                }
                locations.push_back(locationId);
            } else if(msg_p->getRbridgeId().empty()) {
                locations.push_back(FrameworkToolKit::getThisLocationId());
                tracePrintf(TRACE_LEVEL_DEBUG, "debug-debo FrameworkToolKit::getThisLocationId() = %d", FrameworkToolKit::getThisLocationId());
            }

            if(!locations.empty()) {
            	tracePrintf(TRACE_LEVEL_DEBUG, "debug-debo debug_002");
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        if (sendToLocal)
        {
            locationId = NsmUtils::getLocalLocationId();
            tracePrintf(TRACE_LEVEL_DEBUG, "debug-debo debug_003 locationId = %d", locationId);
            if (locationId) {
                locations.push_back(locationId);
                tracePrintf(TRACE_LEVEL_DEBUG, "debug-debo debug_004");
            }
            send_ctx_p->setLocationsToSendToForPhase1(locations);
        }

        for (UI32 i = 0; i < locations.size(); i++)
        {
            if (i ==0) {
                trace(TRACE_LEVEL_INFO, string("final locations:") + locations[i]);
            } else {
                trace(TRACE_LEVEL_INFO, string(":") + locations[i]);
            }
        }

        sendToWaveCluster(send_ctx_p);
        return;
    }

    void IpPolicyLocalWorker::RouteMapLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p)
    {
        PrismLinearSequencerContext *seq_ctx_p  = NULL;
        PBRShowMessage *req_msg_p  = NULL, *resp_msg_p = NULL, *sw_resp_p  = NULL;

        seq_ctx_p   = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
        req_msg_p   = dynamic_cast<PBRShowMessage *> (seq_ctx_p->getPPrismMessage ());
        resp_msg_p  = dynamic_cast<PBRShowMessage *> (ctx_p->getPPrismMessageForPhase1());

        prismAssert (req_msg_p != NULL, __FILE__, __LINE__);
        prismAssert (resp_msg_p != NULL, __FILE__, __LINE__);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        trace(TRACE_LEVEL_INFO, string("Response processing: Num resp:") + locations.size());

        for (i = 0; i < locations.size(); i++)
        {
            if (i ==0) {
                trace(TRACE_LEVEL_INFO, string("locations::") + locations[i]);
            } else {
                trace(TRACE_LEVEL_INFO, string(":") + locations[i]);
            }
        }

        for (i = 0; i < locations.size(); i++)
        {
            if (i ==0)
                trace(TRACE_LEVEL_INFO, string("locations:") + locations[i]);
            else
                trace(TRACE_LEVEL_INFO, string(":") + locations[i]);
        }

        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<PBRShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
            prismAssert (sw_resp_p != NULL, __FILE__, __LINE__);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus()) {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } else {
                ResourceId clusterCompletionStatus = sw_resp_p->getCompletionStatus();
                trace(TRACE_LEVEL_INFO, string("Response processing: The location: ")
                        + locations[i] + string("returned error: ")
                        + FrameworkToolKit::localize(clusterCompletionStatus));
                req_msg_p->setCompletionStatus(clusterCompletionStatus);
                break;
            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;
    }

    ResourceId IpPolicyLocalWorker::sendShowToSsmDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
    {

        trace(TRACE_LEVEL_INFO, string("sendShowToSsmDaemon -- enter"));

        PBRShowMessage          *msg_p  = dynamic_cast<PBRShowMessage *> (ctx_p->getPPrismMessage ());
        ResourceId              status;
        SI32                    mappedId        = 0;
        string                  ifName          = "";
        SsmPbrShowClientMessage *cl_msg_p = new SsmPbrShowClientMessage();

        if (cl_msg_p != NULL)
        {
            char buffer[25];
            buffer[24]      = '\0';
            UI32 bufferSize = sizeof (buffer);
            cl_msg_p->setRouteMapName(msg_p->getRouteMapName());
            trace(TRACE_LEVEL_INFO, string("sendShowToSsmDaemon -- msg_p->getRouteMapName() ->") + msg_p->getRouteMapName());
            trace(TRACE_LEVEL_INFO, string("sendShowToSsmDaemon -- msg_p->getIfType() ->") + msg_p->getIfType());

            if ((msg_p->getIfType() == IF_TYPE_TE) || (msg_p->getIfType() == IF_TYPE_GI)
                    || (msg_p->getIfType() == IF_TYPE_FO))
            {
                /* Three tuple to two tuple conversion */
                if (NsmUtils::isValidThreeTuple(msg_p->getIfName()))
                    NsmUtils::getTwoTupleIfName(msg_p->getIfName(), ifName);
                else
                    ifName = msg_p->getIfName();

                trace(TRACE_LEVEL_INFO, string("sendShowToSsmDaemon -- msg_p->getIfName() ->") + msg_p->getIfName());

                if (msg_p->getIfType() == IF_TYPE_TE)
                    snprintf(buffer, bufferSize, "te%s", ifName.c_str());
                else if (msg_p->getIfType() == IF_TYPE_GI)
                    snprintf(buffer, bufferSize, "gi%s", ifName.c_str());
                else if (msg_p->getIfType() == IF_TYPE_FO)
                    snprintf(buffer, bufferSize, "fo%s", ifName.c_str());
            }
            else if (msg_p->getIfType () == IF_TYPE_PO)
                snprintf(buffer, bufferSize, "po%s", msg_p->getIfName ().c_str());
            else if (msg_p->getIfType () == IF_TYPE_VE)
                snprintf(buffer, bufferSize, "ve0.%s", msg_p->getIfName ().c_str());
            else
                snprintf(buffer, bufferSize, "%s", msg_p->getIfName ().c_str());

            mappedId = DcmToolKit::getLocalMappedId();

            trace(TRACE_LEVEL_INFO, string("sendShowToSsmDaemon -- mappedId->") + mappedId);

            if (mappedId == (-1))
                mappedId = 0;

            trace(TRACE_LEVEL_INFO, string("sendShowToSsmDaemon -- buffer->") + buffer);

            cl_msg_p->setRouteMapName(msg_p->getRouteMapName ());
            cl_msg_p->setIfName(buffer);
            cl_msg_p->setOpCode(msg_p->getOpCode());
            cl_msg_p->setMappedId(mappedId);
            cl_msg_p->setLastStanza(msg_p->getLastStanza());

            trace (TRACE_LEVEL_INFO, "sendToLocalNode: Message being sent to Ssm");

            status = sendSynchronouslyToWaveClient ("ssm", cl_msg_p);

            if (status == WAVE_MESSAGE_SUCCESS)
            {
                if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
                {
                    trace(TRACE_LEVEL_INFO, string("Resp: Client Completion Error MsgId: ") +
                            cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                    msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }

                if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS)
                {
                    trace(TRACE_LEVEL_INFO, string("cl_msg_p->getClientStatus() :") + cl_msg_p->getClientStatus());
                    trace(TRACE_LEVEL_INFO, string("Resp:  Client Error MsgId: ") +
                            cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus()));
                    msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
                    status = msg_p->getCompletionStatus();
                    if(cl_msg_p)
                        delete cl_msg_p;
                    return status;
                }
            }

            if (status != WAVE_MESSAGE_SUCCESS)
            {
                trace(TRACE_LEVEL_INFO, string("Client Message sending failed for MsgId: ") +
                        cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
                if(cl_msg_p)
                    delete cl_msg_p;
                return status;
            }

            trace(TRACE_LEVEL_INFO, string("Client Message processing succesful for MsgId: ") + cl_msg_p->getMessageId());
            msg_p->copyAllBuffers(*cl_msg_p);
            trace(TRACE_LEVEL_INFO, "sendToLocalNode: Successfully protocol daemon");

            status = cl_msg_p->getCompletionStatus();
            if(cl_msg_p)
                delete cl_msg_p;
            return status;
        }
        else
        {
            status = WAVE_MESSAGE_ERROR;
            trace (TRACE_LEVEL_ERROR, "sendToLocalNode: Message creation failed");
        }
        if(cl_msg_p)
            delete cl_msg_p;
        return status;
    }



}
