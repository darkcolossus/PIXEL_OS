#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "include/display.h"
#include "include/keyboard.h"
#include "include/idt.h"
#include "include/paging.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;




void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{
	pagingInit();
	//memInit();
	pageManagementInit();
	kKBInitialize();
	kInitializeDisplay();
	kIDTInitialize();
	initializeRequiredProcesses();

/*
	process * p = initProcess(0x400000,"nombre");
	process * p2 = initProcess(0x410000,"nombere");
	process * p3 = initProcess(0x424000,"nombfgre");

	process * p4 = initProcess(0x400000,"nombre");
	process * p5 = initProcess(0x410000,"nombere");
	process * p6 = initProcess(0x424000,"nombfgre");

		addProcess(p);
		addProcess(p2);
		addProcess(p3);
		addProcess(p4);
		addProcess(p5);
		addProcess(p6);

		printALl();
			//ncPrint("antes ");
		//addProcess(p2);
		//	ncPrint("antes ");
		//addProcess(p3);
		//	ncPrint("antes ");
		//printALl();
		deleteProcess(p5);
		printALl();
		deleteProcess(p);
			printALl();
			*/
/*
	ncNewline();
	ncPrint("  main User stack: 0x");
	ncPrintHex((uint64_t)p->userStack);

	ncNewline();
	ncPrint("  main Kernel stack: 0x");
	ncPrintHex((uint64_t)p->kernelStack);

	ncNewline();
	ncPrint("  main Process PID: ");
	ncPrintDec((uint64_t)p->PID);

	ncNewline();
	ncPrint(" main EntryPoint: 0x");
	ncPrintHex((uint64_t)p->entryPoint);
*/






	((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}
