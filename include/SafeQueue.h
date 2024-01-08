/*
 * @Author: Ziguan Jin 18917950960@163.com
 * @Date: 2024-01-08 12:11:09
 * @LastEditors: Ziguan Jin 18917950960@163.com
 * @LastEditTime: 2024-01-08 12:22:21
 * @FilePath: /thread_pool/SafeQueue.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <mutex>
#include <queue>
#include <sys/errno.h>

template <typename T> class SafeQueue {
public:
  SafeQueue() {}
  SafeQueue(SafeQueue &other) { this->queue_ = std::move(other.queue_); }
  ~SafeQueue() {}

  bool empty() {
    std::unique_lock<std::mutex> lock(this->mutex_);
    return this->queue_.empty();
  }

  int size() {
    std::unique_lock<std::mutex> lock(this->mutex_);
    return this->queue_.size();
  }

  void enqueue(T &t) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->queue_.push(t);
  }

  bool dequeue(T &t) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    if (this->empty()) {
      return false;
    }
    t = std::move(this->queue_.front());
    this->queue_.pop();
    return true;
  }

private:
  std::mutex mutex_;
  std::queue<T> queue_;
};

#endif