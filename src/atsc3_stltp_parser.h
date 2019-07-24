/*
 * atsc3_stltp_parser.h
 *
 *  Created on: Mar 16, 2019
 *      Author: jjustman
 */

#ifndef ATSC3_STLTP_PARSER_H_
#define ATSC3_STLTP_PARSER_H_

#include <assert.h>
#include <stdlib.h>
#include "atsc3_utils.h"
#include "atsc3_ip_udp_rtp_parser.h"
#include "atsc3_stltp_types.h"
#include "atsc3_logging_externs.h"

#if defined (__cplusplus)
extern "C" {
#endif
    
atsc3_ip_udp_rtp_packet_t* atsc3_stltp_udp_packet_inner_prepend_fragment(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet);
atsc3_ip_udp_rtp_packet_t* atsc3_stltp_udp_packet_outer_fragment_check_marker(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet);

atsc3_ip_udp_rtp_packet_t* atsc3_stltp_tunnel_packet_inner_parse_ip_udp_header(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet);
    
atsc3_stltp_tunnel_packet_t* atsc3_stltp_raw_packet_extract_inner_from_outer_packet(atsc3_ip_udp_rtp_packet_t* ip_udp_rtp_packet, atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet_fragment);

bool atsc3_stltp_tunnel_packet_extract_fragment_encapsulated_payload(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet_current);
    
atsc3_stltp_baseband_packet_t* atsc3_stltp_baseband_packet_extract(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet_current);
atsc3_stltp_preamble_packet_t* atsc3_stltp_preamble_packet_extract(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet_current);
atsc3_stltp_timing_management_packet_t* atsc3_stltp_timing_management_packet_extract(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet_current);


void atsc3_stltp_tunnel_packet_clear_completed_inner_packets(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet);
    
void atsc3_rtp_header_dump_outer(atsc3_rtp_header_t* atsc3_rtp_header);
void atsc3_rtp_header_dump_inner(atsc3_rtp_header_t* atsc3_rtp_header);

void atsc3_rtp_header_dump(atsc3_rtp_header_t* atsc3_rtp_header, int spaces);

void atsc3_stltp_tunnel_packet_free(atsc3_stltp_tunnel_packet_t** atsc3_stltp_tunnel_packet);
void atsc3_stltp_tunnel_packet_destroy(atsc3_stltp_tunnel_packet_t** atsc3_stltp_tunnel_packet);
void atsc3_stltp_tunnel_packet_outer_destroy(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet);
    
void atsc3_stltp_tunnel_packet_outer_inner_destroy(atsc3_stltp_tunnel_packet_t* atsc3_stltp_tunnel_packet);

#if defined (__cplusplus)
}
#endif

#define __STLTP_PARSER_ERROR(...)  		__LIBATSC3_TIMESTAMP_ERROR(__VA_ARGS__);
#define __STLTP_PARSER_WARN(...)  		__LIBATSC3_TIMESTAMP_WARN(__VA_ARGS__);
#define __STLTP_PARSER_INFO(...)        __LIBATSC3_TIMESTAMP_INFO(__VA_ARGS__);
#define __STLTP_PARSER_DEBUG(...)  		if(_STLTP_PARSER_DEBUG_ENABLED) { __LIBATSC3_TIMESTAMP_DEBUG(__VA_ARGS__); }
#define __STLTP_PARSER_TRACE(...)       if(_STLTP_PARSER_TRACE_ENABLED) { __LIBATSC3_TIMESTAMP_TRACE(__VA_ARGS__); }

#endif /* ATSC3_STLTP_PARSER_H_ */
