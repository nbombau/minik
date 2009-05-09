#include "../include/pci.h"
#include "../include/pcilist.h"
#include "../include/pcidevlist.h"

#define  PCIBIOS_SUCCESSFUL                0x00

extern PCI_VENTABLE	PciVenTable [];
extern PCI_DEVTABLE	PciDevTable [];

static void print_vendor(unsigned short vendor);
static void print_device(unsigned short vendor, unsigned short device);
static int pcibios_read (unsigned long dato, unsigned int *value);
static unsigned long armaDato (unsigned int bus, unsigned int devfn,
		unsigned int func, unsigned int reg);

static int
pcibios_read (unsigned long dato, unsigned int *value)
{
	write(PCI,&dato, 1);
	read(PCI, value, 1);
    return PCIBIOS_SUCCESSFUL;
}

static void
print_vendor(unsigned short vendor)
{

	int j = 0;
	int flag = 0;
	while (PciVenTable[j].VenId != 0 && flag == 0)
	{
		if (PciVenTable[j].VenId == vendor) {
			printf(PciVenTable[j].VenFull);
			printf("--");
		}
		j++;
	}
}

static void
print_device(unsigned short vendor, unsigned short device)
{
	int j = 0;
	int flag = 0;
	while (PciDevTable[j].VenId != 0 && flag == 0)
	{
		if (PciDevTable[j].VenId == vendor && PciDevTable[j].DevId == device)
		{
			printf(PciDevTable[j].ChipDesc);
			printf("\n");
			flag = 1;
		}
		j++;
	}
}


unsigned short
lspci (void)
{
	unsigned int devfn, l, bus, buses, func;
	unsigned int first_bus, first_devfn, first_i;
	unsigned char hdr_type = 0;
	unsigned short vendor, device;
	unsigned long dato;
	unsigned short tmp = 0;

	unsigned short arr[8];
	int arrind = 0;

	int i = 0;
	int flag;

	first_bus = 0;
		first_devfn = 0;
		first_i = 0;

	buses=256;
		for (bus = first_bus; bus < buses; bus++) {
			for (devfn = first_devfn; devfn < 32; ++devfn) {
				arrind = 0;
				for (func = 0; func < 8; func++) {

					dato = armaDato (bus, devfn, func, 0);

					pcibios_read(dato, &l);
					if (l == 0xffffffff || l == 0x00000000) {
						hdr_type = 0;
						/* si no encuentra un dispositivo, sale del ciclo */
						continue;
					}



					vendor = l & 0xffff;
					device = (l >> 16) & 0xffff;
					flag = 0;
					for (i = 0; flag ==0, i < arrind; i++)
					{
						if (device == arr[i])
						{
							flag = 1;
						}

					}
					if (flag == 1)
					{

						continue;
					}

					arr[arrind++] = device;

					/* imprimo vendor y device */

					print_vendor(vendor);
					printf("--");
					print_device(vendor, device);

				} /* end for func */
			} /* end for devfn */
		} /* end for bus */
	printf("\n");
}

static unsigned long
armaDato (unsigned int bus, unsigned int devfn, unsigned int func, unsigned int reg)
{
	unsigned long dato = 0;
	dato = ((unsigned long) 0x80000000 | (bus << 16) | (devfn << 11) | (func << 8) | (reg));
	return dato;


}
