//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  container/ring_buffer.h
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#ifndef OM_CONTAINER_RING_BUFFER_H
#define OM_CONTAINER_RING_BUFFER_H

#include <vector>
#include <atomic>
#include <cstring>
#include <iostream>

namespace om {
	namespace container {

		template<class T>
		class RingBuffer
		{
		public:
			RingBuffer() = delete;
			RingBuffer(size_t size)
				: 	_size(size), 
					_ring(new std::vector<T>(size)),
					_head(0),
					_tail(0) {}


			void push(T element)
			{
				_ring->operator[](++_head % _size) = element;
			}

			T* front()
			{
				if(_tail < _head) {
					return &(_ring->operator[](_tail % _size));
				}
				return 0;
			}

			inline void pop()
			{
				++_tail;
			}

			inline uint64_t head()
			{
				return _head;
			}

			inline uint64_t tail()
			{
				return _tail;
			}

			size_t occupancy()
			{
				if(_tail < _head)
					return _size - ((_tail + _size) - _head);
				else if(_tail > _head)
					return _size - (_tail - _head);

				return 0;
			}

		private:

			bool _available(uint64_t head, uint64_t tail);

			size_t _size;
			std::vector<T>* _ring;
			std::atomic<uint64_t> _head;
			std::atomic<uint64_t> _tail;
		};
	}
}

#endif
