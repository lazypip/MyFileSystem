#include"MBR.h"
#include"FileSystem.h"
#include<cassert>
#include<iostream>

MBR::MBR(void* in_m_addr)
{
	m_addr = in_m_addr;
	m_len = 16;
	
	assert(m_addr);  // **ע���÷� �����ִ��˳��

	// message record
	//
	//


}

MBR::~MBR()
{

}

void MBR::test()
{
	using std::cout;
	using std::endl;
	cout << "mbr��ʼ��ַΪ: " << m_addr << endl
		<< "������ַΪ: "  << endl
		<< "���� " << 1 << "�ÿ��ռ " << MBRSIZE << "byte" << endl;
}
