/*
 * @Author: Ziguan Jin 18917950960@163.com
 * @Date: 2024-01-08 12:07:39
 * @LastEditors: Ziguan Jin 18917950960@163.com
 * @LastEditTime: 2024-01-08 12:46:18
 * @FilePath: /thread_pool/ThreadPool.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "SafeQueue.h"

class ThreadPool {
public:
    ThreadPool(const int n_threads): m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false){}

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool&&) = delete;
    
private:
  class ThreadWorker {
  private:
    int m_id;
    ThreadPool *m_pool;

  public:
    ThreadWorker(ThreadPool *pool, const int id) : m_pool(pool), m_id(id) {}

    void operator()() {
      std::function<void()> func;
      bool dequeued;
      while (!m_pool->m_shutdown) {
        {
          std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
          if (m_pool->m_queue.empty()) {
            m_pool->m_conditional_lock.wait(lock);
          }
          dequeued = m_pool->m_queue.dequeue(func);
        }
        if (dequeued) {
          func();
        }
      }
    }
  };

  bool m_shutdown;
  SafeQueue<std::function<void()>> m_queue;
  std::vector<std::thread> m_threads;
  std::mutex m_conditional_mutex;
  std::condition_variable m_conditional_lock;
};

#endif