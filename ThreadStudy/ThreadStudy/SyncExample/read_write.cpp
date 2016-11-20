#include "stdafx.h"

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class shared_mutex
{
private:
	// 锁
	mutex m_mutex;
	// 条件变量
	condition_variable m_cond;
	// 是否在写
	bool m_is_w = false;
	// 读者数量
	size_t m_read_c = 0;
	// 读条件
	bool read_cond() const
	{
		return false == m_is_w;
	}
	// 写条件
	bool write_cond() const
	{
		return false == m_is_w && 0 == m_read_c;
	}

public:
	void read()
	{
		unique_lock<mutex> lck(m_mutex);
		// 函数睡眠的时候，如果不是因为锁，如果锁释放了，本函数不会激活
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
		// 匿名函数如果不传指针,当唤醒的时候变量的值依然是睡眠时候的值，
		// 导致函数不能唤醒成功
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

// 下面是测试代码
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

// 多线程总结：
// atomic 在硬件的支持下，为多线程提供了基本的原子操作
// mutex 在atomic支持下为多线程提供了更长的原子尺度（为程序员抽象出来临界区）
// condition_variable 在mutex的支持下为多线提供了更多的控制关系（为程序员抽象出来同步）