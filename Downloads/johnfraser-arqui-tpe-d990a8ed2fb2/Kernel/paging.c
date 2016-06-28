#include "paging.h"
#include <naiveConsole.h>

extern CR0 cr0Read(void);
extern void cr0Write(CR0 cr0);
extern CR3 cr3Read(void);
extern void cr3Write(CR3 cr3);

pml4_entry pml4[512] __attribute__((aligned(0x1000)));
pdpt_entry pdpt[512] __attribute__((aligned(0x1000)));
// static page_entry page;

// void pageEnable(){
// 	CR0 cr0 = cr0Read();
// 	cr0.pg = 1;
// 	cr0Write(cr0);
// }

void pagingInit(){

//creo los vectores

CR3 cr3;

//relleno con 0
memset(pml4, 0, 512 * sizeof(pml4_entry));
memset(pdpt, 0, 512 * sizeof(pdpt_entry));
//memset(page,0, sizeof(page_entry));
//memset(&cr3,0, sizeof(CR3));

//Seteo la pml4
pml4[0].Present = 1;
pml4[0].ReadWrite = 1;
pml4[0].UserSupervisor = 1;
pml4[0].PageLevelWriteThrough = 0;
pml4[0].CacheDisabled = 0;
pml4[0].Accessed = 1;
pml4[0].Ignored = 0;
pml4[0].Reserved = 0;
pml4[0].MustBeZero = 0;
pml4[0].PDPTBaseAddress = ((uint64_t)pdpt)>>12;
pml4[0].AvailableHigh = 0;
pml4[0].Nx = 0;

//Seteo la pdpt
pdpt[0].Present = 1;
pdpt[0].ReadWrite = 1;
pdpt[0].UserSupervisor = 1;
pdpt[0].PageLevelWriteThrough = 1;
pdpt[0].CacheDisabled = 0;
pdpt[0].Accessed = 1;
pdpt[0].Dirty = 1;
pdpt[0].MustBe1 = 1;
pdpt[0].Global = 0;
pdpt[0].Ignored_1 = 0;
pdpt[0].PAT=0;
pdpt[0].Reserved=0;
pdpt[0].PageAddress=0;
pdpt[0].Ignored_2=0;
pdpt[0].Nx=0;

//Seteo la pagina 
/*
page->Present =1;
page->ReadWrite =1;
page->UserSupervisor =1;
page->PageLevelWriteThrough=1;
page->CacheDisabled=0;
page->Accessed =1;
page->Dirty = 1;
page->Pat = 0;
page->Ignored_2=0;
page->PageAddress=0;
page->Ignored_3=0;
page->Nx =0;
*/

//memset(CR3, 0, 64* sizeof(uint64_t));
//Seteo la cr3
cr3 = cr3Read();
// cr3.Ignored = 0;
// cr3.PageLevelWriteThrough = 1;
// cr3.CacheDisabled = 0;
// cr3.Available = 0;
cr3.PML4BaseAddress = ((uint64_t)pml4)>>12;
// cr3.MustBeZero = 0;

cr3Write(cr3);

}

