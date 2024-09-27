#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
#include <memory>

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
        Compare less_than;

        class leftistNode {
        public:
            std::allocator<T> alloc;
            T *val;
            int NPL;  // null path length
            leftistNode *lch;
            leftistNode *rch;

            leftistNode(const T &key) {
                val = alloc.allocate(1);
                std::allocator_traits<decltype(alloc)>::construct(alloc, val, key);
                lch = nullptr;
                rch = nullptr;
                NPL = 0;
            }

//            leftistNode(const leftistNode &other) {
//                val = alloc.allocate(1);
//                std::allocator_traits<decltype(alloc)>::construct(alloc, val, other.key());
//                NPL = 0;
//                lch = rch = nullptr;
//            }


            ~leftistNode() {
                std::allocator_traits<decltype(alloc)>::destroy(alloc, val);
                alloc.deallocate(val, 1);

            }


            T &key() {
                return *val;
            }
        };

    private:
        leftistNode *Root;
        int curLength;

    public:
        /**
         * TODO constructors
         */
        priority_queue() {
            curLength = 0;
            Root = nullptr;
        }

        priority_queue(const priority_queue &other) {
            curLength = 0;
            Root = nullptr;
            copyHeap(Root, other.Root);
        }

        /**
         * TODO deconstructor
         */
        void copyHeap(leftistNode *mRoot, leftistNode *other) {
            push(other->key());

            if (other->lch != nullptr)
                copyHeap(mRoot, other->lch);
            if (other->rch != nullptr)
                copyHeap(mRoot, other->rch);


        }

        void clearHeap(leftistNode *cur) {

            if (cur == nullptr)
                return;

            if (cur->lch != nullptr)
                clearHeap(cur->lch);
            if (cur->rch != nullptr)
                clearHeap(cur->rch);

//            std::cout<<"*";
            delete cur;
            cur = nullptr;
        }

        ~priority_queue() {

            curLength = 0;
            clearHeap(Root);


        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other) {
            if (this == &other)
                return *this;

            if (Root != nullptr)
                clearHeap(Root);
            Root= nullptr;
            curLength = 0;

            copyHeap(Root, other.Root);

            return *this;
        }

        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const {
            if (Root == nullptr)
                throw container_is_empty();
            return Root->key();
        }

        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e) {
            curLength++;


            leftistNode *newInsert = new leftistNode(e);
            Root = merge(Root, newInsert);


        }

        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop() {
            if (curLength == 0)
                throw container_is_empty();
            curLength--;
            leftistNode *l = Root->lch, *r = Root->rch;
            delete Root;
            Root = nullptr;
            Root = merge(l, r);
        }

        /**
         * return the number of the elements.
         */
        size_t size() const {
            return curLength;

        }

        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const {
            if (Root == nullptr)
                return true;
            return false;
        }

        /**
         * merge two priority_queues with at least O(logn) complexity.
         * clear the other priority_queue.
         */

        leftistNode *merge(leftistNode *x, leftistNode *y) {
            if (x == nullptr)
                return y;
            if (y == nullptr)
                return x;

            if (less_than(x->key(), y->key())) {
                leftistNode *tmp = x;
                x = y;
                y = tmp;
            }

            x->rch = merge(x->rch, y);
            if (x->lch == nullptr || (x->lch)->NPL < (x->rch)->NPL) {
                leftistNode *tmp = x->lch;
                x->lch = x->rch;
                x->rch = tmp;
            }

            if (x->rch == nullptr || x->lch == nullptr)
                x->NPL = 0;
            else
                x->NPL = std::min((x->lch)->NPL + 1, (x->rch)->NPL + 1);

            return x;
        }

        void merge(priority_queue &other) {
            curLength += other.curLength;
            Root = merge(Root, other.Root);
            other.curLength=0;
            other.Root= nullptr;
        }
    };

}

#endif