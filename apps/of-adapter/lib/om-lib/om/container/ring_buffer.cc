//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  container/ring_buffer.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include "ring_buffer.h"

template<class T>
bool om::container::RingBuffer<T>::_available(uint64_t head, uint64_t tail)
{
	if(head > tail)
		return (tail + _size) - head > 0;
	else if(tail > head)
		return (tail - head) > _size;
	else // head == tail
		return true;
}
