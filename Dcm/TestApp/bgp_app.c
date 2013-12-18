
#include "uapp.h"
#include "ClientInterface/tlv/tlv_dcm.h"


int unifiedNotifyMessageHandler (const void *inp, void ** const out)
{
	printf("Inside unifiedNotifyMessageHandler \n");
	return (0);
}
int populateUnifiedClientGetData (const void *inp, void ** const out)
{
  	static int isMore = 0;
    printf("Unified Client Get Data message handler entering.\n");
    printf("test.\n");
    char *output = NULL;
	char *input_buffer;
	dcm_tlv_t *tlv_p;
	int tlv_count = 0;
	int j=0;
	int i=0;

    if (!inp)
    {   //Return with an error
        printf("Unified Client Get Data message handler received NULL input structure");
        return (1);
    }
    UnifiedClientMessageInput_t *msg = (UnifiedClientMessageInput_t *) inp;
	input_buffer = (char*)msg->inputBuffer;

	printf("Unified Client Get Data message commandType = %d\n", msg->commandType);
    printf ("Unified Client message has argc=%d\n", msg->commandStringSize);
    printf ("Unified Client message has argv=");

    for(i = 0; i < msg->commandStringSize; i++)
    {
        printf ("%s ", msg->commandString[i]);
    }

     printf ("\n");

    printf("Done with printing arguments.\n");



	if (input_buffer != NULL)
	{
		tlv_count = getTLVCountInBuffer(input_buffer);
		printf("tlv count=%d\n", tlv_count);

		if (tlv_count != 0)
		{
			for (j = 0 ; j < tlv_count; j++)
			{
				tlv_p = getTLVFromBuffer (input_buffer, j);
				switch (tlv_p->type)
				{
					case BGP_HEADER:
					{
						printf("entered bgp header \n");
						printf("value=%s\n", tlv_p->value);
						break;
					}
					default:
					{
						printf("entered default %u\n", tlv_p->type);
						break;
					}
				}
			}
		}
	}


    //printUnifiedClientAttributes (msg);
    printf("Unified Client Get Data message handling complete.\n");

    UnifiedClientMessageOutput_t *result = (UnifiedClientMessageOutput_t *) malloc(sizeof(UnifiedClientMessageOutput_t ));

	if (!isMore)
	{
	  	//first time
    	result->moreRecords = 1;
		isMore = 1;
	}
	else
	{
    	result->moreRecords = 0;
		isMore = 0;
	}

    switch (msg->groupCode)
    {
#if 0
        case BGP_SHOW_IP: /* show ip bgp */
        {
            printf("bgpOperationalDataHandler -- show ip bgp \n");
            break;
        }
        case BGP_SHOW_IP_IPV4_PREFIX: /* show ip bgp ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_IPV4_PREFIX_LONGER_PREFIXES: /* show ip bgp ipv4-prefix longer-prefixes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp ipv4-prefix longer-prefixes \n");
            break;
        }
        case BGP_SHOW_IP_ATTRIBUTE_ENTRIES: /* show ip bgp attribute-entries */
        {
            printf("bgpOperationalDataHandler -- show ip bgp attribute-entries \n");
            break;
        }
        case BGP_SHOW_IP_DAMPENED_PATHS: /* show ip bgp dampened-paths */
        {
            printf("bgpOperationalDataHandler -- show ip bgp dampened-paths \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES: /* show ip bgp filtered-routes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp show ip bgp filtered-routes \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_IPV4_PREFIX: /* show ip bgp filtered-routes ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_IPV4_PREFIX_LONGER_PREFIXES: /* show ip bgp filtered-routes ipv4-prefix longer-prefixes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes ipv4-prefix longer-prefixes\n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_AS_PATH_ACCESS_LIST_NAME: /* show ip bgp filtered-routes as-path-access-list */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes as-path-access-list \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_DETAIL: /* show ip bgp filtered-routes detail */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes detail \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_DETAIL_IPV4_PREFIX: /* show ip bgp filtered-routes detail ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes detail ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_DETAIL_IPV4_PREFIX_IPV4_PREFIX_LONGER_PREFIXES: /* show ip bgp filtered-routes detail ipv4-prefix longer-prefixes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes detail ipv4-prefix longer-prefixes \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_DETAIL_AS_PATH_ACCESS_LIST_NAME: /* show ip bgp filtered-routes detail as-path-access-list */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes detail as-path-access-list \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_DETAIL_PREFIX_LIST_NAME: /* show ip bgp filtered-routes detail prefix-list */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes detail prefix-list \n");
            break;
        }
        case BGP_SHOW_IP_FILTERED_ROUTES_PREFIX_LIST_NAME: /* show ip bgp filtered-routes prefix-list */
        {
            printf("bgpOperationalDataHandler -- show ip bgp filtered-routes prefix-list \n");
            break;
        }
        case BGP_SHOW_IP_FLAP_STATISTICS: /* show ip bgp flap-statistics */
        {
            printf("bgpOperationalDataHandler -- show ip bgp flap-statistics \n");
            break;
        }
        case BGP_SHOW_IP_FLAP_STATISTICS_IPV4_PREFIX: /* show ip bgp flap-statistics ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp flap-statistics ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_FLAP_STATISTICS_IPV4_PREFIX_LONGER_PREFIXES: /* show ip bgp flap-statistics ipv4-prefix longer-prefixes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp flap-statistics ipv4-prefix longer-prefixes \n");
            break;
        }
        case BGP_SHOW_IP_FLAP_STATISTICS_AS_PATH_FILTER_NAME: /* show ip bgp flap-statistics as-path-filter as-path-filter-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp flap-statistics as-path-filter as-path-filter-name \n");
            break;
        }
        case BGP_SHOW_IP_FLAP_STATISTICS_NEIGHBOR_IPV4_ADDRESS: /* show ip bgp flap-statistics neighbor ipv4-address */
        {
            printf("bgpOperationalDataHandler -- show ip bgp flap-statistics neighbor ipv4-address \n");
            break;
        }
        case BGP_SHOW_IP_FLAP_STATISTICS_REGULAR_EXPRESSION_VALUE: /* show ip bgp flap-statistics regular-expression regular-expression-value */
        {
            printf("bgpOperationalDataHandler -- show ip bgp flap-statistics regular-expression regular-expression-value \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS: /* show ip bgp neighbors */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS: /* show ip bgp neighbors ipv4-address */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ADVERTISED_ROUTES: /* show ip bgp neighbors ipv4-address advertised-routes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address advertised-routes \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ADVERTISED_ROUTES_IPV4_PREFIX: /* show ip bgp neighbors ipv4-address advertised-routes ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address advertised-routes ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ADVERTISED_ROUTES_DETAIL: /* show ip bgp neighbors ipv4-address advertised-routes detail */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address advertised-routes detail \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ADVERTISED_ROUTES_DETAIL_IPV4_PREFIX: /* show ip bgp neighbors ipv4-address advertised-routes detail ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address advertised-routes detail ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_FLAP_STATISTICS: /* show ip bgp neighbors ipv4-address flap-statistics */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address flap-statistics \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_LAST_PACKET_WITH_ERROR: /* show ip bgp neighbors ipv4-address last-packet-with-error */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address last-packet-with-error \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_LAST_PACKET_WITH_ERROR_DECODE: /* show ip bgp neighbors ipv4-address last-packet-with-error decode */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address last-packet-with-error decode \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RECEIVED_EXTENDED_COMMUNITY: /* show ip bgp neighbors ipv4-address received extended-community */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address received extended-community \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RECEIVED_PREFIX_FILTER: /* show ip bgp neighbors ipv4-address received prefix-filter */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address received prefix-filter \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RECEIVED_ROUTES: /* show ip bgp neighbors ipv4-address received-routes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address received-routes \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RECEIVED_ROUTES_DETAIL: /* show ip bgp neighbors ipv4-address received-routes detail */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address received-routes detail \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RIB_OUT_ROUTES: /* show ip bgp neighbors ipv4-address rib-out-routes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address rib-out-routes \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RIB_OUT_ROUTES_IPV4_PREFIX: /* show ip bgp neighbors ipv4-address rib-out-routes ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address rib-out-routes ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RIB_OUT_ROUTES_DETAIL: /* show ip bgp neighbors ipv4-address rib-out-routes detail */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address rib-out-routes detail \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_RIB_OUT_ROUTES_DETAIL_IPV4_PREFIX: /* show ip bgp neighbors ipv4-address rib-out-routes detail ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address rib-out-routes detail ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES: /* show ip bgp neighbors ipv4-address routes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_BEST: /* show ip bgp neighbors ipv4-address routes best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes best \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_DETAIL: /* show ip bgp neighbors ipv4-address routes detail */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes detail \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_DETAIL_BEST: /* show ip bgp neighbors ipv4-address routes detail best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes detail best \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_DETAIL_NOT_INSTALLED_BEST: /* show ip bgp neighbors ipv4-address routes detail not-installed-best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes detail not-installed-best \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_DETAIL_UNREACHABLE: /* show ip bgp neighbors ipv4-address routes detail unreachable */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes detail unreachable \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_NOT_INSTALLED_BEST: /* show ip bgp neighbors ipv4-address routes not-installed-best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes not-installed-best \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_UNREACHABLE: /* show ip bgp neighbors ipv4-address routes unreachable */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes unreachable \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_IPV4_ADDRESS_ROUTES_SUMMARY: /* show ip bgp neighbors ipv4-address routes-summary */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors ipv4-address routes-summary \n");
            break;
        }
        case BGP_SHOW_IP_PEER_GROUP: /* show ip bgp peer-group */
        {
            printf("bgpOperationalDataHandler -- show ip bgp peer-group \n");
            break;
        }
        case BGP_SHOW_IP_PEER_GROUP_NAME: /* show ip bgp peer-group peer-group-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp peer-group peer-group-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES: /* show ip bgp routes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_INDEX: /* show ip bgp routes index */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes index \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_IPV4_PREFIX: /* show ip bgp routes ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_IPV4_PREFIX_LONGER_PREFIXES: /* show ip bgp routes ipv4-prefix longer-prefixes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes ipv4-prefix longer-prefixes \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_AGE: /* show ip bgp routes age seconds */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes age seconds \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_IPV4_PREFIX_AS_PATH_ACCESS_LIST_NAME: /* show ip bgp routes as-path-access-list as-path-access-list-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes as-path-access-list as-path-access-list-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_IPV4_PREFIX_AS_PATH_FILTER_NAME: /* show ip bgp routes as-path-filter as-path-filter-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes as-path-filter as-path-filter-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_BEST: /* show ip bgp routes best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes best \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_CIDR_ONLY: /* show ip bgp routes cidr-only */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes cidr-only \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_NUMBER: /* show ip bgp routes community number */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community number \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_STRING: /* show ip bgp routes community string */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community string \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_INTERNET: /* show ip bgp routes community internet */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community internet \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_LOCAL_AS: /* show ip bgp routes community local-as */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community local-as \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_NO_ADVERTISE: /* show ip bgp routes community no-advertise */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community no-advertise \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_NO_EXPORT: /* show ip bgp routes community no-export */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community no-export \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_ACCESS_LIST_NAME: /* show ip bgp routes community-access-list community-access-list-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community-access-list community-access-list-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_COMMUNITY_REG_EXP_VALUE: /* show ip bgp routes community-reg-expression value */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes community-reg-expression value \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_LOCAL: /* show ip bgp routes local */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes local \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_NEIGHBOR_IPV4_ADDRESS: /* show ip bgp routes neighbor ipv4-address */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes neighbor ipv4-address \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_NEXTHOP_IPV4_ADDRESS: /* show ip bgp routes nexthop ipv4-address */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes nexthop ipv4-address \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_NO_BEST: /* show ip bgp routes no-best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes no-best \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_NOT_INSTALLED_BEST: /* show ip bgp routes not-installed-best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes not-installed-best \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_PREFIX_LIST_NAME: /* show ip bgp routes prefix-list prefix-list-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes prefix-list prefix-list-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_REG_EXP_VALUE: /* show ip bgp routes regular-expression value */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes regular-expression value \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_ROUTE_MAP_NAME: /* show ip bgp routes route-map route-map-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes route-map route-map-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_SUMMARY: /* show ip bgp routes summary */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes summary \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_UNREACHABLE: /* show ip bgp routes unreachable */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes unreachable \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL: /* show ip bgp routes detail */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_INDEX: /* show ip bgp routes detail index */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail index \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_IPV4_PREFIX: /* show ip bgp routes detail ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail ipv4-prefix \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_IPV4_PREFIX_LONGER_PREFIXES: /* show ip bgp routes detail ipv4-prefix longer-prefixes */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail ipv4-prefix longer-prefixes \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_AGE: /* show ip bgp routes detail age seconds */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail age seconds \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_AS_PATH_ACCESS_LIST_NAME: /* show ip bgp routes detail as-path-access-list as-path-access-list-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail as-path-access-list as-path-access-list-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_AS_PATH_FILTER_NAME: /* show ip bgp routes detail as-path-filter as-path-filter-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail as-path-filter as-path-filter-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_BEST: /* show ip bgp routes detail best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail best \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_CIDR_ONLY: /* show ip bgp routes detail cidr-only */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail cidr-only \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_NUMBER: /* show ip bgp routes detail community number */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community number \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_STRING: /* show ip bgp routes detail community string */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community string \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_INTERNET: /* show ip bgp routes detail community internet */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community internet \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_LOCAL_AS: /* show ip bgp routes detail community local-as */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community local-as \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_NO_ADVERTISE: /* show ip bgp routes detail community no-advertise */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community no-advertise \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_NO_EXPORT: /* show ip bgp routes detail community no-export */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community no-export \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_ACCESS_LIST_NAME: /* show ip bgp routes detail community-access-list community-access-list-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community-access-list community-access-list-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_COMMUNITY_REG_EXP_VALUE: /* show ip bgp routes detail community-reg-expression value */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail community-reg-expression value \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_LOCAL: /* show ip bgp routes detail local */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail local \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_NEIGHBOR_IPV4_ADDRESS: /* show ip bgp routes detail neighbor ipv4-address */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail neighbor ipv4-address \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_NEXTHOP_IPV4_ADDRESS: /* show ip bgp routes detail nexthop ipv4-address */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail nexthop ipv4-address \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_NO_BEST: /* show ip bgp routes detail no-best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail no-best \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_NOT_INSTALLED_BEST: /* show ip bgp routes detail not-installed-best */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail not-installed-best \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_PREFIX_LIST_NAME: /* show ip bgp routes detail prefix-list prefix-list-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail prefix-list prefix-list-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_REG_EXP_VALUE: /* show ip bgp routes detail regular-expression value */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail regular-expression value \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_ROUTE_MAP_NAME: /* show ip bgp routes detail route-map route-map-name */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail route-map route-map-name \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_SUMMARY: /* show ip bgp routes detail summary */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail summary \n");
            break;
        }
        case BGP_SHOW_IP_ROUTES_DETAIL_UNREACHABLE: /* show ip bgp routes detail unreachable */
        {
            printf("bgpOperationalDataHandler -- show ip bgp routes detail unreachable \n");
            break;
        }
        case BGP_SHOW_IP_SUMMARY: /* show ip bgp summary */
        {
            printf("bgpOperationalDataHandler -- show ip bgp summary \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_LAST_PACKET_WITH_ERROR: /* show ip bgp neighbors last-packet-with-error */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors last-packet-with-error  \n");
            break;
        }
        case BGP_SHOW_IP_NEIGHBORS_ROUTES_SUMMARY: /* show ip bgp neighbors routes-summary */
        {
            printf("bgpOperationalDataHandler -- show ip bgp neighbors routes-summary \n");
            break;
        }
        case BGP_SHOW_DEBUG_IP: /* show debug ip bgp */
        {
            printf("bgpOperationalDataHandler -- show debug ip bgp \n");
            break;
        }

		/* debug commands */
        case BGP_DEBUG_ADDRESS_FAMILY_IPV4_UNICAST: /* debug ip bgp address-family ipv4 unicast */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp address-family ipv4 unicast \n");
            break;
        }
        case BGP_DEBUG_DAMPENING: /* debug ip bgp dampening */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp dampening \n");
            break;
        }
        case BGP_DEBUG_EVENTS: /* debug ip bgp events */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp events \n");
            break;
        }
        case BGP_DEBUG_GENERAL: /* debug ip bgp general */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp general \n");
            break;
        }
        case BGP_DEBUG_GRACEFUL_RESTART: /* debug ip bgp graceful-restart */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp graceful-restart \n");
            break;
        }
        case BGP_DEBUG_IPV4_PREFIX: /* debug ip bgp ip-prefix ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp ip-prefix ipv4-prefix \n");
            break;
        }
        case BGP_DEBUG_IP_PREFIX_LIST: /* debug ip bgp ip-prefix-list */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp ip-prefix-list \n");
            break;
        }
        case BGP_DEBUG_IP_PREFIX_LIST_NAME: /* debug ip bgp ip-prefix-list prefix-list-name */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp ip-prefix-list prefix-list-name \n");
            break;
        }
        case BGP_DEBUG_KEEPALIVES: /* debug ip bgp keepalives */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp keepalives \n");
            break;
        }
        case BGP_DEBUG_NEIGHBOR_IPV4_ADDRESS: /* debug ip bgp neighbor ipv4-address */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp neighbor ipv4-address \n");
            break;
        }
        case BGP_DEBUG_ROUTE_MAP_NAME: /* debug ip bgp route-map route-map-name */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp route-map route-map-name \n");
            break;
        }
        case BGP_DEBUG_ROUTE_SELECTION: /* debug ip bgp route-selection */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp route-selection \n");
            break;
        }
        case BGP_DEBUG_UPDATES: /* debug ip bgp updates */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp updates \n");
            break;
        }
        case BGP_DEBUG_UPDATES_TX: /* debug ip bgp updates tx */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp updates tx \n");
            break;
        }
        case BGP_DEBUG_UPDATES_RX: /* debug ip bgp updates rx */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp updates rx \n");
            break;
        }
        case BGP_DEBUG_PACKET: /* debug ip bgp packet */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp packet \n");
            break;
        }
        case BGP_DEBUG_PACKET_TX: /* debug ip bgp packet tx */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp packet tx \n");
            break;
        }
        case BGP_DEBUG_PACKET_RX: /* debug ip bgp packet rx */
        {
            printf("bgpOperationalDataHandler -- debug ip bgp packet rx \n");
            break;
        }

		/* BGP clear commands */

        case BGP_CLEAR_IP_DAMPENING: /* clear ip bgp dampening */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp dampening \n");
            break;
        }
        case BGP_CLEAR_IP_DAMPENING_IPV4_PREFIX: /* clear ip bgp dampening ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp dampening ipv4-prefix \n");
            break;
        }
        case BGP_CLEAR_IP_FLAP_STATISTICS: /* clear ip bgp flap-statistics */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp flap-statistics \n");
            break;
        }
        case BGP_CLEAR_IP_FLAP_STATISTICS_IPV4_PREFIX: /* clear ip bgp flap-statistics ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp flap-statistics ipv4-prefix \n");
            break;
        }
        case BGP_CLEAR_IP_FLAP_STATISTICS_IPV4_PREFIX_REG_EXP_VAL: /* clear ip bgp flap-statistics regular-expression string */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp flap-statistics regular-expression string \n");
            break;
        }
        case BGP_CLEAR_IP_FLAP_STATISTICS_IPV4_PREFIX_NEIGHBOR_IPV4_PREFIX: /* clear ip bgp flap-statistics neighbor ipv4-address */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp flap-statistics neighbor ipv4-address \n");
            break;
        }
        case BGP_CLEAR_IP_LOCAL_ROUTES: /* clear ip bgp local routes */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp local routes \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO: /* clear ip bgp neighbor as-number */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO_LAST_PACKET_WITH_ERROR: /* clear ip bgp neighbor as-number last-packet-with-error */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number last-packet-with-error \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO_NOTIFICATION_ERRORS: /* clear ip bgp neighbor as-number notification-errors */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number notification-errors \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO_SOFT: /* clear ip bgp neighbor as-number soft */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number soft \n");
            break;
        }
		case BGP_CLEAR_IP_NEIGHBOR_AS_NO_SOFT_IN: /* clear ip bgp neighbor as-number soft in */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number soft in \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO_SOFT_OUT: /* clear ip bgp neighbor as-number soft out */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number soft out \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO_SOFT_OUTBOUND: /* clear ip bgp neighbor as-number soft-outbound */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number soft-outbound \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_AS_NO_TRAFFIC: /* clear ip bgp neighbor as-number traffic */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor as-number traffic \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME: /* clear ip bgp neighbor peer-group-name */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_LAST_PACKET_WITH_ERROR: /* clear ip bgp neighbor peer-group-name last-packet-with-error */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name last-packet-with-error \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_NOTIFICATION_ERRORS: /* clear ip bgp neighbor peer-group-name notification-errors */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name notification-errors \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_SOFT: /* clear ip bgp neighbor peer-group-name soft */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name soft \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_SOFT_IN: /* clear ip bgp neighbor peer-group-name soft in */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name soft in \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_SOFT_IN_PREFIX_FILTER: /* clear ip bgp neighbor peer-group-name soft in prefix-filter */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name soft in prefix-filter \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_SOFT_OUT: /* clear ip bgp neighbor peer-group-name soft out */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name soft out \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_SOFT_OUT_BOUND: /* clear ip bgp neighbor peer-group-name soft-outbound */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name soft-outbound \n");
            break;
        }
        case BGP_CLEAR_IP_PEER_GROUP_NAME_TRAFFIC: /* clear ip bgp neighbor peer-group-name traffic */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor peer-group-name traffic \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS: /* clear ip bgp neighbor ipv4-address */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_LAST_PACKET_WITH_ERROR: /* clear ip bgp neighbor ipv4-address last-packet-with-error */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address last-packet-with-error \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_NOTIFICATION_ERRORS: /* clear ip bgp neighbor ipv4-address notification-errors */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address notification-errors \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_SOFT: /* clear ip bgp neighbor ipv4-address soft */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address soft \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_SOFT_IN: /* clear ip bgp neighbor ipv4-address soft in */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address soft in \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_SOFT_IN_PREFIX_FILTER: /* clear ip bgp neighbor ipv4-address soft in prefix-filter */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address soft in prefix-filter \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_SOFT_OUT: /* clear ip bgp neighbor ipv4-address soft out */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address soft out \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_SOFT_OUT_BOUND: /* clear ip bgp neighbor ipv4-address soft-outbound */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address soft-outbound \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ADDRESS_TRAFFIC: /* clear ip bgp neighbor ipv4-address traffic */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor ipv4-address traffic \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL: /* clear ip bgp neighbor all */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_LAST_PACKET_WITH_ERROR: /* clear ip bgp neighbor all last-packet-with-error */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all last-packet-with-error \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_NOTIFICATION_ERRORS: /* clear ip bgp neighbor all notification-errors */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all notification-errors \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_SOFT: /* clear ip bgp neighbor all soft */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all soft \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_SOFT_IN: /* clear ip bgp neighbor all soft in*/
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all soft in \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_SOFT_IN_PREFIX_FILTER: /* clear ip bgp neighbor all soft in prefix-filter */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all soft in prefix-filter \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_SOFT_OUT: /* clear ip bgp neighbor all soft out */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all soft out \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_SOFT_OUT_BOUND: /* clear ip bgp neighbor all soft-outbound */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all soft-outbound \n");
            break;
        }
        case BGP_CLEAR_IP_NEIGHBOR_ALL_TRAFFIC: /* clear ip bgp neighbor all traffic */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp neighbor all traffic \n");
            break;
        }
        case BGP_CLEAR_IP_BGP_ROUTES: /* clear ip bgp routes */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp routes \n");
            break;
        }
        case BGP_CLEAR_IP_ROUTES_IPV4_ADDRESS: /* clear ip bgp ipv4-address */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp ipv4-address \n");
            break;
        }
        case BGP_CLEAR_IP_BGP_ROUTES_IPV4_PREFIX: /* clear ip bgp ipv4-prefix */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp ipv4-prefix \n");
            break;
        }
        case BGP_CLEAR_IP_TRAFFIC: /* clear ip bgp traffic */
        {
            printf("bgpOperationalDataHandler -- clear ip bgp traffic \n");
            break;
        }

        default:
        {
            break;
        }
#endif
    }


    output = (char *)malloc (100);
	if (output != NULL)
	{
		memset(output, 0, 100);
	}
	int err = 0;
	err = dcmTlvBufferInit(output);

    printf("done with dcmTlvBufferInit\n");

	char header_value[]="myheader";
	err  = appendTLVToBuffer (output, BGP_HEADER, sizeof(header_value), header_value);
	if (err != 0)
	{
		printf("problem with myheader\n");
	}
    printf("done with myheader\n");

	char data_value[]="hello world\n";
	err = appendTLVToBuffer(output, BGP_DATA, 13, data_value);
	if (err != 0)
	{
		printf("problem with data\n");
	}
    printf("done with hello world\n");
	
    result->outputBufferSize = 100;
    result->outputBuffer = output;
    result->bufferTag = 1;

    (*out) = result;
    return (0);
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        printf("usage: %s <mgmt_port_offset>\n", argv[0]);
        return -1;
    }

    int mgmt_port_offset = atoi(argv[1]);

    add_client_message_handler(UNIFIEDCLIENTGETDATA, populateUnifiedClientGetData);
	add_client_message_handler(UNIFIEDCLIENTNOTIFY, unifiedNotifyMessageHandler);

    start_client("bgp", mgmt_port_offset);

    return 0;
}

