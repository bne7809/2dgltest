#pragma once

#include <bitset>

template <typename T, size_t CountPerChunk, size_t NumChunks>
class BlockAllocator {
private:
	static_assert(sizeof(T) >= sizeof(void*), "T too small");
	static_assert(std::is_trivially_copyable_v<T>, "T copy");

	static constexpr size_t Stride = sizeof(T);

	struct Chunk
	{
		std::bitset<CountPerChunk> used;
		alignas(T) unsigned char mem[Stride * CountPerChunk];
	};

	size_t curr;
	Chunk chunks[NumChunks];

	void dealloc(T* m, size_t i)
	{
		curr = i;
		Chunk &c = chunks[i];
		i = (reinterpret_cast<unsigned char*>(m) - c.mem) / Stride;
		c.used[i] = 0;
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
	}
protected:
	BlockAllocator() : curr(0) {}

	T* alloc()
	{
		for(size_t i = 0; i < NumChunks; ++i)
		{
			Chunk &c = chunks[curr];
			if(!c.used.all())
			{
				size_t zero = 0;
				while(c.used[zero]) ++zero;
				c.used[zero] = 1;
				void *ret = c.mem + zero * Stride;;
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
