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

	byte next = m_start_addr[start_block];  // ��¼��һ����ֵ
	for (size_t i = 1; i < block_size; i++) {
		para[i] = next;
		// std::cout << (int)next << std::endl;  test
		next = m_start_addr[next];  // ���һ��ռ�ÿ�ı���ֵΪ1
	}
}

bool FAT::empty_block(int block_count, byte* block_number)
{
	// Ѱ�ҿտ�������
	size_t temp = 0;
	for (size_t index = 27; index < FATSIZE; index++) {
		if (m_start_addr[index] == 0) {  // ���
			block_number[temp] = (byte)index;

			if (temp == block_count - 1)
				break;

			temp++;
		}
	}

	if (block_number[block_count - 1] == 0)   // �޷��ҵ���Ҫ�Ŀ���
		return false;

	// ����ӳ���ϵ  **ע��˴�FAT�߼�**
	for (int i = 0; i <= block_count -2 ; i++) { // ��1��block_count-1������޸�
		m_start_addr[ block_number[i] ] = block_number[i + 1];  // �����һ���������
	}
	m_start_addr[ block_number[block_count - 1] ] = 1;  // ���һ�鸳ֵ

	return true;
}


void FAT::file_delete(byte start_block, byte block_size, byte* para)
{
	file_read(start_block, block_size, para);  // д��para��������

	// FAT�������
	for (int i = 0; i < block_size; i++) {
		m_start_addr[para[i]] = 0;
	}

}
