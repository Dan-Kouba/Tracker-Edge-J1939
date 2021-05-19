// ------------------------------------------------------------------------
// J1939 CAN Connection
// ------------------------------------------------------------------------
#include "ARD1939.h"
#include "Particle.h"

#include <mcp_can.h>
#include <SPI.h>

#ifndef MCP2515_NORMAL_WRITES
#error "MCP2515_NORMAL_WRITES must be defined for the tracker platform!"
#endif // MCP2515_NORMAL_WRITES

#define CAN_SPI_INTERFACE         (SPI1)
#define APP_CONFIG_UNUSED         (-1)
#define CAN_CS_PIN                (CAN_CS)
#define CAN_INT_PIN               (CAN_INT)
#define CAN_RST_PIN               (CAN_RST)
#define BOOST_EN_PIN              (CAN_PWR)
#define CAN_SPEED                 (CAN_500KBPS)
#define CAN_CLOCK                 (MCP_20MHZ)
#define CAN_STBY_PIN              (CAN_STBY)

const char SLEEP_CMD[] = "SLEEP";

typedef struct {
    uint32_t id;
    uint8_t len;
    uint8_t  data[8];
} app_msg_log_t;

uint16_t pin_int;
app_msg_log_t msg;
bool is_sleep = false;

MCP_CAN CAN(CAN_CS_PIN, CAN_SPI_INTERFACE); // Set CS pin

// ------------------------------------------------------------------------
// CAN message ring buffer setup
// ------------------------------------------------------------------------
#define CANMSGBUFFERSIZE 10
struct CANMsg
{
  long lID;
  unsigned char pData[8];
  int nDataLen;
};
CANMsg CANMsgBuffer[CANMSGBUFFERSIZE];
int nWritePointer;
int nReadPointer;

void can_boost_enable(bool enable)
{
    digitalWrite(BOOST_EN_PIN, enable ? HIGH : LOW);
}

// ------------------------------------------------------------------------
// Initialize the CAN controller
// ------------------------------------------------------------------------
byte canInit(void)
{
    pinMode(BOOST_EN_PIN, OUTPUT);
    can_boost_enable(true);

    // CAN peripherial config
    pinMode(CAN_RST_PIN, OUTPUT);
    if(CAN_STBY_PIN != APP_CONFIG_UNUSED)
    {
        pinMode(CAN_STBY_PIN, OUTPUT);
        digitalWrite(CAN_STBY_PIN, LOW);
    }

    digitalWrite(CAN_RST_PIN, LOW);
    delay(50);
    digitalWrite(CAN_RST_PIN, HIGH);

    if (CAN.begin(MCP_RX_ANY, CAN_SPEED, CAN_CLOCK) != CAN_OK)
    {
        Log.error("CAN initial failed");
        return -1;
    }

    // Set NORMAL mode
    if(CAN.setMode(MCP_MODE_NORMAL) == MCP2515_OK) {
        Log.info("CAN mode set");
    }
    else {
      Log.error("CAN mode fail");
      return -1;
    }


    CAN.getCANStatus();
    pinMode(CAN_INT_PIN, INPUT);
    pin_int = CAN_INT_PIN;

    return 0;

}// end canInitialize

// ------------------------------------------------------------------------
// Check CAN controller for error
// ------------------------------------------------------------------------
byte canCheckError(void)
{
  if(CAN.checkError() == 0)
    return 0;
  else return 1;
  
}// end canCheckError

// ------------------------------------------------------------------------
// Transmit CAN message
// ------------------------------------------------------------------------
byte canTransmit(long lID, unsigned char* pData, int nDataLen)
{
  
  if(CAN.sendMsgBuf(lID, CAN_EXTID, nDataLen, pData) == 0)
    return 0;
  else
    return 1;
  
}// end canTransmit

// ------------------------------------------------------------------------
// Receive CAN message
// ------------------------------------------------------------------------
byte canReceive(long* lID, unsigned char* pData, int* nDataLen)
{
  // Declarations
  byte nCnt;

  // In case there is a message, put it into the buffer
  while(CAN.checkReceive() == CAN_MSGAVAIL)
  {
    // Read the message buffer
    CAN.readMsgBuf(&nCnt, &CANMsgBuffer[nWritePointer].pData[0]);
    CANMsgBuffer[nWritePointer].nDataLen = (int)nCnt;
    CANMsgBuffer[nWritePointer].lID = CAN.getCanId();    
    
    if(++nWritePointer == CANMSGBUFFERSIZE)
      nWritePointer = 0;
    
  }

  // Check ring buffer for a message
  if(nReadPointer != nWritePointer)
  {
    // Read the next message buffer entry
    *nDataLen = CANMsgBuffer[nReadPointer].nDataLen;
    *lID = CANMsgBuffer[nReadPointer].lID;

    for(int nIdx = 0; nIdx < *nDataLen; nIdx++)
      pData[nIdx] = CANMsgBuffer[nReadPointer].pData[nIdx];

    if(++nReadPointer == CANMSGBUFFERSIZE)
      nReadPointer = 0;
    
    return 0;
  }
  else return 1;

}// end canReceive

