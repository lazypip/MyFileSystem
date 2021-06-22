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

	// �Ƿ��������ļ�ռ��
	if (!cur_filename.empty()) {  // �Ѿ����ļ����ڴ�
		cout << "Error, close the current file: " << cur_filename << ", and ty again" <<  endl;
		return;
	}

	// Ŀ¼�����
	byte para[2] = { 0 };
	bool is_exit = m_rootdirectory->file_open(filename, para);
	// ��Ϣд��
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

	if (cur_filename.empty()) {  // û�д򿪵��ļ�
		cout << "ERROR : No File Open" << endl;
		return;
	}

	// �޸ĵ�ǰ�ļ���Ϣ
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

	// ��ǰû���ļ���->���ܶ�ȡ
	if (cur_filename.empty()) {
		cout << "ERROR : No File Open" << endl;
		return;
	}

	// ��ǰ�򿪵��ǿ��ļ�
	if (cur_start_block == 1) {
		cout << cur_filename << " : Empty File" << endl;
		return;
	}


	// ���ļ��ǿ�  ���
	byte* para = new byte[cur_block_size];  // ��¼���
	m_fat->file_read(cur_start_block, cur_block_size, para);

		// ��ȡ�ļ� ���ַ���ʽ���  ���ַ = ������ʼ��ַ + ��� * ��byte��С
	char* p_read = nullptr;  // ��ȡָ��
	for (size_t i = 0; i < cur_block_size; i++) {
		p_read = (char*)m_start_addr + para[i] * BLOCK_SIZE;  // ��ǰ���ַ

		// ���
		for (size_t j = 0; j < BLOCK_SIZE; j++) {
			if (!p_read[j])  // ����'\0'����
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

	// �ļ�û�д�
	if (cur_filename.empty()) {
		cout << "ERROR : No File Open" << endl;
		return;
	}

	// �ļ��Ѵ�  ����д��
	string buffer;
	int buffer_len = 0;
	int block_count = 0;  // ��Ҫ�Ŀ���
	bool if_write = 1;  // �Ƿ��д��

	// ��Ϣ��ȡ
	cout << "Input The Info:  ";
	std::getline(cin, buffer);  // getline����ո񵫲���洢
	buffer_len = buffer.length();
	block_count = int(buffer_len / BLOCK_SIZE) + 1;


	// Ŀ���ļ�Ϊ�� �ҵ� block_count���տ鲢д������
	if (cur_start_block == 1) {
		byte* block_number = new byte[block_count];
		for (int i = 0; i < block_count; i++)
			block_number[i] = 0;

		// ���ط���Ŀ�������
		if_write = m_fat->empty_block(block_count, block_number);  
		if (!if_write) {  // �ռ䲻�� �޷�д��
			cout << "ERROR : No Enough Space To Write" << endl;
			return;
		}

		// ********���Է���Ŀ��**************
		/* for (int i = 0; i < block_count; i++) {
			cout << (int)block_number[i] << " ";
		}
		cout << endl; */
		// ************************************

		// д����Ϣ
		byte* pointer_w = nullptr;
		int buffer_index = 0;
		for (int i = 0; i < block_count; i++) {
			pointer_w = (byte*)m_start_addr + BLOCK_SIZE * block_number[i];  // ���ַ
			
			// ����д��
			for (int j = 0; j < BLOCK_SIZE && buffer_index < buffer_len; buffer_index++, j++) 
				pointer_w[j] = buffer[buffer_index];
		}

		// �޸ĵ�ǰ�ļ���curֵ
		cur_start_block = block_number[0];
		cur_block_size = block_count;
		return;
	}


	// **** for updating ****
	cout << "ERROR : Please Delete The File For Another Addition" << endl;

	// Ŀ���ļ��ǿ�  ׷���ļ�
	// �޸ĵ�ǰ�ļ���curֵ

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

	// �ļ�û�д�
	if (cur_filename.empty()) {
		cout << "ERROR : No File Open" << endl;
		return;
	}

	
	// �ļ��Ѿ���
	// ��Ŀ¼������
	m_rootdirectory->file_delete(cur_filename);

	// FAT���� �����ؿ�������
	// ɾ�����ļ�->���������

	// ɾ���ǿ��ļ�->����FAT���
	if (cur_start_block != 1) {
		// FAT����
		byte* para = new byte[cur_block_size];  // ��������
		m_fat->file_delete(cur_start_block, cur_block_size, para);

		// ����Ϣ���
		byte* d_pointer = nullptr;  // ɾ��ָ��

		for (int i = 0; i < cur_block_size; i++) {
			d_pointer = (byte*)m_start_addr + BLOCK_SIZE * para[i];  // ���ַ

			for (int j = 0; j < BLOCK_SIZE; j++)
				d_pointer[j] = 0;

		}
	}


	// �����ǰ�ļ���Ϣ
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
	bool quit = 1;  // �����˳�

	while (quit) {
		// ����ָ��
		cout << ">> ";
		getline(cin, command);
		for (size_t i = 0; i < 7; i++) {
			if (command == command_list[i]) {
				command_num = i;
				break;
			}
		}
		
		// ѡ��ִ��ָ��
		switch (command_num)
		{
		case 0: {  // ls
			ls();
			break; }
		case 1: { // open
			cout << "Input the filename: ";
			getline(cin, filename);  //  **ע��cin����\n��������  �ɷ��������  getchar���жϵ�һ���ǲ��ǣ����Իص�������
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

