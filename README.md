# Aggiornamento

This is the code that goes with with (as of yet unreleased) e-book "Multi-threaded Coding Best Practices C++, NSFW"
(Link to the e-book will be provided upon release.)

These examples demonstrate the concepts discussed in the e-book.

The code is NOT production ready. For example, error checking is minimal. Handling errors is left as an exercise for the reader. Methods of implementation have been chosen for simplicity and clarity.

## Pre-requisites

CMake 2.8.11 or later.

## Status

This is a work in progress.

## MIT License

Copyright (c) <year> <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Examples

### Hello, World!

Ensures things are building correctly.

### Thread

Ensures threading works.

### Single Buffer

Demonstrates two implementations of a single buffer.

The first is a trunk. Everyone has access to the trunk which is protected by a mutex. You can only access the trunk when you have exclusive ownership of the mutex.

The second is an airlock. Two threads alternate accessing the airlock. It is protected by two semaphores. One thread opens the airlock and can put things into it and take things out of it. The thread signals the other thread when it's done. The other thread can then safely access the contents of the airlock.

### Double buffer

Two threads exchange data via a double buffer. The example implementation builds on the airlock from the single buffer example by adding a second buffer.

Each thread initially owns one of the buffers. Each thread can put data into their buffer. When they are done they swap buffers. The first thread to swap blocks until the second thread also swaps.

### Fifo

This example uses a queue to pass messages from one thread to another.

The message queue pre-allocates as many messages as the system will need. These messages are initialy inserted into an "empty" fifo. The sender of a message gets an empty message from the "empty" fifo. Fills it. Adds it to the message fifo. The receiver gets a message from the message fifo. Reads it. Returns it to the "empty" fifo.

The fifos are implemented as a fixed size circular array of pointers to buffers. Buffers are appended to the tail. Buffers are removed from the head. The head and the tail are protected by separate mutexes to minimize contention. This eliminates contention between producers and consumers. Producers still contend with other producers. And consumers still contend with other consumers.

The technique can be used to implement a more complex data pipeline involving many threads by adding more fifos.
