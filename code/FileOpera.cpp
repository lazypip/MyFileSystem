#include"FileSystem.h"
#include<iostream>
#include<cstdlib>
#include<string>

FileSystem::FileSystem()
{
		// 参数初始化
		m_start_addr = Disk;
		m_block_size = BLOCK_SIZE;
		m_disk_len = DISK_MAXLEN;
		m_end_addr = (char*)m_start_addr + m_disk_len;

		// 包含类初始化
		m_mbr = new MBR(m_start_addr);
		m_fat = new FAT(m_start_addr);
		m_rootdirectory = new RootDirectory(m_start_addr);

		// 进程信息初始化
		cur_filename = "";
		cur_start_block = 0;
		cur_block_size = 0;
	
}

FileSystem::~FileSystem() 
{
	m_start_addr = nullptr;
	m_end_addr = nullptr;
	
	delete m_mbr;
	delete m_fat;
	delete m_rootdirectory;
}

void FileSystem::test()
{
	using std::cout;
	using std::endl;

	cout << m_start_addr << "   " << m_end_addr << endl;
	// 其他类测试
	m_rootdirectory->test();
	cout << endl;
	m_fat->test();
	cout << endl;
	m_mbr->test();
}
