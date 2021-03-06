/* Copyright (c) 2016, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 * ------------------------------------------------------------------------ */

// AVR AT90CAN128 breakout board AL-AVREB_CAN
// https://www.alvidi.de/products/DE/AVR_Entwicklungsboards/avr_modul_avreb_can.php
// Tested with 16MHz external crystal

#ifndef XPCC_AL_AVREB_CAN_HPP
#define XPCC_AL_AVREB_CAN_HPP

#include <xpcc/architecture/platform.hpp>
#include <xpcc/debug/logger.hpp>

using namespace xpcc::at90;

namespace Board
{

using systemClock = xpcc::avr::SystemClock;

// Arduino Footprint
using Led0 = xpcc::GpioInverted<GpioOutputF0>;
using Led1 = xpcc::GpioInverted<GpioOutputF1>;
using Led2 = xpcc::GpioInverted<GpioOutputF2>;
using Led3 = xpcc::GpioInverted<GpioOutputF3>;

using Button = xpcc::GpioUnused;
using Leds = xpcc::SoftwareGpioPort< Led3, Led2, Led1, Led0 >;

// Create an IODeviceWrapper around the Uart Peripheral we want to use
using LoggerDevice = xpcc::IODeviceWrapper< Uart1, xpcc::IOBuffer::BlockIfFull >;

inline void
initialize()
{
	systemClock::enable();

	GpioD2::connect(Uart1::Rx);
	GpioD3::connect(Uart1::Tx);
	Uart1::initialize<systemClock, 38400>();

	// xpcc::Clock initialization
	// Clear Timer on Compare Match Mode
	TCCR0A = (1 << WGM01);
	TIMSK0 = (1 << OCIE0A);
#if F_CPU > 16000000
	// Set and enable output compare A
	OCR0A = F_CPU / (1000ul * 256);
	// Set prescaler 256 and enable timer
	TCCR0A = (1 << CS02);
#elif F_CPU > 2000000
	// Set and enable output compare A
	OCR0A = F_CPU / (1000ul * 64);
	// Set prescaler 64 and enable timer
	TCCR0A = (1 << CS01) | (1 << CS00);
#elif F_CPU > 1000000
	// Set and enable output compare A
	OCR0A = F_CPU / (1000ul * 8);
	// Set prescaler 8 and enable timer
	TCCR0A = (1 << CS01);
#endif

	enableInterrupts();
}

}

using namespace Board;
extern xpcc::IOStream serialStream;

#endif	// XPCC_ARDUINO_UNO_HPP
