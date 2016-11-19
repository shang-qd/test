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
	// 生产条件
	condition_variable m_cv_p;
	// 消费条件
	condition_variable m_cv_c;
	// 大小
	size_t m_repertory_size;
	// 数据仓库
	list<data_type> m_repertory;

public:
	producer_consumer(size_t n) : m_repertory_size(n)
	{
	}

	// 得到库存的数量
	size_t repertory_size()
	{
		lock_guard<mutex> lck(m_mutex);
		return m_repertory.size();
	}

	// 生产数据
	void producer(data_type item)
	{
		// 保证了只能一个线程在本函数内活跃，单本函数内阻塞的线程可以很多
		unique_lock<mutex> lck(m_mutex);
		// 睡眠条件: 不满足生产条件
		// 唤醒事件：notify_*后 + 并且能得到锁
		// 唤醒条件：锁是解开的和满足生产条件
		m_cv_p.wait(lck, bind([](size_t *size, list<data_type> repertory) -> bool
		{
			return repertory.size() < *size;
		}, &m_repertory_size, m_repertory));
		m_repertory.push_back(item);
		// 此时并不能让别的线程执行，因为锁还没有释放
		m_cv_c.notify_one();
	};

	// 消费数据
	data_type consumer()
	{
		// 保证了只能一个线程在本函数内活跃，单本函数内阻塞的线程可以很多
		unique_lock<mutex> lck(m_mutex);
		// 睡眠条件: 不满足消费条件
		// 唤醒事件：解锁或者notify
		// 唤醒条件：锁是解开的和满足消费条件
		m_cv_c.wait(lck, bind([](list<data_type> li) -> bool{
			return li.size() > 0;
		}, m_repertory));
		data_type res = *m_repertory.begin();
		m_repertory.pop_front();
		// 此时并不能让别的线程执行，因为锁还没有释放
		m_cv_p.notify_one();
		return res;
	};
};

producer_consumer<int> pc(10);

// 生产者线程
void test_producer()
{
	for (int i = 0; i < 20; i++)
	{
		int data = rand();
		pc.producer(data);
		printf("生产数据:%d 仓库数量: %d \n", data,pc.repertory_size());
		this_thread::sleep_for(chrono::seconds(1));
	}
}

// 消费者线程
void test_consumer()
{
	for (int i = 0; i < 20; i++)
	{
		auto data = pc.consumer();// 消费数据();
		printf("消费数据:%d 仓库数量: %d \n", data,pc.repertory_size());
		this_thread::sleep_for(chrono::seconds(2));
	}
}

int _tmain_abc(int argc, _TCHAR* argv[])
{
	thread t1(test_producer);
	thread t2(test_consumer);
	t1.join();
	t2.join();
	printf("结束 \n");
	
	return 0;
}
