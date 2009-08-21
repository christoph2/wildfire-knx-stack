
#include "eep_hc12.h"

// zum Thema Kompatibilität HC12/HCS12:
/*
"
Appendix C: 68HC912DG128A EEPROM contains detailed information
to assist in software planning for future EEPROM compatibility and
transition to the 68HC912DG128A. Read, write and erase algorithms are
fully compatible with the present EEPROM design. The key change
comes in the form of a self timed state machine for erasing & writing
data. This is implemented using a pre-scaler loaded from a new word
register EEDIV ($00EE) - located in a presently unused location this
register can be written without effect, reading the location will return
unpredictable data.

	Adding 5 bytes of initialisation code to current software to load EEDIV
	(with value appropriate for the application’s crystal frequency, EXTALi)
	will help ensure compatibility.

Other new features for performance improvement are disabled at reset
providing a compatible algorithm for modifying the EEPROM.
"
*/
/*
//
//	EEPROM-Register.
//
EEDIVH	= 0xEE
EEDIVL	= 0xEF
EEMCR	= 0xF0
EEPROT	= 0xF1
EETST	= 0xF2
EEPROG	= 0xF3
*/

//
//	Bit-Definitionen.
//

#define	EEDIV9	0x02
#define	EEDIV8	0x01
#define	EEDIV7	0x80 
#define	EEDIV6	0x40 
#define	EEDIV5	0x20 
#define	EEDIV4	0x10 
#define	EEDIV3	0x08 
#define	EEDIV2	0x04 
#define	EEDIV1	0x02 
#define	EEDIV0	0x01

#define	NOBDML	0x80 
#define	NOSHW	0x40	 	
#define	EESWAI	0x04 
#define	PROTLCK	0x02 	
#define	DMY		0x01

#define	SHPROT	0x80 
#define	BPROT5	0x20 
#define	BPROT4	0x10 
#define	BPROT3	0x08 
#define	BPROT2	0x04 
#define	BPROT1	0x02 
#define	BPROT0	0x01

#define	EREVTN	0x40
#define	ETMSD	0x04
#define	ETMR	0x02
#define	ETMSE	0x01

#define	BULKP	0x80	
#define	_AUTO	0x20 	
#define	_BYTE	0x10 	
#define	ROW		0x08 	
#define	ERASE	0x04 	
#define	EELAT	0x02 	
#define	EEPGM	0x01
        

static bool eep_programming;
	
void eep_init(void)
{
// The EEDIV value is determined by the following formula:
// EEDIV = INT[EXTALi (hz) x (35x10-6)+0.5 ]
//
// Ergibt bei 8MHz: 280 ==> 0x118	
	eep_programming=false;
	EEDIVH=0x118;
//	EEPORT=0x00;
//	EEMCR|=(NOBDML+NOSHW+EESWAI+PROTLCK);
}	


void eep_progbyte(ADDR_T addr,BYTE data)
{
	eep_programming=true;
	
	if (((BYTE)*addr)==data) {
		return;
	}
	
	eep_erasebyte(addr,data);

// 1. Write BYTE, ROW and ERASE to desired value, write EELAT = 1.
	EEPROG=(_BYTE+EELAT);
// 2. Write a byte or an aligned word to an EEPROM address.	
	*addr=data;
// 3. Write EEPGM = 1.
	EEPROG|=EEPGM;
// 4. Wait for programming, t PROG or erase, t ERASE delay time (10ms).
	while ((EEPROG & _AUTO)==_AUTO)
		;
// 5. Write EEPGM = 0.
	EEPROG&=~EEPGM;
// 6. Write EELAT = 0.	
	EEPROG&=~EELAT;

	eep_programming=false;
}

void eep_erasebyte(ADDR_T addr,BYTE data)
{
	if (((BYTE)*addr)++==0) {
		return;
	}	

// 1. Write BYTE, ROW and ERASE to desired value, write EELAT = 1.	
	EEPROG=(_BYTE+ERASE+EELAT+_AUTO);
// 2. Write a byte or an aligned word to an EEPROM address.	
	*addr=data;
// 3. Write EEPGM = 1.	
	EEPROG|=EEPGM;
// 4. Wait for programming, t PROG or erase, t ERASE delay time (10ms).
	while ((EEPROG & _AUTO)==_AUTO)
		;	
// 5. Write EEPGM = 0.
	EEPROG&=~EEPGM;
// 6. Write EELAT = 0 (and BULKP=1).
//	EEPROG&=~EELAT;		
	EEPGM=BULKP;	
}

void eep_protect(bool on)
{
	if (on==true) {
		EEPROT=0xff;
	} else {
		EEPROT=0xc0;
	}
}