#include "stdafx.h"

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class shared_mutex
{
private:
	// ��
	mutex m_mutex;
	// ��������
	condition_variable m_cond;
	// �Ƿ���д
	bool m_is_w = false;
	// ��������
	size_t m_read_c = 0;
	// ������
	bool read_cond() const
	{
		return false == m_is_w;
	}
	// д����
	bool write_cond() const
	{
		return false == m_is_w && 0 == m_read_c;
	}

public:
	void read()
	{
		unique_lock<mutex> lck(m_mutex);
		// ����˯�ߵ�ʱ�����������Ϊ����������ͷ��ˣ����������ἤ��
		m_cond.wait(lck, bind(&shared_mutex::read_cond, this));
		m_read_c++;
	}

	void unread()
	{
		unique_lock<mutex> lck(m_mutex);
		m_read_c--;
		m_cond.notify_all();
	}
	
	void write()
	{
		unique_lock<mutex> lck(m_mutex);
		// ���������������ָ��,�����ѵ�ʱ�������ֵ��Ȼ��˯��ʱ���ֵ��
		// ���º������ܻ��ѳɹ�
		m_cond.wait(lck, bind([](const bool *is_w, const size_t *read_c) -> bool
		{
			return false == *is_w && 0 == *read_c;
		}, &m_is_w, &m_read_c));
		m_is_w = true;
	}

	void unwrite()
	{
		unique_lock<mutex> lck(m_mutex);
		m_is_w = false;
		m_cond.notify_all();
	}
};

class read_lock
{
private:
	shared_mutex *m_sm;
public:
	read_lock(shared_mutex &sm)
	{
		m_sm = &sm;
		m_sm->read();
	}
	~read_lock()
	{
		m_sm->unread();
	}
};

class write_lock
{
private:
	shared_mutex *m_sm;
public:
	write_lock(shared_mutex &sm)
	{
		m_sm = &sm;
		m_sm->write();
	}
	~write_lock()
	{
		m_sm->unwrite();
	}
};

// �����ǲ��Դ���
shared_mutex sm;

void read(int id)
{
	read_lock lck(sm);
	printf("%d r \n",id);
	this_thread::sleep_for(chrono::milliseconds(300));
	printf("%d r end \n",id);
}

void write(int id)
{
	write_lock lck(sm);
	printf("%d w \n", id);
	this_thread::sleep_for(chrono::milliseconds(300));
	printf("%d w end \n", id);
}

int test_read_write()
{
	thread t1(read, 1);
	thread t2(write, 2);
	thread t3(write, 3);
	thread t4(read, 4);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}

// ���߳��ܽ᣺
// atomic ��Ӳ����֧���£�Ϊ���߳��ṩ�˻�����ԭ�Ӳ���
// mutex ��atomic֧����Ϊ���߳��ṩ�˸�����ԭ�ӳ߶ȣ�Ϊ����Ա��������ٽ�����
// condition_variable ��mutex��֧����Ϊ�����ṩ�˸���Ŀ��ƹ�ϵ��Ϊ����Ա�������ͬ����