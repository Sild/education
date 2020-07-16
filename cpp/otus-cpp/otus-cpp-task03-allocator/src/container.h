#pragma once
#include <exception>
#include "allocator.h"
#include <vector>

namespace NCustom {
template<typename T, typename A=std::allocator<T>>
class TLinkedList {
private:
    class TNode;
public:


    template<typename NodeType>
    struct TIterator {
        explicit TIterator(TNode* node): Current(node) {}
        TIterator() = default;
        void operator++() { Current = Current->Next; }
        bool operator!=(const TIterator& rhs) { return Current != rhs.Current; }

        typename std::conditional_t<std::is_const<NodeType>::value, const T&, T&> operator*() { 
		return Current ? Current->Data : throw std::runtime_error("nullpointer call"), Current->Data; 
	}  // nice workflow with unary if. Is it ok?
    private:
        NodeType* Current = nullptr;
    };

    ~TLinkedList() {
        clear();
    }

    template<typename... Args>
    void add(Args&&... args) {
        auto p = Allocator.allocate(1);
        Allocator.construct(p, std::forward<Args>(args)...);
        ++Size;
        if(Last) {
            Last->Next = p;
            Last = Last->Next;
            return;
        }
        Head = Last = p;
    }

    std::size_t size() noexcept {
        return size;
    }

    TIterator<TNode> begin() {
        return TIterator<TNode>{Head};
    }

    TIterator<const TNode> begin() const {
        return TIterator<const TNode>(Head);
    }

    TIterator<TNode> end() {
        return TIterator<TNode*>();
    }

    TIterator<const TNode> end() const {
        return TIterator<const TNode>();
    }

    void clear() noexcept {
        while(Head) {
            auto next = Head->Next;
            Allocator.destroy(Head);
            Allocator.deallocate(Head, 1);
            Head = next;
        }
        Last = nullptr;
    }

    bool empty() noexcept {
        return !Head;
    }
    
private:
    struct TNode {
        explicit TNode(T&& data): Data(std::move(data)) {};
        explicit TNode(T data): Data(std::move(data)) {};
        T Data;
        TNode* Next = nullptr;
    };
    typename A::template rebind<TNode>::other Allocator;
    TNode* Head = nullptr;
    TNode* Last = nullptr;
    size_t Size = 0;

};
}
