#include "stdafx.h"

#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include <list>

using namespace std;

template<class data_type>
class producer_consumer
{
private:
	mutex m_mutex;
	// ��������
	condition_variable m_cv_p;
	// ��������
	condition_variable m_cv_c;
	// ��С
	size_t m_repertory_size;
	// ���ݲֿ�
	list<data_type> m_repertory;

public:
	producer_consumer(size_t n) : m_repertory_size(n)
	{
	}

	// �õ���������
	size_t repertory_size()
	{
		lock_guard<mutex> lck(m_mutex);
		return m_repertory.size();
	}

	// ��������
	void producer(data_type item)
	{
		// ��֤��ֻ��һ���߳��ڱ������ڻ�Ծ�������������������߳̿��Ժܶ�
		unique_lock<mutex> lck(m_mutex);
		// ˯������: ��������������
		// �����¼���notify_*�� + �����ܵõ���
		// �������������ǽ⿪�ĺ�������������
		m_cv_p.wait(lck, bind([](size_t *size, list<data_type> repertory) -> bool
		{
			return repertory.size() < *size;
		}, &m_repertory_size, m_repertory));
		m_repertory.push_back(item);
		// ��ʱ�������ñ���߳�ִ�У���Ϊ����û���ͷ�
		m_cv_c.notify_one();
	};

	// ��������
	data_type consumer()
	{
		// ��֤��ֻ��һ���߳��ڱ������ڻ�Ծ�������������������߳̿��Ժܶ�
		unique_lock<mutex> lck(m_mutex);
		// ˯������: ��������������
		// �����¼�����������notify
		// �������������ǽ⿪�ĺ�������������
		m_cv_c.wait(lck, bind([](list<data_type> li) -> bool{
			return li.size() > 0;
		}, m_repertory));
		data_type res = *m_repertory.begin();
		m_repertory.pop_front();
		// ��ʱ�������ñ���߳�ִ�У���Ϊ����û���ͷ�
		m_cv_p.notify_one();
		return res;
	};
};

producer_consumer<int> pc(10);

// �������߳�
void test_producer()
{
	for (int i = 0; i < 20; i++)
	{
		int data = rand();
		pc.producer(data);
		printf("��������:%d �ֿ�����: %d \n", data,pc.repertory_size());
		this_thread::sleep_for(chrono::seconds(1));
	}
}

// �������߳�
void test_consumer()
{
	for (int i = 0; i < 20; i++)
	{
		auto data = pc.consumer();// ��������();
		printf("��������:%d �ֿ�����: %d \n", data,pc.repertory_size());
		this_thread::sleep_for(chrono::seconds(2));
	}
}

int _tmain_abc(int argc, _TCHAR* argv[])
{
	thread t1(test_producer);
	thread t2(test_consumer);
	t1.join();
	t2.join();
	printf("���� \n");
	
	return 0;
}
