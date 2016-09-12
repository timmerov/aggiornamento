/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
fifo example.
implementation

allocate a chunk of memory.
divide it up into messages.
create two fifos.
one for empty mess
*/

#include <aggiornamento/aggiornamento.h>
#include <container/fifo.h>
#include <container/message-queue.h>

#include <new>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class MessageQueueImpl : public MessageQueue {
    public:
        MessageQueueImpl() = default;
        MessageQueueImpl(const MessageQueueImpl &) = delete;

        virtual ~MessageQueueImpl() throw() {
            delete[] data_;
        }

        int message_size_ = 0;
        Fifo *empty_fifo_;
        Fifo *message_fifo_;
        char *data_ = nullptr;
    };
}

MessageQueue::MessageQueue() {
}

MessageQueue::~MessageQueue() {
}

/*
master thread creates the message queue.
master thread deletes the double buffer.
*/
MessageQueue *MessageQueue::create(
    int message_size,  // size of one message
    int max_count      // maximum number of messages
) throw() {
    auto impl = new(std::nothrow) MessageQueueImpl;
    impl->message_size_ = message_size;
    impl->empty_fifo_ = Fifo::create(max_count);
    impl->message_fifo_ = Fifo::create(max_count);
    auto message_stride = (message_size + 15) / 16 * 16;
    auto size = message_stride * max_count;
    impl->data_ = new(std::nothrow) char[size];

    // add the messages to the empty fifo.
    auto ptr = impl->data_;
    auto empty = impl->empty_fifo_;
    for (auto n = 0; n < max_count; ++n) {
        empty->put(ptr);
        ptr += message_stride;
    }

    return impl;
}

/*
returns message_size passed to the constructor.
*/
int MessageQueue::getMessageSize() throw() {
    auto impl = (MessageQueueImpl *) this;
    return impl->message_size_;
}

/*
returns max_count passed to the constructor.
*/
int MessageQueue::getMaxCount() throw() {
    auto impl = (MessageQueueImpl *) this;
    auto count = impl->empty_fifo_->getMaxCount();
    return count;
}

/*
add an element to the empty fifo.
returns false if the fifo is full.
*/
bool MessageQueue::putEmpty(
    char *ptr
) throw() {
    auto impl = (MessageQueueImpl *) this;
    auto fifo = impl->empty_fifo_;
    auto result = fifo->put(ptr);
    return result;
}

/*
get an element from the empty fifo.
returns nullptr immediately if the fifo is empty.
*/
char *MessageQueue::getEmpty() throw() {
    auto impl = (MessageQueueImpl *) this;
    auto fifo = impl->empty_fifo_;
    auto ptr = fifo->get();
    return ptr;
}

/*
get an element from the empty fifo.
waits for an element to be pushed if the fifo is empty.
*/
char *MessageQueue::getEmptyWait() throw() {
    auto impl = (MessageQueueImpl *) this;
    auto fifo = impl->empty_fifo_;
    auto ptr = fifo->getWait();
    return ptr;
}

/*
add an element to the message fifo.
returns false if the fifo is full.
*/
bool MessageQueue::putMessage(
    char *ptr
) throw() {
    auto impl = (MessageQueueImpl *) this;
    auto fifo = impl->message_fifo_;
    auto result = fifo->put(ptr);
    return result;
}

/*
get an element from the message fifo.
returns nullptr immediately if the fifo is empty.
*/
char *MessageQueue::getMessage() throw() {
    auto impl = (MessageQueueImpl *) this;
    auto fifo = impl->message_fifo_;
    auto ptr = fifo->get();
    return ptr;
}

/*
get an element from the empty fifo.
waits for an element to be pushed if the fifo is empty.
*/
char *MessageQueue::getMessageWait() throw() {
    auto impl = (MessageQueueImpl *) this;
    auto fifo = impl->message_fifo_;
    auto ptr = fifo->getWait();
    return ptr;
}
