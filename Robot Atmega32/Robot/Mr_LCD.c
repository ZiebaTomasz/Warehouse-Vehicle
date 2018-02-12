/*
 * Biblioteka dla LCD Nokia 3310 i innych
 * opartych o sterownik pcd8544.h
 *
 * SunRiver
 *
 * przygotowane dla toolchaina ATMEL 3.4.x
 *
 */

#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "Mr_LCD.h"



static void LCD_Snd    ( byte data, LcdCmdData cd );
static void Delay      ( void );



// --------------------------  bufor cache w SRAM 84*48 bits or 504 bytes
static byte  LcdCache [ LCD_CACHE_SIZE ];
static int   LcdCacheIdx;
static int   LoWaterMark;
static int   HiWaterMark;
static bool  UpdateLcd;


void LCD_Init ( void )
{
    // Pull-up dla pinu reset.
    LCD_PORT |= _BV ( LCD_RST_PIN );

    // Ustawienie pin�w
    LCD_DDR |= _BV( LCD_RST_PIN ) | _BV( LCD_DC_PIN ) | _BV( LCD_CE_PIN ) | _BV( SPI_MOSI_PIN ) | _BV( SPI_CLK_PIN );

    Delay();

    LCD_PORT &= ~( _BV( LCD_RST_PIN ) );
    Delay();
    LCD_PORT |= _BV ( LCD_RST_PIN );

    // W��czenie SPI:
    // Bez przerwa� , MSBit jak pierwszy, Master mode, CPOL->0, CPHA->0, Clk/4

    SPCR = 0x50;

    // Wy�aczenie kontrolera LCD
    LCD_PORT |= _BV( LCD_CE_PIN );

    LCD_Snd( 0x21, LCD_CMD ); //* Rozszerzone rozkazy LCD.
    LCD_Snd( 0xC8, LCD_CMD ); //* Ustawienie LCD Vop (Kontrast).
    LCD_Snd( 0x06, LCD_CMD ); //* Ustawienie stabilizacji Temp.
    LCD_Snd( 0x13, LCD_CMD ); //* LCD tryb bias 1:48.
    LCD_Snd( 0x20, LCD_CMD ); //* LCD Standard Commands,Horizontal addressing mode
    LCD_Snd( 0x0C, LCD_CMD ); //* LCD w tryb normal mode.


    LoWaterMark = LCD_CACHE_SIZE;
    HiWaterMark = 0;

    // Czyszczenie ekranu przed u�yciem
    LCD_Clear();
    LCD_Update();
}

// PCD_Contr
// Ustawia kontrast LCD
// Wartosc  w zakresie 0x00 do 0x7F.

void PCD_Contr ( byte contrast )
{
    LCD_Snd( 0x21, LCD_CMD );

    // Ustawienie poziomu kontrastu
    LCD_Snd( 0x80 | contrast, LCD_CMD );

    // Tryb - horizontal addressing mode.
    LCD_Snd( 0x20, LCD_CMD );
}

// LCD_Clr
// Czyci LCD
void LCD_Clear ( void )
{
	memset(LcdCache,0x00,LCD_CACHE_SIZE);

    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;

    // Ustawienie flagi na true
    UpdateLcd = TRUE;
}

// LCD_GotoXYFont
// Ustawienie kursora z uwzglednieniem bazowej czcionki 5x7
byte LCD_GotoXYFont ( byte x, byte y )
{
    if( x > 14)
        return OUT_OF_BORDER;
    if( y > 6)
        return OUT_OF_BORDER;
    // Kalkulacja indexu.

    LcdCacheIdx = ( x - 1 ) * 6 + ( y - 1 ) * 84;
    return OK;
}

// LCD_Chr
// Wyswietla znaki

byte LCD_Char ( LcdFontSize size, byte ch )
{
    byte i, c;
    byte b1, b2;
    int  tmpIdx;

    if ( LcdCacheIdx < LoWaterMark )
    {

        LoWaterMark = LcdCacheIdx;
    }

    if ( (ch < 0x20) || (ch > 0x7b) )
    {
       // konwersja znaku do wyswietlenia
        ch = 92;
    }

    if ( size == FONT_1X )
    {
        for ( i = 0; i < 5; i++ )
        {
            // kopiowanie tablicy z Flash ROM do LcdCache
            LcdCache[LcdCacheIdx++] = pgm_read_byte(&( FontLookup[ ch - 32 ][ i ] ) ) << 1;
        }
    }
    else if ( size == FONT_2X )
    {
        tmpIdx = LcdCacheIdx - 84;

        if ( tmpIdx < LoWaterMark )
        {
            LoWaterMark = tmpIdx;
        }

        if ( tmpIdx < 0 ) return OUT_OF_BORDER;

        for ( i = 0; i < 5; i++ )
        {
            /* Copy lookup table from Flash ROM to temporary c */
            c = pgm_read_byte(&(FontLookup[ch - 32][i])) << 1;
            // du�y obraz
            // pierwsza cz�sc
            b1 =  (c & 0x01) * 3;
            b1 |= (c & 0x02) * 6;
            b1 |= (c & 0x04) * 12;
            b1 |= (c & 0x08) * 24;

            c >>= 4;
            // druga czesc
            b2 =  (c & 0x01) * 3;
            b2 |= (c & 0x02) * 6;
            b2 |= (c & 0x04) * 12;
            b2 |= (c & 0x08) * 24;

            /* kopiowanie obu czesci do LcdCache */
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx + 82] = b2;
            LcdCache[tmpIdx + 83] = b2;
        }

        // aktualizacja po�ozenia X
        LcdCacheIdx = (LcdCacheIdx + 11) % LCD_CACHE_SIZE;
    }

    if ( LcdCacheIdx > HiWaterMark )
    {
        HiWaterMark = LcdCacheIdx;
    }

    LcdCache[LcdCacheIdx] = 0x00;
    if(LcdCacheIdx == (LCD_CACHE_SIZE - 1) )
    {
        LcdCacheIdx = 0;
        return OK_WITH_WRAP;
    }
    // Inkrementacja indexu
    LcdCacheIdx++;
    return OK;
}

// LCD_Str

byte LCD_String ( LcdFontSize size, byte dataArray[] )
{
    byte tmpIdx=0;
    byte response;
    while( dataArray[ tmpIdx ] != '\0' )
	{
        // wys�anie znaku
		response = LCD_Char( size, dataArray[ tmpIdx ] );
        // OUT_OF_BORDER
        if( response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
		tmpIdx++;
	}
    return OK;
}

// LCD_FStr
// Wyswietla String
// Przyk�ad uzycia :  LcdFStr(FONT_1X, PSTR("Witaj"));

byte LCD_FStr ( LcdFontSize size, const byte *dataPtr )
{
    byte c;
    byte response;
    for ( c = pgm_read_byte( dataPtr ); c; ++dataPtr, c = pgm_read_byte( dataPtr ) )
    {

        response = LCD_Char( size, c );
        if(response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
    }
    return OK;
}

// LCD_Pixel
// Wyswietla pixel o zadanych wsp�rz�dnych X, Y

byte LCD_Pixel ( byte x, byte y, LcdPixelMode mode )
{
    word  index;
    byte  offset;
    byte  data;

    // obliczenie ramek
    if ( x > LCD_X_RES ) return OUT_OF_BORDER;
    if ( y > LCD_Y_RES ) return OUT_OF_BORDER;

    // rekalkulacja ofsetu
    index = ( ( y / 8 ) * 84 ) + x;
    offset  = y - ( ( y / 8 ) * 8 );

    data = LcdCache[ index ];

	// Czyszczenie
    if ( mode == PIXEL_OFF )
    {
        data &= ( ~( 0x01 << offset ) );
    }

    // tryb W�aczony
    else if ( mode == PIXEL_ON )
    {
        data |= ( 0x01 << offset );
    }

    // Tryb Xor
    else if ( mode  == PIXEL_XOR )
    {
        data ^= ( 0x01 << offset );
    }

    // kopiowanie rezultatu do cache
    LcdCache[ index ] = data;

    if ( index < LoWaterMark )
    {
        LoWaterMark = index;
    }

    if ( index > HiWaterMark )
    {
        HiWaterMark = index;
    }
    return OK;
}


// PCD_Line
// Pozwala na rysowanie lini  o zadanych wsp�rz�dnych

byte PCD_Line ( byte x1, byte x2, byte y1, byte y2, LcdPixelMode mode )
{
    int dx, dy, stepx, stepy, fraction;
    byte response;

    dy = y2 - y1;
    dx = x2 - x1;

    if ( dy < 0 )
    {
        dy    = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    // DX negatyw
    if ( dx < 0 )
    {
        dx    = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 1;
    dy <<= 1;

    // rysowanie na pozycji
    response = LCD_Pixel( x1, y1, mode );
    if(response)
        return response;

    // zmiana lub koniec
    if ( dx > dy )
    {
        //frakcja
        fraction = dy - ( dx >> 1);
        while ( x1 != x2 )
        {
            if ( fraction >= 0 )
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            // rysowanie punktu
            response = LCD_Pixel( x1, y1, mode );
            if(response)
                return response;

        }
    }
    else
    {
        //frakcja
        fraction = dx - ( dy >> 1);
        while ( y1 != y2 )
        {
            if ( fraction >= 0 )
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;

            //rysowanie punktu
            response = LCD_Pixel( x1, y1, mode );
            if(response)
                return response;
        }
    }

    //ustawienie flagi

    UpdateLcd = TRUE;
    return OK;
}


// LCD_SBar
// Pozwala na rysowanie s�upka

byte LCD_SBar ( byte baseX, byte baseY, byte height, byte width, LcdPixelMode mode )
{
	byte tmpIdxX,tmpIdxY,tmp;

    byte response;

    // Sprawdzenie ramek
	if ( ( baseX > LCD_X_RES ) || ( baseY > LCD_Y_RES ) ) return OUT_OF_BORDER;

	if ( height > baseY )
		tmp = 0;
	else
		tmp = baseY - height;

    // Rysowanie lini
	for ( tmpIdxY = tmp; tmpIdxY < baseY; tmpIdxY++ )
	{
		for ( tmpIdxX = baseX; tmpIdxX < (baseX + width); tmpIdxX++ )
        {
			response = LCD_Pixel( tmpIdxX, tmpIdxY, mode );
            if(response)
                return response;

        }
	}

    // Ustawienie flagi
	UpdateLcd = TRUE;
    return OK;
}


// LCD_Bars
// Pozwala na rysowanie wielu s�upk�w

byte LCD_Bars ( byte data[], byte numbBars, byte width, byte multiplier )
{
	byte b;
	byte tmpIdx = 0;
    byte response;

	for ( b = 0;  b < numbBars ; b++ )
	{
        // obliczenie ramek (LCD_X_RES)
		if ( tmpIdx > LCD_X_RES ) return OUT_OF_BORDER;

		// kalkulacja osi x
		tmpIdx = ((width + EMPTY_SPACE_BARS) * b) + BAR_X;

		// Rysowanie s�upka
		response = LCD_SBar( tmpIdx, BAR_Y, data[ b ] * multiplier, width, PIXEL_ON);
        if(response == OUT_OF_BORDER)
            return response;
	}

	// Ustawienie flagi na True
	UpdateLcd = TRUE;
    return OK;

}

// LCD_Rect
// Rysuje prostok�t o zadanych parametrach

byte LCD_Rect ( byte x1, byte x2, byte y1, byte y2, LcdPixelMode mode )
{
	byte tmpIdxX,tmpIdxY;
    byte response;

	// Sprawdzenie ramek
	if ( ( x1 > LCD_X_RES ) ||  ( x2 > LCD_X_RES ) || ( y1 > LCD_Y_RES ) || ( y2 > LCD_Y_RES ) )
		// jesli osiagnieto ramke -- powr�t
		return OUT_OF_BORDER;

	if ( ( x2 > x1 ) && ( y2 > y1 ) )
	{
		for ( tmpIdxY = y1; tmpIdxY < y2; tmpIdxY++ )
		{
			// Rysowanie lini poziomej
			for ( tmpIdxX = x1; tmpIdxX < x2; tmpIdxX++ )
            {
				// rysowanie pixeli
				response = LCD_Pixel( tmpIdxX, tmpIdxY, mode );
                if(response)
                    return response;
            }
		}

		// ustawienie flagi
		UpdateLcd = TRUE;
	}
    return OK;
}

// LCD_Img
// Wyswietla bitmape

void LCD_Img ( const byte *imageData )
{

    memcpy_P(LcdCache,imageData,LCD_CACHE_SIZE);

    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;

    UpdateLcd = TRUE;
}

// LCD_Upd
// Kopiuje zawartosc cache do pamieci RAM wyswietlacza

void LCD_Update ( void )
{
    int i;

    if ( LoWaterMark < 0 )
        LoWaterMark = 0;
    else if ( LoWaterMark >= LCD_CACHE_SIZE )
        LoWaterMark = LCD_CACHE_SIZE - 1;

    if ( HiWaterMark < 0 )
        HiWaterMark = 0;
    else if ( HiWaterMark >= LCD_CACHE_SIZE )
        HiWaterMark = LCD_CACHE_SIZE - 1;

    LCD_Snd( 0x80 | ( LoWaterMark % LCD_X_RES ), LCD_CMD );
    LCD_Snd( 0x40 | ( LoWaterMark / LCD_X_RES ), LCD_CMD );

    for ( i = LoWaterMark; i <= HiWaterMark; i++ )
    {
        LCD_Snd( LcdCache[ i ], LCD_DATA );
    }

    LoWaterMark = LCD_CACHE_SIZE - 1;
    HiWaterMark = 0;

	UpdateLcd = FALSE;
}


// LCD_Snd
// Wysy�a dane do wyswietlacza

static void LCD_Snd ( byte data, LcdCmdData cd )
{
    // W�aczenie kontrolera - aktywny przy LOW
    LCD_PORT &= ~( _BV( LCD_CE_PIN ) );

    if ( cd == LCD_DATA )
    {
        LCD_PORT |= _BV( LCD_DC_PIN );
    }
    else
    {
        LCD_PORT &= ~( _BV( LCD_DC_PIN ) );
    }

    // Wys�anie danych do kontrolera
    SPDR = data;

    // Oczekiwanie na koniec
    while ( (SPSR & 0x80) != 0x80 );

    // wy�aczenie kontrolera LCD
    LCD_PORT |= _BV( LCD_CE_PIN );
}

// LCD_DrwBrd
// Rysuje ramke dooko�a ekranu
/*
void LCD_DrwBrd ( void )
{
  unsigned char i, j;

  for(i=0; i<7; i++)
  {
    LCD_gotoXY (0,i);

	for(j=0; j<84; j++)
	{
	  if(j == 0 || j == 83)
		LCD_writeData (0xff);		// first and last column solid fill to make line
	  else if(i == 0)
	    LCD_writeData (0x08);		// row 0 is having only 5 bits (not 8)
	  else if(i == 6)
	    LCD_writeData (0x04);		// row 6 is having only 3 bits (not 8)
	  else
	    LCD_writeData (0x00);
	}
  }
}
*/
// Delay
// konieczne op�nienie dla LCD
static void Delay ( void )
{
    int i;

    for ( i = -32000; i < 32000; i++ );
}
