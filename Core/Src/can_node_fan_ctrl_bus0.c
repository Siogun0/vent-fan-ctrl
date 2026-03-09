/* Do not edit */
/* This file is generated automatically by dbc_conv */

#include "can_node_fan_ctrl_bus0.h"
#include <string.h>
#include <ctype.h>
#include <stdint.h>
//#include "utils.h"

static uint32_t __rx_id_shift__ = 0;
static uint32_t __tx_id_shift__ = 0;
static uint32_t __mb_shift__ = 0;
static uint32_t multiplexor __attribute__ ((unused));

static volatile uint32_t msg_cntr_ctrl_to_fan;
static volatile uint32_t msg_cntr_fan_status;

//====== Auxiliary functions ======
static void can_node_memset(uint8_t *dst, uint32_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++)
    {
        dst[i] = 0;
    }
}

static inline uint32_t ltb32(uint32_t val)
{
    return (val & 0xFF000000) >> 24 | (val & 0x00FF0000) >> 8 | (val & 0x0000FF00) << 8 | (val & 0x000000FF) << 24;
}
static inline uint64_t ltb64(uint64_t val)
{
    return ((uint64_t)ltb32(val & 0xFFFFFFFFUL) << 32) | (uint64_t)ltb32(val >> 32);
}

static inline uint64_t ClearBits(uint64_t src, int offset, int size)
{
    return src & (~(((1ULL << size) - 1ULL) << offset));
}
static inline uint64_t SetBits(int offset, int size, uint64_t value)
{
    return (value & ((1ULL << size) - 1ULL)) << offset;
}
static inline uint64_t SetBitsSigned(int offset, int size, int64_t value)
{
    uint64_t v = value >= 0 ? (uint64_t)value : 1ULL + (((uint64_t)(-value)) ^ (UINT64_MAX >> (64 - size)));
    return SetBits(offset, size, v);
}
static inline uint64_t GetBits(uint64_t src, int offset, int size)
{
    return (uint64_t)((src >> offset) & ((1ULL << size) - 1ULL));
}
static inline int64_t GetBitsSigned(uint64_t src, int offset, int size)
{
    uint64_t v = GetBits(src, offset, size);
    return v & (1ULL << (size - 1)) ? -(int64_t)(1ULL + (v ^ (UINT64_MAX >> (64 - size)))) : (int64_t)v;
}

//====== Init function ======

uint8_t can_node_fan_ctrl_bus0_init(uint32_t mb_shift, uint32_t rx_id_shift, uint32_t tx_id_shift, volatile t_can_node_fan_ctrl_bus0_output *out, volatile t_can_node_fan_ctrl_bus0_input *in)
{
	__mb_shift__    = mb_shift;
    __rx_id_shift__ = rx_id_shift;
    __tx_id_shift__ = tx_id_shift;

    can_node_memset((uint8_t *)out, sizeof(t_can_node_fan_ctrl_bus0_output));
    can_node_memset((uint8_t *)in, sizeof(t_can_node_fan_ctrl_bus0_input));
    
    platform_can_init_tx_mb(0, MBN_TX_FAN_STATUS + __mb_shift__, 0x100 + __tx_id_shift__, 8);
    
    platform_can_init_rx_mb(0, MBN_RX_CTRL_TO_FAN + __mb_shift__, 0x40 + __rx_id_shift__, 8);
    
    msg_cntr_ctrl_to_fan = 2 * ALIVE_THRESHOLD;
    
    msg_cntr_fan_status = 0;
    return(MBN_NEXT_FREE_BUS_0 );    
}

//====== Receive functions ======

void can_node_fan_ctrl_bus0_rx(volatile t_can_node_fan_ctrl_bus0_input *inp)
{
    uint64_t msg;
    
    //CTRL_TO_FAN
    if(platform_can_is_message_arrived(0, MBN_RX_CTRL_TO_FAN + __mb_shift__))
    {
        msg_cntr_ctrl_to_fan = 0;
        
        msg = platform_can_get_mb_data(0, MBN_RX_CTRL_TO_FAN + __mb_shift__);
        
        inp->CTRL_TO_FAN.FAN_1_REQ = ((uint8_t)GetBits(msg, 0, 8));
        inp->CTRL_TO_FAN.FAN_2_REQ = ((uint8_t)GetBits(msg, 8, 8));
        inp->CTRL_TO_FAN.FAN_3_REQ = ((uint8_t)GetBits(msg, 16, 8));
        inp->CTRL_TO_FAN.FAN_4_REQ = ((uint8_t)GetBits(msg, 24, 8));
        
        platform_can_ctrl_to_fan_cb(0x40 + __rx_id_shift__, msg, 8);
    }
    inp->alive.ctrl_to_fan = msg_cntr_ctrl_to_fan >= ALIVE_THRESHOLD * MSG_CYCLE_CTRL_TO_FAN ? 0 : 1;
    
}

//====== Transmit functions ======

void can_node_fan_ctrl_bus0_tx(volatile t_can_node_fan_ctrl_bus0_output *out)
{
    uint64_t msg;
    
    //FAN_STATUS
//    msg_cntr_fan_status = out->tx_now.fan_status ? msg_cntr_fan_status : 0;
    if((msg_cntr_fan_status >= MSG_CYCLE_FAN_STATUS) || (out->tx_now.fan_status == 1))
    {
        msg_cntr_fan_status = 0;
        out->tx_now.fan_status = 0;
        
        msg = 0;
        
        platform_can_fan_status_cb(0x100 + __tx_id_shift__, msg, 8);
        
        //Sinals
        msg |= SetBits(0, 8, (out->FAN_STATUS.FAN_1_ACT));
        msg |= SetBits(8, 8, (out->FAN_STATUS.FAN_2_ACT));
        msg |= SetBits(16, 8, (out->FAN_STATUS.FAN_3_ACT));
        msg |= SetBits(24, 8, (out->FAN_STATUS.FAN_4_ACT));
        
        platform_can_xmit_mb(0, MBN_TX_FAN_STATUS + __mb_shift__, msg);
    }
}

//====== Timers functions ======

void can_node_fan_ctrl_bus0_update_timers(uint32_t time_delta_us)
{
    msg_cntr_ctrl_to_fan += msg_cntr_ctrl_to_fan >= ALIVE_THRESHOLD * MSG_CYCLE_CTRL_TO_FAN ? 0 : time_delta_us;
    msg_cntr_fan_status += time_delta_us;
}

//====== Weak Callback functions ======
__attribute__((weak)) void platform_can_fan_status_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}

__attribute__((weak)) void platform_can_fan_status_checksum_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}

__attribute__((weak)) void platform_can_ctrl_to_fan_cb(uint32_t id, uint64_t msg, uint32_t dlc) {}


