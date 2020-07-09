#include "page.h"
#include "lib/string.h"

// build 2-8MB page
// after init, kernel mappging will be:
// 0xffffff8000000000-0xffffff80007fffff -> 0x000000000000-0x0000007fffff
void page_init()
{
    // skip pml4 pdpe pde
    // pte_base starts at 0xd000
    auto pte_base = (Page_PTE *)(PML4E_ADDR + PAGE_SIZE_4K * 4);
    // one pde can cover up to 1GB area
    auto pde = (Page_PDE *)(PML4E_ADDR + PAGE_SIZE_4K * 2);
    // from pte_base we are going to build 3 entire pte from 0xd000 to 0xffff
    // each pte takes 0x1000 bytes
    bzero(pte_base, 0x1000 * 3);
    auto physical_base = 0x200000;
    auto step = 0x1000;

    for (int i = 1; i <= 3; ++i)
    {
        pde[i] = pde[0];
        pde[i].NEXT = uint64_t(pte_base) >> 12;
        for (int j = 0; j < 512; ++j)
        {
            pte_base[j].P = 1;
            pte_base[j].R_W = 1;
            pte_base[j].PPBA = (physical_base * i + j * step) >> 12;
        }
        // switch to next pte base
        pte_base = (Page_PTE *)(uint64_t(pte_base) + 0x1000);
    }
}