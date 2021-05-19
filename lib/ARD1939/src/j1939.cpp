#include <stdlib.h>
#include <inttypes.h>
#include <SPI.h>

#include "ARD1939.h"

#define d49         10

#define d50         8

#define d31         0x00EA00
#define d32         0x00EAFF
#define d46         0x00
#define d47         0xEA
#define d48         0x00

#define d29         0x00EE00
#define d30         0x00EEFF
#define d25         0x00
#define d26         0xEE
#define d27         0x00
#define d28         6

#define d33         0x00EE00
#define d34         0x00FED8

#define d35         0x00ECFF
#define d36         0x00EC00
#define d37         0xEC
#define d38         7

#define d39         0x00EB00
#define d40         7

#define d41         32
#define d42         16
#define d43         17
#define d44         19
#define d45         255

#define d51         255

struct v01
{
  bool bActive;
  long lPGN;
};
v01 v02[MSGFILTERS];

unsigned char v03[] =
{
  (byte)(NAME_IDENTITY_NUMBER & 0xFF),
  (byte)((NAME_IDENTITY_NUMBER >> 8) & 0xFF),
  (byte)(((NAME_MANUFACTURER_CODE << 5) & 0xFF) | (NAME_IDENTITY_NUMBER >> 16)),
  (byte)(NAME_MANUFACTURER_CODE >> 3),
  (byte)((NAME_FUNCTION_INSTANCE << 3) | NAME_ECU_INSTANCE),
  (byte)(NAME_FUNCTION),
  (byte)(NAME_VEHICLE_SYSTEM << 1),
  (byte)((NAME_ARBITRARY_ADDRESS_CAPABLE << 7) | (NAME_INDUSTRY_GROUP << 4) | (NAME_VEHICLE_SYSTEM_INSTANCE))
};

#define v04                     4
long v05[] =
{
  d29,
  d34,
  d36,
  d39
};

struct v06
{
  bool v07;
  byte MAYBE__STATUS_MSG_PENDING;
  bool v09;
  bool v10;
  bool v11;
  byte v12;
  byte v13;
  byte v14;
  bool v15;
  byte v16;
  byte nSourceAddress;
};
struct v06 v18;


#if TRANSPORT_PROTOCOL == 1
  #define TransportProtocol_d01          0
  #define TransportProtocol_d02          1
  #define TransportProtocol_d03          2
  
  struct TransportProtocol_struct
  {
    byte v20;
    bool v21;
    bool v22;
    bool v23;
    bool v24;
    long lPGN;
    byte v26;
    byte v27;
    byte v28;
    byte v29;
    byte TP_msg_buf[J1939_MSGLEN];
    int nMsgLen;
    byte v32;
  };
  TransportProtocol_struct TP_Struct_1;
  TransportProtocol_struct TP_Struct_2;
  
  #define CONST_32           32
  #define CONST_16           16
  #define CONST_17           17
  #define CONST_19           19
  #define CONST_255          255
  
  #define CONST_1           1
  #define d10               2
  #define d11               3
  
  #define d12               0
  #define d13               1

  byte v62[] = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00};
  byte v63[8];
#endif

struct v35 v38;
struct v35 v39;

#if TRANSPORT_PROTOCOL == 1
  struct v35 v40;
  struct v35 v41;
  
  struct v35 v42;
  struct v35 v43;
  struct v35 v44;
  struct v35 v45;
  struct v35 v46;
  struct v35 v47;
  struct v35 v48;
#endif

int v49;
int v50;

#if TRANSPORT_PROTOCOL == 1
  int v51;
  int v52;
  
  int v53;
  int v54;
  int v55;
  int v56;
  int v57;
  int v58;
  int v59;
#endif

#define d14      	250
#define d15       	100

#if TRANSPORT_PROTOCOL == 1
  #define d16	    50
  #define d17	    750
  
  #define d18       50      
  #define d19		200
  #define d20		500
  #define d21	    750
  #define d22		1250
  #define d23		1250
  #define d24	    1050
#endif

int v60[] = {8, 9, 10, 12, 15};
byte v61;

byte v64;
extern byte canInit(void);
extern byte canCheckError(void);
extern byte canTransmit(long, unsigned char*, int);
extern byte canReceive(long*, unsigned char*, int*);

byte ARD1939::Init(int nSystemTime)
{
  int index;
  f06(&v38);
  f06(&v39);
  v49 = d14 / nSystemTime;
  v50 = d15 / nSystemTime;
  v61 = 0;
  v60[0] = (int)8 / nSystemTime;
  v60[1] = (int)9 / nSystemTime;
  v60[2] = (int)10 / nSystemTime;
  v60[3] = (int)12 / nSystemTime;
  v60[4] = (int)15 / nSystemTime;
  v18.v07 = false;
  v18.MAYBE__STATUS_MSG_PENDING = false;
  v18.v09 = false;
  v18.v10 = false;
  v18.v11 = false;
  v18.v12 = SA_PREFERRED;
  v18.v13 = NULLADDRESS;
  v18.v14 = NULLADDRESS;
  v18.v15 = false;
  v18.v16 = NULLADDRESS;
  v18.nSourceAddress = NULLADDRESS;
  for(index = 0; index < MSGFILTERS; index++)
  {
    v02[index].bActive = false;
    v02[index].lPGN = 0;    
  }
  
#if TRANSPORT_PROTOCOL == 1
  v51 = d16 / nSystemTime;
  v52 = d17 / nSystemTime;

  v53 = d18 / nSystemTime;
  v56 = d21 / nSystemTime;
  v54 = d19 / nSystemTime;
  v55 = d20 / nSystemTime;
  v56 = d21 / nSystemTime;
  v57 = d22 / nSystemTime;
  v58 = d23 / nSystemTime;
  v59 = d24 / nSystemTime;
  f11(d12);
  f12(d12);
#endif
    
  v64 = 0;
  return canInit();
}

void ARD1939::Terminate(void)
{
  Init(SYSTEM_TIME);
}

byte ARD1939::Operate(byte* nMsgId, long* lPGN, byte* pMsg, int* nMsgLen, byte* nDestAddr, byte* nSrcAddr, byte* nPriority)
{
  byte v68;
  byte v82;
  int index;
  f05();
  *nMsgId = f04(lPGN, &pMsg[0], nMsgLen, nDestAddr, nSrcAddr, nPriority);
  if(*nMsgId == J1939_MSG_APP)
  {
    if(*nDestAddr != v18.nSourceAddress
    && *nDestAddr != GLOBALADDRESS)
      *nMsgId = J1939_MSG_NETWORKDATA;
  }
  if(v18.MAYBE__STATUS_MSG_PENDING == true)
  {
    v68 = NORMALDATATRAFFIC;
    switch(*lPGN)
    {
      case d31:
        if(*nMsgId == J1939_MSG_PROTOCOL)
        {
          if(pMsg[0] == d27
          && pMsg[1] == d26
          && pMsg[2] == d25)
          {
            if(*nDestAddr == GLOBALADDRESS)
            {
              Transmit(d28, d29, v18.nSourceAddress, GLOBALADDRESS,
                            &v03[0], 8);
            }
            else if(*nDestAddr == v18.nSourceAddress)
            {
              Transmit(d28, d29, v18.nSourceAddress, *nSrcAddr,
                            &v03[0], 8);
            }
          }
        }
        break;
      
      case d29:
        if(*nSrcAddr == v18.nSourceAddress)
        {
          v82 = f03(&pMsg[0], &v03[0]);
          switch(v82)
          {
            case 0:      
#if TRANSPORT_PROTOCOL == 1          
              f11(d12);
              f12(d12);
#endif           
              Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS, &v03[0], 8);
              v18.MAYBE__STATUS_MSG_PENDING = false;
              v18.v11 = true;
              v68 = ADDRESSCLAIM_FAILED;
              break;             
            case 1:     
#if TRANSPORT_PROTOCOL == 1           
              f11(d12);
              f12(d12);
#endif           
              v18.v07 = false;
              v18.MAYBE__STATUS_MSG_PENDING = false;
              f01(*nSrcAddr, &v03[0]);
              break;          
            case 2:  
              Transmit(d28, d29, v18.nSourceAddress,
                            GLOBALADDRESS, &v03[0], 8);
              break;
          }
        }
        break;
      case d34:
        break;
    }
    
#if TRANSPORT_PROTOCOL == 1
    f13(*lPGN, pMsg, *nMsgLen, *nDestAddr, *nSrcAddr, *nPriority);
    if(*nMsgId == J1939_MSG_NONE)
    {
          if(TP_Struct_1.v32 == true)
          {
              *lPGN = TP_Struct_1.lPGN;
              *nMsgLen = TP_Struct_1.nMsgLen;
              *nDestAddr = TP_Struct_1.v27;
              *nSrcAddr = TP_Struct_1.v26;
              *nPriority = d51;
              for(index = 0; index < TP_Struct_1.nMsgLen; index++)
                  pMsg[index] = TP_Struct_1.TP_msg_buf[index];
              f11(d12);
                  *nMsgId = J1939_MSG_APP;
          }
          else if(TP_Struct_2.v32 == true)
          {
              *lPGN = TP_Struct_2.lPGN;
              *nMsgLen = TP_Struct_2.nMsgLen;
              *nDestAddr = TP_Struct_2.v27;
              *nSrcAddr = TP_Struct_2.v26;
              *nPriority = d51;
              for(index = 0; index < TP_Struct_2.nMsgLen; index++)
                  pMsg[index] = TP_Struct_2.TP_msg_buf[index];
              f12(d12);
                  *nMsgId = J1939_MSG_APP;
          }
    }
#endif
  }
  else if(v18.v11 == true)
  {
    v68 = ADDRESSCLAIM_FAILED;
    switch(*lPGN)
    {
      case d31:
        if(*nMsgId == J1939_MSG_PROTOCOL)
        {
          if(pMsg[0] == d27
          && pMsg[1] == d26
          && pMsg[2] == d25)
          {
            if(*nDestAddr == GLOBALADDRESS)
            {
              v39.v36 = v60[v61++];
              v39.v21 = true;
              if(v61 > 4)
                v61 = 0;
            }
          }
        }
        break;
      case d29:
        break;
      case d34:
        break;
    }
    if(v39.v37 == true)
    {
      f06(&v39);
       Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS,
                    &v03[0], 8);
    }
  }
  else
  {
    v68 = f01(*nSrcAddr, &pMsg[0]);
  }
  return v68;
}

byte ARD1939::f01(byte v16, byte* v91)
{
  byte v68;
  byte v82;
  v68 = ADDRESSCLAIM_INPROGRESS;
  if(v18.v11 == true)
    v68 = ADDRESSCLAIM_FAILED;
  else if(v18.MAYBE__STATUS_MSG_PENDING == true)
    v68 = ADDRESSCLAIM_FINISHED;
  else if(v18.v10 == true)
  {
    if(v39.v37 == true)
    {
        f06(&v39);
        Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                      &v03[0], 8);
        v38.v36 = v49;
        v38.v21 = true;
        v18.v10 = false;
    }
  }
  else
  {
    if(v18.v07 == false)
    {
      if(f02() == true)
      {
        Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                      &v03[0], 8);
        v38.v36 = v49;
        v38.v21 = true;
        v18.v07 = true;
      }
      else
      {
        Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS, &v03[0], 8);
        v18.MAYBE__STATUS_MSG_PENDING = false;
        v18.v11 = true;
        v68 = ADDRESSCLAIM_FAILED;
      }
    }
    else
    {
      if(v38.v37 == true)
      {
        f06(&v38);
        v18.nSourceAddress = v18.v16;
        v18.MAYBE__STATUS_MSG_PENDING = true;
        v68 = ADDRESSCLAIM_FINISHED;
      }
      else
      {
        if(canCheckError() == 1)
        {
          f06(&v38);
          if(++v64 == d49)
          {
            v18.MAYBE__STATUS_MSG_PENDING = false;
            v18.v11 = true;
            v68 = ADDRESSCLAIM_FAILED;            
          }
          else
          {
            canInit();
            v39.v36 = v60[v61++];
            v39.v21 = true;
            if(v61 > 4)
              v61 = 0;
            v18.v10 = true;
          }
        }
        else
          v64 = 0;
        if(v16 == v18.v16)
        {
          v82 = f03(&v91[0], &v03[0]);
          switch(v82)
          {
            case 0:
#if TRANSPORT_PROTOCOL == 1            
              f11(d12);
              f12(d12);
#endif           
              Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS, &v03[0], 8);
              v18.MAYBE__STATUS_MSG_PENDING = false;
              v18.v11 = true;
              v68 = ADDRESSCLAIM_FAILED;
              break;       
            
              case 1:
#if TRANSPORT_PROTOCOL == 1         
              f11(d12);
              f12(d12);
#endif           
              if(f02() == true)
              {
                Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                              &v03[0], 8);
                v38.v36 = v49;
                v38.v21 = true;
              }
              else
              {
                Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS,
                              &v03[0], 8);
                v18.MAYBE__STATUS_MSG_PENDING = false;
                v18.v11 = true;
                v68 = ADDRESSCLAIM_FAILED;
              }
              break;

            case 2:
              Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                            &v03[0], 8);
              v38.v36 = v49;
              v38.v21 = true;
              break;
          }
        }
      }
    }
  }
  return v68;
}

bool ARD1939::f02(void)
{
  bool v72;
  v72 = true;
  if(v18.v12 == NULLADDRESS)
    v18.v09 = true;
  if(v18.v09 == false)
  {
    v18.v16 = v18.v12;
    v18.v09 = true;
  }
  else
  {
    if(v18.v13 == NULLADDRESS || v18.v14 == NULLADDRESS)
    {
      v72 = false;
    }
    else
    {
      if(v18.v16 == NULLADDRESS || v18.v15 == false)
      {
        v18.v16 = v18.v13;
        v18.v15 = true;
      }
      else
      {
        if(v18.v16 < v18.v14)
          v18.v16++;
        else
          v72 = false;
      }
      if(v18.v16 == v18.v12)
      {
        if(v18.v16 < v18.v14)
          v18.v16++;
        else
          v72 = false;
      }
    }
  }
  return v72;
}

byte ARD1939::f03(byte* v92, byte* v93)
{
  byte index;
  for(index = 8; index > 0; index--)
  {
    if(v92[index-1] != v93[index-1])
    {      
      if(v92[index-1] < v93[index-1])
        return 1;
      else
        return 2;
    }
  }
  return 0;
}

byte ARD1939::f04(long* lPGN, byte* pMsg, int* nMsgLen, byte* nDestAddr, byte* nSrcAddr, byte* nPriority)
{
  long v78;
  long v74;
  *lPGN = 0;
  *nMsgLen = 0;
  *nDestAddr = NULLADDRESS;
  *nSrcAddr = NULLADDRESS;
  *nPriority = 0;
  if(canReceive(&v74, &pMsg[0], nMsgLen) == 0)
  {
    v78 = v74 & 0x1C000000;
    *nPriority = (byte)(v78 >> 26);
    *lPGN = v74 & 0x01FFFF00;
    *lPGN = *lPGN >> 8;
    *nSrcAddr = (byte)(v74 & 0x000000FF);
    *nDestAddr = GLOBALADDRESS;
    if(f08(*lPGN) == true)
    {
      *nDestAddr = (byte)(*lPGN & 0xFF);
      *lPGN = *lPGN & 0x01FF00;
    }
    if(f07(lPGN, &pMsg[0]) == true)
      return J1939_MSG_PROTOCOL;
    else
      return J1939_MSG_APP;
  }
  else
    return J1939_MSG_NONE;
}

byte ARD1939::Transmit(byte nPriority, long lPGN, byte nSourceAddress, byte nDestAddress, byte* pData, int nDataLen)
{
  long v74;
  if(nDataLen > J1939_MSGLEN)
    return ERR;
  v74 = ((long)nPriority << 26) + (lPGN << 8) + (long)nSourceAddress;
  if(f08(lPGN) == true)
    v74 = v74 | ((long)nDestAddress << 8);
  if(nDataLen > 8)
#if TRANSPORT_PROTOCOL == 1
    return f10(nPriority, lPGN, nSourceAddress, nDestAddress, pData, nDataLen);
#else
    return ERR;
#endif
  else
    return canTransmit(v74, pData, nDataLen);
}

void ARD1939::f05(void)
{
  if(v38.v21 == true && v38.v37 == false)
  {
    if(--v38.v36 == 0)
      v38.v37 = true;
  }
  if(v39.v21 == true && v39.v37 == false)
    if(--v39.v36 == 0)
      v39.v37 = true;
#if TRANSPORT_PROTOCOL == 1
  if(v40.v21 == true && v40.v37 == false)
    if(--v40.v36 == 0)
        v40.v37 = true;
  if(v41.v21 == true && v41.v37 == false)
    if(--v41.v36 == 0)
        v41.v37 = true;
  if(v42.v21 == true && v42.v37 == false)
    if(--v42.v36 == 0)
        v42.v37 = true;
  if(v43.v21 == true && v43.v37 == false)
    if(--v43.v36 == 0)
        v43.v37 = true;
  if(v44.v21 == true && v44.v37 == false)
    if(--v44.v36 == 0)
        v44.v37 = true;
  if(v45.v21 == true && v45.v37 == false)
    if(--v45.v36 == 0)
        v45.v37 = true;
  if(v46.v21 == true && v46.v37 == false)
    if(--v46.v36 == 0)
        v46.v37 = true;
  if(v47.v21 == true && v47.v37 == false)
    if(--v47.v36 == 0)
        v47.v37 = true;
  if(v48.v21 == true && v48.v37 == false)
    if(--v48.v36 == 0)
        v48.v37 = true;
#endif
}

void ARD1939::f06(struct v35* v75)
{
  v75->v36 = 0;
  v75->v21 = false;
  v75->v37 = false;
}

bool ARD1939::f07(long* lPGN, byte* v83)
{
  bool v69;
  byte index;
  byte v84;
  byte v85;
  v69 = false;
  v84 = (byte)((*lPGN & 0x00FF00) >> 8);
  v85 = (byte)(*lPGN & 0x0000FF);
  switch(v84)
  {
    case d47:
      if(v85 == GLOBALADDRESS)
      {
        if(v83[0] == d27
        && v83[1] == d26
        && v83[2] == d25)
          v69 = true;
      }
      else
      {
        *lPGN = *lPGN & 0x00FF00;
        if(v83[0] == d27
        && v83[1] == d26
        && v83[2] == d25)
          v69 = true;
      }
      break;
  
    default:
      for(index = 0; index < v04 - 1; index++)
      {
        if(*lPGN == v05[index])
        {
          v69 = true;
          break;
        }
      }
      break;
  }
  return v69;
}

bool ARD1939::f08(long lPGN)
{
  if(lPGN > 0 && lPGN <= 0xEFFF)
    return true;
  if(lPGN > 0x10000 && lPGN <= 0x1EFFF)
    return true;
  return false;
}

byte ARD1939::GetSourceAddress(void)
{
   return v18.nSourceAddress;
}

void ARD1939::SetPreferredAddress(byte nAddr)
{
  v18.v12 = nAddr;
}

void ARD1939::SetAddressRange(byte nAddrBottom, byte nAddrTop)
{
  v18.v13 = nAddrBottom;
  v18.v14 = nAddrTop;
}

void ARD1939::SetNAME(long lIdentityNumber, int nManufacturerCode, byte nFunctionInstance, byte nECUInstance,
                  byte nFunction, byte nVehicleSystem, byte nVehicleSystemInstance, byte nIndustryGroup, byte nArbitraryAddressCapable)
{
  v03[0] = (byte)(lIdentityNumber & 0xFF);
  v03[1] = (byte)((lIdentityNumber >> 8) & 0xFF);
  v03[2] = (byte)(((nManufacturerCode << 5) & 0xFF) | (lIdentityNumber >> 16));
  v03[3] = (byte)(nManufacturerCode >> 3);
  v03[4] = (byte)((nFunctionInstance << 3) | nECUInstance);
  v03[5] = (byte)(nFunction);
  v03[6] = (byte)(nVehicleSystem << 1);
  v03[7] = (byte)((nArbitraryAddressCapable << 7) | (nIndustryGroup << 4) | (nVehicleSystemInstance));
}

byte ARD1939::SetMessageFilter(long lPGN)
{
  byte v94;
  int index;
  v94 = ERR;
  if((lPGN & 0x00FF00) == d31)
      lPGN = d31;
  if(f09(lPGN) == true)
    v94 = OK;
  else
  {
    for(index = 0; index < MSGFILTERS; index++)
    {
      if(v02[index].bActive == false)
      {
        v02[index].bActive = true;
        v02[index].lPGN = lPGN;
        v94 = OK;
        break;
      }
    }
  }
  return v94;
}

void ARD1939::DeleteMessageFilter(long lPGN)
{
  int index;
  if((lPGN & 0x00FF00) == d31)
      lPGN = d31;
  for(index = 0; index < MSGFILTERS; index++)
  {
    if(v02[index].lPGN == lPGN)
    {
      v02[index].bActive = false;
      v02[index].lPGN = 0;
      break;
    }
  }
}

bool ARD1939::f09(long lPGN)
{
  bool v69;
  int index;
  v69 = false;
  if((lPGN & 0x00FF00) == d31)
      lPGN = d31;
  for(index = 0; index < MSGFILTERS; index++)
  {
    if(v02[index].bActive == true
    && v02[index].lPGN == lPGN)
    {
      v69 = true;
      break;
    }
  }
  return v69;
}

#if TRANSPORT_PROTOCOL == 1
byte ARD1939::f13(long lPGN, byte* pMsg, int nMsgLen, byte nDestAddr, byte nSrcAddr, byte nPriority)
{
  byte v94;
  int nPointer;
  int index;
  v94 = OK;
  if(TP_Struct_1.v20 == TransportProtocol_d03 && TP_Struct_1.v21 == true)
  {
      if(TP_Struct_1.v22 == false)
      {
        v62[0] = CONST_32;
        v62[1] = (byte)(TP_Struct_1.nMsgLen & 0xFF);
        v62[2] = (byte)(TP_Struct_1.nMsgLen >> 8);
        v62[3] = TP_Struct_1.v28;
        v62[4] = 0xFF;
        v62[5] = (byte)(TP_Struct_1.lPGN & 0x0000FF);
        v62[6] = (byte)((TP_Struct_1.lPGN & 0x00FF00) >> 8);
        v62[7] = (byte)(TP_Struct_1.lPGN >> 16);
        v94 = Transmit(d38, d36, TP_Struct_1.v26, GLOBALADDRESS, &v62[0], 8);
        v40.v36 = v51;
        v40.v21 = true;        
        TP_Struct_1.v22 = true;
      }
      else
      {
        if(v40.v37 == true )
        {
          nPointer = TP_Struct_1.v29 * 7;
          v63[0] = ++TP_Struct_1.v29;
          for(index = 0; index < 7; index++)
            v63[index+1] = TP_Struct_1.TP_msg_buf[nPointer + index];
          v94 = Transmit(d40, d39, TP_Struct_1.v26, GLOBALADDRESS, &v63[0], 8);
          if(TP_Struct_1.v29 == TP_Struct_1.v28)
          {
            f11(d12);
          }
          else
          {
            v40.v36 = v51;
            v40.v21 = true;      
            v40.v37 = false;  
          }
        }
      }
  }
  if(lPGN == d36 && pMsg[0] == CONST_32 && TP_Struct_1.v20 == TransportProtocol_d01
  && TP_Struct_1.v32 == false)
  {    
      TP_Struct_1.lPGN = (((long)pMsg[7]) << 16) + (((long)pMsg[6]) << 8) + (long)pMsg[5];
      if(f09(TP_Struct_1.lPGN) == true)
      {
        TP_Struct_1.nMsgLen = (int)pMsg[1] + ((int)(pMsg[2]) << 8);
        if(TP_Struct_1.nMsgLen > J1939_MSGLEN)
        {
          f11(d12);
        }
        else
        {
          TP_Struct_1.v20 = TransportProtocol_d02;
          TP_Struct_1.v21 = true;
          TP_Struct_1.v26 = nSrcAddr;
          TP_Struct_1.v27 = nDestAddr;
          TP_Struct_1.v28 = pMsg[3];
          TP_Struct_1.v29 = 0;
          v41.v36 = v52;
          v41.v21 = true;
        }
       }
      else 
        TP_Struct_1.lPGN = 0;
  }
  if(TP_Struct_1.v20 == TransportProtocol_d02 && v41.v37 == true)
  {
    f11(d12);
  }

  if(TP_Struct_1.v20 == TransportProtocol_d02 && TP_Struct_1.v21 == true
  && lPGN == d39 && nSrcAddr == TP_Struct_1.v26 && nDestAddr == TP_Struct_1.v27)
  {
    nPointer = ((int)pMsg[0] - 1) * 7;
    for(index = 1; index < 8; index++)
      TP_Struct_1.TP_msg_buf[nPointer++] = pMsg[index];
    if(++TP_Struct_1.v29 == TP_Struct_1.v28)
    {
    f11(d13);
    TP_Struct_1.v32 = true;
    }
  }
  if(TP_Struct_2.v20 == TransportProtocol_d03 && TP_Struct_2.v21 == true
  && lPGN == d36 && pMsg[0] == d45)
  {
    f12(d12);
  }
  
  if(TP_Struct_2.v20 == TransportProtocol_d03 && TP_Struct_2.v21 == true
  && lPGN == d36 && pMsg[0] == CONST_19)
  {
    f12(d12);
  }
  if(TP_Struct_2.v20 == TransportProtocol_d03 && TP_Struct_2.v21 == true)
  {
      if(TP_Struct_2.v23 == false)
      {
        v62[0] = CONST_16;
        v62[1] = (byte)(TP_Struct_2.nMsgLen & 0xFF);
        v62[2] = (byte)(TP_Struct_2.nMsgLen >> 8);
        v62[3] = TP_Struct_2.v28;
        v62[4] = 0xFF;
        v62[5] = (byte)(TP_Struct_2.lPGN & 0x0000FF);
        v62[6] = (byte)((TP_Struct_2.lPGN & 0x00FF00) >> 8);
        v62[7] = (byte)(TP_Struct_2.lPGN >> 16);
        v94 = Transmit(d38, d36, TP_Struct_2.v26, TP_Struct_2.v27, &v62[0], 8);
        v43.v36 = v54;
        v43.v21 = true;        
        TP_Struct_2.v23 = true;
      }
      else
      {
        if(v43.v37 == true)
        {
          v62[0] = CONST_255;
          v62[1] = d11;
          v62[2] = 0xFF;
          v62[3] = 0xFF;
          v62[4] = 0xFF;
          v62[5] = (byte)(TP_Struct_2.lPGN & 0x0000FF);
          v62[6] = (byte)((TP_Struct_2.lPGN & 0x00FF00) >> 8);
          v62[7] = (byte)(TP_Struct_2.lPGN >> 16);
          v94 = Transmit(d38, d36, TP_Struct_2.v26, TP_Struct_2.v27, &v62[0], 8);
          f12(d12);
        }
        if(lPGN == d36 && nDestAddr == TP_Struct_2.v26 && pMsg[0] == CONST_17)
        {
          f06(&v43);
          v42.v36 = v53;
          v42.v21 = true;        
          TP_Struct_2.v24 = true;
        }
        if(TP_Struct_2.v24 == true && v42.v37 == true)
        {
          nPointer = TP_Struct_2.v29 * 7;
          v63[0] = ++TP_Struct_2.v29;
          for(index = 0; index < 7; index++)
            v63[index+1] = TP_Struct_2.TP_msg_buf[nPointer + index];
          v94 = Transmit(d40, d39, TP_Struct_2.v26, TP_Struct_2.v27, &v63[0], 8);
          if(TP_Struct_2.v29 == TP_Struct_2.v28)
          {
            f06(&v42);
            v47.v36 = v58;
            v47.v21 = true;
          }
          else
          {
            v42.v36 = v51;
            v42.v21 = true;      
            v42.v37 = false;  
          }
        }
        if(v47.v37 == true)
        {
          v62[0] = CONST_255;
          v62[1] = d11;
          v62[2] = 0xFF;
          v62[3] = 0xFF;
          v62[4] = 0xFF;
          v62[5] = (byte)(TP_Struct_2.lPGN & 0x0000FF);
          v62[6] = (byte)((TP_Struct_2.lPGN & 0x00FF00) >> 8);
          v62[7] = (byte)(TP_Struct_2.lPGN >> 16);
          v94 = Transmit(d38, d36, TP_Struct_2.v26, TP_Struct_2.v27, &v62[0], 8);
          f12(d12);
        }
      }
  }
  if(lPGN == d36 && nDestAddr == v18.nSourceAddress && pMsg[0] == CONST_16)
  {
    int v77;
    v77 = (int)pMsg[1] + ((int)(pMsg[2]) << 8);
    if(TP_Struct_2.v20 != TransportProtocol_d01 || v77 > J1939_MSGLEN  || TP_Struct_2.v32 == true)
    {
        v62[0] = CONST_255;
        if(TP_Struct_2.nMsgLen > J1939_MSGLEN)
          v62[1] = d10;
        else
          v62[1] = CONST_1;
        v62[2] = 0xFF;
        v62[3] = 0xFF;
        v62[4] = 0xFF;
        v62[5] = pMsg[5];
        v62[6] = pMsg[6];
        v62[7] = pMsg[7];
        v94 = Transmit(d38, d36, nDestAddr, nSrcAddr, &v62[0], 8);
    }
    else
    {
      TP_Struct_2.lPGN = (((long)pMsg[7]) << 16) + (((long)pMsg[6]) << 8) + (long)pMsg[5];
      if(f09(TP_Struct_2.lPGN) == true)
      {
        TP_Struct_2.v20 = TransportProtocol_d02;
        TP_Struct_2.v21 = true;
        TP_Struct_2.v24 = true;
        TP_Struct_2.v26 = nSrcAddr;
        TP_Struct_2.v27 = nDestAddr;
        TP_Struct_2.v28 = pMsg[3];
        TP_Struct_2.v29 = 0;
        TP_Struct_2.nMsgLen = (int)pMsg[1] + ((int)(pMsg[2]) << 8);
        for(index = 0; index < 8; index++)
          v62[index] = pMsg[index];
        v62[0] = CONST_17;
        v62[1] = v62[3];    
        v62[2] = 1;              
        v62[3] = 0xFF;          
        v94 = Transmit(d38, d36, nDestAddr, nSrcAddr, &v62[0], 8);
        v45.v36 = v56;
        v45.v21 = true;
      }
      else
      {       
        v62[0] = CONST_255;
        v62[1] = d10;
        v62[2] = 0xFF;
        v62[3] = 0xFF;
        v62[4] = 0xFF;
        v62[5] = pMsg[5];
        v62[6] = pMsg[6];
        v62[7] = pMsg[7];
        v94 = Transmit(d38, d36, nDestAddr, nSrcAddr, &v62[0], 8);
      }
    }
  }
  if(TP_Struct_2.v20 == TransportProtocol_d02 && TP_Struct_2.v21 == true)
  {
      if(v45.v37 == true)
      {
        f12(d12);
        v62[0] = CONST_255;
        v62[1] = d11;
        v62[2] = 0xFF;
        v62[3] = 0xFF;
        v62[4] = 0xFF;
        v62[5] = (byte)(TP_Struct_2.lPGN & 0x0000FF);
        v62[6] = (byte)((TP_Struct_2.lPGN & 0x00FF00) >> 8);
        v62[7] = (byte)(TP_Struct_2.lPGN >> 16);
        v94 = Transmit(d38, d36, TP_Struct_2.v27, TP_Struct_2.v26, &v62[0], 8);
      }
      if(lPGN == d39 && nDestAddr == TP_Struct_2.v27 && nSrcAddr == TP_Struct_2.v26)
      {
        nPointer = ((int)pMsg[0] - 1) * 7;
        for(index = 1; index < 8; index++)
          TP_Struct_2.TP_msg_buf[nPointer++] = pMsg[index];
        if(++TP_Struct_2.v29 == TP_Struct_2.v28)
        {
          v62[0] = CONST_19;
          v62[1] = (byte)(TP_Struct_2.nMsgLen & 0x00FF);         
          v62[2] = (byte)((TP_Struct_2.nMsgLen & 0x00FF) >> 8);   
          v62[3] = TP_Struct_2.v28;
          v62[4] = 0xFF;
          v62[5] = (byte)(TP_Struct_2.lPGN & 0x0000FF);
          v62[6] = (byte)((TP_Struct_2.lPGN & 0x00FF00) >> 8);
          v62[7] = (byte)(TP_Struct_2.lPGN >> 16);
          v94 = Transmit(d38, d36, TP_Struct_2.v27, TP_Struct_2.v26, &v62[0], 8);
          f12(d13);
          TP_Struct_2.v32 = true;
        }
      }
  }
  return v94;
}

byte ARD1939::f10(byte nPriority, long lPGN, byte nSourceAddress, byte nDestAddress, byte* pData, int v77)
{
  byte v68;
  int index;    
  struct TransportProtocol_struct* v89;
  v68 = OK;
  if(nDestAddress != GLOBALADDRESS)
    v89 = &TP_Struct_2;
  else 
    v89 = &TP_Struct_1;
  if(v89->v20 != TransportProtocol_d01 || v77 > J1939_MSGLEN)
    v68 = ERR;
  else
  {
    for(index = 0; index < v77; index++)
      v89->TP_msg_buf[index] = pData[index];
    for(index = v77; index < (v77 + 7); index++)
    {
      if(index >= J1939_MSGLEN) break;
      v89->TP_msg_buf[index] = 0xFF;
    }
    v89->lPGN = lPGN;
    v89->nMsgLen = v77;
    v89->v26 = nSourceAddress;
    v89->v27 = nDestAddress;
    index = v77;
    v89->v28 = 0;
    while(index > 0)
    {
      index = index - 7;
      v89->v28++;
    }
    v89->v29 = 0;
    v89->v20 = TransportProtocol_d03;
    v89->v21 = true;
  }
  return v68;
}

void ARD1939::f11(byte v90)
{
    if(v90 == d12)
    {
        TP_Struct_1.v20 = TransportProtocol_d01;
        TP_Struct_1.v21 = false;
        TP_Struct_1.v22 = false;
        TP_Struct_1.lPGN = 0;
        TP_Struct_1.v26 = GLOBALADDRESS;
        TP_Struct_1.v27 = GLOBALADDRESS;
        TP_Struct_1.v28 = 0;
        TP_Struct_1.v29 = 0;
        TP_Struct_1.nMsgLen = 0;
        TP_Struct_1.v32 = false;
    }
    f06(&v40);
    f06(&v41);
}

void ARD1939::f12(byte v90)
{
    if(v90 == d12)
    {
        TP_Struct_2.v20 = TransportProtocol_d01;
        TP_Struct_2.v21 = false;
        TP_Struct_2.v23 = false;
        TP_Struct_2.v24 = false;
        TP_Struct_2.lPGN = 0;
        TP_Struct_2.v26 = GLOBALADDRESS;
        TP_Struct_2.v27 = GLOBALADDRESS;
        TP_Struct_2.v28 = 0;
        TP_Struct_2.v29 = 0;
        TP_Struct_2.nMsgLen = 0;
        TP_Struct_2.v32 = false;	
    }
    f06(&v42);
    f06(&v43);
    f06(&v44);
    f06(&v45);
    f06(&v46);
    f06(&v47);
    f06(&v48);
}

#endif

