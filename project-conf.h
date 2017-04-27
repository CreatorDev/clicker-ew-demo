/**
 * @file
 * Clicker provisioning library Project Config.
 *
 * @author Imagination Technologies
 *
 * @copyright Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated group
 * companies and/or licensors.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

/* Custom channel and PAN ID configuration for your project. */

#define RF_CHANNEL                  23
#define IEEE802154_CONF_PANID       0xAAAA

#define UIP_CONF_TCP                   0
#define REST_MAX_CHUNK_SIZE            512
#undef COAP_MAX_OPEN_TRANSACTIONS
#define COAP_MAX_OPEN_TRANSACTIONS     4
#define COAP_LINK_FORMAT_FILTERING     0
#define COAP_PROXY_OPTION_PROCESSING   0
#define COAP_OBSERVE_CLIENT            1
#define UIP_CONF_TCP                   0
#define UIP_CONF_BUFFER_SIZE           4096

#define GLOBAL_IPv6_ADDR
#define GLOBAL_IPv6_ADDR1              0xFDFE
#define GLOBAL_IPv6_ADDR2              0x326B
#define GLOBAL_IPv6_ADDR3              0xF4E0
#define GLOBAL_IPv6_ADDR4              0x10
#define GLOBAL_IPv6_ADDR5              0x0
#define GLOBAL_IPv6_ADDR6              0x0
#define GLOBAL_IPv6_ADDR7              0x0
#define GLOBAL_IPv6_ADDR8              0x0

//this should be uncommented for UDP version, for two other versions please coment it.
#define UIP_CONF_UDP                   1
#endif /* __PROJECT_CONF_H__ */
