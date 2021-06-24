#include"FileSystem.h"
#include"RootDirectory.h"
#include<iostream>
#include<cassert>
#include<string>

RootDirectory::RootDirectory(void* in_m_start_addr)
{
	m_start_addr = (entry*)in_m_start_addr + int((MBRSIZE + FATSIZE) / 8);
	m_end_addr = m_start_addr + DIRECTORY_ENTRY_NUM;

	assert(m_start_addr);

	index = 0;
}

RootDirectory::~RootDirectory()
{

}

void RootDirectory::filename_show()
{
	using std::cout;
	using std::endl;

	bool is_empty = 1;  // �Ƿ����ļ�
	entry cur_entry;  // 64λ�����Ƽ�¼
	byte block_num;  // ���
	char name_part;

	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // ����Ŀ¼��
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);
		
		if (block_num) {  // ��ָ���0�����ǿ� �����ļ�
			is_empty = 0;
			cur_entry = cur_entry >> 16;  // �������ļ���

			// �ļ������
			for (size_t i = 0; i < NAME_MAX; i++) {  // ��λ���������Ϊ���
				name_part = cur_entry % (1 << 8);
				if (name_part == 0)  // ������  ����ɾ����
					break;

				cout << name_part;
				cur_entry = cur_entry >> 8;
			}
			cout << "  ";
		}
	}

	if (is_empty)  // û���ļ�
		cout << "No File Exits";
	cout << endl;
}

bool RootDirectory::file_open(std::string filename, byte* para)
{
	using std::cout;
	using std::endl;
	using std::string;

	char name_compare[6];  // ƥ���ļ���
	strcpy_s(name_compare, filename.c_str());

	bool is_exist = 1;  // �ļ�filename �Ƿ����
	entry cur_entry;  // 64λ�����Ƽ�¼��ǰ����
	byte block_num;  // ���
	byte file_block_size;  // �ļ�ռ�ÿ���
	char name_part;

	// Ѱ���ļ� �ļ����ڣ������Ϣ
	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // ����Ŀ¼��
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);
		file_block_size = (cur_entry >> 8) % (1 << 8);

		if (block_num) {  // ��ָ���0�����ǿ� �����ļ�
			cur_entry = cur_entry >> 16;  // ��ǰ������ļ���

			// �ļ����Ƚ�
			for (size_t i = 0; i < NAME_MAX; i++) {  // ��λ���������Ϊ���
				name_part = cur_entry % (1 << 8);
				// ��Ϣ�Ƚ�
				if (name_part != name_compare[i]) {// ��Ϣ�����
					is_exist = 0;
					break;
				}
				cur_entry = cur_entry >> 8;
			}

			// ��ǰĿ¼��ƥ��ɹ���д�������Ϣ
			if (is_exist) {
				para[0] = block_num;
				para[1] = file_block_size;
				return is_exist;
			}
		}
		is_exist = 1;  // reset value
	}


	// �ļ������� �������ļ�  
	is_exist = false;
	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // ����Ŀ¼��
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);

		if (block_num == 0) {  // Ŀ¼��Ϊ�� �ҵ�����λ��
			// �޸�Ŀ¼��
			void* cur_addr = m_start_addr + index;  // ��ǰĿ¼����ʼ��ַ
			byte* tmp = (byte*)cur_addr;
			*tmp = 1;  // �޸ĳ�ʼ���Ϊ1, �����ļ�����
			tmp += 1;
			*tmp = 0;  // �޸Ŀ���Ϊ0

			char* name_update = (char*)cur_addr + 2;  // �ļ�����ַ
			for (size_t i = 0; i < 6; i++) {
				name_update[i] = name_compare[i];
			}

			// ���ݽ�����Ϣ
			para[0] = 1;
			para[1] = 0;
			break;
		}	
	}
	return is_exist;
}


void RootDirectory::file_delete(std::string filename)
{
	using std::cout;
	using std::endl;
	using std::string;

	char name_compare[6];  // ƥ���ļ���
	strcpy_s(name_compare, filename.c_str());

	entry cur_entry;  // 64λ�����Ƽ�¼��ǰ����
	bool found = 1;  // �Ƿ�ƥ�䵽Ŀ��Ŀ¼��
	byte block_num;  // ��ʼ���
	char name_part;

	// Ѱ���ļ� ������Ŀ¼��
	for (index = 0; index < DIRECTORY_ENTRY_NUM; index++) {  // ����Ŀ¼��
		cur_entry = m_start_addr[index];
		block_num = cur_entry % (1 << 8);

		if (block_num) {  // ��ָ���0�����ǿ� �����ļ�
			cur_entry = cur_entry >> 16;  // ��ǰ������ļ���

			// �ļ����Ƚ�
			for (size_t i = 0; i < NAME_MAX; i++) {  // ��λ���������Ϊ���
				name_part = cur_entry % (1 << 8);
				// ��Ϣ�Ƚ�
				if (name_part != name_compare[i]) {  // ��Ϣ�����
					found = false;
					break;
				}
				cur_entry = cur_entry >> 8;
			}

			// ��ǰĿ¼��ƥ��ɹ�������Ŀ¼����ϢΪ0
			if (found) {
				m_start_addr[index] = 0;
				return;
			}

		}
		found = true;  // value reset
	}
}
