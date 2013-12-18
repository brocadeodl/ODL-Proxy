/* **
 * uapp.c
 *
 * June 15, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "uapp.h"

int printUnifiedClientUpdate(const void *inp, void ** const out)
{
    printf("Unified Client Update message handler entering.\n");

    if (!inp)
    {
        printf("Unified Client Update message handler received NULL input structure");
    }

    UnifiedClientMessage_t *msg = (UnifiedClientMessage_t *) inp;

    printUnifiedClientAttributes (msg);

    printf("Unified Client Update message handling complete.\n");

    int i;

    for(i = 0; i < msg->attributeSize; i++)
    {

        if (!strcmp(msg->attributeNames[i],"bgpPassword"))
        {
            UnifiedClientMessageOutputForConfig_t *result = (UnifiedClientMessageOutputForConfig_t *) malloc (sizeof (UnifiedClientMessageOutputForConfig_t));
 
            result->attributeNames = (const char**)malloc (sizeof(char*));
            result->attributeValues = (const char**)malloc (sizeof(char*));
 
            result->attributeNames[0] = msg->attributeNames[i];
            result->attributeValues[0] = "encryptedpass";
            result->attributeSize = 1;
            result->warningCode = 0;
            (*out) = result;
            break;
        }
    }

    return (0);
}

int printUnifiedClientCreate(const void *inp, void ** const out)
{
    printf("Unified Client Create message handler entering.\n");

    if (!inp)
    {
        printf("Unified Client Create message handler received NULL input structure");
    }

    UnifiedClientMessage_t *msg = (UnifiedClientMessage_t *) inp;

    printUnifiedClientAttributes (msg);

    printf("Unified Client Create message handling complete.\n");

    int i;

    for(i = 0; i < msg->attributeSize; i++)
    {
        if (!strcmp(msg->attributeNames[i],"bgpPassword"))
        {
            UnifiedClientMessageOutputForConfig_t *result = (UnifiedClientMessageOutputForConfig_t *) malloc (sizeof (UnifiedClientMessageOutputForConfig_t));
 
            result->attributeNames = (const char**)malloc (sizeof(char*));
            result->attributeValues = (const char**)malloc (sizeof(char*));
 
            result->attributeNames[0] = msg->attributeNames[i];
            result->attributeValues[0] = "encryptedpass";
            result->attributeSize = 1;
            result->warningCode = 0;
            (*out) = result;
            break;
        }
    }


    return (0);
}

int printUnifiedClientDelete(const void *inp, void ** const out)
{
    printf("Unified Client Delete message handler entering.\n");

    if (!inp)
    {
        printf("Unified Client Delete message handler received NULL input structure");
    }

    UnifiedClientMessage_t *msg = (UnifiedClientMessage_t *) inp;

    printUnifiedClientAttributes (msg);

    printf("Unified Client Delete message handling complete.\n");
    return (0);
}


int printUnifiedClientAttributes (UnifiedClientMessage_t *msg)
{
    int i = 0;
    uint32_t j = 0;

	printf("\n\n-----------------------------------------------------------------");
    printf("Unified Client message handled. attribute size = %d \n", msg->attributeSize);

    printf("Unified Client message is for the Managed Object = %s \n", msg->managedObjectName);

    printf("Unified Client message has the group code = %d \n", msg->groupCode);

    for(i = 0; i < msg->parentManagedObjectNamesSize; i++)
    {
        printf (" ParentMO %d = %s\n",i+1,msg->parentManagedObjectNames[i]);
    }

    for(i = 0; i < msg->attributeSize; i++)
    {
         printf("AttributeName[%d] = %s,", i, msg->attributeNames[i]);

         switch ( msg->attributeValues[i].type)
         {
             case WaveNs::WAVE_CVALUE_STRING:
                 printf("AttributeValue[%d] = %s\n", i, wave_cvalue_get_string (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_UI32:
                 printf("AttributeValue[%d] = %u\n", i, wave_cvalue_get_ui32 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_SI32:
                 printf("AttributeValue[%d] = %d\n", i, wave_cvalue_get_si32 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_SI64:
                 printf("AttributeValue[%d] = %lld  \n", i, (long long int) wave_cvalue_get_si64 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_UI64:
                 printf("AttributeValue[%d] = %llu \n", i,(unsigned long long int) wave_cvalue_get_ui64 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_UI16:
                 printf("AttributeValue[%d] = %hu\n", i, wave_cvalue_get_ui16 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_SI16:
                 printf("AttributeValue[%d] = %hd\n", i, wave_cvalue_get_si16 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_UI8:
                 printf("AttributeValue[%d] = %u\n", i, wave_cvalue_get_ui8 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_SI8:
                 printf("AttributeValue[%d] = %d\n", i, wave_cvalue_get_si8 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_CHAR:
                 printf("AttributeValue[%d] = %c\n", i, wave_cvalue_get_char (&(msg->attributeValues[i])));
                 break;

             case WaveNs::WAVE_CVALUE_STRING_VECTOR:
                 {
               	     struct wave_string_vector string_v = wave_cvalue_get_string_vector (&(msg->attributeValues[i])); 
                 	 for( j = 0; j < string_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] = %s\n", i, string_v.values[j] );
                 	 }
                 }       
                 break;
             case WaveNs::WAVE_CVALUE_SI32_VECTOR:
                 {     
                     struct wave_si32_vector si32_v = wave_cvalue_get_si32_vector (&(msg->attributeValues[i])); 
                 	 for(j = 0; j < (uint32_t)si32_v.size; j++)
                 	 {
                 	     printf("AttributeValue[%d] = %d\n", i, si32_v.values[j]);
                 	 }
                 }
                 break;
             case WaveNs::WAVE_CVALUE_UI32_VECTOR:
                 {
                 	 struct wave_ui32_vector ui32_v = wave_cvalue_get_ui32_vector (&(msg->attributeValues[i])); 
                 	 for(j = 0; j < ui32_v.size; j++)
                 	 {
                 	     printf("AttributeValue[%d] = %u\n", i, ui32_v.values[j] );
                 	 }
                 }    
                 break;
             case WaveNs::WAVE_CVALUE_SI64_VECTOR:
                 {
                     struct wave_si64_vector si64_v  = wave_cvalue_get_si64_vector (&(msg->attributeValues[i])); 
                 	 for(j = 0; j < si64_v.size; j++)
                 	 {
                 	     printf("AttributeValue[%d] = %lld\n", i, (long long int) si64_v.values[j]);
                 	 }
                 }
                 break;
             case WaveNs::WAVE_CVALUE_UI64_VECTOR:
                 {
                     struct wave_ui64_vector ui64_v  = wave_cvalue_get_ui64_vector (&(msg->attributeValues[i])); 
                 	 for(j = 0; j< ui64_v.size; j++)
                 	 {
                 	     printf("AttributeValue[%d] = %llu\n", i, (unsigned long long int) ui64_v.values[j]);
                 	 }
                 }
                 break;

             case WaveNs::WAVE_CVALUE_BITMAP:
                 {
                     struct wave_bitmap bitmap_v = wave_cvalue_get_bitmap (&(msg->attributeValues[i]));
              	     printf("AttributeValue[%d],  number_of_bits = %d , number_of_bitblocks = %d \n", i, bitmap_v.number_of_bits, bitmap_v.number_of_bitblocks );
                 }
                 break;
             
			 case WaveNs::WAVE_CVALUE_DATE:
                 {
                     struct wave_date date_value = wave_cvalue_get_date (&(msg->attributeValues[i]));
                     printf("AttributeValue[%d] = date : year=%hu, month=%u, day=%u, hour=%u, min=%u \n", i, date_value.year, date_value.month, date_value.day, date_value.timezone_hours, date_value.timezone_minutes );
                 }
                 break;
             case WaveNs::WAVE_CVALUE_DATE_VECTOR:
                 {
                     struct wave_date_vector date_v = wave_cvalue_get_date_vector (&(msg->attributeValues[i]));
                 	 for( j = 0; j < date_v.size; j++ )
                 	 {
                  	     printf("AttributeValue[%d] = date : year=%hu , month=%u, day=%u, hour=%u, min=%u \n", i, date_v.values[j].year, date_v.values[j].month, date_v.values[j].day, date_v.values[j].timezone_hours, date_v.values[j].timezone_minutes );
				 	 }
                 }
                 break;
             case WaveNs::WAVE_CVALUE_DATE_TIME:
                 {
                 	 struct wave_date_time date_time_value = wave_cvalue_get_date_time (&(msg->attributeValues[i]));
                  	 printf("AttributeValue[%d] = date : year=%hu , month=%u, day=%u, hour=%u, min=%u, sec=%u, timezone_hour=%u, timezone_minutes=%u \n", i, date_time_value.year, date_time_value.month,date_time_value.day, date_time_value.hour, date_time_value.min, date_time_value.sec, date_time_value.timezone_hours, date_time_value.timezone_minutes );
                 }
                 break;
             case WaveNs::WAVE_CVALUE_DATE_TIME_VECTOR:
                 {
                 	 struct wave_date_time_vector date_time_v = wave_cvalue_get_date_time_vector(&(msg->attributeValues[i]));
                 	 for(j=0;j<date_time_v.size;j++)
                 	 {
                  	     printf("AttributeValue[%d] = date : year=%hu , month=%u, day=%u,hour=%u,min=%u sec=%u timezone_hour=%u timezone_minutes=%u \n", i,date_time_v.datetime_values[j].year,date_time_v.datetime_values[j].month,date_time_v.datetime_values[j].day,date_time_v.datetime_values[j].hour,date_time_v.datetime_values[j].min,date_time_v.datetime_values[j].sec,date_time_v.datetime_values[j].timezone_hours,date_time_v.datetime_values[j].timezone_minutes );
				 	 }
                 }
                 break;
             case WaveNs::WAVE_CVALUE_DECIMAL_64:
                 {
                 	 struct wave_decimal_64 decimal64 = wave_cvalue_get_decimal_64 (&(msg->attributeValues[i]));
                 	 printf("AttributeValue[%d] = decimal_64 : decimal64.decimal_value=%lld  decimal64.fraction_digits=%u \n", i, (long long int)decimal64.decimal_value, decimal64.fraction_digits );
                 }
                 break;
             case WaveNs::WAVE_CVALUE_DECIMAL_64_VECTOR:
                 {
                 	 struct wave_decimal_64_vector decimal64_v = wave_cvalue_get_decimal_64_vector (&(msg->attributeValues[i]));
                 	 for( j = 0; j < decimal64_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] = decimal_64 : decimal64.decimal_value=%lld  decimal64.fraction_digits=%u \n", i, (long long int) decimal64_v.decimal_64_values[j].decimal_value, decimal64_v.decimal_64_values[j].fraction_digits );
                 	 }    
                 }
                 break;
             case WaveNs::WAVE_CVALUE_HOST:
                 {
                 	 struct wave_host host_value = wave_cvalue_get_host (&(msg->attributeValues[i]));
                 	 printf("AttributeValue[%d] = host : host_value.address=%s  host_value.type=%d \n", i, host_value.address, host_value.host_type );
                 }
                 break;
             case WaveNs::WAVE_CVALUE_IPADDRESS_MASK:
                 {
                     struct wave_ipvx_prefix ipnmask = wave_cvalue_get_ipaddress (&(msg->attributeValues[i]));
                     if (ipnmask.family == AF_INET)
                     {
                 	    printf ("AttributeValue[%d] =ipaddress_networkmask : masklen=%d \n", i, ipnmask.prefixlen);
                     }   
                     else if (ipnmask.family == AF_INET6)
                     {
                 	    printf ("AttributeValue[%d] =ipaddress_networkmask : masklen=%d \n", i, ipnmask.prefixlen );
                     }   
                     // calling wave_ipvx_prefix2str directly in printf will cause memory leak as the function allocates a buffer and returns the pointer
                     // the callee should take care of deallocationg the memory
                     char *str = wave_ipvx_prefix2str (&ipnmask);
                     printf (" Attribute is  WAVE_CVALUE_IPADDRESS_MASK the value of ip/mask is : %s \n", str);
                     free (str);
                 }
                 break;
             case WaveNs::WAVE_CVALUE_IPADDRESS:
                 {
                     struct wave_ipvx_prefix ipvx = wave_cvalue_get_ipaddress (&(msg->attributeValues[i]));

                     if (ipvx.family == AF_INET)
                     {
                 	    printf ("AttributeValue[%d] =ipvxaddress : type=IPV4 (AF_INET) \n", i);
                     }   
                     else if (ipvx.family == AF_INET6)
                     {
                 	    printf ("AttributeValue[%d] =ipvxaddress : type=IPV6 (AF_INET6) \n", i);
                     }   
                     // calling wave_ipvx_prefix2str directly in printf will cause memory leak as the function allocates a buffer and returns the pointer
                     // the callee should take care of deallocationg the memory
                     char *str = wave_ipvx_prefix2str (&ipvx);
                     printf (" Attribute is  WAVE_CVALUE_IPADDRESS the value of Ip is  : %s \n", str);
                     free (str);
                 }
                 break;
             case WaveNs::WAVE_CVALUE_IPADDRESS_VECTOR:
                 {
                     struct wave_ipaddress_vector ipvx_v = wave_cvalue_get_ipaddress_vector (&(msg->attributeValues[i]));
                 	 for ( j = 0; j < ipvx_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] =ipvxaddress : ip=%s  type=%d \n", i, ipvx_v.values[j].ip_address, ipvx_v.values[j].ip_type );
                 	 }
                 }
                 break;
             case WaveNs::WAVE_CVALUE_LARGEOBJECT:
                 printf("AttributeValue[%d] = %s\n", i, "WaveNs::WAVE_CVALUE_LARGEOBJECT" );
                 break;
             case WaveNs::WAVE_CVALUE_SNMP_OBJECT_ID:
                 {
                 	 struct wave_snmp_objectid oid = wave_cvalue_get_snmp_objectid (&(msg->attributeValues[i]));
                 	 for( j = 0 ; j < (unsigned int)oid.length; j++)
                 	 {
                 		 printf("AttributeValue[%d] =snmp_objectid : oid.oid[]=%u  oid.length=%u \n", i, oid.oid[j], oid.length );
                 	 }
                 }
                 break;
             case WaveNs::WAVE_CVALUE_TIME:
                 {
                     struct wave_time time_value = wave_cvalue_get_time (&(msg->attributeValues[i]));
                  	 printf("AttributeValue[%d] = time : hour=%u  min=%u sec=%u micro=%u timezone_hour=%u timezone_minutes=%u \n", i, time_value.hour,time_value.min, time_value.sec, time_value.micro, time_value.timezone, time_value.timezone_minutes );
                 }
                 break;
             case WaveNs::WAVE_CVALUE_TIME_VECTOR:      
                 {
                 	 struct wave_time_vector time_v = wave_cvalue_get_time_vector(&(msg->attributeValues[i]));
                 	 for( j = 0; j < time_v.size; j++ )
                 	 {
                  	     printf("AttributeValue[%d] = time : hour=%u min=%u sec=%u micro=%u timezone_hour=%u timezone_minutes=%u \n", i, time_v.values[j].hour, time_v.values[j].min, time_v.values[j].sec, time_v.values[j].micro, time_v.values[j].timezone, time_v.values[j].timezone_minutes );
				 	 }
                 }
                 break;
             
             case WaveNs::WAVE_CVALUE_MAC_ADDRESS:
                 printf("AttributeValue[%d] = %s\n", i, wave_cvalue_get_macaddress (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_MAC_ADDRESS2:
                 printf("AttributeValue[%d] = %s\n", i, wave_cvalue_get_macaddress2 (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_WORLD_WIDE_NAME:
                 printf("AttributeValue[%d] = %s\n", i, wave_cvalue_get_world_wide_name (&(msg->attributeValues[i])));
                 break;
             case WaveNs::WAVE_CVALUE_MAC_ADDRESS_VECTOR:
                 {
               	     struct wave_string_vector string_v = wave_cvalue_get_string_vector (&(msg->attributeValues[i])); 
                 	 for( j = 0; j < string_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] = %s\n", i, string_v.values[j] );
                 	 }
                 }       
                 break;

             case WaveNs::WAVE_CVALUE_MAC_ADDRESS2_VECTOR:
                 {
               	     struct wave_string_vector string_v = wave_cvalue_get_string_vector (&(msg->attributeValues[i])); 
                 	 for( j = 0; j < string_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] = %s\n", i, string_v.values[j] );
                 	 }
                 }       
                 break;
             case WaveNs::WAVE_CVALUE_WORLD_WIDE_NAME_VECTOR:
                 {
               	     struct wave_string_vector string_v = wave_cvalue_get_string_vector (&(msg->attributeValues[i])); 
                 	 for( j = 0; j < string_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] = %s\n", i, string_v.values[j] );
                 	 }
                 }       
                 break;
             case WaveNs::WAVE_CVALUE_MAC_RANGE:
                 {
               	     struct wave_string_vector string_v = wave_cvalue_get_string_vector (&(msg->attributeValues[i])); 
                 	 for( j = 0; j < string_v.size; j++ )
                 	 {
                 	     printf("AttributeValue[%d] = %s\n", i, string_v.values[j] );
                 	 }
                 }       
                 break;
			 default:
                 printf("AttributeValue[%d] = %d\n", i, wave_cvalue_get_ui32 (&(msg->attributeValues[i])));
				 break;
         }
    }

    printf ("Unified Client message handling complete.\n");
    return (0);
}

/**
 * Start the client process and wait forever.... Handles unified plugin
 * create/update/delete messages.
 */
void start_client(const char *name, int mgmt_port_offset) {
    int mgmt_port = 9109 + mgmt_port_offset;

    printf("***** Starting client \"%s\"; management port %d *****\n", name, mgmt_port);

    add_client_message_handler(UNIFIEDCLIENTUPDATE, printUnifiedClientUpdate);
    add_client_message_handler(UNIFIEDCLIENTCREATE, printUnifiedClientCreate);
    add_client_message_handler(UNIFIEDCLIENTDELETE, printUnifiedClientDelete);
    add_client_message_handler(UNIFIEDCLIENTPOSTBOOT, printUnifiedClientCreate);

    WaveNs::FrameworkToolKit::setManagementInterfaceReceiverPort(mgmt_port);

    /* Initialize Dcmd */
    int ret = dcm_initialize_client (name);

    if (ret != 0 ) {
        printf("Failed :: Dcm-client Initialization failed from sample client... \n");
        assert(ret==0);
    } else {
        printf("Passed :: Dcm-Client Initialization passed from sample client... \n");
    }

    while(1) {
        sleep(1000000);
    }
}

