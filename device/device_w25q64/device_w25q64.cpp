#include "device_w25q64.h"

#include "z_include.h"

#ifdef USE_DEVICE_W25Q64

// ��ֲ ����
#ifdef __Z_STM32F103C8T6_H
#define DEVICE_W25Q64_00_SCLK_GPIO_PORT	SCLK_00_GPIO_Port
#define DEVICE_W25Q64_00_SCLK_PIN				SCLK_00_Pin
#define DEVICE_W25Q64_00_MOSI_GPIO_PORT	MOSI_00_GPIO_Port
#define DEVICE_W25Q64_00_MOSI_PIN				MOSI_00_Pin
#define DEVICE_W25Q64_00_MISO_GPIO_PORT	MISO_00_GPIO_Port
#define DEVICE_W25Q64_00_MISO_PIN				MISO_00_Pin
#define DEVICE_W25Q64_00_CS_GPIO_PORT		CS_00_GPIO_Port
#define DEVICE_W25Q64_00_CS_PIN					CS_00_Pin
#endif
// ��ֲ ����

#define W25Q64_SIZE       (8 * 1024 * 1024)  // 8MB������
#define BLOCK_SIZE_1KB    (1024)             // 1KB���С
#define PATTERN           (0xAA)             // ���ģʽ
#define SECTOR_SIZE_4KB   (4096)             // 4KB������С

#define FILL_PATTERN     	0xAA    // ���ģʽ
#define PROGRESS_STEP_KB 	64      // ÿ64KB���½���

//W25Q64ָ���1
#define W25Q64_WRITE_ENABLE						          	0x06
#define W25Q64_WRITE_DISABLE                      0x04
#define W25Q64_READ_STATE_REGISTER_1				      0x05
#define W25Q64_READ_STATE_REGISTER_2				      0x35
#define W25Q64_WRITE_STATE_REGISTER				      	0x01
#define W25Q64_PAGE_PROGRAM							      		0x02
#define W25Q64_QUAD_PAGE_PROGRAM				          0x32
#define W25Q64_BLOCK_ERASE_64KB						      	0xD8
#define W25Q64_BLOCK_ERASE_32KB						      	0x52
#define W25Q64_SECTOR_ERASE_4KB						      	0x20
#define W25Q64_CHIP_ERASE							      			0xC7
#define W25Q64_ERASE_SUSPEND					          	0x75
#define W25Q64_ERASE_RESUME							      		0x7A
#define W25Q64_POWER_DOWN							      			0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE				      0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET			    0xFF
#define W25Q64_Release_Power_Down_HPM_Device_ID		0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID				      0x90
#define W25Q64_READ_UUIQUE_ID						      		0x4B
#define W25Q64_JEDEC_ID								      			0x9F


//W25Q64ָ���2(��ָ��)
#define W25Q64_Read_Data						          		0x03
#define W25Q64_Fast_Read						          		0x0B
#define W25Q64_Fast_Read_Dual_Output				    	0x3B
#define W25Q64_Fast_Read_Dual_IO					      	0xBB
#define W25Q64_Fast_Read_Quad_Output				    	0x6B
#define W25Q64_Fast_Read_Quad_IO					      	0xEB
#define W25Q64_Octal_Word_Read_Quad_IO				    0xE3


#define W25Q64_DUMMY_BYTE													0xFF


#define W25Q64_TOTAL_SIZE        0x800000  // 8MB��������
#define W25Q64_PAGE_SIZE         256       // Ӳ��ҳ��С

typedef enum
{
    OP_READ,
    OP_PAGE_WRITE,
    OP_SECTOR_ERASE,
    OP_BLOCK_ERASE,
    OP_CHIP_ERASE
} W25Q64_OperationType;


// ���������ȴ�ʱ�䣨��λms������оƬ�ֲ����ֵ�Ŵ�
const uint32_t g_op_timeout[5] = {
    10,    // OP_READ
    10,    // OP_PAGE_WRITE
    500,   // OP_SECTOR_ERASE
    1000,  // OP_BLOCK_ERASE
    30000  // OP_CHIP_ERASE
};


DEVICE_W25Q64::DEVICE_W25Q64(Z_GPIO_PORT sclk_port,
														 Z_GPIO_PIN  sclk_pin,
														 Z_GPIO_PORT mosi_port,
														 Z_GPIO_PIN mosi_pin,
														 Z_GPIO_PORT miso_port,
														 Z_GPIO_PIN miso_pin,
														 Z_GPIO_PORT cs_port,
														 Z_GPIO_PIN cs_pin)
    : m_sclk_port(sclk_port),
		  m_sclk_pin(sclk_pin),
		  m_mosi_port(mosi_port),
		  m_mosi_pin(mosi_pin),
		  m_miso_port(miso_port),
		  m_miso_pin(miso_pin),
		  m_cs_port(cs_port),
		  m_cs_pin(cs_pin),
device_4spi(sclk_port,sclk_pin,mosi_port,mosi_pin,miso_port,miso_pin,cs_port,cs_pin,Z_LEVEL_LOW,DEVICE_4SPI::SPI4_MODE0,DEVICE_4SPI::SPI4_MSB_FIRST)
{

}

void DEVICE_W25Q64::init(void)
{
	device_4spi.init();
	uint32_t w25a64_id = 0;
	read_id(&w25a64_id);
}

void DEVICE_W25Q64::deinit(void)
{
	device_4spi.deinit();
}


// дʹ��
Z_STATE DEVICE_W25Q64::write_enable(void)
{
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_WRITE_ENABLE);
    device_4spi.release_cs();
		bsp_delay_us(1);
	
    uint64_t start = z_systick_get();
    uint8_t state;
    do
    {
        device_4spi.active_cs();
        device_4spi.transfer_8bit(W25Q64_READ_STATE_REGISTER_1);
        state = device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE);
        device_4spi.release_cs();
        if (z_systick_get() - start > 10)
        {
            z_printf("[ERROR] WriteEnable timeout! state=0x%02X\r\n", state);
            return Z_ERROR;
        }
    }
    while ((state & 0x03) != 0x02);   // �ȴ�WEL��λ

    return Z_OK;  // �����سɹ�/ʧ��״̬
}


// ������֤
Z_STATE DEVICE_W25Q64::verify_erase(uint32_t address, uint32_t size)
{
    uint8_t buf[256] = {0};
    uint32_t last_reported = 0;  // �ϴα���Ľ��Ȱٷֱ�
    uint32_t total_bytes = 0;    // ��У���ֽ���������

    z_printf("Verify Erase 0%%");  // ��ʼ������ʾ

    for (uint32_t offset = 0; offset < size; offset += sizeof(buf))
    {
        // ���㱾��У�����ݿ��С
        uint32_t verify_size = (size - offset > sizeof(buf)) ?
                               sizeof(buf) : size - offset;

        /*----------- ��ȡ���� -----------*/
        if (read_data(address + offset, buf, verify_size) != Z_OK)
        {
            z_printf("\n[ERROR] Read failed @ 0x%06lX\r\n", address + offset); // \n���б���������Ϣ
            return Z_ERROR;
        }

        /*----------- У������ -----------*/
        z_fori (0,verify_size)
        {
            if (buf[i] != 0xFF)
            {
                z_printf("\n[ERROR] 0x%02X @ 0x%06lX\r\n",  // ���б���������Ϣ
                         buf[i], address + offset + i);
                return Z_ERROR;
            }
        }

        /*----------- ���ȼ�������ʾ -----------*/
        total_bytes += verify_size;
        uint32_t progress = total_bytes * 100 / size;  // �����ٷֱ�

        // ���ȱ仯����1% �� ���һ��ʱ������ʾ
        if ((progress > last_reported) || (total_bytes == size))
        {
            z_printf("\rVerify Erase %lu%%", progress); // \r���ǵ�ǰ��
            last_reported = progress;
        }
    }

    z_printf("\rVerify Erase 100%%\n");  // ���ջ���
    return Z_OK;
}



// ��ֹдʹ��
Z_STATE DEVICE_W25Q64::write_disable(void)
{
    // ����д��ֹ����
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_WRITE_DISABLE);
    device_4spi.release_cs();
		bsp_delay_us(1);
	
    // ���WELλ�Ƿ�����
    uint64_t start = z_systick_get();
    uint8_t state;
    do
    {
        device_4spi.active_cs();
        device_4spi.transfer_8bit( W25Q64_READ_STATE_REGISTER_1);
        state = device_4spi.transfer_8bit( W25Q64_DUMMY_BYTE);
        device_4spi.release_cs();

        // ��ʱ��������ʱ��̣ܶ�����10ms��
        if (z_systick_get() - start > 10)
        {
            z_printf("[ERROR] WriteDisable timeout! state=0x%02X\r\n", state);
            return Z_ERROR;
        }
    }
    while (state & 0x03);   // �ȴ�WELλ����

    return Z_OK;
}



// �ȴ� W25Q64 оƬ��æ״̬����
Z_STATE DEVICE_W25Q64::wait_for_busy_state(W25Q64_OperationType op_type)
{
    uint64_t start_tick = z_systick_get();
    const uint32_t timeout = g_op_timeout[op_type];
    uint8_t state = 0;
    do
    {
        device_4spi.active_cs();
        device_4spi.transfer_8bit( W25Q64_READ_STATE_REGISTER_1);
        state = device_4spi.transfer_8bit( W25Q64_DUMMY_BYTE);
        device_4spi.release_cs();

        if (!(state & 0x01))
        {
            return Z_OK; // ����״̬
        }

        // ÿ100ms��ӡ�ȴ���ʾ
        if ((z_systick_get() - start_tick) % 100 == 0)
        {
//            z_printf("Waiting for %s...\r\n",
//                    op_type == OP_CHIP_ERASE ? "Chip Erase" :
//                    op_type == OP_BLOCK_ERASE ? "Block Erase" :
//                    "Operation");
        }
    }
    while ((z_systick_get() - start_tick) < timeout);

    z_printf("Timeout! OpType:%d, Elapsed:%lums\r\n",
             op_type, (uint32_t)(z_systick_get() - start_tick));
    return Z_ERROR;
}

// ���д����
uint8_t DEVICE_W25Q64::read_state_reg1(void)
{
    uint8_t num = 0;
    device_4spi.active_cs();
    device_4spi.transfer_8bit( W25Q64_READ_STATE_REGISTER_1);
    num = device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE);
    device_4spi.release_cs();
    return num;
}



// ��������
Z_STATE DEVICE_W25Q64::sector_erase(uint32_t Sector_Address)
{
    // ���д����
    if(write_enable() != Z_OK)
    {
        return Z_ERROR;
    }

    // ���д����״̬
    uint8_t num = read_state_reg1();
    if(num & 0x1C)
    {
        return Z_ERROR;
    }

    // ִ�в���
    device_4spi.active_cs();
    device_4spi.transfer_8bit( W25Q64_SECTOR_ERASE_4KB);
    device_4spi.transfer_8bit( (Sector_Address >> 16) & 0xFF);
    device_4spi.transfer_8bit( (Sector_Address >> 8) & 0xFF);
    device_4spi.transfer_8bit( Sector_Address & 0xFF);
    device_4spi.release_cs();

    // �ȴ��������
    Z_STATE state = wait_for_busy_state(OP_SECTOR_ERASE);

    if(write_disable() != Z_OK)
    {
        return Z_ERROR;
    }

    return state;
}



// �����
Z_STATE DEVICE_W25Q64::block_erase(uint32_t Block_Address)
{
    if(write_enable() != Z_OK)
    {
        return Z_ERROR;
    }
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_BLOCK_ERASE_64KB);
    device_4spi.transfer_8bit((Block_Address & 0xFF0000) >> 16);
    device_4spi.transfer_8bit((Block_Address & 0xFF00) >> 8);
    device_4spi.transfer_8bit(Block_Address & 0xFF);
    device_4spi.release_cs();
    Z_STATE state = wait_for_busy_state(OP_BLOCK_ERASE);

    if(write_disable() != Z_OK)
    {
        return Z_ERROR;
    }
    return state;
}



// ����оƬ����
Z_STATE DEVICE_W25Q64::chip_erase(void)
{
    if(write_enable() != Z_OK)
    {
        return Z_ERROR;
    }

    // ���д����״̬
    uint8_t num = read_state_reg1();
    if(num & 0x1C)
    {
        return Z_ERROR;
    }

    device_4spi.active_cs();
    device_4spi.transfer_8bit( W25Q64_CHIP_ERASE);
    device_4spi.release_cs();

    Z_STATE state = wait_for_busy_state(OP_CHIP_ERASE);
    if(write_disable() != Z_OK)
    {
        return Z_ERROR;
    }

    return state;
}



// ҳд
Z_STATE DEVICE_W25Q64::page_write(uint32_t address, uint8_t *data, uint16_t data_size)
{
    // ����ҳ��СУ��
    if (data_size > W25Q64_PAGE_SIZE)
    {
        return Z_ERROR;
    }

    if(write_enable() != Z_OK)
    {
        return Z_ERROR;
    }

    device_4spi.active_cs();
    device_4spi.transfer_8bit( W25Q64_PAGE_PROGRAM);
    device_4spi.transfer_8bit( (address >> 16) & 0xFF);
    device_4spi.transfer_8bit( (address >> 8) & 0xFF);
    device_4spi.transfer_8bit( address & 0xFF);
		
		device_4spi.transfer_8bit_bulk(data,NULL,data_size);
//    z_fori (0,data_size)
//    {
//        device_4spi.transfer_8bit( data[i]);
//    }
    device_4spi.release_cs();

    Z_STATE state = wait_for_busy_state(OP_PAGE_WRITE);
    if(write_disable() != Z_OK)
    {
        return Z_ERROR;
    }
    return state;
}



// ���ݶ�ȡ
Z_STATE DEVICE_W25Q64::read_data(uint32_t address, uint8_t *data, uint32_t data_size)
{
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_Read_Data);
    device_4spi.transfer_8bit((address & 0xFF0000) >> 16);
    device_4spi.transfer_8bit((address & 0xFF00) >> 8);
    device_4spi.transfer_8bit(address & 0xFF);
		device_4spi.transfer_8bit_bulk(NULL,data,data_size);
//    z_fori (0,data_size)
//    {
//        data[i] = device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE);
//    }
    Z_STATE state = wait_for_busy_state(OP_READ);
    device_4spi.release_cs();
    return state;
}



// ��ȡID
void DEVICE_W25Q64::read_id(uint32_t *ID)
{
    // 0xef4017
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_JEDEC_ID);         // ��ID��ָ��
    *ID = device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE); // ����ID��Ĭ��Ϊ0xEF
    *ID <<= 8;
    *ID |= device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE); // �豸ID����ʾ�洢���ͣ�Ĭ��Ϊ0x40
    *ID <<= 8;
    *ID |= device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE); // �豸ID����ʾ������Ĭ��Ϊ0x17
    device_4spi.release_cs();
}



// �Զ�����
Z_STATE DEVICE_W25Q64::auto_erase(uint32_t address, uint32_t size)
{
    if (size == 0)
    {
        return Z_ERROR; // ��Ч����
    }

    // ������Ҫ������4KB�������ʼ�ͽ������ַ
    uint32_t start_block_4k = address & ~(0xFFF);
    uint32_t end_block_4k = (address + size - 1) & ~(0xFFF);
    uint32_t current = start_block_4k;

    while (current <= end_block_4k)
    {
        uint32_t block_size;
        uint32_t block_start;

        // ���ȳ���64KB�����
        block_size = 0x10000; // 64KB
        block_start = current & ~(block_size - 1); // ���뵽64KB����ʼ��ַ

        if (block_start <= end_block_4k)
        {
            // ����64KB��
            if (block_erase(block_start) != Z_OK)
            {
                return Z_ERROR;
            }
            current = block_start + block_size; // �ƶ�����һ��
        }
        else
        {
            // �޷���64KB������ʹ��4KB����
            if (sector_erase(current) != Z_OK)
            {
                return Z_ERROR;
            }
            current += 0x1000; // �ƶ�����һ��4KB��
        }
    }
    return Z_OK;
}



Z_STATE DEVICE_W25Q64::write_data(uint32_t address, uint8_t *data, uint32_t size)
{
    /*----------- ����У�� -----------*/
    if (!data || size == 0 ||
            address >= W25Q64_TOTAL_SIZE ||
            (address + size) > W25Q64_TOTAL_SIZE)
    {
        return Z_ERROR;
    }

    /*----------- �Զ����� -----------*/
    if (auto_erase(address, size) != Z_OK)
    {
        return Z_ERROR;
    }

    /*----------- �ֿ�д�� -----------*/
    while (size > 0)
    {
        uint16_t chunk_size;
        uint32_t page_offset = address % W25Q64_PAGE_SIZE;

        // �����ֿ��߼����ϸ���ѭҳ��С���ƣ�
        if (page_offset != 0)
        {
            // ��ǰҳʣ��ռ�
            chunk_size = W25Q64_PAGE_SIZE - page_offset;
            chunk_size = (chunk_size > size) ? size : chunk_size;
        }
        else
        {
            // ��ҳ��ʼʱ���д��256�ֽ�
            chunk_size = (size > W25Q64_PAGE_SIZE) ? W25Q64_PAGE_SIZE : size;
        }

        // ִ��ҳд�루����ת��Ϊuint16_t��
        if (page_write(address, data, (uint16_t)chunk_size) != Z_OK)
        {
            return Z_ERROR;
        }


        // ��֤д�루��ѡ���ϸ�ģʽ���ã�

        static uint8_t verify_buf[256]; // ʹ�þ�̬������
        uint16_t verify_size = (chunk_size > sizeof(verify_buf)) ?
                               sizeof(verify_buf) : chunk_size;

        uint8_t state = read_data(address, verify_buf, verify_size);
        if(state!=Z_OK)
        {
            return Z_ERROR;
        }

        if (memcmp(data, verify_buf, verify_size) != 0)
        {
            return Z_ERROR;
        }

        // ����ָ��
        address += chunk_size;
        data += chunk_size;
        size -= chunk_size;
    }

    return Z_OK;
}



void DEVICE_W25Q64::fill_1k_blocks(void)
{
    uint8_t buffer[1024] = {0};
    uint32_t total_kb = W25Q64_SIZE / 1024;
    uint32_t processed_kb = 0;

    // ��ʼ�����ģʽ
    memset(buffer, FILL_PATTERN, sizeof(buffer));

    // ������������4KB���룩
    for(uint32_t sec_addr = 0; sec_addr < W25Q64_SIZE; sec_addr += 4096)
    {
        // ��������
        if(sector_erase(sec_addr) != Z_OK)
        {
            z_printf("Erase failed at 0x%06lX\r\n", sec_addr);
            return;
        }

        // д��4��1KB��
        for(uint16_t offset = 0; offset < 4096; offset += 1024)
        {
            if(write_data(sec_addr + offset, buffer, 1024) != Z_OK)
            {
                z_printf("Write failed at 0x%06lX\r\n",
                         sec_addr + offset);
                return;
            }

            // ���½���
            if((++processed_kb % PROGRESS_STEP_KB) == 0)
            {
                float progress = (float)processed_kb * 100 / total_kb;
                z_printf("Progress: %.1f%% (Wrote %luKB)\r",
                         progress, processed_kb);
            }
        }
    }
    z_printf("\nFill completed! Total: %luKB\n", processed_kb);
}


void DEVICE_W25Q64::verify_1k_blocks(void)
{
    uint8_t read_buf[1024] = {0};
    uint32_t total_kb = W25Q64_SIZE / 1024;
    uint32_t errors = 0;
    uint32_t verified_kb = 0;

    for(uint32_t addr = 0; addr < W25Q64_SIZE; addr += 1024)
    {
        // ��ȡ���ݿ�
        if(read_data(addr, read_buf, 1024) != Z_OK)
        {
            z_printf("Read failed at 0x%06lX\r\n", addr);
            errors++;
            continue;
        }

        // ����У��
        uint8_t error_flag = 0;
        for(int i = 0; i < 1024; i++)
        {
            if(read_buf[i] != FILL_PATTERN)
            {
                if(!error_flag)   // ÿ����ֻ�����׸�����
                {
                    z_printf("Err@0x%06lX: expect 0x%02X got 0x%02X\n",
                             addr+i, FILL_PATTERN, read_buf[i]);
                }
                error_flag = 1;
            }
        }
        if(error_flag) errors++;

        // ������ʾ
        if((++verified_kb % PROGRESS_STEP_KB) == 0)
        {
            float progress = (float)verified_kb * 100 / total_kb;
            z_printf("Verifying: %.1f%% (%luKB)\r", progress, verified_kb);
        }
    }

    // ���ɱ���
    z_printf("\nVerification complete!\n");
    z_printf("Total blocks: %lu\n", total_kb);
    z_printf("Error blocks: %lu (%.2f%%)\n",
             errors, (float)errors*100/total_kb);
    z_printf("Good blocks:  %lu (%.2f%%)\n",
             total_kb-errors, 100.0 - (float)errors*100/total_kb);
}


void DEVICE_W25Q64::test(void)
{
    // �׶�1������ͨ�Ų���
    uint32_t w25a64_id = 0;
		read_id(&w25a64_id);
    z_printf("[1/5] Chip ID: 0x%06lX %s\r\n",
             w25a64_id, (w25a64_id==0xEF4017)?"OK":"ERROR");
    if(w25a64_id != 0xEF4017) return;

    // �׶�2������������
    z_printf("[2/5] Single sector test...\r\n");
    uint8_t test_data[4] = {0xA5, 0x5A, 0xF0, 0x0F};
    uint8_t read_buf[4] = {0};

    if(sector_erase(0x1000) != Z_OK)
    {
        z_printf("[ERROR] Sector erase failed!\r\n");
        return;
    }

    if(page_write(0x1000, test_data, 4) != Z_OK)
    {
        z_printf("[ERROR] Page write failed!\r\n");
        return;
    }

    if(read_data(0x1000, read_buf, 4) != Z_OK)
    {
        z_printf("[ERROR] Read verify failed!\r\n");
        return;
    }

    z_printf("Write-Read: %02X %02X %02X %02X %s\r\n",
             read_buf[0], read_buf[1], read_buf[2], read_buf[3],
             (memcmp(test_data, read_buf, 4)?"FAIL":"OK"));

    // �׶�3��ȫƬ��������
    z_printf("[3/5] Full chip erase...\r\n");
    if(chip_erase() != Z_OK)
    {
        z_printf("[ERROR] Chip erase failed!\r\n");
        return;
    }

    if(verify_erase(0, 8*1024*1024) != Z_OK)
    {
        z_printf("[ERROR] Chip erase verify failed!\r\n");
        return;
    }

    // �׶�4������д�����
    z_printf("[4/5] Bulk write test...\r\n");
    fill_1k_blocks();

    // �׶�5������У�����
    z_printf("[5/5] Data verification...\r\n");
    verify_1k_blocks();

    z_printf("[TEST] All tests completed!\r\n");

//    while(1)
//    {

//    }
}



// ��ֲ ����
DEVICE_W25Q64 device_w25q64_00(DEVICE_W25Q64_00_SCLK_GPIO_PORT,DEVICE_W25Q64_00_SCLK_PIN,\
													 DEVICE_W25Q64_00_MOSI_GPIO_PORT,DEVICE_W25Q64_00_MOSI_PIN,\
													 DEVICE_W25Q64_00_MISO_GPIO_PORT,DEVICE_W25Q64_00_MISO_PIN,\
													 DEVICE_W25Q64_00_CS_GPIO_PORT,DEVICE_W25Q64_00_CS_PIN\
);
// ��ֲ ����


void device_w25q64_init()
{
// ��ֲ ����
	device_w25q64_00.init();
// ��ֲ ����
}

void device_w25q64_deinit()
{
// ��ֲ ����
	device_w25q64_00.deinit();
// ��ֲ ����
}


#endif


