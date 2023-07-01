#include "bsp_flash.h"

#define PAGE_WRITE_START_ADDR  ((uint32_t)0x0800F000) /* Start from 60K */
#define PAGE_WRITE_END_ADDR    ((uint32_t)0x08010000) /* End at 64K */
#define FLASH_PAGE_SIZE                   4096


int flash_write_data(uint16_t halfWordBuf[], uint32_t numOfHalfWord)
{
    uint32_t NbrOfPage = 0;
    uint32_t EraseCounter = 0x0, Address = 0x0;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    int i = 0, ret = 0;
    
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV2;
    __disable_irq(); 

    FLASH_Unlock();

    NbrOfPage = (PAGE_WRITE_END_ADDR - PAGE_WRITE_START_ADDR) / FLASH_PAGE_SIZE;

    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    do {
        /* Erase */
        for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
        {
            FLASHStatus = FLASH_ErasePage(PAGE_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));  //Erase 4KB

            if(FLASHStatus != FLASH_COMPLETE)
            {
                ret = -1;
                break;
            }
        }

        /* Write */
        Address = PAGE_WRITE_START_ADDR;
        for (i = 0; (i < numOfHalfWord) && (Address + 2*i < PAGE_WRITE_END_ADDR); i++)
        {
            FLASHStatus = FLASH_ProgramHalfWord(Address + 2*i, halfWordBuf[i]);
            if (FLASH_COMPLETE != FLASHStatus)
            {
                ret = -2;
                break;
            }
        }

        /* Check */
        Address = PAGE_WRITE_START_ADDR;
        for (i = 0; (i < numOfHalfWord) && (Address + 2*i < PAGE_WRITE_END_ADDR); i++)
        {
            if((*(__IO uint16_t*)(Address + 2*i)) != halfWordBuf[i])
            {
                ret = -3;
                break;
            }
        }
    } while (0);

    FLASH_Lock();

    RCC->CFGR0 &= ~(uint32_t)RCC_HPRE_DIV2;
    __enable_irq();
    
    return ret;
}

void flash_read_data(uint16_t halfWordBuf[], uint32_t numOfHalfWord)
{
    uint32_t Address = 0x0;
    int i = 0;
    
    /* Check */
    Address = PAGE_WRITE_START_ADDR;
    for (i = 0; (i < numOfHalfWord) && (Address + 2*i < PAGE_WRITE_END_ADDR); i++)
    {
        halfWordBuf[i] = *(__IO uint16_t*)(Address + 2*i);
    }
}
