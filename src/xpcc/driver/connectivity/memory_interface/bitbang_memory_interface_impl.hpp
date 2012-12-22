

#ifndef XPCC__BITBANG_MEMORY_INTERFACE_HPP
	#error "Don't include this file directly, use 'bitbang_memory_interface.hpp' instead!"
#endif

// ----------------------------------------------------------------------------

template <typename PORT, typename CS, typename CD, typename WR>
PORT xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::port;

template <typename PORT, typename CS, typename CD, typename WR>
CS xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::cs;

template <typename PORT, typename CS, typename CD, typename WR>
CD xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::cd;

template <typename PORT, typename CS, typename CD, typename WR>
WR xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::wr;

template <typename PORT, typename CS, typename CD, typename WR>
void xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::initialize()
{
	cs.setOutput(true);
	cd.setOutput(false);
	wr.setOutput(false);
}

template <typename PORT, typename CS, typename CD, typename WR>
void xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::writeRegister(uint8_t reg)
{
	CD::reset();

	CS::reset();

	WR::reset();
	PORT::write(0);
	xpcc::delay_us(1);
	WR::set();		// Low-to-High strobe
	xpcc::delay_us(1);

	WR::reset();
	PORT::write(reg);
	xpcc::delay_us(1);
	WR::set();		// Low-to-High strobe
	xpcc::delay_us(1);

	CS::set();
}

template <typename PORT, typename CS, typename CD, typename WR>
void xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::writeData(uint16_t data)
{
	CD::set();

	CS::reset();

	WR::reset();
	PORT::write(data>>8);
	xpcc::delay_us(1);
	WR::set();		// Low-to-High strobe
	xpcc::delay_us(1);

	WR::reset();
	PORT::write(data);
	xpcc::delay_us(1);
	WR::set();		// Low-to-High strobe
	xpcc::delay_us(1);

	CS::set();
}

template <typename PORT, typename CS, typename CD, typename WR>
void xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::writeDataMult(uint16_t data, uint16_t count)
{
	CD::set();

	CS::reset();

	for (; count > 0; --count)
	{
		WR::reset();
		PORT::write(data>>8);
		xpcc::delay_us(1);
		WR::set();		// Low-to-High strobe
		xpcc::delay_us(1);

		WR::reset();
		PORT::write(data);
		xpcc::delay_us(1);
		WR::set();		// Low-to-High strobe
		xpcc::delay_us(1);
	}

	CS::set();
}

template <typename PORT, typename CS, typename CD, typename WR>
void xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::writeCommand(uint8_t command, uint16_t data)
{
	writeRegister(command);
	writeData(data);
}


template <typename PORT, typename CS, typename CD, typename WR>
void xpcc::BitbangMemoryInterface<PORT, CS, CD, WR>::writeRam(uint8_t * addr, uint16_t size)
{
	CD::set();

	CS::reset();

	for (; size > 0; --size)
	{
		WR::reset();
		PORT::write(*(addr++));
		xpcc::delay_us(1);
		WR::set();		// Low-to-high strobe
		xpcc::delay_us(1);
	}

	CS::set();
}
