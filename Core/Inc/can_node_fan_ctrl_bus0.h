/* Do not edit */
/* This file is generated automatically by dbc_conv */

#ifndef __CAN_NODE_FAN_CTRL_BUS0_H__
#define __CAN_NODE_FAN_CTRL_BUS0_H__
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <can_platform.h>

#define ALIVE_THRESHOLD 10

typedef struct
{
    uint8_t ctrl_to_fan;
} t_can_node_fan_ctrl_bus0_input_alive;

typedef struct
{
    struct
    {
        uint8_t FAN_1_REQ;
        uint8_t FAN_2_REQ;
        uint8_t FAN_3_REQ;
        uint8_t FAN_4_REQ;
    } CTRL_TO_FAN;
    t_can_node_fan_ctrl_bus0_input_alive alive;
} t_can_node_fan_ctrl_bus0_input;

typedef struct
{
    uint32_t fan_status;
    uint32_t fan_power;
} t_can_node_fan_ctrl_bus0_output_tx_now;

typedef struct
{
    struct
    {
        uint8_t FAN_1_ACT;
        uint8_t FAN_2_ACT;
        uint8_t FAN_3_ACT;
        uint8_t FAN_4_ACT;
    } FAN_STATUS;
    struct
    {
        int16_t CPU_TEMP;
        float V_3V3;
        float V_12V;
    } FAN_POWER;
    t_can_node_fan_ctrl_bus0_output_tx_now tx_now;
} t_can_node_fan_ctrl_bus0_output;

#define MBN_TX_FAN_STATUS 0
#define MBN_TX_FAN_POWER 1
#define MBN_RX_CTRL_TO_FAN 2

#define MBN_FIRST_BUS_0 0
#define MBN_TOTAL_BUS_0 3
#define MBN_NEXT_FREE_BUS_0 3

#ifndef CAN_EID_FLG
#define CAN_EID_FLG 0x80000000UL
#endif

#ifndef __MAILBOX_CALLBACK_TYPE__
#define __MAILBOX_CALLBACK_TYPE__
typedef void (*t_mailbox_callback)(uint32_t id, uint64_t msg, uint32_t dlc);
#endif

#define MSG_CYCLE_FAN_STATUS 100000
#define MSG_CYCLE_FAN_POWER 1000000
#define MSG_CYCLE_CTRL_TO_FAN 100000

//====== API functions ======
void can_node_fan_ctrl_bus0_tx(volatile t_can_node_fan_ctrl_bus0_output *out);
void can_node_fan_ctrl_bus0_rx(volatile t_can_node_fan_ctrl_bus0_input *inp);
void can_node_fan_ctrl_bus0_update_timers(uint32_t time_delta_us);
uint8_t can_node_fan_ctrl_bus0_init(uint32_t mb_shift, uint32_t rx_id_shift, uint32_t tx_id_shift, volatile t_can_node_fan_ctrl_bus0_output *out, volatile t_can_node_fan_ctrl_bus0_input *in);

//====== Planform driver functions declaration ======
void platform_can_init_rx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc);
void platform_can_init_tx_mb(uint32_t bus_id, uint32_t mbn, uint32_t id, uint32_t dlc);
void platform_can_xmit_mb(uint32_t bus_id, uint32_t mbn, uint64_t msg);
uint64_t platform_can_get_mb_data(uint32_t bus_id, uint32_t mbn);
uint32_t platform_can_is_message_arrived(uint32_t bus_id, uint32_t mbn);

//====== Callback functions ======
void platform_can_fan_status_cb(uint32_t id, uint64_t msg, uint32_t dlc);
void platform_can_fan_power_cb(uint32_t id, uint64_t msg, uint32_t dlc);

void platform_can_fan_status_checksum_cb(uint32_t id, uint64_t msg, uint32_t dlc);
void platform_can_fan_power_checksum_cb(uint32_t id, uint64_t msg, uint32_t dlc);

void platform_can_ctrl_to_fan_cb(uint32_t id, uint64_t msg, uint32_t dlc);


#endif

