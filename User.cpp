#include"FileSystem.h"
#include"FAT.h"
#include"MBR.h"
#include"RootDirectory.h"
#include<iostream>
#include<cstdlib>
#include<cassert>
#include<string>

void FileSystem::ls()
{
	m_rootdirectory->filename_show();
}

void FileSystem::open(std::string filename)
{
	using std::cout;
	using std::endl;
	using std::string;

	// 是否有其他文件占用
	if (!cur_filename.empty()) {  // 已经有文件正在打开
		cout << "Error, close the current file: " << cur_filename << ", and ty again" <<  endl;
		return;
	}

	// 目录项操作
	byte para[2] = { 0 };
	bool is_exit = m_rootdirectory->file_open(filename, para);
	// 信息写入
	cur_filename = filename;
	cur_start_block = para[0];
	cur_block_size = para[1];

	if (is_exit) {
		cout << "File Open Succss" << endl;
		return;
	}

	cout << "New File Created" << endl;
	return;
}

void FileSystem::close()
{
	using std::cout;
	using std::endl;
	using std::string;

	if (cur_filename.empty()) {  // 没有打开的文件
		cout << "ERROR : No File Open" << endl;
		return;
	}

	// 修改当前文件信息
	string tmp = cur_filename;
	cur_filename = "";
	cur_start_block = 0;
	cur_block_size = 0;
	if (cur_filename.empty())
		cout << "File: " << tmp << " has been closed " << endl;
	else
		cout << "Close Error" << endl;
}

void FileSystem::read()
{
	using std::cout;
	using std::endl;
	using std::string;

	// 当前没有文件打开->不能读取
	if (cur_filename.empty()) {
		cout << "ERROR : No File Open" << endl;
		return;
	}

	// 当前打开的是空文件
	if (cur_start_block == 1) {
		cout << cur_filename << " : Empty File" << endl;
		return;
	}


	// 打开文件非空  输出
	byte* para = new byte[cur_block_size];  // 记录块号
	m_fat->file_read(cur_start_block, cur_block_size, para);

		// 读取文件 以字符形式输出  块基址 = 磁盘起始地址 + 块号 * 块byte大小
	char* p_read = nullptr;  // 读取指针
	for (size_t i = 0; i < cur_block_size; i++) {
		p_read = (char*)m_start_addr + para[i] * BLOCK_SIZE;  // 当前块基址

		// 输出
		for (size_t j = 0; j < BLOCK_SIZE; j++) {
			if (!p_read[j])  // 读到'\0'结束
				break;
			cout << p_read[j];
		}
	}
	cout << endl;
	return;
}


void FileSystem::write()
{
	using std::cout;
	using std::endl;
	using std::cin;
	using std::string;
	using std::getline;

	// 文件没有打开
	if (cur_filename.empty()) {
		cout << "ERROR : No File Open" << endl;
		return;
	}

	// 文件已打开  进行写入
	string buffer;
	int buffer_len = 0;
	int block_count = 0;  // 需要的块数
	bool if_write = 1;  // 是否可写入

	// 信息获取
	cout << "Input The Info:  ";
	std::getline(cin, buffer);  // getline读入空格但不会存储
	buffer_len = buffer.length();
	block_count = int(buffer_len / BLOCK_SIZE) + 1;


	// 目标文件为空 找到 block_count个空块并写入数据
	if (cur_start_block == 1) {
		byte* block_number = new byte[block_count];
		for (int i = 0; i < block_count; i++)
			block_number[i] = 0;

		// 返回分配的块索引号
		if_write = m_fat->empty_block(block_count, block_number);  
		if (!if_write) {  // 空间不足 无法写入
			cout << "ERROR : No Enough Space To Write" << endl;
			return;
		}

		// ********测试分配的块号**************
		/* for (int i = 0; i < block_count; i++) {
			cout << (int)block_number[i] << " ";
		}
		cout << endl; */
		// ************************************

		// 写入信息
		byte* pointer_w = nullptr;
		int buffer_index = 0;
		for (int i = 0; i < block_count; i++) {
			pointer_w = (byte*)m_start_addr + BLOCK_SIZE * block_number[i];  // 块基址
			
			// 块内写入
			for (int j = 0; j < BLOCK_SIZE && buffer_index < buffer_len; buffer_index++, j++) 
				pointer_w[j] = buffer[buffer_index];
		}

		// 修改当前文件的cur值
		cur_start_block = block_number[0];
		cur_block_size = block_count;
		return;
	}


	// **** for updating ****
	cout << "ERROR : Please Delete The File For Another Addition" << endl;

	// 目标文件非空  追加文件
	// 修改当前文件的cur值

	// **** for updating ****


	return;
}


void FileSystem::file_delete()
{
	using std::cout;
	using std::endl;
	using std::cin;
	using std::string;
	using std::getline;

	// 文件没有打开
	if (cur_filename.empty()) {
		cout << "ERROR : No File Open" << endl;
		return;
	}

	
	// 文件已经打开
	// 将目录项置零
	m_rootdirectory->file_delete(cur_filename);

	// FAT置零 并返回块索引号
	// 删除空文件->无需操作块

	// 删除非空文件->操作FAT与块
	if (cur_start_block != 1) {
		// FAT置零
		byte* para = new byte[cur_block_size];  // 块索引号
		m_fat->file_delete(cur_start_block, cur_block_size, para);

		// 块信息清空
		byte* d_pointer = nullptr;  // 删除指针

		for (int i = 0; i < cur_block_size; i++) {
			d_pointer = (byte*)m_start_addr + BLOCK_SIZE * para[i];  // 块基址

			for (int j = 0; j < BLOCK_SIZE; j++)
				d_pointer[j] = 0;

		}
	}


	// 清除当前文件信息
	string tmp = cur_filename;
	cur_filename = "";
	cur_start_block = 0;
	cur_block_size = 0;
	cout << "File " << tmp << " has beed deleted " << endl;
}


void FileSystem::user_opera()
{
	using std::cin;
	using std::cout;
	using std::endl;
	using std::string;
	using std::getline;

	string command;
	string command_list[8] = { "ls", "open", "close", "read", "write", "delete", "quit", "others" };
	size_t command_num = 7;
	string filename;
	bool quit = 1;  // 控制退出

	while (quit) {
		// 输入指令
		cout << ">> ";
		getline(cin, command);
		for (size_t i = 0; i < 7; i++) {
			if (command == command_list[i]) {
				command_num = i;
				break;
			}
		}
		
		// 选择执行指令
		switch (command_num)
		{
		case 0: {  // ls
			ls();
			break; }
		case 1: { // open
			cout << "Input the filename: ";
			getline(cin, filename);  //  **注意cin保留\n至缓冲区  可分情况设置  getchar先判断第一个是不是，可以回到缓冲区
			while (filename.empty() || filename.length() > 5) {
				cout << ">> ERROR : Please input the correct filename: " ;
				getline(cin, filename);
			}
			open(filename);
			break; }
		case 2: { // close
			close();
			break;
		}
		case 3: { // read
			read();
			break;
		}
		case 4: { // write
			write();
			break;
		}
		case 5: { // delete
			file_delete();
			break;
		}
		case 6: { // quit
			quit = 0;
			break;
		}
		default: {
			if (command.empty()) 
				break;
			cout << "Invalid Command" << endl; }
		}

		command_num = 7;  // reset value
	}

	cout << "Quit Successful" << endl;
}

