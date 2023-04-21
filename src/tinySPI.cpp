// Arduino tinySPI Library Copyright (C) 2018 by Jack Christensen and
// licensed under GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// Arduino hardware SPI master library for
// ATtiny24/44/84, ATtiny25/45/85, ATtiny261/461/861, ATtiny2313/4313.
//
// https://github.com/JChristensen/tinySPI
// Jack Christensen 24Oct2013

#include <tinySPI.h>

void tinySPI::begin(int8_t MOSI, int8_t MISO, int8_t SCK)
{
    USICR &= ~(_BV(USISIE) | _BV(USIOIE) | _BV(USIWM1));
    USICR |= _BV(USIWM0) | _BV(USICS1) | _BV(USICLK);
    if(SCK >= 0) {
        SPI_DDR_PORT |= _BV(SCK);   // set the USCK pin as output
    }
    if(MOSI >= 0) {
        SPI_DDR_PORT |= _BV(MOSI);     // set the DO pin as output
    }
    if(MISO >= 0) {
        SPI_DDR_PORT &= ~_BV(MISO);    // set the DI pin as input
    }
}

void tinySPI::setDataMode(uint8_t spiDataMode)
{
    if (spiDataMode == SPI_MODE1)
        USICR |= _BV(USICS0);
    else
        USICR &= ~_BV(USICS0);
}

uint8_t tinySPI::transfer(uint8_t spiData)
{
    USIDR = spiData;
    USISR = _BV(USIOIF);                // clear counter and counter overflow interrupt flag
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   // ensure a consistent clock period
    {
        while ( !(USISR & _BV(USIOIF)) ) USICR |= _BV(USITC);
    }
    return USIDR;
}

void tinySPI::end()
{
    USICR &= ~(_BV(USIWM1) | _BV(USIWM0));
}

tinySPI SPI;
