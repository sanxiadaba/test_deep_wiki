#include "device_w25q64.h"

#include "z_include.h"

#ifdef USE_DEVICE_W25Q64

// 移植 下面
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
// 移植 上面

#define W25Q64_SIZE       (8 * 1024 * 1024)  // 8MB总容量
#define BLOCK_SIZE_1KB    (1024)             // 1KB块大小
#define PATTERN           (0xAA)             // 填充模式
#define SECTOR_SIZE_4KB   (4096)             // 4KB扇区大小

#define FILL_PATTERN     	0xAA    // 填充模式
#define PROGRESS_STEP_KB 	64      // 每64KB更新进度

//W25Q64指令表1
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


//W25Q64指令集表2(读指令)
#define W25Q64_Read_Data						          		0x03
#define W25Q64_Fast_Read						          		0x0B
#define W25Q64_Fast_Read_Dual_Output				    	0x3B
#define W25Q64_Fast_Read_Dual_IO					      	0xBB
#define W25Q64_Fast_Read_Quad_Output				    	0x6B
#define W25Q64_Fast_Read_Quad_IO					      	0xEB
#define W25Q64_Octal_Word_Read_Quad_IO				    0xE3


#define W25Q64_DUMMY_BYTE													0xFF


#define W25Q64_TOTAL_SIZE        0x800000  // 8MB容量定义
#define W25Q64_PAGE_SIZE         256       // 硬件页大小

typedef enum
{
    OP_READ,
    OP_PAGE_WRITE,
    OP_SECTOR_ERASE,
    OP_BLOCK_ERASE,
    OP_CHIP_ERASE
} W25Q64_OperationType;


// 各操作最大等待时间（单位ms，根据芯片手册典型值放大）
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


// 写使能
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
    while ((state & 0x03) != 0x02);   // 等待WEL置位

    return Z_OK;  // 仅返回成功/失败状态
}


// 擦除验证
Z_STATE DEVICE_W25Q64::verify_erase(uint32_t address, uint32_t size)
{
    uint8_t buf[256] = {0};
    uint32_t last_reported = 0;  // 上次报告的进度百分比
    uint32_t total_bytes = 0;    // 已校验字节数计数器

    z_printf("Verify Erase 0%%");  // 初始进度显示

    for (uint32_t offset = 0; offset < size; offset += sizeof(buf))
    {
        // 计算本次校验数据块大小
        uint32_t verify_size = (size - offset > sizeof(buf)) ?
                               sizeof(buf) : size - offset;

        /*----------- 读取数据 -----------*/
        if (read_data(address + offset, buf, verify_size) != Z_OK)
        {
            z_printf("\n[ERROR] Read failed @ 0x%06lX\r\n", address + offset); // \n换行保留错误信息
            return Z_ERROR;
        }

        /*----------- 校验数据 -----------*/
        z_fori (0,verify_size)
        {
            if (buf[i] != 0xFF)
            {
                z_printf("\n[ERROR] 0x%02X @ 0x%06lX\r\n",  // 换行保留错误信息
                         buf[i], address + offset + i);
                return Z_ERROR;
            }
        }

        /*----------- 进度计算与显示 -----------*/
        total_bytes += verify_size;
        uint32_t progress = total_bytes * 100 / size;  // 整数百分比

        // 进度变化超过1% 或 最后一步时更新显示
        if ((progress > last_reported) || (total_bytes == size))
        {
            z_printf("\rVerify Erase %lu%%", progress); // \r覆盖当前行
            last_reported = progress;
        }
    }

    z_printf("\rVerify Erase 100%%\n");  // 最终换行
    return Z_OK;
}



// 禁止写使能
Z_STATE DEVICE_W25Q64::write_disable(void)
{
    // 发送写禁止命令
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_WRITE_DISABLE);
    device_4spi.release_cs();
		bsp_delay_us(1);
	
    // 检查WEL位是否清零
    uint64_t start = z_systick_get();
    uint8_t state;
    do
    {
        device_4spi.active_cs();
        device_4spi.transfer_8bit( W25Q64_READ_STATE_REGISTER_1);
        state = device_4spi.transfer_8bit( W25Q64_DUMMY_BYTE);
        device_4spi.release_cs();

        // 超时处理（典型时间很短，设置10ms）
        if (z_systick_get() - start > 10)
        {
            z_printf("[ERROR] WriteDisable timeout! state=0x%02X\r\n", state);
            return Z_ERROR;
        }
    }
    while (state & 0x03);   // 等待WEL位清零

    return Z_OK;
}



// 等待 W25Q64 芯片的忙状态结束
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
            return Z_OK; // 就绪状态
        }

        // 每100ms打印等待提示
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

// 检查写保护
uint8_t DEVICE_W25Q64::read_state_reg1(void)
{
    uint8_t num = 0;
    device_4spi.active_cs();
    device_4spi.transfer_8bit( W25Q64_READ_STATE_REGISTER_1);
    num = device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE);
    device_4spi.release_cs();
    return num;
}



// 扇区擦除
Z_STATE DEVICE_W25Q64::sector_erase(uint32_t Sector_Address)
{
    // 解除写保护
    if(write_enable() != Z_OK)
    {
        return Z_ERROR;
    }

    // 检查写保护状态
    uint8_t num = read_state_reg1();
    if(num & 0x1C)
    {
        return Z_ERROR;
    }

    // 执行擦除
    device_4spi.active_cs();
    device_4spi.transfer_8bit( W25Q64_SECTOR_ERASE_4KB);
    device_4spi.transfer_8bit( (Sector_Address >> 16) & 0xFF);
    device_4spi.transfer_8bit( (Sector_Address >> 8) & 0xFF);
    device_4spi.transfer_8bit( Sector_Address & 0xFF);
    device_4spi.release_cs();

    // 等待擦除完成
    Z_STATE state = wait_for_busy_state(OP_SECTOR_ERASE);

    if(write_disable() != Z_OK)
    {
        return Z_ERROR;
    }

    return state;
}



// 块擦除
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



// 整个芯片擦除
Z_STATE DEVICE_W25Q64::chip_erase(void)
{
    if(write_enable() != Z_OK)
    {
        return Z_ERROR;
    }

    // 检查写保护状态
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



// 页写
Z_STATE DEVICE_W25Q64::page_write(uint32_t address, uint8_t *data, uint16_t data_size)
{
    // 新增页大小校验
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



// 数据读取
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



// 读取ID
void DEVICE_W25Q64::read_id(uint32_t *ID)
{
    // 0xef4017
    device_4spi.active_cs();
    device_4spi.transfer_8bit(W25Q64_JEDEC_ID);         // 读ID号指令
    *ID = device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE); // 厂商ID，默认为0xEF
    *ID <<= 8;
    *ID |= device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE); // 设备ID，表示存储类型，默认为0x40
    *ID <<= 8;
    *ID |= device_4spi.transfer_8bit(W25Q64_DUMMY_BYTE); // 设备ID，表示容量，默认为0x17
    device_4spi.release_cs();
}



// 自动擦除
Z_STATE DEVICE_W25Q64::auto_erase(uint32_t address, uint32_t size)
{
    if (size == 0)
    {
        return Z_ERROR; // 无效参数
    }

    // 计算需要擦除的4KB对齐的起始和结束块地址
    uint32_t start_block_4k = address & ~(0xFFF);
    uint32_t end_block_4k = (address + size - 1) & ~(0xFFF);
    uint32_t current = start_block_4k;

    while (current <= end_block_4k)
    {
        uint32_t block_size;
        uint32_t block_start;

        // 优先尝试64KB块擦除
        block_size = 0x10000; // 64KB
        block_start = current & ~(block_size - 1); // 对齐到64KB块起始地址

        if (block_start <= end_block_4k)
        {
            // 擦除64KB块
            if (block_erase(block_start) != Z_OK)
            {
                return Z_ERROR;
            }
            current = block_start + block_size; // 移动到下一块
        }
        else
        {
            // 无法用64KB擦除，使用4KB擦除
            if (sector_erase(current) != Z_OK)
            {
                return Z_ERROR;
            }
            current += 0x1000; // 移动到下一个4KB块
        }
    }
    return Z_OK;
}



Z_STATE DEVICE_W25Q64::write_data(uint32_t address, uint8_t *data, uint32_t size)
{
    /*----------- 参数校验 -----------*/
    if (!data || size == 0 ||
            address >= W25Q64_TOTAL_SIZE ||
            (address + size) > W25Q64_TOTAL_SIZE)
    {
        return Z_ERROR;
    }

    /*----------- 自动擦除 -----------*/
    if (auto_erase(address, size) != Z_OK)
    {
        return Z_ERROR;
    }

    /*----------- 分块写入 -----------*/
    while (size > 0)
    {
        uint16_t chunk_size;
        uint32_t page_offset = address % W25Q64_PAGE_SIZE;

        // 修正分块逻辑（严格遵循页大小限制）
        if (page_offset != 0)
        {
            // 当前页剩余空间
            chunk_size = W25Q64_PAGE_SIZE - page_offset;
            chunk_size = (chunk_size > size) ? size : chunk_size;
        }
        else
        {
            // 新页起始时最多写入256字节
            chunk_size = (size > W25Q64_PAGE_SIZE) ? W25Q64_PAGE_SIZE : size;
        }

        // 执行页写入（必须转换为uint16_t）
        if (page_write(address, data, (uint16_t)chunk_size) != Z_OK)
        {
            return Z_ERROR;
        }


        // 验证写入（可选，严格模式启用）

        static uint8_t verify_buf[256]; // 使用静态缓冲区
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

        // 更新指针
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

    // 初始化填充模式
    memset(buffer, FILL_PATTERN, sizeof(buffer));

    // 按扇区操作（4KB对齐）
    for(uint32_t sec_addr = 0; sec_addr < W25Q64_SIZE; sec_addr += 4096)
    {
        // 擦除扇区
        if(sector_erase(sec_addr) != Z_OK)
        {
            z_printf("Erase failed at 0x%06lX\r\n", sec_addr);
            return;
        }

        // 写入4个1KB块
        for(uint16_t offset = 0; offset < 4096; offset += 1024)
        {
            if(write_data(sec_addr + offset, buffer, 1024) != Z_OK)
            {
                z_printf("Write failed at 0x%06lX\r\n",
                         sec_addr + offset);
                return;
            }

            // 更新进度
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
        // 读取数据块
        if(read_data(addr, read_buf, 1024) != Z_OK)
        {
            z_printf("Read failed at 0x%06lX\r\n", addr);
            errors++;
            continue;
        }

        // 数据校验
        uint8_t error_flag = 0;
        for(int i = 0; i < 1024; i++)
        {
            if(read_buf[i] != FILL_PATTERN)
            {
                if(!error_flag)   // 每个块只报告首个错误
                {
                    z_printf("Err@0x%06lX: expect 0x%02X got 0x%02X\n",
                             addr+i, FILL_PATTERN, read_buf[i]);
                }
                error_flag = 1;
            }
        }
        if(error_flag) errors++;

        // 进度显示
        if((++verified_kb % PROGRESS_STEP_KB) == 0)
        {
            float progress = (float)verified_kb * 100 / total_kb;
            z_printf("Verifying: %.1f%% (%luKB)\r", progress, verified_kb);
        }
    }

    // 生成报告
    z_printf("\nVerification complete!\n");
    z_printf("Total blocks: %lu\n", total_kb);
    z_printf("Error blocks: %lu (%.2f%%)\n",
             errors, (float)errors*100/total_kb);
    z_printf("Good blocks:  %lu (%.2f%%)\n",
             total_kb-errors, 100.0 - (float)errors*100/total_kb);
}


void DEVICE_W25Q64::test(void)
{
    // 阶段1：基础通信测试
    uint32_t w25a64_id = 0;
		read_id(&w25a64_id);
    z_printf("[1/5] Chip ID: 0x%06lX %s\r\n",
             w25a64_id, (w25a64_id==0xEF4017)?"OK":"ERROR");
    if(w25a64_id != 0xEF4017) return;

    // 阶段2：单扇区测试
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

    // 阶段3：全片擦除测试
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

    // 阶段4：批量写入测试
    z_printf("[4/5] Bulk write test...\r\n");
    fill_1k_blocks();

    // 阶段5：数据校验测试
    z_printf("[5/5] Data verification...\r\n");
    verify_1k_blocks();

    z_printf("[TEST] All tests completed!\r\n");

//    while(1)
//    {

//    }
}



// 移植 下面
DEVICE_W25Q64 device_w25q64_00(DEVICE_W25Q64_00_SCLK_GPIO_PORT,DEVICE_W25Q64_00_SCLK_PIN,\
													 DEVICE_W25Q64_00_MOSI_GPIO_PORT,DEVICE_W25Q64_00_MOSI_PIN,\
													 DEVICE_W25Q64_00_MISO_GPIO_PORT,DEVICE_W25Q64_00_MISO_PIN,\
													 DEVICE_W25Q64_00_CS_GPIO_PORT,DEVICE_W25Q64_00_CS_PIN\
);
// 移植 上面


void device_w25q64_init()
{
// 移植 下面
	device_w25q64_00.init();
// 移植 上面
}

void device_w25q64_deinit()
{
// 移植 下面
	device_w25q64_00.deinit();
// 移植 上面
}


#endif


