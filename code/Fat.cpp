#include"FAT.h"
#include"FileSystem.h"
#include<iostream>
#include<cassert>

FAT::FAT(void* in_m_start_addr)
{
	m_start_addr = (byte*)in_m_start_addr;
	m_start_addr += MBRSIZE;

	assert(m_start_addr);

	m_end_addr = (byte*)m_start_addr + FATSIZE;
	index = 0;
}

FAT::~FAT()
{

}

void FAT::file_read(byte start_block, byte block_size, byte* para)
{
	para[0] = start_block;  // start_block >= 27
	if (block_size == 1)
		return;

	byte next = m_start_addr[start_block];  // 记录下一跳的值
	for (size_t i = 1; i < block_size; i++) {
		para[i] = next;
		// std::cout << (int)next << std::endl;  test
		next = m_start_addr[next];  // 最后一个占用块的表项值为1
	}
}

bool FAT::empty_block(int block_count, byte* block_number)
{
	// 寻找空块索引号
	size_t temp = 0;
	for (size_t index = 27; index < FATSIZE; index++) {
		if (m_start_addr[index] == 0) {  // 块空
			block_number[temp] = (byte)index;

			if (temp == block_count - 1)
				break;

			temp++;
		}
	}

	if (block_number[block_count - 1] == 0)   // 无法找到需要的块数
		return false;

	// 建立映射关系  **注意此处FAT逻辑**
	for (int i = 0; i <= block_count -2 ; i++) { // 第1至block_count-1个块的修改
		m_start_addr[ block_number[i] ] = block_number[i + 1];  // 存放下一项的索引号
	}
	m_start_addr[ block_number[block_count - 1] ] = 1;  // 最后一块赋值

	return true;
}


void FAT::file_delete(byte start_block, byte block_size, byte* para)
{
	file_read(start_block, block_size, para);  // 写入para块索引号

	// FAT块号置零
	for (int i = 0; i < block_size; i++) {
		m_start_addr[para[i]] = 0;
	}

}
