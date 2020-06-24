#pragma once
#include "lib/stdint.h"
#include "lib/string.h"
#include "buddy_system.h"

// the virtual memory that kernel start, mapped to physical 0x0
constexpr uint64_t PAGE_OFFSET = 0xffffff8000000000;
constexpr uint64_t KETNEL_PAGE_OFFSET = PAGE_OFFSET;                               // alias
constexpr uint64_t KETNEL_MEM_BITMAP_OFFSET = KETNEL_PAGE_OFFSET + 0x200000;       // alias
constexpr uint64_t KETNEL_MEM_BUDDY_SYSTEM_OFFSET = KETNEL_PAGE_OFFSET + 0x200000; // alias
constexpr uint64_t KETNEL_MEM_ZONE_SYSTEM_OFFSET = KETNEL_PAGE_OFFSET + 0x200000;  // alias

constexpr uint16_t ENTRIES_PER_PAGE = 512;

// 4k page needs 12 bits offset
constexpr uint16_t PAGE_4K_SHIFT = 12;

// 4k page size is 4096 bytes
constexpr uint16_t PAGE_4K_SIZE = (1UL << PAGE_4K_SHIFT);

// use to mask the low 12 bits
constexpr uint64_t PAGE_4K_MASK = (~(PAGE_4K_SIZE - 1));

constexpr auto BUDDY_ZONE_LOW_INDEX = 0;
constexpr auto BUDDY_ZONE_NORMAL_INDEX = 1;

// round up the addr to 4k boundary
#define PAGE_4K_ALIGN(addr) (((uint64_t)(addr) + PAGE_4K_SIZE - 1) & PAGE_4K_MASK)

#define Virt_To_Phy(addr) ((uint8_t *)(addr)-PAGE_OFFSET)
#define Phy_To_Virt(addr) ((uint8_t *)((uint8_t *)(addr) + PAGE_OFFSET))

#define Virt_To_2M_Page(kaddr) (memory_management_struct.pages_struct + (Virt_To_Phy(kaddr) >> PAGE_2M_SHIFT))
#define Phy_to_2M_Page(kaddr) (memory_management_struct.pages_struct + ((unsigned long)(kaddr) >> PAGE_2M_SHIFT))

#define flush_tlb()               \
    do                            \
    {                             \
        unsigned long tmpreg;     \
        __asm__ __volatile__(     \
            "movq	%%cr3,	%0	\n\t" \
            "movq	%0,	%%cr3	\n\t" \
            : "=r"(tmpreg)        \
            :                     \
            : "memory");          \
    } while (0)

inline uint64_t Get_CR3()
{
    void *addr;
    __asm__ __volatile__(
        "movq	%%cr3,	%0	\n\t"
        : "=r"(addr)
        :
        : "memory");
    return reinterpret_cast<uint64_t>(addr);
}

struct NO_ALIGNMENT E820
{
    uint64_t address;
    uint64_t length;
    uint32_t type;
};

struct Zone
{
    BuddySystem *buddy;
};

struct MemoryDescriptor
{
    // zone entry for the entire physical memory
    BuddySystem **buddys;
    // the whole size in bytes that zones takes, contiguous
    uint64_t buddys_size;
    // same as zones_size / sizeof(Zone)
    uint64_t buddys_count;

    static MemoryDescriptor &GetInstance()
    {
        static MemoryDescriptor instance;
        return instance;
    }

    uint64_t EndAddress()
    {
        return (uint64_t) & this->buddys[this->buddys_count];
    }
};
void memory_init();

extern "C"
{
    Page *alloc_pages(uint8_t buddy_index, uint32_t pages_count, uint64_t page_flags);
}