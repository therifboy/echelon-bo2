#ifndef __BITARRAY_H__
#define __BITARRAY_H__

template<int BIT_COUNT>
struct bitarray
{
	int array[(BIT_COUNT + 31) / 32];

	void setBit(unsigned int pos)
	{
		if (pos < BIT_COUNT)
		{
			array[pos / 32] |= 0x80000000 >> (pos % 32);
		}
	}

	bool testBit(unsigned int pos)
	{
		if (pos < BIT_COUNT)
		{
			return (array[pos / 32] & (0x80000000 >> (pos % 32))) != 0;
		}
		return false;
	}

	void resetBit(unsigned int pos)
	{
		if (pos < BIT_COUNT)
		{
			array[pos / 32] &= ~(0x80000000 >> (pos % 32));
		}
	}
};

#endif /* __BITARRAY_H__ */
