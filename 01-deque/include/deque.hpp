#ifndef _DEQUE_H
#define _DEQUE_H

#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
#include <iostream>
#include <memory>
#include <cassert>
using namespace std;

/* NOTE: Deque, ArrayDeque, ListDeque Declaration modification is not allowed.
 * Fill in the TODO sections in the following code. */
template <typename T>
class Deque {
public:
    virtual ~Deque() = default;

    /* NOTE: We won't implement push functions that take rvalue references. */
    virtual void push_front(const T&) = 0;
    virtual void push_back(const T&) = 0;

    /* NOTE: Unlike STL implementations which have separate `front` and
       pop_front` functions, we have one unified method for removing an elem. */
    virtual std::optional<T> remove_front() = 0;
    virtual std::optional<T> remove_back() = 0;

    virtual bool empty() = 0;
    virtual size_t size() = 0;

    virtual T& operator[](size_t) = 0;
};

template <typename T>
class ArrayDeque : public Deque<T> {
public:
    ArrayDeque();
    ~ArrayDeque() = default;

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;
    size_t capacity();

    T& operator[](size_t) override;

private:
    std::unique_ptr<T[]> arr;
    size_t front;
    size_t back;
    size_t size_;
    size_t capacity_;

    void resize();
};

template <typename T>
ArrayDeque<T>::ArrayDeque() :
    front{63 /* You can change this */},
    back{0 /* You can change this */},
    size_{0}, capacity_{64} {
    arr = std::make_unique<T[]>(capacity_);
}

template <typename T>
void ArrayDeque<T>::push_front(const T& item) {
    if (back == front) resize();
    arr[front--] = item;
    size_++;
}

template <typename T>
void ArrayDeque<T>::push_back(const T& item) {
    if (back == front) resize();
    arr[back++] = item;
    size_++;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_front() {
    if (front < capacity_ - 1) {
	size_--;
	return arr[++front];
    }
    else if (back) {
	auto temp = arr[back-- - size_--];
	for (auto i = 1; i <= back; i++) arr[i - 1] = arr[i];
	return temp;    
    }
    else return std::nullopt;
}

template <typename T>
std::optional<T> ArrayDeque<T>::remove_back() {
    if (back) {
	size_--;    
	return arr[--back];
    }
    else if (front < capacity_ - 1) {
	auto temp = arr[front++ + size_--];
	for (auto i = capacity_ - 2; i >= front; i--) arr[i + 1] = arr[i];
	return temp;
    }
    else return std::nullopt;
}

template <typename T>
void ArrayDeque<T>::resize() {
    unique_ptr<T[]> a(make_unique<T[]>(capacity_ * 2));
    for (int i = 0; i < back; i++) a[i] = arr[i];
    for (int i = capacity_ - 1; i > front; i--) a[i + capacity_] = arr[i];
    front += capacity_;
    capacity_ *= 2;
    arr = make_unique<T[]>(capacity_);
    for (int i = 0; i < capacity_; i++) arr[i] = a[i];
}

template <typename T>
bool ArrayDeque<T>::empty() {
    if (size_ == 0) return true;
    return false;
}

template <typename T>
size_t ArrayDeque<T>::size() {
    if (size_) return size_;
    return 0;
}

template <typename T>
size_t ArrayDeque<T>::capacity() {
    if (capacity_) return capacity_;
    return 0;
}

template <typename T>
T& ArrayDeque<T>::operator[](size_t idx) {
    if (front + idx < capacity_ - 1) return arr[front + 1 + idx];
    else if (back - size_ + idx >= 0) return arr[back - size_ + idx];
    return *new T{};
}

template<typename T>
struct ListNode {
    std::optional<T> value;
    ListNode* prev;
    ListNode* next;

    ListNode() : value(std::nullopt), prev(this), next(this) {}
    ListNode(const T& t) : value(t), prev(this), next(this) {}

    ListNode(const ListNode&) = delete;
};

template<typename T>
class ListDeque : public Deque<T> {
public:
    ListDeque();
    ~ListDeque();

    void push_front(const T&) override;
    void push_back(const T&) override;

    std::optional<T> remove_front() override;
    std::optional<T> remove_back() override;

    bool empty() override;
    size_t size() override;

    T& operator[](size_t) override;

    size_t size_ = 0;
    ListNode<T>* sentinel = nullptr;
};

template<typename T>
ListDeque<T>::ListDeque() : sentinel(new ListNode<T>{}), size_(0) {}

template<typename T>
void ListDeque<T>::push_front(const T& t) {
    if (++size_ == 1) {
	ListNode<T>* temp = new ListNode<T>(t);
	temp->next = sentinel;
	temp->prev = sentinel;
	sentinel->prev = temp;
	sentinel->next = temp;
    }
    else if (size_ > 0){
    	ListNode<T>* temp = new ListNode<T>(t);
	temp->prev = sentinel;
	temp->next = sentinel->next;
	sentinel->next->prev = temp;
	sentinel->next = temp;
    }
    else sentinel = new ListNode<T>(t);
}

template<typename T>
void ListDeque<T>::push_back(const T& t) {
    if (++size_ == 1) {
	ListNode<T>* temp = new ListNode<T>(t);
	temp->next = sentinel;
	temp->prev = sentinel;
	sentinel->next = temp;
	sentinel->prev = temp;
    }
    else if (size_ > 0) {
    	ListNode<T>* temp = new ListNode<T>(t);
	temp->next = sentinel;
	temp->prev = sentinel->prev;
	sentinel->prev->next = temp;
	sentinel->prev = temp;
    }
    else sentinel = new ListNode<T>(t);
}

template<typename T>
std::optional<T> ListDeque<T>::remove_front() {
    if (size_-- != 0) {
	auto value_ = sentinel->next->value;
	sentinel->next->next->prev = sentinel;
	sentinel->next = sentinel->next->next;
	return value_;
    }
    return std::nullopt;
}

template<typename T>
std::optional<T> ListDeque<T>::remove_back() {
    if (size_-- != 0) {
	auto value_ = sentinel->prev->value;
	sentinel->prev->prev->next = sentinel;
	sentinel->prev = sentinel->prev->prev;
	return value_;
    }
    return std::nullopt;
}

template<typename T>
bool ListDeque<T>::empty() {
    if (size_ == 0) return true;
    return false;
}

template<typename T>
size_t ListDeque<T>::size() {
    if (size_) return size_;
    return 0;
}

template<typename T>
T& ListDeque<T>::operator[](size_t idx) {
    if (idx >= 0 && idx < size_) {
	ListNode<T>* temp = sentinel;
	for (auto i = 0; i <= idx; i++) temp = temp->next;
	return temp->value.value();
    }
    return *new T{};
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListNode<T>& n) {
    if (n.value)
        os << n.value.value();

    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ListDeque<T>& l) {
    auto np = l.sentinel->next;
    while (np != l.sentinel) {
        os << *np << ' ';
        np = np->next;
    }

    return os;
}

template<typename T>
ListDeque<T>::~ListDeque() {
   for (auto i = 0; i < size_; i++){
       sentinel = sentinel->next;
       delete sentinel->prev;
   }
   delete sentinel;
}

#endif // _DEQUE_H
