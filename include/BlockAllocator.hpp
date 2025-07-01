#pragma once

#include <bitset>
#include <iostream>

template <typename T, size_t CountPerChunk, size_t NumChunks>
class BlockAllocator {
private:
	static_assert(sizeof(T) >= sizeof(void*), "T too small");
	static_assert(std::is_trivially_copyable_v<T>, "T copy");

	static constexpr size_t Stride = sizeof(T);
	static constexpr size_t Total = CountPerChunk * NumChunks;
	static constexpr size_t BlockSize = Stride * CountPerChunk;

	struct Chunk
	{
		void *first;
		alignas(T) unsigned char mem[Stride * CountPerChunk];
		std::bitset<CountPerChunk> used;
	};

	size_t curr;
	Chunk chunks[NumChunks];

	inline void *&getNextPtr(void *ptr)
	{
		return *reinterpret_cast<void**>(ptr);
	}

	void dealloc(T* m, size_t i)
	{
		curr = i;
		Chunk &c = chunks[i];
		i = (reinterpret_cast<unsigned char*>(m) - c.mem) / Stride;
		c.used[i] = 0;
		getNextPtr(m) = c.first;
		c.first = m;
	}

	void compact_chunk(size_t i)
	{
		Chunk &c = chunks[i];

		size_t back = CountPerChunk - 1;

		for(size_t front = 0; front < back; ++front)
		{
			if(!c.used[front])
			{
				while(back > front && !c.used[back])
					--back;

				if(back <= front) break;

				std::memcpy(c.mem + front * Stride, c.mem + back * Stride, Stride);

				c.used[front] = 1;
				c.used[back] = 0;

				--back;
			}
		}

		++back;

		unsigned char *off = c.mem + back * Stride;

		c.first = off;

		for(size_t j = back; j < CountPerChunk - 1; ++j)
		{
			*reinterpret_cast<void**>(off) = reinterpret_cast<void*>(off + Stride);
			off += Stride;
		}

		getNextPtr(off) = nullptr;
	}
protected:
	BlockAllocator() : curr(0)
	{
		for(size_t i = 0; i < NumChunks; ++i)
		{
			Chunk& c = chunks[i];
			unsigned char *off = c.mem;
			c.first = off;
			for(size_t j = 0; j < CountPerChunk - 1; ++j)
			{
				*reinterpret_cast<void**>(off) = reinterpret_cast<void*>(off + Stride);
				off += Stride;
			}
			getNextPtr(off) = nullptr;
			c.used.reset();
		}
	}

	T* alloc()
	{
		for(size_t i = 0; i < NumChunks; ++i)
		{
			Chunk &c = chunks[curr];
			std::cout << c.first << '\n';
			if(c.first)
			{
				void *ret = c.first;
				c.first = getNextPtr(c.first);
				c.used[(static_cast<unsigned char*>(ret) - c.mem) / Stride] = 1;
				return static_cast<T*>(ret);
			}
			curr = (curr + 1) % NumChunks;
		}
		return nullptr;
	}

	
	template <typename F, typename K>
	void update_del_if(F&& f, K&& k)
	{
		for(size_t i = 0; i < NumChunks; ++i)
		{
			Chunk &c = chunks[i];
			size_t count = c.used.count();
			bool comp = false;

			for(size_t j = 0; j < count; ++j)
			{
				T& t = *reinterpret_cast<T*>(c.mem + j * Stride);
				f(t);
				if(k(t))
				{
					comp = true;
					dealloc(&t, i);
				}
			}

			if(comp) compact_chunk(i);
		}
	}
};
