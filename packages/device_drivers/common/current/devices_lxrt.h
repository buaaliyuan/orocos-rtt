
#ifndef DEVICES_LXRT_H
#define DEVICES_LXRT_H

#ifdef __cplusplus
extern "C" {
#endif

//#define CONFIG_RTAI_CPUS 4 // only defined for SMP systems !
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#undef DECLARE
#define DECLARE static inline

    // Need this for u32 etc type definitions
#ifndef __KERNEL__
#define __KERNEL__
#include <asm/types.h>
#undef __KERNEL__
#else
#include <asm/types.h>
#endif

#ifdef HAVE_APCI1032

#ifdef __KERNEL__
/* APCI1032 Prototype functions */
int i_APCI1032_CheckAndGetPCISlotNumber ( unsigned char *pb_SlotArray );
int i_APCI1032_SetBoardInformation ( unsigned char b_SlotNumber, int * pi_BoardHandle );
int i_APCI1032_GetHardwareInformation ( int i_BoardHandle, unsigned int * pui_BaseAddress, unsigned char * pb_InterruptNbr, unsigned char * pb_SlotNumber );
int i_APCI1032_CloseBoardHandle ( int i_BoardHandle );

int i_APCI1032_Read1DigitalInput ( int i_BoardHandle, unsigned char b_Channel, unsigned long * pb_ChannelValue );
int i_APCI1032_Read8DigitalInput ( int i_BoardHandle, unsigned char b_Port, unsigned long * pb_PortValue );
int i_APCI1032_Read16DigitalInput ( int i_BoardHandle, unsigned char b_Port, unsigned long * pul_InputValue );
int i_APCI1032_Read32DigitalInput ( int i_BoardHandle, unsigned long * pul_InputValue );
#endif
#endif //HAVE_APCI1032

#ifdef HAVE_APCI2200
// we typedef to void since the address has no meaning in user space, it's just a number.
typedef void apci2200_device;

#ifdef __KERNEL__
apci2200_device* apci2200_get_device( void );
unsigned int apci2200_get_input_status( apci2200_device* dev );
unsigned int apci2200_get_output_status( apci2200_device* dev );
void apci2200_set_output_on( apci2200_device* dev, unsigned int on );
void apci2200_set_output_off( apci2200_device* dev, unsigned int off );
/*
void apci2200_watchdog_init( apci2200_device* dev, short access_mode, short time_unit, int reload_value );
void apci2200_watchdog_start( void );
void apci2200_watchdog_stop( void );
void apci2200_watchdog_get_status( void );
void apci2200_watchdog_trigger( void );
*/
unsigned int apci2200_get_number_off_digital_outputs( void );
#endif
#endif //HAVE_APCI2200

#ifdef HAVE_APCI1710
// same as above
typedef void apci1710_device;
typedef void apci1710_module;

#ifdef __KERNEL__
apci1710_device* apci1710_get_device( void );
int apci1710_ssi_init( apci1710_device* dev, int moduleNb, int ssiProfile, int positionBitLength, int turnBitLength, int countingMode, unsigned int ssiOutputClock );
int apci1710_ssi_read( apci1710_device* dev, int moduleNb, int selectedSSI, unsigned int* result );
int apci1710_ssi_read_all( apci1710_device* dev, int moduleNb, unsigned int* result );
int apci1710_ssi_refresh(apci1710_device* dev, int moduleNb );
int apci1710_ssi_read_latest(apci1710_device* dev, int moduleNb, int selectedSSI, unsigned int* result );
int apci1710_ssi_read_latest_all( apci1710_device* dev, int moduleNb, unsigned int* result );

// INCREMENTAL ENCODER FUNCTIONS
apci1710_module* apci1710_incr_create_module(void);
int apci1710_incr_cleanup_module(apci1710_module* module);
int apci1710_incr_init( apci1710_device* dev, unsigned int moduleNb, int counterRange, int firstCounterModus, int firstCounterOption, int secondCounterModus, int secondCounterOption, apci1710_module* module );
int apci1710_incr_counter_auto_test( apci1710_module* module );
int apci1710_incr_clear_counter_value( apci1710_module* module );
int apci1710_incr_clear_all_counter_value( apci1710_module* module );
int apci1710_incr_set_input_filter( apci1710_module* module, int PCIclock, int filter );
int apci1710_incr_latch_counter( apci1710_module* module, unsigned int latchRegister );
int apci1710_incr_read_latch_register_status( apci1710_module* module, unsigned int latchRegister, int* latchStatus );
int apci1710_incr_read_latch_register_value( apci1710_module* module, unsigned int latchRegister, unsigned int* result );
int apci1710_incr_read_16bit_counter_value( apci1710_module* module, unsigned int latchRegister, u32* result );
int apci1710_incr_enable_latch_interrupt( apci1710_module* module );
int apci1710_incr_disable_latch_interrupt( apci1710_module* module );
int apci1710_incr_read_32bit_counter_value( apci1710_module* module, u32* result );
int apci1710_incr_write_16bit_counter_value( apci1710_module* module, unsigned int selectedCounter, u32 writeValue );
int apci1710_incr_write_32bit_counter_value( apci1710_module* module, u32 writeValue );
int apci1710_incr_init_index( apci1710_module* module, int referenceAction, int indexOperation, int autoMode, int interruptMode );
int apci1710_incr_enable_index( apci1710_module* module );
int apci1710_incr_disable_index( apci1710_module* module );
int apci1710_incr_get_index_status( apci1710_module* module, int* result );
int apci1710_incr_set_digital_channel_on( apci1710_module* module );
int apci1710_incr_set_digital_channel_off( apci1710_module* module );
int apci1710_incr_cleanup_module( apci1710_module* module );
#endif
#endif //HAVE_APCI1710


#define HAVE_JR3
#ifdef HAVE_JR3
struct ForceArray
{
  float Fx;
  float Fy;
  float Fz;
  float Tx;
  float Ty;
  float Tz;
};
#ifdef __KERNEL__
extern void JR3DSP_check_sensor_and_DSP( unsigned int dsp );
extern void JR3DSP_set_offsets(const struct ForceArray* offsets, unsigned int dsp);
extern unsigned short JR3DSP_get_error_word(unsigned int dsp);
extern void JR3DSP_setUnits_N_dNm_mmX10(unsigned int dsp);
extern void JR3DSP_getDataFromFilter0(struct ForceArray* data, unsigned int dsp);
extern void JR3DSP_getDataFromFilter1(struct ForceArray* data, unsigned int dsp);
extern void JR3DSP_getDataFromFilter2(struct ForceArray* data, unsigned int dsp);
extern void JR3DSP_getDataFromFilter3(struct ForceArray* data, unsigned int dsp);
extern void JR3DSP_getDataFromFilter4(struct ForceArray* data, unsigned int dsp);
extern void JR3DSP_getDataFromFilter5(struct ForceArray* data, unsigned int dsp);
extern void JR3DSP_getDataFromFilter6(struct ForceArray* data, unsigned int dsp);
#endif
#endif //HAVE_JR3


// Every module that extends LXRT needs a unique MYIDX (1-15).
#define MYIDX			15

#define APCI1032_READ1          0
#define APCI1032_READ8          1
#define APCI1032_READ16         2
#define APCI1032_READ32	        3

#define APCI2200_GETDEVICE	10
#define APCI2200_GETINPUTSTATUS	11
#define APCI2200_GETOUTPUTSTATUS	12
#define APCI2200_SETOUTPUTON	13
#define APCI2200_SETOUTPUTOFF	14
#define APCI2200_WATCHDOGINIT	15
#define APCI2200_WATCHDOGSTART	16
#define APCI2200_WATCHDOGSTOP	17
#define APCI2200_WATCHDOGGETSTATUS	18
#define APCI2200_WATCHDOGTRIGGER	19
#define APCI2200_GETNUMBERDO            20

#define APCI1710_GETDEVICE              21
#define APCI1710_SSI_INIT               22
#define APCI1710_SSI_READ               23
#define APCI1710_SSI_READALL            24
#define APCI1710_SSI_REFRESH            25
#define APCI1710_SSI_READLATEST         26
#define APCI1710_SSI_READLATESTALL      27

#define APCI1710_INCR_INIT              30
#define APCI1710_INCR_COUNTER_AUTOTEST  31
#define APCI1710_INCR_CLEAR_COUNTER     32
#define APCI1710_INCR_SET_INPFILTER     33
#define APCI1710_INCR_LATCH_COUNTER     34
#define APCI1710_INCR_READ_LATCH_REGSTATUS 35
#define APCI1710_INCR_READ_LATCH_REGVALUE  36
#define APCI1710_INCR_READ_16_COUNT_VALUE  37
#define APCI1710_INCR_ENABLE_LATCH_INT     38
#define APCI1710_INCR_DISABLE_LATCH_INT    39
#define APCI1710_INCR_READ_32_COUNT_VALUE  40
#define APCI1710_INCR_WRITE_16_COUNT_VALUE 41
#define APCI1710_INCR_WRITE_32_COUNT_VALUE 42
#define APCI1710_INCR_INIT_INDEX           43
#define APCI1710_INCR_ENABLE_INDEX         44
#define APCI1710_INCR_DISABLE_INDEX        45
#define APCI1710_INCR_GET_INDEX_STATUS     46
#define APCI1710_INCR_SET_DIG_CHANNEL_ON   47
#define APCI1710_INCR_SET_DIG_CHANNEL_OFF  48
#define APCI1710_INCR_CLEANUP              49
#define APCI1710_INCR_CREATE               50


#define JR3DSP_CHECK_SENSOR_AND_DSP         60
#define JR3DSP_SET_OFFSETS                  61
#define JR3DSP_GET_ERROR_WORD               62
#define JR3DSP_SETUNITS_N_DNM_MMx10         63
#define JR3DSP_GETDATAFROMFILTER0           64
#define JR3DSP_GETDATAFROMFILTER1           65
#define JR3DSP_GETDATAFROMFILTER2           66
#define JR3DSP_GETDATAFROMFILTER3           67
#define JR3DSP_GETDATAFROMFILTER4           68
#define JR3DSP_GETDATAFROMFILTER5           69
#define JR3DSP_GETDATAFROMFILTER6           70



#ifndef __KERNEL__

#include <stdarg.h>
//#include <rtai_lxrt_common.h>
#include <os/fosi.h>

union rtai_lxrt_t rtai_lxrt(short int dynx, short int lsize, int srq, void *arg);

#define SIZARG sizeof(arg)

    // hack until C++ inlining is fixed
#undef DECLARE
#define DECLARE static inline 

#ifdef HAVE_APCI1032
DECLARE int i_APCI1032_Read1DigitalInput ( int i_BoardHandle, unsigned char b_Channel, unsigned long * pb_ChannelValue )
{
  unsigned long pb_val; int retval;
  struct { int i_BoardHandle; unsigned char b_Channel; unsigned long * pb_ChannelValue; } arg = {i_BoardHandle, b_Channel, &pb_val };
  retval = rtai_lxrt(MYIDX, SIZARG, APCI1032_READ1, &arg).i[LOW];
  memcpy(pb_ChannelValue, &pb_val, sizeof(unsigned long) );
  return retval;
}

DECLARE int i_APCI1032_Read8DigitalInput ( int i_BoardHandle, unsigned char b_Port, unsigned long * pb_PortValue )
{
  unsigned long pb_val; int retval;
  struct { int i_BoardHandle; unsigned char b_Port; unsigned long * pb_PortValue; } arg = {i_BoardHandle, b_Port, &pb_val };
  retval = rtai_lxrt(MYIDX, SIZARG, APCI1032_READ8, &arg).i[LOW];
  memcpy(pb_PortValue, &pb_val, sizeof(unsigned long) );
  return retval;
}

DECLARE int i_APCI1032_Read16DigitalInput ( int i_BoardHandle, unsigned char b_Port, unsigned long * pul_InputValue )
{
  unsigned long pb_val; int retval;
  struct { int i_BoardHandle; unsigned char b_Port; unsigned long * pul_InputValue; } arg = {i_BoardHandle, b_Port, &pb_val };
  retval = rtai_lxrt(MYIDX, SIZARG, APCI1032_READ16, &arg).i[LOW];
  memcpy(pul_InputValue, &pb_val, sizeof(unsigned long) );
  return retval;
}

DECLARE int i_APCI1032_Read32DigitalInput ( int i_BoardHandle, unsigned long * pul_InputValue )
{
  unsigned long pb_val; int retval;
  struct { int i_BoardHandle; unsigned long * pul_InputValue; } arg = {i_BoardHandle, &pb_val };
  retval = rtai_lxrt(MYIDX, SIZARG, APCI1032_READ32, &arg).i[LOW];
  memcpy(pul_InputValue, &pb_val, sizeof(unsigned long) );
  return retval;
}
#endif
/***************************************************************************
 ***************************************************************************
 **************************************************************************/

#ifdef HAVE_APCI2200
DECLARE apci2200_device* apci2200_get_device( void )
{
  struct { int var; } arg = { 0 };
  return rtai_lxrt(MYIDX, SIZARG, APCI2200_GETDEVICE, &arg).v[LOW];
}

DECLARE unsigned int apci2200_get_input_status( apci2200_device* dev )
{
  struct { apci2200_device* dev; } arg = { dev };
  return rtai_lxrt(MYIDX, SIZARG, APCI2200_GETINPUTSTATUS, &arg).i[LOW];
}

DECLARE unsigned int apci2200_get_output_status( apci2200_device* dev )
{
  struct { apci2200_device* dev; } arg = { dev };
  return rtai_lxrt(MYIDX, SIZARG, APCI2200_GETOUTPUTSTATUS, &arg).i[LOW];
}

DECLARE void apci2200_set_output_on( apci2200_device* dev, unsigned int on )
{
  struct { apci2200_device* dev; unsigned int on; } arg = { dev, on };
  rtai_lxrt(MYIDX, SIZARG, APCI2200_SETOUTPUTON, &arg);
}

DECLARE void apci2200_set_output_off( apci2200_device* dev, unsigned int off )
{
  struct { apci2200_device* dev; unsigned int off; } arg = { dev, off };
  rtai_lxrt(MYIDX, SIZARG, APCI2200_SETOUTPUTOFF, &arg);
}

/**
 * Not yet supported, but easy to do
 *
void apci2200_watchdog_init( apci2200_device* dev, short access_mode, short time_unit, int reload_value );
void apci2200_watchdog_start( void );
void apci2200_watchdog_stop( void );
void apci2200_watchdog_get_status( void );
void apci2200_watchdog_trigger( void );
 */

DECLARE unsigned int apci2200_get_number_off_digital_outputs( void )
{
  struct { int val; } arg = { 0 };
  return rtai_lxrt(MYIDX, SIZARG, APCI2200_GETNUMBERDO, &arg).i[LOW];
}

#endif

/***************************************************************************
 ***************************************************************************
 **************************************************************************/

#ifdef HAVE_APCI1710

typedef void apci1710_device;

/** if the bits needs conversion to binary code */
#define COUNTINGMODE_BINARY 1


DECLARE apci1710_device* apci1710_get_device( void )
{
  struct { int val; } arg = { 0 };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_GETDEVICE, &arg).v[LOW];
}


DECLARE int apci1710_ssi_init( apci1710_device* dev, int moduleNb, int ssiProfile, int positionBitLength, int turnBitLength, int countingMode, unsigned int ssiOutputClock )
{
  struct { apci1710_device* dev; int moduleNb; int ssiProfile; int positionBitLength; int turnBitLength; int countingMode; unsigned int ssiOutputClock; } 
    arg = { dev, moduleNb, ssiProfile,  positionBitLength, turnBitLength, countingMode, ssiOutputClock };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_SSI_INIT, &arg).i[LOW];
}

DECLARE int apci1710_ssi_read( apci1710_device* dev, int moduleNb, int selectedSSI, unsigned int* result )
{
  struct { apci1710_device* dev; int moduleNb; int selectedSSI; unsigned int* _result;  } arg = { dev, moduleNb, selectedSSI, result };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_SSI_READ, &arg).i[LOW];
}


DECLARE int apci1710_ssi_read_all( apci1710_device* dev, int moduleNb, unsigned int* result )
{
  struct { apci1710_device* dev; int moduleNb; unsigned int *_result; int size; } arg = { dev, moduleNb, result, 3*sizeof(unsigned int) };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_SSI_READALL, &arg).i[LOW];
}

DECLARE int apci1710_ssi_refresh(apci1710_device* dev, int moduleNb )
{
  struct { apci1710_device* dev; int moduleNb; } arg = { dev, moduleNb };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_SSI_REFRESH, &arg).i[LOW];
}

DECLARE int apci1710_ssi_read_latest(apci1710_device* dev, int moduleNb, int selectedSSI, unsigned int* result )
{
  struct { apci1710_device* dev; int moduleNb; int selectedSSI; unsigned int* result;  } arg = { dev,moduleNb, selectedSSI, result };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_SSI_READLATEST, &arg).i[LOW];
}

DECLARE int apci1710_ssi_read_latest_all( apci1710_device* dev, int moduleNb, unsigned int* result )
{
  struct { apci1710_device* dev; int moduleNb; unsigned int* _result; int size; } arg = { dev,moduleNb, result, 3*sizeof(unsigned int) };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_SSI_READLATESTALL, &arg).i[LOW];
}


/***************************************************************************
 ***************************************************************************
 **************************************************************************/

/**
 * Not yet implemented, but analog to the ssi stuff.
 */
DECLARE int apci1710_incr_init( apci1710_device* dev, unsigned int moduleNb, int counterRange, int firstCounterModus, int firstCounterOption, int secondCounterModus, int secondCounterOption, apci1710_module* module )
{
  struct { apci1710_device* dev; unsigned int moduleNb; int counterRange; int firstCounterModus; int firstCounterOption; int secondCounterModus;  int secondCounterOption; apci1710_module* module; } 
    arg = { dev, moduleNb, counterRange, firstCounterModus, firstCounterOption, secondCounterModus, secondCounterOption, module };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_INCR_INIT, &arg).i[LOW];
}

DECLARE apci1710_module* apci1710_incr_create_module()
{
  struct { int val; } 
    arg = { 0 };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_INCR_CREATE, &arg).v[LOW];
}

DECLARE int apci1710_incr_cleanup_module( apci1710_module* module )
{
  struct { apci1710_module* module; } 
    arg = { module };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_INCR_CLEANUP, &arg).i[LOW];
}

DECLARE int apci1710_incr_latch_counter( apci1710_module* module, unsigned int latchRegister )
{
  struct { apci1710_module* module; unsigned int latchRegister; } 
    arg = { module, latchRegister };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_INCR_LATCH_COUNTER, &arg).i[LOW];
}
DECLARE int apci1710_incr_read_latch_register_value( apci1710_module* module, unsigned int latchRegister, unsigned int* result )
{
  struct { apci1710_module* module; unsigned int latchRegister; unsigned int* result; } 
    arg = { module, latchRegister, result};
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_INCR_READ_LATCH_REGVALUE, &arg).i[LOW];
}

DECLARE int apci1710_incr_clear_counter_value( apci1710_module* module )
{
  struct { apci1710_module* module; } 
    arg = { module };
  return rtai_lxrt(MYIDX, SIZARG, APCI1710_INCR_CLEAR_COUNTER, &arg).i[LOW];
}

/*
int apci1710_incr_counter_auto_test( apci1710_module* module );
int apci1710_incr_clear_all_counter_value( apci1710_module* module );
int apci1710_incr_set_input_filter( apci1710_module* module, int PCIclock, int filter );
int apci1710_incr_read_latch_register_status( apci1710_module* module, unsigned int latchRegister, int* latchStatus );

int apci1710_incr_read_16bit_counter_value( apci1710_module* module, unsigned int latchRegister, u32* result );
int apci1710_incr_enable_latch_interrupt( apci1710_module* module );
int apci1710_incr_disable_latch_interrupt( apci1710_module* module );
int apci1710_incr_read_32bit_counter_value( apci1710_module* module, u32* result );
int apci1710_incr_write_16bit_counter_value( apci1710_module* module, unsigned int selectedCounter, u32 writeValue );
int apci1710_incr_write_32bit_counter_value( apci1710_module* module, u32 writeValue );
int apci1710_incr_init_index( apci1710_module* module, int referenceAction, int indexOperation, int autoMode, int interruptMode );
int apci1710_incr_enable_index( apci1710_module* module );
int apci1710_incr_disable_index( apci1710_module* module );
int apci1710_incr_get_index_status( apci1710_module* module, int* result );
int apci1710_incr_set_digital_channel_on( apci1710_module* module );
int apci1710_incr_set_digital_channel_off( apci1710_module* module );
*
**/

#endif // HAVE_APCI1710

/***************************************************************************
 ***************************************************************************
 **************************************************************************/


#if 0

JR3DSP_SET_OFFSETS





#endif

#ifdef HAVE_JR3

DECLARE void JR3DSP_check_sensor_and_DSP( unsigned int dsp )
{
  struct { unsigned int dsp; } arg = { dsp };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_CHECK_SENSOR_AND_DSP, &arg);
}

// UR1(1,3)
DECLARE void JR3DSP_set_offsets(const struct ForceArray* offsets, unsigned int dsp)
{
    struct ForceArray v_off;
    memcpy(&v_off, offsets, sizeof(struct ForceArray));
    struct { struct ForceArray* offsets; unsigned int dsp; unsigned int size; } arg = {&v_off, dsp, sizeof(struct ForceArray)};
    rtai_lxrt(MYIDX, SIZARG, JR3DSP_SET_OFFSETS, &arg);
}


DECLARE unsigned short JR3DSP_get_error_word(unsigned int dsp)
{
  unsigned short retval;
  struct { unsigned int dsp; } arg = { dsp };
  retval = rtai_lxrt(MYIDX, SIZARG, JR3DSP_GET_ERROR_WORD, &arg).i[LOW];
  return retval;
}

DECLARE void JR3DSP_setUnits_N_dNm_mmX10(unsigned int dsp)
{
  struct { unsigned int dsp; } arg = { dsp };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_SETUNITS_N_DNM_MMx10, &arg);
}

DECLARE void JR3DSP_getDataFromFilter0(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER0, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}

DECLARE void JR3DSP_getDataFromFilter1(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER1, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}

DECLARE void JR3DSP_getDataFromFilter2(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER2, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}

DECLARE void JR3DSP_getDataFromFilter3(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER3, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}

DECLARE void JR3DSP_getDataFromFilter4(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER4, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}

DECLARE void JR3DSP_getDataFromFilter5(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER5, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}

DECLARE void JR3DSP_getDataFromFilter6(struct ForceArray* data, unsigned int dsp)
{
  struct ForceArray pb_val;
  struct { struct ForceArray* data; unsigned int dsp; unsigned int size;} arg = { &pb_val, dsp, sizeof(struct ForceArray) };
  rtai_lxrt(MYIDX, SIZARG, JR3DSP_GETDATAFROMFILTER6, &arg);
  memcpy(data, &pb_val, sizeof(struct ForceArray) );
}



#endif //HAVE_JR3

/***************************************************************************
 ***************************************************************************
 **************************************************************************/


#endif /* __KERNEL__ */


#ifdef __cplusplus
}
#endif // "C"

#endif
