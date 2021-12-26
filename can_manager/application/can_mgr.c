
#include "can_mgr.h"

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include "app_threadx.h"


#define N_CAN_INSTANCES                 3
#define N_OBSERVERS_PER_CAN_INSTANCE    5
#define CAN_PACKET_N_BYTES              8
#define TX_QUEUE_N_ITEMS                8
#define RX_QUEUE_N_ITEMS                8


typedef struct
{
    uint32_t  address;
    uint32_t  mask;
    TX_QUEUE *pQueue;
} obs_item_t;


typedef struct
{
    TX_QUEUE*  pqTx;       // Queue for incoming messages from app to transmitter
    TX_QUEUE*  pqRx;       // Queue for incoming messages from receiver ISR to app
    uint8_t    qTxArray[TX_QUEUE_N_ITEMS * CAN_PACKET_N_BYTES];
    uint8_t    qRxArray[RX_QUEUE_N_ITEMS * CAN_PACKET_N_BYTES];
    obs_item_t observers[N_OBSERVERS_PER_CAN_INSTANCE];
} cmgr_cb_t;


typedef struct
{
    uint32_t id;
    uint8_t  payload[CAN_PACKET_N_BYTES];
} can_pkt_t;



static cmgr_cb_t cmgr_cb[N_CAN_INSTANCES] = {0};

// TODO TX flags for each instance





void cmgr_rx_insert(uint8_t* pData, uint32_t nBytes, uint32_t canInstIdx)
{

}


bool cmgr_rx_subscribe(uint32_t address, uint32_t mask, TX_QUEUE* pQueue, uint32_t canInstIdx)
{
    obs_item_t itemToInsert = {.address = address, .mask = mask, .pQueue = pQueue};

    if(pQueue && canInstIdx < N_CAN_INSTANCES)
    {
        for(int i = 0; i < N_OBSERVERS_PER_CAN_INSTANCE; i++)
        {
            if(!cmgr_cb[canInstIdx].observers[i].pQueue)    // Slot empty when pointer is NULL
            {
                cmgr_cb[canInstIdx].observers[i] = itemToInsert;
                return true;
            }
        }
    }

    return false;
}


bool cmgr_rx_unsubscribe(uint32_t address, uint32_t mask, TX_QUEUE* pQueue, uint32_t canInstIdx)
{
    obs_item_t itemToRemove = {.address = address, .mask = mask, .pQueue = pQueue};

    if(pQueue && canInstIdx < N_CAN_INSTANCES)
    {
        for(int i = 0; i < N_OBSERVERS_PER_CAN_INSTANCE; i++)
        {
            if(cmgr_cb[canInstIdx].observers[i].address == itemToRemove.address
            && cmgr_cb[canInstIdx].observers[i].mask    == itemToRemove.mask
            && cmgr_cb[canInstIdx].observers[i].pQueue  == itemToRemove.pQueue)
            {
                cmgr_cb[canInstIdx].observers[i].pQueue = NULL;
                return true;
            }
        }
    }

    return false;
}


void cmgr_rx_thread()
{

    while(1)
    {
        // TODO if any of the RX queues (1 per CAN instance) has any item, process data


    }
}


// Restriction considered: packets are always the same size
bool cmgr_tx_packet(uint8_t* pData, uint32_t address, uint32_t canInstIdx, ULONG wait)
{
    can_pkt_t packet;
    UINT status;

    if(pData && canInstIdx < N_CAN_INSTANCES)
    {
        packet.id = address;
        for(int i = 0; i < CAN_PACKET_N_BYTES; i++)
            packet.payload[i] = pData[i];

        status = tx_queue_send(cmgr_cb[canInstIdx].pqTx, &packet, wait);
        return (status == TX_SUCCESS);
    }

    return false;
}


void cmgr_tx_thread()
{

    while(1)
    {
        // TODO if any of the TX queues (1 per CAN instance) has any item, process data

        
    }
}


void cmgr_set_listener(/*TODO callback, */ uint32_t canInstIdx)
{

}


void cmgr_init()
{


}
