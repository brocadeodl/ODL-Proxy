#include <stdio.h>
#include <unistd.h>

#include "Framework/Utils/FrameworkToolKit.h"
#include "ValClientInterface/DcmClientInterface.h"
#include <assert.h>
#include "ClientInterface/Rmon/RmonClientInterface.h"
#include "ClientInterface/Rmon/rmon_common.h"
#include "ValClientInterface/DcmClientInterface.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "DcmCStatus.h"
#include "WaveCStatus.h"

int create_rmon_event(const void *inp, void ** const out)
{
    rmon_event_message_t *msg = (rmon_event_message_t*)inp;
    printf("Input eventIndex: %u\n", msg->eventIndex);
    printf("Input log: %d\n", msg->log);
    printf("Input communityName: %s\n", msg->communityName);
    printf("Input description: %s\n", msg->description);
    printf("Input owner: %s\n", msg->owner);

	*out = NULL;

    return (WRC_WAVE_MESSAGE_SUCCESS);
}

int delete_rmon_event(const void *inp, void ** const out)
{
	int *eventIndex = (int*)inp;
	printf("Input eventIndex=%d\n", *eventIndex);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int create_rmon_stats(const void *inp, void ** const out)
{
	rmon_stats_message_t *msg = (rmon_stats_message_t*)inp;
	printf("Input ifname=%s\n", msg->ifname);
	printf("Input stats_index=%u\n", msg->stats_index);
    printf("Input owner: %s\n", msg->owner);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int delete_rmon_stats(const void *inp, void ** const out)
{
	rmon_stats_message_t *msg = (rmon_stats_message_t*)inp;
	printf("Input ifname=%s\n", msg->ifname);
	printf("Input stats_index=%u\n", msg->stats_index);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int create_rmon_history(const void *inp, void ** const out)
{
	rmon_history_message_t *msg = (rmon_history_message_t*)inp;
	printf("Input ifname=%s\n", msg->ifname);
	printf("Input history_index=%u\n", msg->history_index);
	printf("Input buckets=%u\n", msg->buckets);
	printf("Input interval=%u\n", msg->interval);
    printf("Input owner: %s\n", msg->owner);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int delete_rmon_history(const void *inp, void ** const out)
{
	rmon_history_message_t *msg = (rmon_history_message_t*)inp;
	printf("Input ifname=%s\n", msg->ifname);
	printf("Input history_index=%u\n", msg->history_index);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int create_rmon_alarm(const void *inp, void ** const out)
{
	rmon_alarm_message_t *msg = (rmon_alarm_message_t*)inp;
	printf("Input alarm_index=%u\n", msg->alarm_index);
	printf("Input interval=%u\n", msg->interval);
	printf("Input sample_type =%u\n", msg->sample_type);
	printf("Input rising_event=%u\n", msg->rising_event);
	printf("Input rising_threshold=%u\n", msg->rising_threshold);
	printf("Input falling_event=%u\n", msg->falling_event);
	printf("Input falling_threshold=%u\n", msg->falling_threshold);
	printf("Input owner=%s\n", msg->owner);
	printf("Input snmp_oid=%s\n", msg->snmp_oid);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int delete_rmon_alarm(const void *inp, void ** const out)
{
	rmon_alarm_message_t *msg = (rmon_alarm_message_t*)inp;
	printf("Input alarm_index=%u\n", msg->alarm_index);
	*out = NULL;
	return (WRC_WAVE_MESSAGE_SUCCESS);
}


void fillPayload (rmon_show_output_msg_t **p, int cmd, int show_all, int show_brief)
{
	int num_records = 2;
	int *int_ptr = NULL;

	switch (cmd)
	{
		case SHOW_RMON_HISTORY_CMD:
		{
			rmon_show_history_message_t *ptr = NULL;
		  	(*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) + ((num_records - 1)*sizeof (rmon_show_history_message_t)));
			(*p)->record_type = SHOW_RMON_HISTORY;
		  	(*p)->num_records = num_records;

			int_ptr = (int*)(*p);
			int_ptr = int_ptr + 2;

			ptr = (rmon_show_history_message_t*)int_ptr;

			ptr->history_index = 5;
			ptr->interface_index = 10;
			strncpy(ptr->interface_name, "tengigabyteEthernet 0/1", DCM_IFNAME_LENGTH);
			ptr->interface_name[DCM_IFNAME_LENGTH-1] = '\0';
			ptr->buckets_requested = 10;
			ptr->buckets_granted = 5;
			ptr->sampling_interval = 6;
			strncpy(ptr->owner, DEFAULT_OWNER, RMON_MAX_WORD_LEN);	

			ptr++;

			ptr->history_index = 6;
			ptr->interface_index = 10;
			strncpy(ptr->interface_name, "tengigabyteEthernet 0/1", DCM_IFNAME_LENGTH);
			ptr->interface_name[DCM_IFNAME_LENGTH-1] = '\0';
			ptr->buckets_requested = 10;
			ptr->buckets_granted = 5;
			ptr->sampling_interval = 6;
			strncpy(ptr->owner, DEFAULT_OWNER, RMON_MAX_WORD_LEN);	
			
			break;
		}
		
		case SHOW_RMON_HISTORY_STATISTICS_CMD:
        {
			char buf[] = "123456789" ;
            rmon_show_history_statistics_message_t *ptr = NULL;
            (*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) +
					((num_records - 1)*sizeof (rmon_show_history_statistics_message_t)));
            (*p)->record_type = SHOW_RMON_HISTORY_STATISTICS;
            (*p)->num_records = num_records;

            int_ptr = (int*)(*p);
            int_ptr = int_ptr + 2;

            ptr = (rmon_show_history_statistics_message_t*)int_ptr;

            ptr->history_index = 5;
            ptr->sample_index = 10;

			strncpy(ptr->packets, buf, DCM_BUFFER_SIZE);
            ptr->packets[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->octets, buf, DCM_BUFFER_SIZE);
            ptr->octets[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->drop_events, buf, DCM_BUFFER_SIZE);
            ptr->drop_events[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->broadcasts, buf, DCM_BUFFER_SIZE);
            ptr->broadcasts[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->multicasts, buf, DCM_BUFFER_SIZE);
            ptr->multicasts[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->crc, buf, DCM_BUFFER_SIZE);
            ptr->crc[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->undersize_packets, buf, DCM_BUFFER_SIZE);
            ptr->undersize_packets[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->oversize_packets, buf, DCM_BUFFER_SIZE);
            ptr->oversize_packets[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->fragments, buf, DCM_BUFFER_SIZE);
            ptr->fragments[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->jabbers, buf, DCM_BUFFER_SIZE);
            ptr->jabbers[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->collisions, buf, DCM_BUFFER_SIZE);
            ptr->collisions[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->utilization, buf, DCM_BUFFER_SIZE);
            ptr->utilization[DCM_BUFFER_SIZE - 1] = '\0';

            ptr++;

			ptr->history_index = 5;
            ptr->sample_index = 10;

            strncpy(ptr->packets, buf, DCM_BUFFER_SIZE);
            ptr->packets[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->octets, buf, DCM_BUFFER_SIZE);
            ptr->octets[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->drop_events, buf, DCM_BUFFER_SIZE);
            ptr->drop_events[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->broadcasts, buf, DCM_BUFFER_SIZE);
            ptr->broadcasts[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->multicasts, buf, DCM_BUFFER_SIZE);
            ptr->multicasts[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->crc, buf, DCM_BUFFER_SIZE);
            ptr->crc[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->undersize_packets, buf, DCM_BUFFER_SIZE);
            ptr->undersize_packets[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->oversize_packets, buf, DCM_BUFFER_SIZE);
            ptr->oversize_packets[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->fragments, buf, DCM_BUFFER_SIZE);
            ptr->fragments[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->jabbers, buf, DCM_BUFFER_SIZE);
            ptr->jabbers[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->collisions, buf, DCM_BUFFER_SIZE);
            ptr->collisions[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->utilization, buf, DCM_BUFFER_SIZE);
            ptr->utilization[DCM_BUFFER_SIZE - 1] = '\0';

			break;
		}

		case SHOW_RMON_STATISTICS_CMD:
		{
			rmon_show_statistics_message_t *ptr = NULL;
			rmon_show_statistics_brief_message_t *ptr1 = NULL;
			char buf[] = "123456789" ;

			if (show_brief == 1)
			{
			
		  		(*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) +
						((num_records - 1)*sizeof (rmon_show_statistics_brief_message_t)));
				(*p)->record_type = SHOW_RMON_STATISTICS_BRIEF;
		  		(*p)->num_records = num_records;

				int_ptr = (int*)(*p);
				int_ptr = int_ptr + 2;

				ptr1 = (rmon_show_statistics_brief_message_t*)int_ptr;

				ptr1->stats_index = 5;
				ptr1->interface_index = 10;
				strncpy(ptr1->interface_name, "tengigabyteEthernet 0/1", DCM_IFNAME_LENGTH);
            	ptr1->interface_name[DCM_IFNAME_LENGTH-1] = '\0';

				ptr1++;

				ptr1->stats_index = 6;
				ptr1->interface_index = 10;
				strncpy(ptr1->interface_name, "tengigabyteEthernet 0/1", DCM_IFNAME_LENGTH);
            	ptr1->interface_name[DCM_IFNAME_LENGTH-1] = '\0';
				break;

			}

			
		  	(*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) + ((num_records - 1)*sizeof (rmon_show_statistics_message_t)));
			(*p)->record_type = SHOW_RMON_STATISTICS;
		  	(*p)->num_records = num_records;

			int_ptr = (int*)(*p);
			int_ptr = int_ptr + 2;

			ptr = (rmon_show_statistics_message_t*)int_ptr;

			ptr->stats_index = 5;
			ptr->interface_index = 10;

			strncpy(ptr->interface_name, "tengigabyteEthernet 0/1", DCM_IFNAME_LENGTH);
            ptr->interface_name[DCM_IFNAME_LENGTH-1] = '\0';

            strncpy(ptr->packets, buf, DCM_BUFFER_SIZE);
            ptr->packets[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->bytes, buf, DCM_BUFFER_SIZE);
            ptr->bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_dropped, buf, DCM_BUFFER_SIZE);
            ptr->packets_dropped[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->multicasts, buf, DCM_BUFFER_SIZE);
            ptr->multicasts[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->broadcasts, buf, DCM_BUFFER_SIZE);
            ptr->broadcasts[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->under_size, buf, DCM_BUFFER_SIZE);
            ptr->under_size[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->jabbers, buf, DCM_BUFFER_SIZE);
            ptr->jabbers[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->crc, buf, DCM_BUFFER_SIZE);
            ptr->crc[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->fragments, buf, DCM_BUFFER_SIZE);
            ptr->fragments[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->collisions, buf, DCM_BUFFER_SIZE);
            ptr->collisions[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_64_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_64_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_65_to_127_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_65_to_127_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_128_to_255_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_128_to_255_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_256_to_511_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_256_to_511_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_512_to_1023_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_512_to_1023_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_1024_to_1518_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_1024_to_1518_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_over_1518_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_over_1518_bytes[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->owner, DEFAULT_OWNER, RMON_MAX_WORD_LEN);
			ptr->owner[RMON_MAX_WORD_LEN - 1] = '\0';

			ptr->status = 6;

			ptr++;

			ptr->stats_index = 6;
            ptr->interface_index = 10;

            strncpy(ptr->interface_name, "tengigabyteEthernet 0/1", DCM_IFNAME_LENGTH);
            ptr->interface_name[DCM_IFNAME_LENGTH-1] = '\0';

            strncpy(ptr->packets, buf, DCM_BUFFER_SIZE);
            ptr->packets[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->bytes, buf, DCM_BUFFER_SIZE);
            ptr->bytes[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->packets_dropped, buf, DCM_BUFFER_SIZE);
            ptr->packets_dropped[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->multicasts, buf, DCM_BUFFER_SIZE);
            ptr->multicasts[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->broadcasts, buf, DCM_BUFFER_SIZE);
            ptr->broadcasts[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->under_size, buf, DCM_BUFFER_SIZE);
            ptr->under_size[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->jabbers, buf, DCM_BUFFER_SIZE);
            ptr->jabbers[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->crc, buf, DCM_BUFFER_SIZE);
            ptr->crc[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->fragments, buf, DCM_BUFFER_SIZE);
            ptr->fragments[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->collisions, buf, DCM_BUFFER_SIZE);
            ptr->collisions[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_64_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_64_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_65_to_127_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_65_to_127_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_128_to_255_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_128_to_255_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_256_to_511_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_256_to_511_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_512_to_1023_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_512_to_1023_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->packets_1024_to_1518_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_1024_to_1518_bytes[DCM_BUFFER_SIZE - 1] = '\0';

			strncpy(ptr->packets_over_1518_bytes, buf, DCM_BUFFER_SIZE);
            ptr->packets_over_1518_bytes[DCM_BUFFER_SIZE - 1] = '\0';

            strncpy(ptr->owner, DEFAULT_OWNER, RMON_MAX_WORD_LEN);
            ptr->owner[RMON_MAX_WORD_LEN - 1] = '\0';

            ptr->status = 6;
		
			break;
		}
		case SHOW_RMON_EVENTS_CMD:
		{

			rmon_show_events_message_t *event = NULL;

            (*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) + ((num_records )*sizeof (rmon_show_events_message_t)));
			if (show_brief == 1)
			{
                (*p)->record_type = SHOW_RMON_EVENTS_BRIEF;
			}
			else
			{
				(*p)->record_type = SHOW_RMON_EVENTS;
			}	
            (*p)->num_records = num_records;
			int_ptr = (int*)(*p);
			int_ptr = int_ptr + 2;

			event = (rmon_show_events_message_t *)int_ptr;
			
			event->index = 5;
			event->status = DCM_RMON_VALID_STATUS;
			strcpy(event->description,"test1");
			event->type = 4;//trap type
			strcpy(event->community,"snmp");
			strcpy(event->owner,"brocade");
			event++;
			event->index = 6;
			event->status = DCM_RMON_VALID_STATUS;
			strcpy(event->description,"test2");
			event->type = 3;//trap type
			strcpy(event->community,"snmp");
			strcpy(event->owner,"brocade");
			break;
			
		}	
			
		case SHOW_RMON_ALARMS_CMD:
		{

			rmon_show_alarms_message_t *alarms = NULL;

            (*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) + ((num_records)*sizeof (rmon_show_alarms_message_t)));
			if (show_brief == 1)
			{
                (*p)->record_type = SHOW_RMON_ALARMS_BRIEF;
			}
			else
			{
				(*p)->record_type = SHOW_RMON_ALARMS;
			}	
            (*p)->num_records = num_records;
			int_ptr = (int*)(*p);
			int_ptr = int_ptr + 2;

			alarms = (rmon_show_alarms_message_t *)int_ptr;
			
			alarms->index = 5;
			alarms->status = 2;
			sprintf(alarms->variable, "1.3.6.1.2.1.16.1.1.1.8.9 \n");
			alarms->interval = 10;
			alarms->sample_type = 1;
			alarms->value = 20;
			alarms->rising_threshold = 30;
			alarms->rising_event = 40;
			alarms->falling_threshold = 10;
			alarms->falling_event = 5;
			strcpy(alarms->owner, "brocade");
			
			alarms++;

			alarms->index = 6;
			alarms->status = 2;
			sprintf(alarms->variable, "1.3.6.1.2.1.16.1.1.1.18.19 \n");
			alarms->interval = 15;
			alarms->sample_type = 2;
			alarms->value = 30;
			alarms->rising_threshold = 40;
			alarms->rising_event = 35;
			alarms->falling_threshold = 15;
			alarms->falling_event = 10;
			strcpy(alarms->owner, "brocade");
			break;
			
		}	
		case SHOW_RMON_LOGS_CMD:
		{

			rmon_show_logs_message_t *ptr = NULL;

            (*p) = (rmon_show_output_msg_t *) calloc(1, sizeof(rmon_show_output_msg_t) + ((num_records - 1)*sizeof (rmon_show_logs_message_t)));
            (*p)->record_type = SHOW_RMON_LOGS;
            (*p)->num_records = num_records;

			int_ptr = (int*)(*p);
			int_ptr = int_ptr + 2;

			ptr = (rmon_show_logs_message_t *)int_ptr;

            ptr->eventIndex = 100;
            ptr->eventConf = 1;
            strcpy(ptr->eventDescription, "event");
            ptr->eventType = 1;
            strcpy(ptr->eventCommunity, "eventCommunity");
            ptr->eventLastTimeSent = 10000;
            strcpy(ptr->eventOwner, "eventOwner");
            ptr->eventStatus = 1;
            ptr->nextLogEntry = 0;
            ptr->logTable[0].logEventIndex = 17;
            ptr->logTable[0].logIndex = 777;
            ptr->logTable[0].logTime = 36000;

			break;
		}

	}
}

int show_rmon_command_handler(const void *inp, void ** const out)
{
  	rmon_show_input_msg_t *msg = (rmon_show_input_msg_t*)inp;
	rmon_show_output_msg_t *payload_p = NULL;
	switch(msg->command_code)
	{
		case SHOW_RMON_HISTORY_CMD:
		  printf("CMD=History\n");
		  break;
		case SHOW_RMON_HISTORY_STATISTICS_CMD:
		  printf("CMD=History Statistics\n");
		  break;
		case SHOW_RMON_STATISTICS_CMD:
		  printf("CMD=Statistics\n");
		  break;
		case SHOW_RMON_EVENTS_CMD:
		  printf("CMD=events\n");
		  break;
		case SHOW_RMON_ALARMS_CMD:
		  printf("CMD=Alarms\n");
		  break;
		case SHOW_RMON_LOGS_CMD:
		  printf("CMD=Logs\n");
		  break;
		default:
		  printf("CMD=%d\n", msg->command_code);
	}
	printf("Index = %d\n", msg->index);
	printf("show_all = %d\n", msg->show_all);
	printf("show_brief = %d\n", msg->show_brief);

	fillPayload(&payload_p, msg->command_code, msg->show_all, msg->show_brief);

   	*out = payload_p;

	return (WRC_WAVE_MESSAGE_SUCCESS);
}

int main(int argc, char **argv)
{
    int ret = 0;
	int mgmt_port = 9109;

	 if (argc != 2) {
        printf("usage: rmon_app.Linux.i686 mgmt_port\n");
        return WRC_WAVE_MESSAGE_SUCCESS;
    }
    mgmt_port += atoi(argv[1]);


	setRmonMsgHandlerForClient();

	add_client_message_handler(RMONCLIENTCREATERMONEVENT,
        create_rmon_event);
	add_client_message_handler(RMONCLIENTDELETERMONEVENT,
        delete_rmon_event);
	add_client_message_handler(RMONCLIENTCREATERMONALARM,
        create_rmon_alarm);
	add_client_message_handler(RMONCLIENTDELETERMONALARM,
        delete_rmon_alarm);
	add_client_message_handler(RMONCLIENTCREATECOLLECTIONSTATISTICS,
        create_rmon_stats);
    add_client_message_handler(RMONCLIENTDELETECOLLECTIONSTATISTICS,
        delete_rmon_stats);
    add_client_message_handler(RMONCLIENTCREATECOLLECTIONHISTORY,
        create_rmon_history);
    add_client_message_handler(RMONCLIENTDELETECOLLECTIONHISTORY,
        delete_rmon_history);
    add_client_message_handler(RMONSHOW,
        show_rmon_command_handler);

    WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);
    
    /* Initialize Dcmd */
    ret = dcm_initialize_client ("rmon");
    if(ret != 0 )
    {
        printf("Failed :: Dcm-client Initialization failed from rmon client... \n");
        assert(ret==0);
    }
    else
    {
        printf("Passed :: Dcm-Client Initialization passed from rmon client... \n");
    } 


    while(1)
        sleep(100);
}
