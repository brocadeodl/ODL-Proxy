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

#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/NasConfigManagedObject.h"
#include "Sflow/Global/SflowProfileManagedObject.h"
#include "Policer/Global/PolicyClassMapManagedObject.h"
#include "Nsm/Local/FcoeWorker/FcoeportAttrLocalManagedObject.h"
#include "Nsm/Global/FcoeWorker/FcoeportAttrGlobalManagedObject.h"
#include "Nsm/Local/PfcFlowControlLocalManagedObject.h"
#include "Nsm/Local/QosCosToProfileIdManagedObject.h"
#include "Nsm/Local/QosCosToTailDropManagedObject.h"
#include "Qos/Global/DscpMarkListManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Qos/Common/QosUtils.h"
#include "Qos/Global/DscpBitMap.h"
#include "Framework/Types/UI32Range.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "DcmCStatus.h"
#include "Policer/Global/ClassMapManagedObject.h"
#include "Policer/Global/PoClassMapManagedObject.h"



namespace DcmNs
{

  ResourceId QosUtils:: getQosMapObjectIdByMapName(string mapName, ObjectId &mapObjectId, QosMapType mapType,
  PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
    trace(TRACE_LEVEL_DEBUG,string ("getQosMapObjectIdByMapName"));
    string className;
    string columnName;
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    switch (mapType)
    {
      case COS_MUTATION_MAP:
      {
        className = CosToCosMutationMapManagedObject::getClassName();
        columnName = "cosToCosMutationMapName";
        break;
      }
      case COS_TRAFFIC_CLASS_MAP:
      {
        className = CosToTrafficClassMapManagedObject::getClassName();
        columnName = "cosToTrafficClassMapName";
        break;
      }
	  case DSCP_MUTATION_MAP:
      {
        className = DscpToDscpMutationMapManagedObject::getClassName();
        columnName = "dscpToDscpMutationMapName";
        break;
      }
      case DSCP_TRAFFIC_CLASS_MAP:
      {
        className = DscpToTrafficClassMapManagedObject::getClassName();
        columnName = "dscpToTrafficClassMapName";
        break;
      }
      case DSCP_COS_MAP:
      {
        className = DscpToCosMapManagedObject::getClassName();
        columnName = "dscpToCosMapName";
        break;
      }
      case CEE_MAP: {
          className = CeeMapManagedObject::getClassName();
          columnName = "ceeMapName";
    	  break;
      }
      case SFLOW_POLICY_MAP : {
          className = SflowProfileManagedObject::getClassName();
          columnName = "profileName";
    	  break;
      }
      case MQC_POLICY_MAP : {
          className = PolicyClassMapManagedObject::getClassName();
          columnName = "policyClassMapName";
    	  break;
      }
      default:
      {
        return(WAVE_MESSAGE_ERROR);
      }
    }

    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
    syncQueryCtxt.addAndAttribute (new AttributeString(&mapName, columnName));
    syncQueryCtxt.addSelectField("objectId");
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (pResults && pResults->size() == 1)
    {
      mapObjectId = (*pResults)[0]->getObjectId();
      trace(TRACE_LEVEL_DEBUG,string ("Fetched ObjectId for map") + mapName);
      status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
      status = WAVE_MESSAGE_ERROR;
    }
    if (pResults) {
      if(pPrismLinearSequencerContext) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        pResults->clear();
        delete pResults;
      } else {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
      }
    }
    return (status);
  }

  /* API : getReservePriorityBitMapFromCeeMap
   * Description : Computes BitMap which indicates the COS values for which PFC
   * is enabled.
   */

  ResourceId QosUtils::getReservePriorityBitMapFromCeeMap(int &reservePriorityBitmap)
  {
    trace(TRACE_LEVEL_DEBUG, "getReservePriorityBitMapFromCeeMap");
	ResourceId status = WAVE_MESSAGE_SUCCESS;
    // Global "default" CEE map is created intially, and only this single CEE map is allowed globally.
    // search for this map first.
    CeeMapManagedObject *pCeeMapManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
    syncQueryCtxt.addAndAttribute (new AttributeString("default","ceeMapName"));
    vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

    if (pResults && pResults->size() > 0) {
      pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
    }
    else {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: no default CEE created yet");
      if (pResults)
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
      return (WRC_SSM_DCM_QOS_VCS_ERROR);
    }

    int fabricPriority = 7;

    // find the fabric priority and lossless priority
    WaveManagedObjectPointer<PriorityMapManagedObject> p_pm;
    p_pm = pCeeMapManagedObject->getPriorityMap();
    if (p_pm.operator -> () == NULL) {
      trace (TRACE_LEVEL_INFO, "vcsQosSanity: Void CEE");
      if (pResults)
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
      if (FrameworkToolKit::isConfigReplayInProgress())
        return (WAVE_MESSAGE_SUCCESS);
      else
        return (WRC_SSM_DCM_QOS_VCS_ERROR);
    }

    PriorityGroupManagedObject *p_pg = NULL;
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos0ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x1;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos1ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x2;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos2ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x4;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos3ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE map priority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x8;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos4ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mappriority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x10;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos5ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mappriority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x20;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos6ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mappriority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x40;
      delete p_pg;
    }
    p_pg = dynamic_cast<PriorityGroupManagedObject *> (queryManagedObject(p_pm->getCos7ToPriorityGrouMap()));
    if (p_pg == NULL) {
      trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for default CEE mappriority-group-table failed");
      status = (WRC_SSM_DCM_QOS_VCS_ERROR);
    }
    else {
      if (p_pg->getPfc())
        reservePriorityBitmap |= 0x80;
      delete p_pg;
    }

    if (pResults)
      WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    if (status != WAVE_MESSAGE_SUCCESS)
      return status;

    reservePriorityBitmap |= (1<<fabricPriority);

	return (WRC_WAVE_MESSAGE_SUCCESS);
  }

  /*
 * API : checkMarkListCompatibilityWithCeeMap
 * Description : This API checks if the DSCP markTo values conflict with
 * lossless cos values in cee-map
 */

bool QosUtils::checkMarkListCompatibilityWithCeeMap(
		vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector,
		int reservePriorityBitmap) {

	int bitMap = 0;
	UI32 i = 0;
	vector<UI32> valVector;
	vector<UI32> valVector1;
	vector<UI32> valVector2;

	trace(
			TRACE_LEVEL_DEBUG,
			string(
					"checkMarkListCompatibilityWithCeeMap reservePriorityBitmap=")
					+ reservePriorityBitmap);

	for (i = 0; i < markListVector.size(); i++) {
		bitMap |= (1 << markListVector[i]->getToValue());
	}

	trace(TRACE_LEVEL_DEBUG,
			string("checkMarkListCompatibilityWithCeeMap bitMap=") + bitMap);

	//Check whether any marked cos/tc conflicts with the lossless cos.
	if (reservePriorityBitmap & bitMap) {
		return false;
	}

	//Unmarked cos/tc values, which are configured by default can also conflict with lossless cos.
	//For e.g:dscp-cos map has dscp values 24-31 mapped to 3 and 56-63 mapped to 7 by default.
	//The default dscp map is always bound to conflict with the default cee.
	//Even if the user maps dscp 56-63 to other than 3/7 cos, then also dscp 24-31 continue to map to cos 3
	//which is still in conflict with default cee.

	//Expand 8 bit cee priority map to 64 bit map.
	DscpBitMap ceeMapExpanded(64);
	ceeMapExpanded.reset();

	int position = 0;

	if (reservePriorityBitmap & 0x1) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 8;
	if (reservePriorityBitmap & 0x2) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 16;
	if (reservePriorityBitmap & 0x4) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 24;
	if (reservePriorityBitmap & 0x8) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 32;
	if (reservePriorityBitmap & 0x10) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 40;
	if (reservePriorityBitmap & 0x20) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 48;
	if (reservePriorityBitmap & 0x40) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	position = 56;
	if (reservePriorityBitmap & 0x80) {
		for (i = 0; i < 8; i++, position++) {
			ceeMapExpanded.setValue(position, 1);
		}
	}
	ceeMapExpanded.getValuesInVector(valVector);

	trace(TRACE_LEVEL_DEBUG,
			string("checkMarkListCompatibilityWithCeeMap ceeMapExpanded<"));

	for (unsigned int j = 0; j < valVector.size(); j++) {
		trace(TRACE_LEVEL_DEBUG, string("") + valVector[j]);
	}
	trace(TRACE_LEVEL_DEBUG, string(">"));

	//Construct 64 bit dscp mark list map.
	DscpBitMap dscpMarkListBitMap(64);
	dscpMarkListBitMap.reset();

	for (i = 0; i < markListVector.size(); i++) {
		vector<UI32> markListVectore;
		string markListString = markListVector[i]->getMarkList();
		UI32Range(markListString).getUI32RangeVector(markListVectore);
		dscpMarkListBitMap.setValuesFromVector(markListVectore);
	}

	dscpMarkListBitMap.getValuesInVector(valVector1);

	trace(TRACE_LEVEL_DEBUG,
			string("checkMarkListCompatibilityWithCeeMap dscpMarkListBitMap<"));

	for (unsigned int k = 0; k < valVector1.size(); k++) {
		trace(TRACE_LEVEL_DEBUG, string("") + valVector1[k]);
	}
	trace(TRACE_LEVEL_DEBUG, string(">"));

	dscpMarkListBitMap.andWith(ceeMapExpanded);

	dscpMarkListBitMap.getValuesInVector(valVector2);

	trace(TRACE_LEVEL_DEBUG,
			string("checkMarkListCompatibilityWithCeeMap ceeMapExpanded<"));

	for (unsigned int l = 0; l < valVector2.size(); l++) {
		trace(TRACE_LEVEL_DEBUG, string("") + valVector2[l]);
	}
	trace(TRACE_LEVEL_DEBUG, string(">"));

	//And the 2 64 bit maps and verify whethere all the lossless dscp bits are mapped to lossy cos.
	if (!(dscpMarkListBitMap == ceeMapExpanded)) {
		return false;
	}

	return true;
}

  bool QosUtils::doesMapExist(QosMapType mapType, string mapName, ResourceId &status)
  {
    UI32 count = 0;
    string className;
    string columnName;

    switch (mapType)
    {
      case COS_MUTATION_MAP:
      {
        className = CosToCosMutationMapManagedObject::getClassName();
        columnName = "cosToCosMutationMapName";
        break;
      }
      case COS_TRAFFIC_CLASS_MAP:
      {
        className = CosToTrafficClassMapManagedObject::getClassName();
        columnName = "cosToTrafficClassMapName";
        break;
      }
	  case DSCP_MUTATION_MAP:
      {
        className = DscpToDscpMutationMapManagedObject::getClassName();
        columnName = "dscpToDscpMutationMapName";
        break;
      }
      case DSCP_TRAFFIC_CLASS_MAP:
      {
        className = DscpToTrafficClassMapManagedObject::getClassName();
        columnName = "dscpToTrafficClassMapName";
        break;
      }
      case DSCP_COS_MAP:
      {
        className = DscpToCosMapManagedObject::getClassName();
        columnName = "dscpToCosMapName";
        break;
      }
      case CEE_MAP:
	  {
		className =  CeeMapManagedObject::getClassName();
		columnName = "ceeMapName";
		break;
	  }
      case SFLOW_POLICY_MAP: {
  		className =  SflowProfileManagedObject::getClassName();
  		columnName = "profileName";
  		break;
      }
      case MQC_POLICY_MAP: {
    	  className = PolicyClassMapManagedObject::getClassName();
    	  columnName = "policyClassMapName";
    	  break;
      }
      default:
      {
		status = WAVE_MESSAGE_ERROR;
        return false;
      }
    }

    WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
    syncQueryCtxt.addAndAttribute (new AttributeString(&mapName, columnName));
    status = querySynchronouslyForCount(&syncQueryCtxt, count);
    if (status != WAVE_MESSAGE_SUCCESS)
    {
      trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount failed for map:") + mapName);
      status = WRC_WAVE_MESSAGE_ERROR;
      return false;
    }
    else
    {
      if (count == 0)
      {
        status = WRC_SSM_QOS_MAP_NOT_FOUND;
        return false;
      }
    }
    status = WRC_WAVE_MESSAGE_SUCCESS;
    return true;
  }


  /*
   * API : vcsL3QosSanity
   * Description : This API does VCS related validations for L3 Qos module.
   * Input :
   *  oper - QOS_INTERFACE_CEE_MAP || QOS_INTERFACE_DSCP_TRAFFIC_CLASS || QOS_INTERFACE_DSCP_COS 
   *  mapName - Qos map Name
   *  apply - Set to TRUE if a map is getting applied on an interface.
   *    In case of global map changes this attribute is set to FALSE
   *  reservePriorityBitMap - This value is passed as input paramater for oper
   *  QOS_INTERFACE_CEE_MAP. This bitMap indicates the cos values that are
   *  either lossless or of fabric priority
   *  markToValue - This value is set when the mark value in global map is changed.
   */

  ResourceId QosUtils::vcsL3QosSanity(SI32 oper, string mapName, bool apply, int reservePriorityBitmap, SI32 markToValue)
  {
    UI32 i = 0;
    PoPhyIntfManagedObject *pPoPhyIntfManagedObject = NULL;
    vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector;

    // this sanity only applies to VCS mode
    if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED) {
      return (WAVE_MESSAGE_SUCCESS);
    }

    if ((oper != QOS_INTERFACE_CEE_MAP) && (oper != QOS_INTERFACE_DSCP_TRAFFIC_CLASS) && (oper != QOS_INTERFACE_DSCP_COS)) {
      return (WAVE_MESSAGE_SUCCESS);
    }

    trace (TRACE_LEVEL_DEBUG, string ("vcsL3QosSanity reservePriorityBitmap=") + reservePriorityBitmap);
    trace (TRACE_LEVEL_DEBUG, string ("vcsL3QosSanity markToValue=") + markToValue);

    if (oper == QOS_INTERFACE_CEE_MAP) {

      trace (TRACE_LEVEL_DEBUG, "vcsL3QosSanity QOS_INTERFACE_CEE_MAP");

      // traverse the interfaces with DSCP-TRAFFIC_CLASS, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext syncQueryCtxtDscpTc(PoPhyIntfManagedObject::getClassName());
	  syncQueryCtxtDscpTc.addSelectField("qosDscpTrafficClassMap");
      /*syncQueryCtxtDscpTc.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "qosDscpTrafficClassMap", PoPhyIntfManagedObject::getClassName()),
          WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/

      vector<WaveManagedObject *> *pResultsDscpTc  = querySynchronously (&syncQueryCtxtDscpTc);

	  DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;

      if (pResultsDscpTc != NULL)
	  {
        for (i = 0; i < pResultsDscpTc->size(); i++)
        {
          pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject*>((*pResultsDscpTc)[i]);
          if (pPoPhyIntfManagedObject->getQosDscpTrafficClassMap() != ObjectId::NullObjectId)
          {
            pDscpToTrafficClassMapManagedObject =
			  dynamic_cast<DscpToTrafficClassMapManagedObject *> (queryManagedObject(pPoPhyIntfManagedObject->getQosDscpTrafficClassMap()));
            if (pDscpToTrafficClassMapManagedObject == NULL) {
              trace(TRACE_LEVEL_ERROR, "vcsL3QosSanity failed: searching for DscpToTrafficClassMapManagedObject failed");
              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpTc);
              return (WRC_SSM_DCM_QOS_VCS_ERROR);
            }
            else {
			  markListVector = pDscpToTrafficClassMapManagedObject->getDscpMarkList(); 
			  if (!checkMarkListCompatibilityWithCeeMap (markListVector, reservePriorityBitmap)) {
				  WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpTc);
                  delete pDscpToTrafficClassMapManagedObject;
				  return WRC_SSM_DCM_QOS_VCS_DSCP_TC;
              }
            }
            if(NULL != pDscpToTrafficClassMapManagedObject) {
            	delete pDscpToTrafficClassMapManagedObject;
            }
          }
        }
	  }

	  if (pResultsDscpTc)
	  {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpTc);
	  }

      // traverse the policy-maps with DSCP-TRAFFIC_CLASS, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext qosServicePolicySyncQueryCtxt(PoClassMapManagedObject::getClassName());
      qosServicePolicySyncQueryCtxt.addSelectField("dscpTrafficClassMapId");
      /*qosServicePolicySyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "dscpTrafficClassMapId", PoClassMapManagedObject::getClassName()),
          WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/

      vector<WaveManagedObject *> *pResultsDscpTc2  = querySynchronously (&qosServicePolicySyncQueryCtxt);
      PoClassMapManagedObject *pPoClassMapManagedObject = NULL;

      pDscpToTrafficClassMapManagedObject = NULL;

      if (pResultsDscpTc2 != NULL)
      {
        for (i = 0; i < pResultsDscpTc2->size(); i++)
        {
          pPoClassMapManagedObject = dynamic_cast<PoClassMapManagedObject*>((*pResultsDscpTc2)[i]);
          if (pPoClassMapManagedObject->getDscpTrafficClassMap() != ObjectId::NullObjectId)
          {
            pDscpToTrafficClassMapManagedObject =
              dynamic_cast<DscpToTrafficClassMapManagedObject *> (queryManagedObject(pPoClassMapManagedObject->getDscpTrafficClassMap()));
            if (pDscpToTrafficClassMapManagedObject == NULL) {
              trace(TRACE_LEVEL_ERROR, "vcsL3QosSanity failed: searching for DscpToTrafficClassMapManagedObject failed");
              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpTc2);
              return (WRC_SSM_DCM_QOS_VCS_ERROR);
            }
            else {
              markListVector = pDscpToTrafficClassMapManagedObject->getDscpMarkList();
              if (!checkMarkListCompatibilityWithCeeMap (markListVector, reservePriorityBitmap))
              {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpTc2);
                delete pDscpToTrafficClassMapManagedObject;
                return WRC_SSM_DCM_QOS_VCS_DSCP_TC;
              }
            }
            if(NULL != pDscpToTrafficClassMapManagedObject) {
            	delete pDscpToTrafficClassMapManagedObject;
            }
          }
        }
      }

      if (pResultsDscpTc2) {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpTc2);
      }


    // traverse the interfaces with DSCP-COS, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext syncQueryCtxtDscpCos(PoPhyIntfManagedObject::getClassName());
	  syncQueryCtxtDscpCos.addSelectField("qosDscpCosMap");
      /*syncQueryCtxtDscpCos.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "qosDscpCosMap", PoPhyIntfManagedObject::getClassName()),
          WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/

      vector<WaveManagedObject *> *pResultsDscpCos  = querySynchronously (&syncQueryCtxtDscpCos);

      pPoPhyIntfManagedObject = NULL;
	  DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;

      if (pResultsDscpCos != NULL)
      {
        for (i = 0; i < pResultsDscpCos->size(); i++)
        {
          pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject*>((*pResultsDscpCos)[i]);
          if (pPoPhyIntfManagedObject->getQosDscpCosMap() != ObjectId::NullObjectId)
          {
            pDscpToCosMapManagedObject =
			  dynamic_cast<DscpToCosMapManagedObject*> (queryManagedObject(pPoPhyIntfManagedObject->getQosDscpCosMap()));
            if (pDscpToCosMapManagedObject == NULL) {
              trace(TRACE_LEVEL_ERROR, "vcsL3QosSanity failed: searching for DscpToCosMapManagedObject failed");
              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpCos);
              return (WRC_SSM_DCM_QOS_VCS_ERROR);
            }
            else {
			  markListVector = pDscpToCosMapManagedObject->getDscpMarkList(); 
			  if (!checkMarkListCompatibilityWithCeeMap(markListVector, reservePriorityBitmap))
              {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpCos);
                delete pDscpToCosMapManagedObject;
                return WRC_SSM_DCM_QOS_VCS_DSCP_COS;
              }
            }
            if(NULL != pDscpToCosMapManagedObject) {
            	delete pDscpToCosMapManagedObject;
            }
          }
        }
	  }

	  if (pResultsDscpCos)
	  {
		WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpCos);
	  }


    // traverse the qos policy-maps with DSCP-COS, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext qosServicePolicysyncQueryCtxtDscpCos(PoClassMapManagedObject::getClassName());
      qosServicePolicysyncQueryCtxtDscpCos.addSelectField("dscpCosMapId");
      /*qosServicePolicysyncQueryCtxtDscpCos.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "dscpCosMapId", PoClassMapManagedObject::getClassName()),
          WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/

      vector<WaveManagedObject *> *pResultsDscpCos2  = querySynchronously (&qosServicePolicysyncQueryCtxtDscpCos);

      pPoClassMapManagedObject = NULL;
      pDscpToCosMapManagedObject = NULL;

      if (pResultsDscpCos2 != NULL)
      {
        for (i = 0; i < pResultsDscpCos2->size(); i++)
        {
          pPoClassMapManagedObject = dynamic_cast<PoClassMapManagedObject*>((*pResultsDscpCos2)[i]);
          if (pPoClassMapManagedObject->getDscpCoSMap() != ObjectId::NullObjectId)
          {
            pDscpToCosMapManagedObject =
              dynamic_cast<DscpToCosMapManagedObject*> (queryManagedObject(pPoClassMapManagedObject->getDscpCoSMap()));
            if (pDscpToCosMapManagedObject == NULL) {
              trace(TRACE_LEVEL_ERROR, "vcsL3QosSanity failed: searching for DscpToCosMapManagedObject failed");
              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpCos2);
              return (WRC_SSM_DCM_QOS_VCS_ERROR);
            }
            else {
              markListVector = pDscpToCosMapManagedObject->getDscpMarkList();
              if (!checkMarkListCompatibilityWithCeeMap(markListVector, reservePriorityBitmap))
              {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpCos2);
                delete pDscpToCosMapManagedObject;
                return WRC_SSM_DCM_QOS_VCS_DSCP_COS;
              }
            }
            if(NULL != pDscpToCosMapManagedObject) {
            	delete pDscpToCosMapManagedObject;
            }
          }
        }
      }

      if (pResultsDscpCos2)
      {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResultsDscpCos2);
      }

    }

	else {

    /* compute reservePriorityBitmap. In case of CEE MAP changes,
     * 'reservePriorityBitmap' is passed as input parameter.
     */

	  if (!apply) // if mark value was changed in a map 
	  {
        int bitMap = 0;
	    bitMap = 1 << markToValue;
	    if (bitMap & reservePriorityBitmap)
	    {
          return WRC_SSM_DCM_QOS_VCS_ERROR; 
	    }
	  }
	  else // a map is getting applied on an interface
	  {
        if (oper == QOS_INTERFACE_DSCP_TRAFFIC_CLASS)
	    {
          DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
          WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToTrafficClassMapManagedObject::getClassName());
          syncQueryCtxt.addAndAttribute (new AttributeString(&mapName, "dscpToTrafficClassMapName"));
          vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

          if (NULL != pResults)
          {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults)
            {
              trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
              return (WRC_SSM_QOS_MAP_NOT_FOUND);
            }
			else
            {
              pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
              markListVector = pDscpToTrafficClassMapManagedObject->getDscpMarkList();
              if (!checkMarkListCompatibilityWithCeeMap(markListVector, reservePriorityBitmap))
              {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                return WRC_SSM_DCM_QOS_VCS_DSCP_TC;
              }
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
          }
          else
          {
            return (WRC_SSM_QOS_MAP_NOT_FOUND);
          }
        }
        if (oper == QOS_INTERFACE_DSCP_COS)
        {
          DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
          WaveManagedObjectSynchronousQueryContext dscpToCoSsyncQueryCtxt(DscpToCosMapManagedObject::getClassName());
          dscpToCoSsyncQueryCtxt.addAndAttribute (new AttributeString(&mapName, "dscpToCosMapName"));
          vector<WaveManagedObject *> *pDscpCoSResults = querySynchronously (&dscpToCoSsyncQueryCtxt);

          if (NULL != pDscpCoSResults)
          {
            UI32 numberOfResults = pDscpCoSResults->size ();
            if (0 == numberOfResults)
            {
              trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
              return (WRC_SSM_QOS_MAP_NOT_FOUND);
            }
			else
            {
              pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pDscpCoSResults)[0]);
              markListVector = pDscpToCosMapManagedObject->getDscpMarkList();
              if (!checkMarkListCompatibilityWithCeeMap(markListVector, reservePriorityBitmap))
              {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDscpCoSResults);
                return WRC_SSM_DCM_QOS_VCS_DSCP_COS;
              }
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pDscpCoSResults);
          }
          else
          {
            return (WRC_SSM_QOS_MAP_NOT_FOUND);
          }
        }
	  }
	}
	return (WRC_WAVE_MESSAGE_SUCCESS);
  }

  /*
   * API : vcsQosSanity
   * Description : This API does VCS related validations for Qos module.
   * Input :
   *  oper - QOS_INTERFACE_CEE_MAP || QOS_INTERFACE_COS_MUTATION
   *  mapName - Qos map Name
   *  apply - Set to TRUE if a map is getting applied on an interface.
   *    In case of global map changes this attribute is set to FALSE
   *  cosMutationBitMap - Bit map indicating the mutated cos values.
   *    This is set in case of global cos-mutation map creation/ modification
   *  reservePriorityBitMap - This value is passed as input paramater for oper QOS_INTERFACE_CEE_MAP
   */

  ResourceId QosUtils::vcsQosSanity(SI32 oper, string mapName, bool apply, int cosMutationBitmap, int reservePriorityBitmap, int defaultCoS)
  {
    ResourceId status = WAVE_MESSAGE_SUCCESS;
	int bitMap = 0;
	UI32 i = 0;

    // this sanity only applies to VCS mode
    if (DcmToolKit::isVcsEnabled () != WAVE_PERSISTENCE_CLUSTER_ENABLED) {
      return (WAVE_MESSAGE_SUCCESS);
    }
    
    //The below 2 defects contradict each other.
    //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
    //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

    if ((oper != QOS_INTERFACE_CEE_MAP) && (oper != QOS_INTERFACE_COS_MUTATION) /*&& (oper != QOS_INTERFACE_DEFAULT_COS)*/) {
      return (WAVE_MESSAGE_SUCCESS);
    }

	/* compute reservePriorityBitmap. In case of CEE MAP changes,
	 * 'reservePriorityBitmap' is passed as input parameter. 
     */
	if (oper != QOS_INTERFACE_CEE_MAP)
	{
	  reservePriorityBitmap = 0;
	  status = getReservePriorityBitMapFromCeeMap(reservePriorityBitmap);

	  if (status != WAVE_MESSAGE_SUCCESS)
	  {
        trace (TRACE_LEVEL_ERROR, "getReservePriorityBitMapFromCeeMap failed");
	    return (status);
	  }
	}

    if (oper == QOS_INTERFACE_CEE_MAP)
    {
      trace (TRACE_LEVEL_DEBUG, "QosGlobalWorker::vcsQosSanity, CEE apply");

      // traverse the non-CEE interface, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(PoPhyIntfManagedObject::getClassName());
      //syncQueryCtxt1.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "qosCosMap", PoPhyIntfManagedObject::getClassName()),
      //		  WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);
      syncQueryCtxt1.addSelectField("qosCosMap");
      syncQueryCtxt1.addSelectField("defaultCos");
      vector<WaveManagedObject *> *pResults1 = querySynchronously (&syncQueryCtxt1);


      PoPhyIntfManagedObject *pPoPhyIntfManagedObject = NULL;
      CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;


      if (pResults1 == NULL) /* Qos map is not applied on any interface */
        return (WRC_WAVE_MESSAGE_SUCCESS);

      for (i = 0; i < pResults1->size(); i ++)
      {
        pPoPhyIntfManagedObject = dynamic_cast<PoPhyIntfManagedObject*>((*pResults1)[i]);
        if (pPoPhyIntfManagedObject->getQosCosMap() != ObjectId::NullObjectId)
        {
          pCosToCosMutationMapManagedObject =
			  dynamic_cast<CosToCosMutationMapManagedObject *> (queryManagedObject(pPoPhyIntfManagedObject->getQosCosMap()));
          if (pCosToCosMutationMapManagedObject == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for CosMutation map failed");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults1);
            return (WRC_SSM_DCM_QOS_VCS_ERROR);
          }
          else {
		    bitMap = 0;
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos0MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos1MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos2MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos3MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos4MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos5MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos6MutatedCosValue());
		    bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos7MutatedCosValue());

            if ((bitMap & reservePriorityBitmap) != 0) {
              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults1);
              delete pCosToCosMutationMapManagedObject;
              trace(TRACE_LEVEL_ERROR, string ("vcsQosSanity failed:mutated CoS conflict with lossless priority or fabric priority in map ") + mapName);
              return (WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
            }
          }
          if(NULL != pCosToCosMutationMapManagedObject) {
        	  delete pCosToCosMutationMapManagedObject;
          }
        }
        //Validate whether default cos on an interface/PO conflicts with the lossless cos.
        /*int j = pPoPhyIntfManagedObject->getDefaultCos();
		if (((1 << j) & reservePriorityBitmap) != 0) {
			trace(
					TRACE_LEVEL_ERROR,
					string(
							"default Cos conflict with lossless priority or fabric priority on interface ")
							+ pPoPhyIntfManagedObject->getIfName());
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pResults1);
			return (WRC_SSM_DCM_QOS_VCS_COS_DEFAULT);
		} */
      }

     if (pResults1) {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults1);
     }


      // traverse the MQC maps, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext servicePolicySyncQueryCtxt(PoClassMapManagedObject::getClassName());
      /*servicePolicySyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "cosMutationMapId", PoClassMapManagedObject::getClassName()),
          WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/
      servicePolicySyncQueryCtxt.addSelectField("cosMutationMapId");
      servicePolicySyncQueryCtxt.addSelectField("cos");
      servicePolicySyncQueryCtxt.addSelectField("trafficClass");
      vector<WaveManagedObject *> *pResults3 = querySynchronously (&servicePolicySyncQueryCtxt);


      PoClassMapManagedObject *pPoClassMapManagedObject = NULL;
      pCosToCosMutationMapManagedObject = NULL;


      if (pResults3 == NULL) /* Qos map is not applied on any interface */
        return (WRC_WAVE_MESSAGE_SUCCESS);

      for (i = 0; i < pResults3->size(); i ++)
      {
        pPoClassMapManagedObject = dynamic_cast<PoClassMapManagedObject*>((*pResults3)[i]);
        if (pPoClassMapManagedObject->getCoSMutationMap() != ObjectId::NullObjectId)
        {
          pCosToCosMutationMapManagedObject =
              dynamic_cast<CosToCosMutationMapManagedObject *> (queryManagedObject(pPoClassMapManagedObject->getCoSMutationMap()));
          if (pCosToCosMutationMapManagedObject == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for CosMutation map failed");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults3);
            return (WRC_SSM_DCM_QOS_VCS_ERROR);
          }
          else {
            bitMap = 0;
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos0MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos1MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos2MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos3MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos4MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos5MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos6MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos7MutatedCosValue());

                if ((bitMap & reservePriorityBitmap) != 0) {
                  trace(TRACE_LEVEL_ERROR, string ("vcsQosSanity failed:mutated CoS conflict with lossless priority or fabric priority in map ") + mapName);
                  WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults3);
                  delete pCosToCosMutationMapManagedObject;
                  return (WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
                }
            }
          if(NULL != pCosToCosMutationMapManagedObject) {
        	  delete pCosToCosMutationMapManagedObject;
          }
        }
        
        //Validate whether default cos/tc on an policy-class-map conflicts with the lossless cos.
        //The below 2 defects contradict each other.
        //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
        //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

		/*int j = pPoClassMapManagedObject->getCoS();
		if (((1 << j) & reservePriorityBitmap) != 0) {
			trace(
					TRACE_LEVEL_ERROR,
					string(
							"default Cos conflict with lossless priority or fabric priority on interface "));
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pResults3);
			return (WRC_SSM_DCM_QOS_VCS_COS_DEFAULT);
		}
		j = pPoClassMapManagedObject->getTrafficClass();
		if (((1 << j) & reservePriorityBitmap) != 0) {
			trace(
					TRACE_LEVEL_ERROR,
					string(
							"default Cos conflict with lossless priority or fabric priority on interface "));
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					pResults3);
			return (WRC_SSM_DCM_QOS_VCS_COS_DEFAULT);
		}
        */
      }
    
      if(pResults3) {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults3);
      }

      // traverse the QoS profiles, searching for potential confliction
      WaveManagedObjectSynchronousQueryContext qosProfileSyncQueryCtxt(QosProfileCfgManagedObject::getClassName());
      /*pQosProfileCfgManagedObject.addAndAttribute(new AttributeObjectIdAssociation(ObjectId::NullObjectId, "cosMutationMapId", QosProfileCfgManagedObject::getClassName()),
          WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/
      qosProfileSyncQueryCtxt.addSelectField("defaultCosValue");
      qosProfileSyncQueryCtxt.addSelectField("cosMutation");
      vector<WaveManagedObject *> *qosProfileResults = querySynchronously (&qosProfileSyncQueryCtxt);


      QosProfileCfgManagedObject *pQosProfileCfgManagedObject = NULL;
      pCosToCosMutationMapManagedObject = NULL;


      if (qosProfileResults == NULL) /* Qos map is not applied on any interface */
        return (WRC_WAVE_MESSAGE_SUCCESS);

      for (i = 0; i < qosProfileResults->size(); i ++)
      {
        pQosProfileCfgManagedObject = dynamic_cast<QosProfileCfgManagedObject*>((*qosProfileResults)[i]);
        if (pQosProfileCfgManagedObject->getCosMutation() != ObjectId::NullObjectId)
        {
          pCosToCosMutationMapManagedObject =
              dynamic_cast<CosToCosMutationMapManagedObject *> (queryManagedObject(pQosProfileCfgManagedObject->getCosMutation()));
          if (pCosToCosMutationMapManagedObject == NULL) {
            trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for CosMutation map failed");
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(qosProfileResults);
            return (WRC_SSM_DCM_QOS_VCS_ERROR);
          }
          else {
            bitMap = 0;
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos0MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos1MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos2MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos3MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos4MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos5MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos6MutatedCosValue());
            bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos7MutatedCosValue());

                if ((bitMap & reservePriorityBitmap) != 0) {
                  trace(TRACE_LEVEL_ERROR, string ("vcsQosSanity failed:mutated CoS conflict with lossless priority or fabric priority in map ") + mapName);
                  WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(qosProfileResults);
                  delete pCosToCosMutationMapManagedObject;
                  return (WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
                }
            }
          if(NULL != pCosToCosMutationMapManagedObject) {
        	  delete pCosToCosMutationMapManagedObject;
          }
        }
        //Validate whether default cos on an port-profile conflicts with the lossless cos.
		/*int j = pQosProfileCfgManagedObject->getDefaultCosValue();
		if (((1 << j) & reservePriorityBitmap) != 0) {
			trace(
					TRACE_LEVEL_ERROR,
					string(
							"default Cos conflict with lossless priority or fabric priority on interface "));
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(
					qosProfileResults);
			return (WRC_SSM_DCM_QOS_VCS_COS_DEFAULT);
		}*/
      }

      if(qosProfileResults) {
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(qosProfileResults);
      }


    }

    else if (oper == QOS_INTERFACE_COS_MUTATION)
    {
      UI32 count = 0;
      ObjectId cosMapObjectId;
      trace (TRACE_LEVEL_DEBUG, "QosGlobalWalker::vcsQosSanity, Cos_Mutation");
      // this is the case that Global CoS-Mutation map is updated. Need to check
      // if it is activated on any interface. If yes, need to check if the new
      // mapping conflicts with cee default configuration.
      if (!apply)
	  {
        status = QosUtils::getQosMapObjectIdByMapName(mapName, cosMapObjectId, COS_MUTATION_MAP, NULL);
        if (status != WAVE_MESSAGE_SUCCESS) /* map doesnot exist. New map creation */
        {
          trace (TRACE_LEVEL_DEBUG, string ("getQosMapObjectIdByMapName cosMap failed. Assuming as new map creation"));
          return(WRC_WAVE_MESSAGE_SUCCESS);
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt1(PoPhyIntfManagedObject::getClassName());
        syncQueryCtxt1.addAndAttribute(new AttributeObjectIdAssociation(cosMapObjectId, "qosCosMap", PoPhyIntfManagedObject::getClassName()));
        status = querySynchronouslyForCount(&syncQueryCtxt1, count);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
          trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosCosMap on PoPhy failed"));
          return(WRC_SSM_DCM_QOS_VCS_ERROR);
        }
        else
        {
          if (count != 0)
          {
            if (cosMutationBitmap & reservePriorityBitmap) {
              trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: mutated CoS conflict with lossless priority or fabric priority");
              return(WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
            }
          }
        }
        /*
         * Checking if Map was applied on any port-profile.
         */
        count = 0;
        WaveManagedObjectSynchronousQueryContext syncQosProfileQueryCtxt (QosProfileCfgManagedObject::getClassName());
        syncQosProfileQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(cosMapObjectId, "cosMutation", QosProfileCfgManagedObject::getClassName()));
        status = querySynchronouslyForCount (&syncQosProfileQueryCtxt, count);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
          trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosCosMap on PO failed"));
          return(WRC_SSM_DCM_QOS_VCS_ERROR);
        }
        else
        {
          if (count != 0)
          {
            if (cosMutationBitmap & reservePriorityBitmap) {
              trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: mutated CoS conflict with lossless priority or fabric priority");
              return(WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
            }
          }
        }

        /*
         * Checking if Map was applied on any qos service-policy.
         */
        count = 0;
        WaveManagedObjectSynchronousQueryContext syncQosServicePolicyQueryCtxt (PoClassMapManagedObject::getClassName());
        syncQosServicePolicyQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(cosMapObjectId, "cosMutationMapId", PoClassMapManagedObject::getClassName()));
        status = querySynchronouslyForCount (&syncQosServicePolicyQueryCtxt, count);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
          trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for cosMutationMapId on policy-map failed"));
          return(WRC_SSM_DCM_QOS_VCS_ERROR);
        }
        else
        {
          if (count != 0)
          {
            if (cosMutationBitmap & reservePriorityBitmap) {
              trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: mutated CoS conflict with lossless priority or fabric priority");
              return(WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
            }
          }
        }

      }
	  else /* this is the case where a qos map is applied on an interface or on a qos service-policy */
	  {
        // search for the CosToCosMutationMapManagedObject
        CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt2(CosToCosMutationMapManagedObject::getClassName());
        syncQueryCtxt2.addAndAttribute (new AttributeString(&mapName,"cosToCosMutationMapName"));
        vector<WaveManagedObject *> *pResults2 = querySynchronously (&syncQueryCtxt2);

        if (pResults2 && pResults2->size() > 0) {
          pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject*>((*pResults2)[0]);
          trace(TRACE_LEVEL_INFO, "Successfully found cos-mutation map object");
        }
        else {
          trace(TRACE_LEVEL_ERROR, "vcsQosSanity failed: searching for Cos Mutation map failed");
          if (pResults2)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults2);
          return (WRC_SSM_DCM_QOS_VCS_ERROR);
        }

        // since interval CEE map is applied on the ISL trunk initially, no needto check if CEE is activated on the interface.
        // check if the applied cos-mutation map conflict with reserved priorities.

		bitMap = 0;
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos0MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos1MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos2MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos3MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos4MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos5MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos6MutatedCosValue());
		bitMap |= (1 << pCosToCosMutationMapManagedObject->getCos7MutatedCosValue());

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults2);

        if ((bitMap & reservePriorityBitmap) != 0) {
          trace(TRACE_LEVEL_ERROR, string ("vcsQosSanity failed: mutated CoS conflict with lossless priority or fabric priority in map ") + mapName);
          return (WRC_SSM_DCM_QOS_VCS_COS_MUTATE);
        }
      }
    }
    //The below 2 defects contradict each other.
    //TR000407684 [FC: Change in behavior of "default CoS" from Hercules+ to Hydra.] => Added below case to fix thos.
    //and TR000454577 FC: PFC enable should be allowed on default-CoS to support end-to-end legacy pause.  => Commenting the below conde, to fix this.

    /*else if (QOS_INTERFACE_DEFAULT_COS == oper) {
		if (((1 << defaultCoS) & reservePriorityBitmap) != 0) {
			trace(
					TRACE_LEVEL_ERROR,
					string(
							"default CoS conflict with lossless priority or fabric priority on interface "));
			return (WRC_NSM_DCM_QOS_VCS_COS_DEFAULT);
		}
	}
    */
    return (WAVE_MESSAGE_SUCCESS);
  }

  bool QosUtils:: isInterfaceQosNonCeeConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
  {
    if ((QOS_DEFAULT_QOS_TRUST != pPhyIntfLocalManagedObject->getQosTrust()) ||
      (QOS_DEFAULT_FLOWCONTROL != pPhyIntfLocalManagedObject->getFlowControlRx())||
      (QOS_DEFAULT_FLOWCONTROL != pPhyIntfLocalManagedObject->getFlowControlTx())||
      (0 != pPhyIntfLocalManagedObject->getPfcFlowControl().size()) ||
      (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosTrafficClassMap())||
      (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosCosMap()) ||
	  (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosDscpMap()) ||
	  (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosDscpTrafficClassMap()) ||
	  (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosDscpCosMap()) ||
	  (QOS_DEFAULT_QOS_TRUST != pPhyIntfLocalManagedObject->getQosDscpTrust()) ||
      (0 != pPhyIntfLocalManagedObject->getCosProfile().size()) ||
      (0 != pPhyIntfLocalManagedObject->getCosTailDrop().size())
	  )
	{
      return (true);
	}
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceQosNonCeeConfigured(PoIntfManagedObject *pPoIntfManagedObject)
  {
    if ((QOS_DEFAULT_QOS_TRUST != pPoIntfManagedObject->getQosTrust()) ||
      (QOS_DEFAULT_FLOWCONTROL != pPoIntfManagedObject->getFlowControlRx())||
      (QOS_DEFAULT_FLOWCONTROL != pPoIntfManagedObject->getFlowControlTx())||
      (0 != pPoIntfManagedObject->getPfcFlowControl().size()) ||
      (ObjectId::NullObjectId != pPoIntfManagedObject->getQosTrafficClassMap())||
      (ObjectId::NullObjectId != pPoIntfManagedObject->getQosCosMap()) ||
	  (ObjectId::NullObjectId != pPoIntfManagedObject->getQosDscpMap()) ||
	  (ObjectId::NullObjectId != pPoIntfManagedObject->getQosDscpTrafficClassMap()) ||
	  (ObjectId::NullObjectId != pPoIntfManagedObject->getQosDscpCosMap()) ||
	  (QOS_DEFAULT_QOS_TRUST != pPoIntfManagedObject->getQosDscpTrust()) ||
      (0 != pPoIntfManagedObject->getCosProfile().size()) ||
      (0 != pPoIntfManagedObject->getCosTailDrop().size()))
	{
      return (true);
	}
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceQosMLSConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
  {
    if ((QOS_DEFAULT_FLOWCONTROL != pPhyIntfLocalManagedObject->getFlowControlRx())||
      (QOS_DEFAULT_FLOWCONTROL != pPhyIntfLocalManagedObject->getFlowControlTx())||
      (0 != pPhyIntfLocalManagedObject->getPfcFlowControl().size()) ||
      (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosTrafficClassMap())||
      (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosCosMap()) ||
	  (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosDscpMap()) ||
	  (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosDscpTrafficClassMap()) ||
	  (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getQosDscpCosMap()) ||
      (0 != pPhyIntfLocalManagedObject->getCosProfile().size()) ||
      (0 != pPhyIntfLocalManagedObject->getCosTailDrop().size())
	  )
	{
      return (true);
	}
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceQosMLSConfigured(PoIntfManagedObject *pPoIntfManagedObject)
  {
    if ((QOS_DEFAULT_FLOWCONTROL != pPoIntfManagedObject->getFlowControlRx())||
      (QOS_DEFAULT_FLOWCONTROL != pPoIntfManagedObject->getFlowControlTx())||
      (0 != pPoIntfManagedObject->getPfcFlowControl().size()) ||
      (ObjectId::NullObjectId != pPoIntfManagedObject->getQosTrafficClassMap())||
      (ObjectId::NullObjectId != pPoIntfManagedObject->getQosCosMap()) ||
	  (ObjectId::NullObjectId != pPoIntfManagedObject->getQosDscpMap()) ||
	  (ObjectId::NullObjectId != pPoIntfManagedObject->getQosDscpTrafficClassMap()) ||
	  (ObjectId::NullObjectId != pPoIntfManagedObject->getQosDscpCosMap()) ||
      (0 != pPoIntfManagedObject->getCosProfile().size()) ||
      (0 != pPoIntfManagedObject->getCosTailDrop().size()))
	{
      return (true);
	}
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceQosCeeConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
  {
    if (ObjectId::NullObjectId != pPhyIntfLocalManagedObject->getCeeMap())
    {
      return (true);
    }
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceQosCeeConfigured(PoIntfManagedObject *pPoIntfManagedObject)
  {
    if (ObjectId::NullObjectId != pPoIntfManagedObject->getCeeMap())
    {
      return (true);
    }
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceFcoeConfigured(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
  {
    WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> pFcoeportAttrLocalManagedObject;
    pFcoeportAttrLocalManagedObject = WaveManagedObjectPointer<FcoeportAttrLocalManagedObject> (pPhyIntfLocalManagedObject->getFcoeport());
    if (NULL != pFcoeportAttrLocalManagedObject.operator-> ())
    {
      return (true);
    }
    else
    {
      return (false);
    }
  }

  bool QosUtils:: isInterfaceFcoeConfigured(PoIntfManagedObject *pPoIntfManagedObject)
  {
    WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> pFcoeportAttrGlobalManagedObject;
    pFcoeportAttrGlobalManagedObject = WaveManagedObjectPointer<FcoeportAttrGlobalManagedObject> (pPoIntfManagedObject->getFcoeport());
    if (NULL != pFcoeportAttrGlobalManagedObject.operator-> ())
    {
      return (true);
    }
    else
    {
      return (false);
    }
  }

    bool QosUtils::isInterfaceMQCConfigured(
			PoIntfManagedObject *pPoIntfManagedObject) {
		return ((ObjectId::NullObjectId != pPoIntfManagedObject->m_inputPolicyMapId)
				|| (ObjectId::NullObjectId
						!= pPoIntfManagedObject->m_outputPolicyMapId));
	}

	bool QosUtils::isInterfaceMQCConfigured(
			PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject) {
		return ((ObjectId::NullObjectId
				!= pPhyIntfLocalManagedObject->m_inputPolicyMapId)
				|| (ObjectId::NullObjectId
						!= pPhyIntfLocalManagedObject->m_outputPolicyMapId));
	}

  ResourceId QosUtils:: getQosMapNameByObjectId(string &mapName, ObjectId mapObjectId, QosMapType mapType,
		  PrismLinearSequencerContext *pPrismLinearSequencerContext) {

		trace(TRACE_LEVEL_DEBUG, string("getQosMapNameByObjectId"));
		string className;
		string columnName;
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		switch (mapType) {
		case COS_MUTATION_MAP: {
			className = CosToCosMutationMapManagedObject::getClassName();
			columnName = "cosToCosMutationMapName";
			break;
		}
		case COS_TRAFFIC_CLASS_MAP: {
			className = CosToTrafficClassMapManagedObject::getClassName();
			columnName = "cosToTrafficClassMapName";
			break;
		}
		case DSCP_MUTATION_MAP: {
			className = DscpToDscpMutationMapManagedObject::getClassName();
			columnName = "dscpToDscpMutationMapName";
			break;
		}
		case DSCP_TRAFFIC_CLASS_MAP: {
			className = DscpToTrafficClassMapManagedObject::getClassName();
			columnName = "dscpToTrafficClassMapName";
			break;
		}
		case DSCP_COS_MAP: {
			className = DscpToCosMapManagedObject::getClassName();
			columnName = "dscpToCosMapName";
			break;
		}
		case CEE_MAP: {
			className = CeeMapManagedObject::getClassName();
			columnName = "ceeMapName";
			break;
		}
		case SFLOW_POLICY_MAP : {
	  		className =  SflowProfileManagedObject::getClassName();
	  		columnName = "profileName";
	  		break;
	      }
		default: {
			return (WAVE_MESSAGE_ERROR);
		}
		}

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
		syncQueryCtxt.addAndAttribute(
				new AttributeObjectId(mapObjectId, "objectId"));
		syncQueryCtxt.addSelectField(columnName);

		vector<WaveManagedObject *> *pResults = querySynchronously(&syncQueryCtxt);

		if (pResults && pResults->size() == 1) {

			switch(mapType)
			{
				case COS_MUTATION_MAP:
				{
					mapName =
							((dynamic_cast <CosToCosMutationMapManagedObject *> ((*pResults)[0])))->getCosToCosMutationMapName();
					break;
				}
				case COS_TRAFFIC_CLASS_MAP:
				{
					mapName =
							(dynamic_cast <CosToTrafficClassMapManagedObject*> ((*pResults)[0]))->getCosToTrafficClassMapName();
					break;
				}
				case DSCP_MUTATION_MAP:
				{
					mapName =
							(dynamic_cast <DscpToDscpMutationMapManagedObject*> ((*pResults)[0]))->getDscpToDscpMutationMapName();
					break;
				}
				case DSCP_TRAFFIC_CLASS_MAP:
				{
					mapName =
							(dynamic_cast <DscpToTrafficClassMapManagedObject*> ((*pResults)[0]))->getDscpToTrafficClassMapName();
					break;
				}
				case DSCP_COS_MAP:
				{
					mapName =
							(dynamic_cast <DscpToCosMapManagedObject*> ((*pResults)[0]))->getDscpToCosMapName();
					break;
				}
				case CEE_MAP:
				{
					mapName =
							(dynamic_cast <CeeMapManagedObject*> ((*pResults)[0]))->getCeeMapName();
					break;
				}
                case SFLOW_POLICY_MAP: {
                    mapName = (dynamic_cast <SflowProfileManagedObject*> ((*pResults)[0]))->getProfileName();
                    break;
                }
				default:
				{
					return (WAVE_MESSAGE_ERROR);
				}
			}
			trace(TRACE_LEVEL_DEBUG, string("Fetched mapName for map") + mapName);
			status = WAVE_MESSAGE_SUCCESS;
		} else {
			status = WAVE_MESSAGE_ERROR;
		}
		if (pResults) {
			if (pPrismLinearSequencerContext) {
				pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(
						*pResults);
				pResults->clear();
				delete pResults;
			} else {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
		}
		return (status);
  }


	ResourceId QosUtils::isMacACLReferredByAnyClassMap(
				const string &policyMapName) {

		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 count = 0;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
				ClassMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute(
				new AttributeString(policyMapName, "accessListName"));
		status = querySynchronouslyForCount(&syncQueryCtxt, count);
		trace(TRACE_LEVEL_DEBUG,string ("isMacACLReferredByAnyClassMap acl reference count ") + count);

		if ((WAVE_MESSAGE_SUCCESS == status) && (0 != count)) {
			status = WRC_SSM_DCM_ERR_MAC_ACL_ACTIVE_ON_A_CLASS_MAP;
		}

		return status;

	}

	/* 
	 * API - isCeeMapSetToDefault
	 * Description - Indicates if the cee-map is set to factory default or not
	 * Returns - WAVE_MESSAGE_SUCCESS if cee-map exists.
	 *			 Else, appropriate ResourceId.
	 */

	ResourceId QosUtils::isCeeMapSetToDefault() {
		string ceeMapName = "default";
		UI32 i = 0;
		ObjectId pgid1ObjectId, pgid2ObjectId, pgid150ObjectId;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		set <PriorityGroupInfo, PriorityGroupCompare> defaultPgSet;
		set <PriorityGroupInfo, PriorityGroupCompare> configuredPgSet;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CeeMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(ceeMapName,"ceeMapName"));
		vector<WaveManagedObject *> *pCeeMapResults = querySynchronously (&syncQueryCtxt);
		CeeMapManagedObject *pCeeMapManagedObject = NULL;

		if (NULL != pCeeMapResults) {
			UI32 numberOfResults = pCeeMapResults->size ();
			if (1 == numberOfResults) {
				pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pCeeMapResults)[0]);
			}
			else {
				trace (TRACE_LEVEL_ERROR, string("isCeeMapSetToDefault numberOfResults not one and is =") + numberOfResults);
				status = WAVE_MESSAGE_ERROR;
			}
		} else {
			trace (TRACE_LEVEL_ERROR, string("isCeeMapSetToDefault query is NULL"));
			status = WAVE_MESSAGE_ERROR;
		}

		if (status != WAVE_MESSAGE_SUCCESS) {
			if (pCeeMapResults) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCeeMapResults);
			}
			return (status);
		}

		vector<WaveManagedObjectPointer<PriorityGroupManagedObject> > pgid_vector = pCeeMapManagedObject->getPriorityGroup();
		WaveManagedObjectPointer<PriorityMapManagedObject> priorityMap = pCeeMapManagedObject->getPriorityMap();


		/* 1. if pgid has more than 10 entries (pgid1,pgid2 and 8 strict priorities), return error.*/
		if (pgid_vector.size() != 10) {
			trace (TRACE_LEVEL_ERROR, string("pgid vector is not 10. Value=") + pgid_vector.size());
			status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			if (pCeeMapResults) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCeeMapResults);
			}
			return (status);
		}

		/* 2. Check if all pgids are as that of default cee-map */

		/* 2.a. Populate defaultPgSet values. This is the factory setting */
			PriorityGroupInfo pgObject1(string("1"), (SI32)40, (SI32)1);
			defaultPgSet.insert (pgObject1);
			PriorityGroupInfo pgObject2(string("2"), (SI32)60, (SI32)0);
			defaultPgSet.insert (pgObject2);
			PriorityGroupInfo pgObject150(string("15.0"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject150);
			PriorityGroupInfo pgObject151(string("15.1"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject151);
			PriorityGroupInfo pgObject152(string("15.2"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject152);
			PriorityGroupInfo pgObject153(string("15.3"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject153);
			PriorityGroupInfo pgObject154(string("15.4"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject154);
			PriorityGroupInfo pgObject155(string("15.5"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject155);
			PriorityGroupInfo pgObject156(string("15.6"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject156);
			PriorityGroupInfo pgObject157(string("15.7"), (SI32)0, (SI32)0);
			defaultPgSet.insert (pgObject157);
			trace (TRACE_LEVEL_DEVEL, string("defaultPgSet size=") + defaultPgSet.size());


		/* 2.b. Populate configuredPgSet values from current cee-map configuration */ 
		for (i = 0 ; i < pgid_vector.size(); i++) {
		  	string pgIdValue =  pgid_vector[i]->getPgId();
			SI32 weight = pgid_vector[i]->getWeight();
			SI32 pfc = pgid_vector[i]->getPfc();
			trace (TRACE_LEVEL_DEVEL, string("i=")+ i +string("pgid=") + pgIdValue + string("weight=")+ weight + string("pfc=") + pfc);
			PriorityGroupInfo pgObject (pgIdValue, weight, pfc);
			configuredPgSet.insert(pgObject);

			if (pgIdValue == "1")
			  	pgid1ObjectId = pgid_vector[i]->getObjectId();	
			else if (pgIdValue == "2")
				pgid2ObjectId = pgid_vector[i]->getObjectId();
			else if (pgIdValue == "15.0")
				pgid150ObjectId = pgid_vector[i]->getObjectId();
		} 

		trace (TRACE_LEVEL_ERROR, string("Default PGID set values=") + defaultPgSet.size());
		set <PriorityGroupInfo, PriorityGroupCompare>::iterator dit;
		for (dit = defaultPgSet.begin(); dit !=defaultPgSet.end(); dit++) {
			trace (TRACE_LEVEL_DEVEL, string("set pgid=") + dit->pgId + string("weight=") + dit->weight + string("pfc=") + dit->pfc);
		}

		trace (TRACE_LEVEL_ERROR, string("Configured PGID set values")+configuredPgSet.size());
		set <PriorityGroupInfo, PriorityGroupCompare>::iterator cit;
		for (cit = configuredPgSet.begin(); cit !=configuredPgSet.end(); cit++) {
			trace (TRACE_LEVEL_DEVEL, string("set pgid=") + cit->pgId + string("weight=") + cit->weight + string("pfc=") + cit->pfc);
		}

		/* 2.c. Check if defaultPgSet and configuredPgSet is same or not.*/
		defaultPgSet.insert(configuredPgSet.begin(), configuredPgSet.end());
		if (defaultPgSet.size() != 10) { // 10 = pgid1, pgid2 and the 8 strict priorities
			trace (TRACE_LEVEL_ERROR, string("Configured pgids are not as factory default.Value=")+ defaultPgSet.size());
			set <PriorityGroupInfo, PriorityGroupCompare>::iterator it;
			for (it = defaultPgSet.begin(); it !=defaultPgSet.end(); it++) {
				trace (TRACE_LEVEL_DEVEL, string("set pgid=") + it->pgId + string("weight=") + it->weight + string("pfc=") + it->pfc);
			}
			status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			goto last;
		}

		/* 3. Check if priority-map is set to default cee-map values */
		for (i = 0 ; i < 8; i++) {
			if (priorityMap->getCos0ToPriorityGrouMap() != pgid2ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos1ToPriorityGrouMap() != pgid2ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos2ToPriorityGrouMap() != pgid2ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos3ToPriorityGrouMap() != pgid1ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos4ToPriorityGrouMap() != pgid2ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos5ToPriorityGrouMap() != pgid2ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos6ToPriorityGrouMap() != pgid2ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			} else if (priorityMap->getCos7ToPriorityGrouMap() != pgid150ObjectId) {
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			}
		}
last:
		if (pCeeMapResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCeeMapResults);
		}
		return (status);

	}
	/* 
	 * API - isAutoNasEnabled 
	 * Description - Indicates if AUTO NAS is enabled or not
	 * Returns - WAVE_MESSAGE_SUCCESS if auto nas is enabled.
	 *			 Else, appropriate ResourceId.
	 */

	ResourceId QosUtils::isAutoNasEnabled() {
    	UI32 count = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NasConfigManagedObject::getClassName());
    	status = querySynchronouslyForCount(&syncQueryCtxt, count);
		if (status != WAVE_MESSAGE_SUCCESS) {
			trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount failed for isAutoNasEnabled:"));
			status = WRC_WAVE_MESSAGE_ERROR;
    	} else {
			if (count == 0) {
				trace (TRACE_LEVEL_ERROR, string ("isAutoNasEnabled count is 0"));
				status = WRC_WAVE_MESSAGE_ERROR;
			}
		}
		return (status);
	}
}
