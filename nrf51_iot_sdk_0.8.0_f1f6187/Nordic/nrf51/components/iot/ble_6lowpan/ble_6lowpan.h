/* Copyright (c)  2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file ble_6lowpan.h
 *
 * @defgroup ble_6lowpan BLE 6LoWPAN library
 * @ingroup iot_sdk_6lowpan
 * @{
 * @brief 6LoWPAN techniques defined for BLE.
 *
 * @details This module implements 6LoWPAN techniques defined for BLE, including
 * IP and UDP header compression and decompression and conversion of EUI-48 BLE
 *          addresses to EUI-64 and on to IPv6 addresses. This layer does not implement IP level
 *          functionality of neighbor discovery etc.
 * @note Currently, only the 6LoWPAN node (host) role is supported.
 */

#ifndef BLE_6LOWPAN_H__
#define BLE_6LOWPAN_H__

#include <stdint.h>
#include "iot_defines.h"
#include "iot_common.h"

/**@brief Maximum 6LoWPAN interface supported by module. */
#define BLE_6LOWPAN_MAX_INTERFACE                          1

/**@brief Maximum transmit packets that are buffered per interface.
 *
 * @details FIFO size must be a power of 2.
 */
#define BLE_6LOWPAN_TX_FIFO_SIZE                           16

/**@brief Asynchronous event identifiers type. */
typedef enum
{
    BLE_6LO_EVT_ERROR,                                                                              /**< Notification of an error in the module. */
    BLE_6LO_EVT_INTERFACE_ADD,                                                                      /**< Notification of a new 6LoWPAN interface added. */
    BLE_6LO_EVT_INTERFACE_DELETE,                                                                   /**< Notification of a 6LoWPAN interface deleted. */
    BLE_6LO_EVT_INTERFACE_DATA_RX,                                                                  /**< Notification of an IP packet received on the interface. */
}ble_6lowpan_event_id_t;

/**@brief Event parameters associated with the BLE_6LO_EVT_INTERFACE_DATA_RX event. */
typedef struct
{
    uint8_t               * p_packet;                                                               /**< Uncompressed IPv6 packet received. This memory is available to the application until it is freed by the application using mem_free. */
    uint16_t                packet_len;                                                             /**< Length of IPv6 packet. */
    iot_context_id_t        rx_contexts;                                                            /**< RX contexts used in stateful decompression. IPV6_CONTEXT_IDENTIFIER_NONE if not used. */
}ble_6lowpan_data_rx_t;

/**@brief Asynchronous event parameter type. */
typedef union
{
    ble_6lowpan_data_rx_t rx_event_param;                                                           /**< Parameters notified with the received packet. */
}ble_6lowpan_event_param_t;

/**@brief Asynchronous event type. */
typedef struct
{
    ble_6lowpan_event_id_t    event_id;                                                             /**< Event identifier. */
    ble_6lowpan_event_param_t event_param;                                                          /**< Event parameters. */
    uint32_t                  event_result;                                                         /**< Result of event being notified. */
}ble_6lowpan_event_t;

/**@brief Asynchronous event notification callback type. */
typedef void (*ble_6lowpan_evt_handler_t) (iot_interface_t     * p_interface,
                                           ble_6lowpan_event_t * p_event);


/**@brief Initialization parameters type. */
typedef struct
{
    eui64_t                    * p_eui64;                                                           /**< EUI-64 address. */
    ble_6lowpan_evt_handler_t    event_handler;                                                     /**< Asynchronous event notification callback registered to receive 6LoWPAN events. */
}ble_6lowpan_init_t;


/**@brief Initializes the module.
 *
 * @param[in]  p_init  Initialization parameters.
 *
 * @retval NRF_SUCCESS If initialization was successful. Otherwise, an error code is returned.
 */
uint32_t  ble_6lowpan_init(const ble_6lowpan_init_t * p_init);

/**@brief   Sends IPv6 packet on the 6LoWPAN interface.
 *
 * @details This function is used to send an IPv6 packet on the interface. 6LoWPAN compression techniques are
 *          applied on the packet before the packet is transmitted. The packet might not be
 *          transferred to the peer immediately based on the flow control on the BLE Link. In this
 *          case, the packet is queued to be transferred later.
 *
 * @param[in]  p_interface  Identifies the interface on which the packet is to be sent.
 * @param[in]  p_packet     IPv6 packet to be sent. Memory for the packet should be allocated using
 *                          mem_alloc and should not be freed. The module is 
 *                          responsible for freeing the memory using
 *                          mem_free. The module will free the packet once the transmission is
 *                          complete or the packet can no longer be transmitted (in case of link
 *                          disconnection.)
 * @param[in]  packet_len   Length of the IPv6 packet.
 *
 * @retval NRF_SUCCESS If the send request was successful.
 */
uint32_t  ble_6lowpan_interface_send(const iot_interface_t * p_interface,
                                     const uint8_t         * p_packet,
                                     uint16_t                packet_len);

#endif //BLE_6LOWPAN_H__

/** @} */
