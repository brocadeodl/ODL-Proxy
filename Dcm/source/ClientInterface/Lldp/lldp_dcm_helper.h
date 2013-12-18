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


inline void 
lldp_mgmt_gbl_resp_ntoh (lldp_mgmt_gbl_resp_t   *resp_p) 
{
    lldp_mgmt_profile_master_t  *entry_p = &(resp_p->entry);

    entry_p->basic_tlvs_tx_enable = ntohl(entry_p->basic_tlvs_tx_enable);
    entry_p->sub_tlvs_tx_enable = ntohs(entry_p->sub_tlvs_tx_enable);
    entry_p->reinit_delay = ntohs(entry_p->reinit_delay);
    entry_p->msg_tx_hold = ntohl(entry_p->msg_tx_hold);
    entry_p->msg_tx_interval = ntohl(entry_p->msg_tx_interval);
    entry_p->tx_delay = ntohl(entry_p->tx_delay);

    resp_p->ret_code = (lldp_mgmt_err_code_t) 
                        ntohl((uint32_t)resp_p->ret_code);
}

inline void 
lldp_mgmt_show_intf_resp_ntoh (lldp_mgmt_show_intf_resp_t   *resp_p)
{
    lldp_mgmt_show_intf_t   *entry_p = &(resp_p->entry);

    entry_p->msg_tx_hold = ntohl(entry_p->msg_tx_hold);
    entry_p->msg_tx_interval = ntohl(entry_p->msg_tx_interval);
    entry_p->reinit_delay = ntohl(entry_p->reinit_delay);
    entry_p->tx_delay = ntohl(entry_p->tx_delay);
    entry_p->basic_tlvs_tx_enable = ntohl(entry_p->basic_tlvs_tx_enable);
    entry_p->sub_tlvs_tx_enable = ntohs(entry_p->sub_tlvs_tx_enable);
    resp_p->ret_code = (lldp_mgmt_err_code_t) 
                        ntohl((uint32_t)resp_p->ret_code);
}
    
inline void 
lldp_mgmt_dcbx_ctrl_tlv_ntoh (lldp_mgmt_dcbx_ctrl_tlv_t *entry_p) 
{
    entry_p->type = ntohs(entry_p->type);
    entry_p->length = ntohs(entry_p->length);
    entry_p->seq_no = ntohl(entry_p->seq_no);
    entry_p->ack_no = ntohl(entry_p->ack_no);
}

inline void 
lldp_mgmt_dcbx_tlv_hdr_ntoh (lldp_mgmt_dcbx_tlv_hdr_t   *entry_p) 
{
    entry_p->type = ntohs(entry_p->type);
    entry_p->length = ntohs(entry_p->length);
    entry_p->featureSyncNo = ntohl(entry_p->featureSyncNo);
}

inline void 
lldp_mgmt_vlan_list_ntoh (lldp_mgmt_vlan_list_t *entry_p,
												u_int8_t in_nbo)
{
    u_int32_t   num_vlans = 0, i = 0;

    if (in_nbo) { 
        num_vlans = ntohl(entry_p->num_vlans);
    } else { 
        num_vlans = entry_p->num_vlans;
    }

    for (i = 0; i < num_vlans; i++ ) {
        entry_p->vlans[i].vlan_id = ntohs(entry_p->vlans[i].vlan_id);
    }
    entry_p->num_vlans = ntohl(entry_p->num_vlans);
}

inline void 
lldp_mgmt_show_nbr_detail_resp_ntoh (lldp_mgmt_show_nbr_detail_resp_t  *resp_p,
																	u_int8_t in_nbo)
{
    lldp_mgmt_show_nbr_detail_intf_t    *entry_p = NULL;
    u_int32_t                           num_records = 0, i = 0;

    if (in_nbo) { 
        num_records = ntohl(resp_p->num_records);
    } else {
        num_records = resp_p->num_records;
    }

    resp_p->len = ntohl(resp_p->len);
    resp_p->ret_code = (lldp_mgmt_err_code_t)
                        ntohl((u_int32_t)resp_p->ret_code);
    resp_p->num_records = ntohl(resp_p->num_records);


    for (i = 0; i < num_records; i++) { 
        entry_p = &(resp_p->intf_entries[i]);

        entry_p->frames_out_total = ntohl(entry_p->frames_out_total);
        entry_p->frames_in_total = ntohl(entry_p->frames_in_total);
    
        entry_p->remote_info_valid = ntohl(entry_p->remote_info_valid);
        entry_p->remote_dcbx_valid = ntohl(entry_p->remote_dcbx_valid);
        entry_p->rx_ttl = ntohs(entry_p->rx_ttl);
        entry_p->remaining_life = ntohs(entry_p->remaining_life);

        entry_p->remote_if_number = ntohl( entry_p->remote_if_number);

        entry_p->remote_port_vlan_id = ntohs(entry_p->remote_port_vlan_id);

        entry_p->remote_pp_vlan_id = ntohs(entry_p->remote_pp_vlan_id);
        entry_p->remote_protocol = ntohs(entry_p->remote_protocol);

        entry_p->remote_sys_cap = ntohs(entry_p->remote_sys_cap);

        entry_p->remote_sys_cap_enabled = ntohs(
                                          entry_p->remote_sys_cap_enabled);
        entry_p->remote_autonego_cap = ntohs(entry_p->remote_autonego_cap);

        entry_p->remote_oper_mau_type = ntohs(entry_p->remote_oper_mau_type);
        entry_p->remote_max_frame_size = ntohs(entry_p->remote_max_frame_size);

        entry_p->remote_link_aggr_id = ntohl(entry_p->remote_link_aggr_id);
        entry_p->remote_oid_length = ntohl(entry_p->remote_oid_length);

        entry_p->remote_link_prim = ntohs(entry_p->remote_link_prim);
        entry_p->remote_link_brcd = ntohs(entry_p->remote_link_brcd);

        entry_p->remote_dcbx.rcvAckNo = ntohl( entry_p->remote_dcbx.rcvAckNo);
        lldp_mgmt_dcbx_ctrl_tlv_ntoh(&(entry_p->remote_dcbx.ctrl));        

        lldp_mgmt_dcbx_tlv_hdr_ntoh(&(entry_p->remote_dcbx.ets.hdr));
        lldp_mgmt_dcbx_tlv_hdr_ntoh(&(entry_p->remote_dcbx.pfc.hdr));
        lldp_mgmt_dcbx_tlv_hdr_ntoh(&(entry_p->remote_dcbx.iscsi_app.hdr));
        lldp_mgmt_dcbx_tlv_hdr_ntoh(&(entry_p->remote_dcbx.fcoe_app.hdr));
        lldp_mgmt_dcbx_tlv_hdr_ntoh(&
                (entry_p->remote_dcbx.fcoe_logic_link.hdr));
        lldp_mgmt_dcbx_tlv_hdr_ntoh(&(entry_p->remote_dcbx.lan_logic_link.hdr));

        entry_p->features_present = ntohs(entry_p->features_present);
        entry_p->features_changed = ntohs(entry_p->features_changed);
        lldp_mgmt_vlan_list_ntoh(&(entry_p->remote_vlan_list), in_nbo);
    }
}

inline void 
lldp_mgmt_show_stats_resp_ntoh (lldp_mgmt_show_stats_resp_t  *resp_p,
																	u_int8_t in_nbo)
{
    lldp_mgmt_show_stats_intf_t    *entry_p = NULL;
    u_int32_t                      num_records = 0, i = 0;

    if (in_nbo) { 
        num_records = ntohl(resp_p->num_records);
    } else {
        num_records = resp_p->num_records;
    }

    resp_p->ret_code = (lldp_mgmt_err_code_t)
                        ntohl((u_int32_t)resp_p->ret_code);
    resp_p->num_records = ntohl(resp_p->num_records);


    for (i = 0; i < num_records; i++) { 
        entry_p = &(resp_p->stats[i]);

        entry_p->frames_out_total = ntohl(entry_p->frames_out_total);
        entry_p->ageouts_total = ntohl(entry_p->ageouts_total);
        entry_p->frames_discarded_total = ntohl(entry_p->frames_discarded_total);
        entry_p->frames_in_errors_total = ntohs(entry_p->frames_in_errors_total);
        entry_p->frames_in_total = ntohs(entry_p->frames_in_total);

        entry_p->tlvs_discarded_total = ntohl( entry_p->tlvs_discarded_total);

        entry_p->tlvs_unrecognized_total = ntohs(entry_p->tlvs_unrecognized_total);
    }
}


