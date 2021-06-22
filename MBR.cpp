#include"MBR.h"
#include"FileSystem.h"
#include<cassert>
#include<iostream>

MBR::MBR(void* in_m_addr)
{
	m_addr = in_m_addr;
	m_len = 16;
	
	assert(m_addr);  // **注意用法 类包含执行顺序

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
	cout << "mbr初始地址为: " << m_addr << endl
		<< "结束地址为: "  << endl
		<< "共有 " << 1 << "项，每项占 " << MBRSIZE << "byte" << endl;
}
