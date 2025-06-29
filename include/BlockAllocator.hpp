#pragma once

#include <bitset>

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
	std::bitset<NumChunks> should_comp;

	inline void *&getNextPtr(void *ptr)
	{
		return *reinterpret_cast<void**>(ptr);
	}

	void dealloc(T* m)
	{
		size_t chunk_index = (reinterpret_cast<unsigned char*>(m) - chunks[0].mem) / BlockSize;
		curr = chunk_index;
		Chunk &c = chunks[chunk_index];
		size_t i = (reinterpret_cast<unsigned char*>(m) - c.mem) / Stride;
		if(!c.used.test(i)) return;
		c.used.reset(i);
		getNextPtr(m) = c.first;
		c.first = m;
		should_comp.set(chunk_index);
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
				*reinterpret_cast<void**>(off) = reinterpret_cast<void*>(off += Stride);
			getNextPtr(off) = nullptr;
		}
	}

	T* alloc()
	{
		for(size_t i = 0; i < NumChunks; ++i)
		{
			Chunk &c = chunks[curr];
			if(c.first)
			{
				void *ret = c.first;
				c.first = getNextPtr(c.first);
				c.used.set((static_cast<unsigned char*>(ret) - c.mem) / Stride);
				return static_cast<T*>(ret);
			}
			curr = (curr + 1) % NumChunks;
		}
		return nullptr;
	}

	void compact()
	{
		if(should_comp.none())
			return;
		for(size_t i = 0; i < NumChunks; ++i)
		{
			if(should_comp.test(i))
			{
				size_t active = 0;
				size_t last_used = 0;
				bool scattered = false;

				Chunk &c = chunks[i];
				for(size_t j = 0; j < CountPerChunk; ++j)
				{
					if(c.used.test(j))
					{
						if(j != active)
						{
							c.used.reset(j);
							c.used.set(active);
							*reinterpret_cast<T*>(c.mem + active * Stride) = *reinterpret_cast<T*>(c.mem + j * Stride);
						}
						++active;
						if (j - last_used > 1)
							scattered = true;
						last_used = j;
					}
				}

				if(scattered)
				{
					unsigned char *off = c.mem + active * Stride;
					c.first = off;
					for(size_t j = active; j < CountPerChunk - 1; ++j)
						*reinterpret_cast<void**>(off) = reinterpret_cast<void*>(off += Stride);
					getNextPtr(off) = nullptr;
				}

				should_comp.reset(i);
			}
		}
	}

	template <typename F, typename K>
	void update_del_if(F&& f, K&& k)
	{
		for(size_t i = 0; i < NumChunks; ++i)
		{
			Chunk &c = chunks[i];
			size_t count = c.used.count();

			for(size_t j = 0; j < count; ++j)
			{
				T& t = *reinterpret_cast<T*>(c.mem + j * Stride);
				f(t);
				if(k(t))
					dealloc(&t);
			}
		}
	}
};
