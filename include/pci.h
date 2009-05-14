#ifndef PCI_H_
#define PCI_H_

#include "defs.h"


unsigned short lspci (void);

#define USBCMD		        0
#define USBCMD_RS		0x0001	/* Run/Stop */
#define USBCMD_HCRESET	        0x0002	/* Host reset */
#define USBCMD_GRESET		0x0004	/* Global reset */
#define USBCMD_EGSM		0x0008	/* Global Suspend Mode */
#define USBCMD_FGR		0x0010	/* Force Global Resume */
#define USBCMD_SWDBG		0x0020	/* SW Debug mode */
#define USBCMD_CF		0x0040	/* Config Flag (sw only) */
#define USBCMD_MAXP		0x0080	/* Max Packet (0 = 32, 1 = 64) */

/* Status register */
#define USBSTS		        2
#define USBSTS_USBINT		0x0001	/* Interrupt due to IOC */
#define USBSTS_ERROR		0x0002	/* Interrupt due to error */
#define USBSTS_RD		0x0004	/* Resume Detect */
#define USBSTS_HSE		0x0008	/* Host System Error - basically PCI problems */
#define USBSTS_HCPE		0x0010	/* Host Controller Process Error - the scripts were buggy */
#define USBSTS_HCH		0x0020	/* HC Halted */

/* Interrupt enable register */
#define USBINTR		4
#define USBINTR_TIMEOUT	0x0001	/* Timeout/CRC error enable */
#define USBINTR_RESUME	0x0002	/* Resume interrupt enable */
#define USBINTR_IOC		0x0004	/* Interrupt On Complete enable */
#define USBINTR_SP		0x0008	/* Short packet interrupt enable */

#define USBFRNUM	6
#define USBFLBASEADD	8
#define USBSOF		12

/* USB port status and control registers */
#define USBPORTSC1	16
#define USBPORTSC2	18
#define USBPORTSC_CCS		0x0001	/* Current Connect Status ("device present") */
#define USBPORTSC_CSC		0x0002	/* Connect Status Change */
#define USBPORTSC_PE		0x0004	/* Port Enable */
#define USBPORTSC_PEC		0x0008	/* Port Enable Change */
#define USBPORTSC_LS		0x0030	/* Line Status */
#define USBPORTSC_RD		0x0040	/* Resume Detect */
#define USBPORTSC_LSDA	0x0100	/* Low Speed Device Attached */
#define USBPORTSC_PR		0x0200	/* Port Reset */
#define USBPORTSC_SUSP	0x1000	/* Suspend */


#define FALSE 0
#define TRUE 1

typedef struct {
	unsigned short vendor;
	unsigned short device;
	unsigned int classcode;
	unsigned int bar;
} classT;

int tdT2 [4];
char desc[18];
char desc2[18];

#endif /*PCI_H_*/
