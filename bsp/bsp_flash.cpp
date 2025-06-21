#include "bsp_flash.h"
// flash�洢ʹ��

#include "z_include.h"


// flash��ʼ�洢��ַ
// ���1KB
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
// flash��ʼ��ַ
#define BSP_FLASH_ALL_START_ADDR 0x8000000
// flash��С
#define BSP_FLASH_ALL_SIZE 			 64*Z_1K
// ���1KB�����洢
#define BSP_FLASH_USE_SAVE_SIZE  Z_1K
#endif
// ��ֲ ����

#define BSP_FLASH_SAVE_START_ADDR (BSP_FLASH_ALL_START_ADDR+BSP_FLASH_ALL_SIZE-BSP_FLASH_USE_SAVE_SIZE)

// һЩ�õ���flash����
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
// FLASH��С
#define STM32FLASH_SIZE         BSP_FLASH_ALL_SIZE
//FLASH��ʼ��ַ
#define STM32FLASH_BASE         FLASH_BASE
//FLASH������ַ
#define STM32FLASH_END          (STM32FLASH_BASE | STM32FLASH_SIZE)
/* FLASHҳ��С */
#define STM32FLASH_PAGE_SIZE    FLASH_PAGE_SIZE
/* FLASH��ҳ�� */
#define STM32FLASH_PAGE_NUM     (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)

uint16_t FlashBuffer[STM32FLASH_PAGE_SIZE >> 1];

/**
 @brief �ڲ�Flash��ȡ
 @param address -[in] ��ȡ�ĵ�ַ
 @param pData -[out] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return �����ɹ����ֽ���
*/
uint32_t Internal_ReadFlash(uint32_t addrStart, void *pData, uint32_t dataLen)
{
    uint32_t nread = dataLen;
    uint8_t *pBuffer = (uint8_t *)pData;
    const uint8_t *pAddr = (const uint8_t *)addrStart;

    if(!pData || addrStart < STM32FLASH_BASE || addrStart > STM32FLASH_END)
    {
        return 0;
    }

    while(nread >= sizeof(uint32_t) && (((uint32_t)pAddr) <= (STM32FLASH_END - 4)))
    {
        *(uint32_t *)pBuffer = *(uint32_t *)pAddr;
        pBuffer += sizeof(uint32_t);
        pAddr += sizeof(uint32_t);
        nread -= sizeof(uint32_t);
    }

    while(nread && (((uint32_t)pAddr) < STM32FLASH_END))
    {
        *pBuffer++ = *pAddr++;
        nread--;
    }

    return dataLen - nread;
}

/**
 @brief �ڲ�Flash�޼��д��
 @param address -[in] д��ĵ�ַ
 @param pData -[in] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return ʵ��д�������������λ���ֽ�
*/
uint32_t Internal_WriteFlashNoCheck(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen)
{
    uint32_t nwrite = dataLen;
    uint32_t addrmax = STM32FLASH_END - 2;

    while(nwrite)
    {
        if(addrStart > addrmax)
        {
            break;
        }

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addrStart, *pData);
        if((*(__IO uint16_t*) addrStart) != *pData)
        {
            break;
        }

        nwrite--;
        pData++;
        addrStart += 2;
    }
    return (dataLen - nwrite);
}

/**
 @brief �ڲ�Flashд��
 @param address -[in] д��ĵ�ַ
 @param pData -[in] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return ʵ��д�������������λ���ֽ�
*/
uint32_t Internal_WriteFlash(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen)
{
    uint32_t i = 0;
    uint32_t pagepos = 0;         // ҳλ��
    uint32_t pageoff = 0;         // ҳ��ƫ�Ƶ�ַ
    uint32_t pagefre = 0;         // ҳ�ڿ���ռ�
    uint32_t offset = 0;          // Address��FLASH�е�ƫ��
    uint32_t nwrite = dataLen;    // ��¼ʣ��Ҫд���������
    const uint16_t *pBuffer = (const uint16_t *)pData;

    /* �Ƿ���ַ */
    if(addrStart < STM32FLASH_BASE || addrStart > (STM32FLASH_END - 2) || dataLen == 0 || pData == NULL)
    {
        return 0;
    }

    /* ����FLASH */
    HAL_FLASH_Unlock();

    /* ����ƫ�Ƶ�ַ */
    offset = addrStart - STM32FLASH_BASE;
    /* ���㵱ǰҳλ�� */
    pagepos = offset / STM32FLASH_PAGE_SIZE;
    /* ����Ҫд���ݵ���ʼ��ַ�ڵ�ǰҳ�ڵ�ƫ�Ƶ�ַ */
    pageoff = ((offset % STM32FLASH_PAGE_SIZE) >> 1);
    /* ���㵱ǰҳ�ڿ���ռ� */
    pagefre = ((STM32FLASH_PAGE_SIZE >> 1) - pageoff);
    /* Ҫд������������ڵ�ǰҳ������ */
    if(nwrite <= pagefre)
    {
        pagefre = nwrite;
    }

    while(nwrite != 0)
    {
        /* ����Ƿ�ҳ */
        if(pagepos >= STM32FLASH_PAGE_NUM)
        {
            break;
        }

        /* ��ȡһҳ */
        Internal_ReadFlash(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE);

        /* ����Ƿ���Ҫ���� */
        for(i = 0; i < pagefre; i++)
        {
            if(*(FlashBuffer + pageoff + i) != 0xFFFF) /* FLASH������Ĭ������ȫΪ0xFF */
            {
                break;
            }
        }

        if(i < pagefre)
        {
            uint32_t count = 0;
            uint32_t index = 0;
            uint32_t PageError = 0;
            FLASH_EraseInitTypeDef pEraseInit;

            /* ����һҳ */
            pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
            pEraseInit.PageAddress = STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE;
            pEraseInit.Banks = FLASH_BANK_1;
            pEraseInit.NbPages = 1;
            if(HAL_FLASHEx_Erase(&pEraseInit, &PageError) != HAL_OK)
            {
                break;
            }

            /* ���Ƶ����� */
            for(index = 0; index < pagefre; index++)
            {
                *(FlashBuffer + pageoff + index) = *(pBuffer + index);
            }

            /* д��FLASH */
            count = Internal_WriteFlashNoCheck(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE >> 1);
            if(count != (STM32FLASH_PAGE_SIZE >> 1))
            {
                nwrite -= count;
                break;
            }
        }
        else
        {
            /* ���������ֱ��д */
            uint32_t count = Internal_WriteFlashNoCheck(addrStart, pBuffer, pagefre);
            if(count != pagefre)
            {
                nwrite -= count;
                break;
            }
        }

        pBuffer += pagefre;         /* ��ȡ��ַ����         */
        addrStart += (pagefre << 1);  /* д���ַ����         */
        nwrite -= pagefre;          /* ����ʣ��δд�������� */

        pagepos++;     /* ��һҳ           */
        pageoff = 0;   /* ҳ��ƫ�Ƶ�ַ����  */

        /* ����ʣ���������´�д�������� */
        pagefre = nwrite >= (STM32FLASH_PAGE_SIZE >> 1) ? (STM32FLASH_PAGE_SIZE >> 1) : nwrite;
    }

    /* ����FLASH */
    HAL_FLASH_Lock();

    return ((dataLen - nwrite) << 1);
}

/**
 @brief �ڲ�Flashҳ����
 @param pageAddress -[in] ��������ʼ��ַ
 @param nbPages -[in] ����ҳ��
 @return 0 - �ɹ���-1 - ʧ��
*/
int Internal_ErasePage(uint32_t pageAddress, uint32_t nbPages)
{
    uint32_t pageError = 0;
    FLASH_EraseInitTypeDef eraseInit;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = pageAddress;
    eraseInit.Banks = FLASH_BANK_1;
    eraseInit.NbPages = 1;
    if(HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK)
    {
        return -1;
    }
    return 0;
}

#endif
// ��ֲ ����



uint32_t bsp_flash_write(uint8_t buf[],uint32_t size)
{
    uint32_t count = 0;
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
    count =  Internal_WriteFlash(BSP_FLASH_SAVE_START_ADDR, (uint16_t *)(buf), size);
#endif
// ��ֲ ����
    return count;
}

void bsp_flash_read(uint8_t buf[],uint32_t size)
{
    uint32_t addr = BSP_FLASH_SAVE_START_ADDR;
    z_fori(0,size)
    {
        buf[i] = *((volatile uint8_t*)(addr+i));
    }
}

void bsp_flash_init()
{
//	bsp_flash_test();
// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H

#endif
// ��ֲ ����
}

void bsp_flash_test()
{
#define BSP_FLASH_TEST_ARR_NUM 128
    uint8_t flash_test_write_arr[BSP_FLASH_TEST_ARR_NUM] = {0};
    z_fori(0,BSP_FLASH_TEST_ARR_NUM)
    {
        flash_test_write_arr[i] = i;
    }
    uint32_t num = bsp_flash_write(flash_test_write_arr,BSP_FLASH_TEST_ARR_NUM);
    z_printf("write num is %d\r\n",num);
    uint8_t flash_test_read_arr[BSP_FLASH_TEST_ARR_NUM] = {0};
    bsp_flash_read(flash_test_read_arr,BSP_FLASH_TEST_ARR_NUM);
    z_printf("read flash is :\r\n",num);
    z_printf_uint8_arr(flash_test_read_arr,BSP_FLASH_TEST_ARR_NUM);
}

