/*
 * atsc3_stltp_types.c
 *
 *  Created on: Jul 23, 2019
 *      Author: jjustman
 */


#include "atsc3_stltp_types.h"

int _STLTP_TYPES_DEBUG_ENABLED = 1;
int _STLTP_TYPES_TRACE_ENABLED = 1;

//L1_detail vector(s)

ATSC3_VECTOR_BUILDER_METHODS_PARENT_IMPLEMENTATION(L1_detail_signaling);
ATSC3_VECTOR_BUILDER_METHODS_IMPLEMENTATION(L1_detail_signaling, L1D_bonded_bsid_block);
ATSC3_VECTOR_BUILDER_METHODS_ITEM_FREE(L1D_bonded_bsid_block);

ATSC3_VECTOR_BUILDER_METHODS_PARENT_IMPLEMENTATION(timing_management_packet)
ATSC3_VECTOR_BUILDER_METHODS_IMPLEMENTATION(timing_management_packet, bootstrap_timing_data);
ATSC3_VECTOR_BUILDER_METHODS_ITEM_FREE(bootstrap_timing_data);
ATSC3_VECTOR_BUILDER_METHODS_IMPLEMENTATION(timing_management_packet, per_transmitter_data);
ATSC3_VECTOR_BUILDER_METHODS_ITEM_FREE(per_transmitter_data);


ATSC3_VECTOR_BUILDER_METHODS_PARENT_IMPLEMENTATION(atsc3_stltp_tunnel_packet)
ATSC3_VECTOR_BUILDER_METHODS_IMPLEMENTATION(atsc3_stltp_tunnel_packet, atsc3_stltp_baseband_packet);
ATSC3_VECTOR_BUILDER_METHODS_IMPLEMENTATION(atsc3_stltp_tunnel_packet, atsc3_stltp_preamble_packet);
ATSC3_VECTOR_BUILDER_METHODS_IMPLEMENTATION(atsc3_stltp_tunnel_packet, atsc3_stltp_timing_management_packet);

inline const char *ATSC3_CTP_STL_PAYLOAD_TYPE_TO_STRING(int code) {
    switch (code) {
        case ATSC3_STLTP_PAYLOAD_TYPE_TUNNEL:
            return ATSC3_STLTP_PAYLOAD_TYPE_TUNNEL_STRING;
        case ATSC3_STLTP_PAYLOAD_TYPE_TIMING_MANAGEMENT_PACKET:
            return ATSC3_STLTP_PAYLOAD_TYPE_TIMING_MANAGEMENT_PACKET_STRING;
        case ATSC3_STLTP_PAYLOAD_TYPE_PREAMBLE_PACKET:
            return ATSC3_STLTP_PAYLOAD_TYPE_PREAMBLE_PACKET_STRING;
        case ATSC3_STLTP_PAYLOAD_TYPE_BASEBAND_PACKET:
            return ATSC3_STLTP_PAYLOAD_TYPE_BASEBAND_PACKET_STRING;
    }
    return "";
}

/*
 create a new stltp_baseband packet for stltp inner extraction, then hand off to baseband re-segmentation
*/
atsc3_stltp_baseband_packet_t* atsc3_stltp_baseband_packet_new_and_init(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {

    atsc3_stltp_baseband_packet_t* atsc3_stltp_baseband_packet = atsc3_stltp_baseband_packet_new();
    
    //ref ip_udp_rtp_packet_outer w/ rtp_header_outer
    atsc3_stltp_baseband_packet->ip_udp_rtp_packet_outer = atsc3_ip_udp_rtp_packet_duplicate(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer);

    //ref ip_udp_rtp_packet_inner w/ rtp_header_inner
    atsc3_stltp_baseband_packet->ip_udp_rtp_packet_inner = atsc3_ip_udp_rtp_packet_duplicate(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner);
    
    //TODO: inner packet size, only if we are from a marker
    atsc3_stltp_baseband_packet->payload_length = atsc3_stltp_baseband_packet->ip_udp_rtp_packet_inner->rtp_header->packet_offset;
    
    return atsc3_stltp_baseband_packet;
}


/**
 
 todo: 2019-03-16
 
 free any udp_packet reassembly payloads
 
 todo: 2019-07-23 - clear vector(s)
 
 **/

void atsc3_stltp_tunnel_packet_clear_completed_inner_packets(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {
    if(atsc3_stltp_tunnel_packet) {
        if(atsc3_stltp_tunnel_packet->atsc3_stltp_baseband_packet_v.count) {
            for(int i=0; i < atsc3_stltp_tunnel_packet->atsc3_stltp_baseband_packet_v.count; i++) {
                atsc3_stltp_baseband_packet_free_v(atsc3_stltp_tunnel_packet->atsc3_stltp_baseband_packet_v.data[i]);
            }
            atsc3_stltp_tunnel_packet_clear_atsc3_stltp_baseband_packet(atsc3_stltp_tunnel_packet);
        }
        
        if(atsc3_stltp_tunnel_packet->atsc3_stltp_baseband_packet_v.data) {
            free(atsc3_stltp_tunnel_packet->atsc3_stltp_baseband_packet_v.data);
            atsc3_stltp_tunnel_packet->atsc3_stltp_baseband_packet_v.data = NULL;
        }
        
        if(atsc3_stltp_tunnel_packet->atsc3_stltp_preamble_packet_v.count) {
            for(int i=0; i < atsc3_stltp_tunnel_packet->atsc3_stltp_preamble_packet_v.count; i++) {
                atsc3_stltp_preamble_packet_free_v(atsc3_stltp_tunnel_packet->atsc3_stltp_preamble_packet_v.data[i]);
            }
            atsc3_stltp_tunnel_packet_clear_atsc3_stltp_preamble_packet(atsc3_stltp_tunnel_packet);
        }
        
        if(atsc3_stltp_tunnel_packet->atsc3_stltp_preamble_packet_v.data) {
            free(atsc3_stltp_tunnel_packet->atsc3_stltp_preamble_packet_v.data);
            atsc3_stltp_tunnel_packet->atsc3_stltp_preamble_packet_v.data = NULL;
        }
        
        if(atsc3_stltp_tunnel_packet->atsc3_stltp_timing_management_packet_v.count) {
            for(int i=0; i < atsc3_stltp_tunnel_packet->atsc3_stltp_timing_management_packet_v.count; i++) {
                atsc3_stltp_timing_management_packet_free_v(atsc3_stltp_tunnel_packet->atsc3_stltp_timing_management_packet_v.data[i]);
            }
            atsc3_stltp_tunnel_packet_clear_atsc3_stltp_timing_management_packet(atsc3_stltp_tunnel_packet);
        }
        
        if(atsc3_stltp_tunnel_packet->atsc3_stltp_timing_management_packet_v.data) {
            free(atsc3_stltp_tunnel_packet->atsc3_stltp_timing_management_packet_v.data);
            atsc3_stltp_tunnel_packet->atsc3_stltp_timing_management_packet_v.data = NULL;
        }
    }
}




/**
 free both inner and outer packets if inner/outer data block_t don't match (refragmentation or concatenation/segmentation),
 oterwise only block_release one _t

 THIS METHOD IS DANGEROUS
 
 full destroy of all member*'s
 
 make sure to clone before re-assigning ptr vals to another tunnel packet reference

 **/


void atsc3_stltp_tunnel_packet_free(atsc3_stltp_tunnel_packet_t** atsc3_stltp_tunnel_packet_p) {
    if(atsc3_stltp_tunnel_packet_p) {
        atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet = *atsc3_stltp_tunnel_packet_p;
        if(atsc3_stltp_tunnel_packet) {
            if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner) {
                atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner);
            }
            
            if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer) {
                atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer);
            }
            
            if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_pending_refragmentation_outer) {
                atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_pending_refragmentation_outer);
            }
            
            if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_pending_concatenation_inner) {
                atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_pending_concatenation_inner);
            }
            
            if(atsc3_stltp_tunnel_packet->atsc3_baseband_packet_short_fragment) {
                block_Destroy(&atsc3_stltp_tunnel_packet->atsc3_baseband_packet_short_fragment);
            }
            
            
            atsc3_stltp_tunnel_packet_clear_completed_inner_packets(atsc3_stltp_tunnel_packet);
            free(atsc3_stltp_tunnel_packet);
            atsc3_stltp_tunnel_packet = NULL;
        }
        *atsc3_stltp_tunnel_packet_p = NULL;
    }
}

//hard destrory of inner/outer packets
void atsc3_stltp_tunnel_packet_destroy(atsc3_stltp_tunnel_packet_t** atsc3_stltp_tunnel_packet_p) {
    if(atsc3_stltp_tunnel_packet_p) {
        atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet = *atsc3_stltp_tunnel_packet_p;
        if(atsc3_stltp_tunnel_packet) {
            
            atsc3_ip_udp_rtp_packet_destroy_outer_inner(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer, &atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner);
            
            atsc3_stltp_tunnel_packet_clear_completed_inner_packets(atsc3_stltp_tunnel_packet);
            free(atsc3_stltp_tunnel_packet);
            atsc3_stltp_tunnel_packet = NULL;
        }
        *atsc3_stltp_tunnel_packet_p = NULL;
    }
}


//hard destrory of outer packets and corresponding data block_t*'s
//do not destroy the inner ip_udp_rtp packet payload, as we will need it if we are un-segmenting inner packets
void atsc3_stltp_tunnel_packet_outer_destroy(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {
    if(atsc3_stltp_tunnel_packet) {
        
        //check to make sure we don't doublefree inner/outer -> data, so only free one instance of block_t but update both as being cleared
        if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer && atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner &&
           atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer->data == atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner->data) {
            block_Destroy(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer->data);
            atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner->data = NULL;
        }
        if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer) {
            atsc3_ip_udp_rtp_packet_destroy(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer);
            atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer = NULL;
        }
    }
}



//hard destrory of outer packets and corresponding data block_t*'s
void atsc3_stltp_tunnel_packet_inner_destroy(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {
    if(atsc3_stltp_tunnel_packet) {
        
        if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer && atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner &&
           atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer->data == atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner->data) {
            // we can't delete the pointer for outer/inner data, so just free our rtp header
            atsc3_rtp_header_free(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner->rtp_header);
        } else if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner) {
            atsc3_ip_udp_rtp_packet_destroy(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner);
            atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner = NULL;
        }
    }
}


//hard destrory of INNER and outer packets and corresponding data block_t*'s
//do not destroy the inner ip_udp_rtp packet payload, as we will need it if we are un-segmenting inner packets
void atsc3_stltp_tunnel_packet_outer_inner_destroy(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {
    if(atsc3_stltp_tunnel_packet) {
        
//        //check to make sure we don't doublefree inner/outer -> data, so only free one instance of block_t but update both as being cleared
//        if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer && atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner &&
//           atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer->data == atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner->data) {
//            block_Destroy(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer->data);
//            atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner->data = NULL;
//        }
        if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer) {
            atsc3_ip_udp_rtp_packet_destroy(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer);
            atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer = NULL;
        }
        if(atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner) {
            atsc3_ip_udp_rtp_packet_destroy(&atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner);
            atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner = NULL;
        }
    }
}



/*
 free the complete baseband packet, including its internal payload buffer for baseband re-assembly
 */
void atsc3_stltp_baseband_packet_free_v(atsc3_stltp_baseband_packet_t* atsc3_stltp_baseband_packet) {
    if(atsc3_stltp_baseband_packet) {

        atsc3_ip_udp_rtp_packet_destroy_outer_inner(&atsc3_stltp_baseband_packet->ip_udp_rtp_packet_outer, &atsc3_stltp_baseband_packet->ip_udp_rtp_packet_inner);
        
        //TODO: move this over to block_t
        if(atsc3_stltp_baseband_packet->payload) {
            free(atsc3_stltp_baseband_packet->payload);
            atsc3_stltp_baseband_packet->payload = NULL;
        }
    }
}


/*
 free only the ip_udp_rtp_packet outer/inner data payloads, as we will have rebuilt what the baseband packet needs in its ->payload
 do not remove the rtp_header outer and inner, as this data may be needed for coorelation of the timestamp field(s)
 */
void atsc3_stltp_baseband_packet_free_outer_inner_data(atsc3_stltp_baseband_packet_t* atsc3_stltp_baseband_packet) {
    
    if(atsc3_stltp_baseband_packet->ip_udp_rtp_packet_outer && atsc3_stltp_baseband_packet->ip_udp_rtp_packet_outer->data) {
        block_Destroy(&atsc3_stltp_baseband_packet->ip_udp_rtp_packet_outer->data);
    }
    
    if(atsc3_stltp_baseband_packet->ip_udp_rtp_packet_inner && atsc3_stltp_baseband_packet->ip_udp_rtp_packet_inner->data) {
        block_Destroy(&atsc3_stltp_baseband_packet->ip_udp_rtp_packet_inner->data);
    }
}

//copy/paste warning - atsc3_stltp_preamble_packet
void atsc3_stltp_preamble_packet_free_outer_inner_data(atsc3_stltp_preamble_packet_t* atsc3_stltp_preamble_packet) {
    
    if(atsc3_stltp_preamble_packet->ip_udp_rtp_packet_outer && atsc3_stltp_preamble_packet->ip_udp_rtp_packet_outer->data) {
        block_Destroy(&atsc3_stltp_preamble_packet->ip_udp_rtp_packet_outer->data);
    }
    
    if(atsc3_stltp_preamble_packet->ip_udp_rtp_packet_inner && atsc3_stltp_preamble_packet->ip_udp_rtp_packet_inner->data) {
        block_Destroy(&atsc3_stltp_preamble_packet->ip_udp_rtp_packet_inner->data);
    }
}

//copy/paste warning - atsc3_stltp_timing_management_packet
void atsc3_stltp_timing_management_packet_free_outer_inner_data(atsc3_stltp_timing_management_packet_t* atsc3_stltp_timing_management_packet) {
    
    if(atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_outer && atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_outer->data) {
        block_Destroy(&atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_outer->data);
    }
    
    if(atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_inner && atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_inner->data) {
        block_Destroy(&atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_inner->data);
    }
}



void atsc3_stltp_preamble_packet_free_v(atsc3_stltp_preamble_packet_t* atsc3_stltp_preamble_packet) {
    if(atsc3_stltp_preamble_packet) {
        
        //this should be all boilerplate
        atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_preamble_packet->ip_udp_rtp_packet_outer);
        atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_preamble_packet->ip_udp_rtp_packet_inner);
        //this should be all boilerplate
    
        if(atsc3_stltp_preamble_packet->payload) {
            free(atsc3_stltp_preamble_packet->payload);
            atsc3_stltp_preamble_packet->payload = NULL;        
        }
        //let vector_v initiate the pointer free
        //free(atsc3_stltp_preamble_packet);
    }
}

/**
 
 atsc3_ip_udp_rtp_packet_t*      ip_udp_rtp_packet_outer;
 atsc3_rtp_header_t*             rtp_header_outer; //pointer from ip_udp_rtp_packet_outer->rtp_header
 
 atsc3_ip_udp_rtp_packet_t*      ip_udp_rtp_packet_inner;
 atsc3_rtp_header_t*             rtp_header_inner; //pointer from ip_udp_rtp_packet_outer->rtp_header

 **/
void atsc3_stltp_timing_management_packet_free_v(atsc3_stltp_timing_management_packet_t* atsc3_stltp_timing_management_packet) {
    if(atsc3_stltp_timing_management_packet) {
        
        //this shoudl be all boilerplate
        atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_outer);
        atsc3_ip_udp_rtp_packet_free(&atsc3_stltp_timing_management_packet->ip_udp_rtp_packet_inner);
        //this shoudl be all boilerplate

        //stltp CTP specific attibutes here
        if(atsc3_stltp_timing_management_packet->payload) {
            free(atsc3_stltp_timing_management_packet->payload);
            atsc3_stltp_timing_management_packet->payload = NULL;
        }
    }
}

/*
 free complete object including ptr*this
*/
void atsc3_stltp_baseband_packet_free(atsc3_stltp_baseband_packet_t** atsc3_stltp_baseband_packet_p) {
    if(atsc3_stltp_baseband_packet_p) {
        atsc3_stltp_baseband_packet_t* atsc3_stltp_baseband_packet = *atsc3_stltp_baseband_packet_p;
        if(atsc3_stltp_baseband_packet) {
            atsc3_stltp_baseband_packet_free_v(atsc3_stltp_baseband_packet);
            free(atsc3_stltp_baseband_packet);
            atsc3_stltp_baseband_packet = NULL;
        }
        *atsc3_stltp_baseband_packet_p = NULL;
    }
}

void atsc3_stltp_preamble_packet_free(atsc3_stltp_preamble_packet_t** atsc3_stltp_preamble_packet_p) {
    if(atsc3_stltp_preamble_packet_p) {
        atsc3_stltp_preamble_packet_t* atsc3_stltp_preamble_packet = *atsc3_stltp_preamble_packet_p;
        if(atsc3_stltp_preamble_packet) {
            atsc3_stltp_preamble_packet_free_v(atsc3_stltp_preamble_packet);
            free(atsc3_stltp_preamble_packet);
            atsc3_stltp_preamble_packet = NULL;
        }
        *atsc3_stltp_preamble_packet_p = NULL;
    }
}

void atsc3_stltp_timing_management_packet_free(atsc3_stltp_timing_management_packet_t** atsc3_stltp_timing_management_packet_p) {
    if(atsc3_stltp_timing_management_packet_p) {
        atsc3_stltp_timing_management_packet_t* atsc3_stltp_timing_management_packet = *atsc3_stltp_timing_management_packet_p;
        if(atsc3_stltp_timing_management_packet) {
            atsc3_stltp_timing_management_packet_free_v(atsc3_stltp_timing_management_packet);
            free(atsc3_stltp_timing_management_packet);
            atsc3_stltp_timing_management_packet = NULL;
        }
        *atsc3_stltp_timing_management_packet_p = NULL;
    }
}


/**
 header dump methods
 **/


void atsc3_rtp_header_dump_outer(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {
    atsc3_ip_udp_rtp_packet_t* atsc3_ip_udp_rtp_packet_outer = atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_outer;
    atsc3_rtp_header_t* atsc3_rtp_header = atsc3_ip_udp_rtp_packet_outer->rtp_header;
    udp_flow_t atsc3_udp_flow = atsc3_ip_udp_rtp_packet_outer->udp_flow;
    
    __STLTP_TYPES_DEBUG(" ---outer: payload_type: %s (%hhu), sequence_number: %d (p: %p), dst: %u.%u.%u.%u:%u, pos: %d, size: %d---",
                         ATSC3_CTP_STL_PAYLOAD_TYPE_TO_STRING(atsc3_rtp_header->payload_type),
                         atsc3_rtp_header->payload_type,
                         atsc3_rtp_header->sequence_number,
                         atsc3_ip_udp_rtp_packet_outer,
                         __toipandportnonstruct(atsc3_udp_flow.dst_ip_addr, atsc3_udp_flow.dst_port),
                        atsc3_ip_udp_rtp_packet_outer->data ? atsc3_ip_udp_rtp_packet_outer->data->i_pos : -1,
                        atsc3_ip_udp_rtp_packet_outer->data ? atsc3_ip_udp_rtp_packet_outer->data->p_size : -1);
    atsc3_rtp_header_dump(atsc3_rtp_header, 5);
    __STLTP_TYPES_DEBUG(" ---outer: end---");
}

void atsc3_rtp_header_dump_inner(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet) {
    atsc3_ip_udp_rtp_packet_t* atsc3_ip_udp_rtp_packet_inner = atsc3_stltp_tunnel_packet->ip_udp_rtp_packet_inner;
    atsc3_rtp_header_t* atsc3_rtp_header = atsc3_ip_udp_rtp_packet_inner->rtp_header;
    udp_flow_t atsc3_udp_flow = atsc3_ip_udp_rtp_packet_inner->udp_flow;
    
    __STLTP_TYPES_DEBUG("  ---inner: payload_type: %s  (%hhu), sequence_number: %d, dst: %u.%u.%u.%u:%u---",
                         ATSC3_CTP_STL_PAYLOAD_TYPE_TO_STRING(atsc3_rtp_header->payload_type),
                         atsc3_rtp_header->payload_type,
                         atsc3_rtp_header->sequence_number,
                         __toipandportnonstruct(atsc3_udp_flow.dst_ip_addr, atsc3_udp_flow.dst_port));
    atsc3_rtp_header_dump(atsc3_rtp_header, 9);
    __STLTP_TYPES_DEBUG(" ---inner: end---");

}

void atsc3_rtp_header_dump(atsc3_rtp_header_t* atsc3_rtp_header, int spaces) {
    
    __STLTP_TYPES_DEBUG("%*smarker: %x,  version: %x, padding: %x, extension: %x, csrc_count: %x",
                        spaces, "",
                        atsc3_rtp_header->marker,
                        atsc3_rtp_header->version,
                        atsc3_rtp_header->padding,
                        atsc3_rtp_header->extension,
                        atsc3_rtp_header->csrc_count);
    
    //tunnel packet has packet offset
    if(atsc3_rtp_header->payload_type == 0x61) {
        
        __STLTP_TYPES_DEBUG("%*ssequence_number: 0x%x (%u), timestamp: 0x%x (%u)  packet_offset: 0x%x (%u)", spaces, "",
                             atsc3_rtp_header->sequence_number, atsc3_rtp_header->sequence_number,
                             atsc3_rtp_header->timestamp, atsc3_rtp_header->timestamp,
                             atsc3_rtp_header->packet_offset, atsc3_rtp_header->packet_offset);
    } else {
        __STLTP_TYPES_DEBUG("%*ssequence_number: 0x%x (%u), timestamp: 0x%x (%u)  packet length: 0x%x (%u)", spaces, "",
                             atsc3_rtp_header->sequence_number, atsc3_rtp_header->sequence_number,
                             atsc3_rtp_header->timestamp, atsc3_rtp_header->timestamp,
                             atsc3_rtp_header->packet_offset, atsc3_rtp_header->packet_offset);
    }
}
