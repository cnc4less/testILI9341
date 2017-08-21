/* testILI9341 --- display a seven-segment readout on the ILI9341 LCD    2017-08-15 */

#include <SPI.h>
//#include "CHGUK01.h"

#define LED_PIN   (PA3)  /* P1.16 */

#define MAXPRI    (255)

/* I/O port connections for ILI9341 LCD on SPI */
#define SS_PIN (PA4)
#define DC_PIN (PB0)
#define RST_PIN (PB1)

#define MAXX (240)
#define MAXY (320)

#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDSTATUS   0x09
#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0D
#define ILI9341_RDSIGMODE  0x0E
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_SLPIN   0x10    // Enter sleep mode
#define ILI9341_SLPOUT  0x11    // Exit sleep mode
#define ILI9341_PTLON   0x12    // Enter partial mode
#define ILI9341_NORON   0x13    // Enter normal mode

#define ILI9341_INVOFF  0x20    // Inversion off
#define ILI9341_INVON   0x21    // Inversion on
#define ILI9341_GAMMASET 0x26   // Set gamma curve
#define ILI9341_DISPOFF 0x28    // Display off
#define ILI9341_DISPON  0x29    // Display on

#define ILI9341_CASET   0x2A    // Column address set
#define ILI9341_PASET   0x2B    // Page address set
#define ILI9341_RAMWR   0x2C    // Memory write
#define ILI9341_RGBSET  0x2D    // Colour set
#define ILI9341_RAMRD   0x2E    // Memory read

#define ILI9341_PTLAR   0x30    // Partial area
#define ILI9341_VSCRDEF 0x33    // Vertical scrolling definition
#define ILI9341_TEOFF   0x34    // Tearing effect line off
#define ILI9341_TEON    0x35    // Tearing effect line on
#define ILI9341_MADCTL  0x36    // Memory access control
#define ILI9341_VSCRSADD 0x37   // Vertical scrolling start address
#define ILI9341_IDMOFF  0x38    // Idle mode off
#define ILI9341_IDMON   0x39    // Idle mode on
#define ILI9341_PIXSET  0x3A    // Pixel format set
#define ILI9341_WRMEMC  0x3C    // Write memory continue
#define ILI9341_RDMEMC  0x3E    // Read memory continue

#define ILI9341_SETTSC  0x44    // Set tear scanline
#define ILI9341_GETSCL  0x45    // Get scanline

#define ILI9341_WRDISBV 0x51    // Write display brightness
#define ILI9341_RDDISBV 0x52    // Read display brightness
#define ILI9341_WRCTRLD 0x53    // Write control display
#define ILI9341_RDCTRLD 0x54    // Read control display
#define ILI9341_WRCABC  0x55    // Write content adaptive brightness control
#define ILI9341_RDCABC  0x56    // Read content adaptive brightness control
#define ILI9341_WRCMIN  0x5E    // Write CABC minimum brightness
#define ILI9341_RDCMIN  0x5F    // Read CABC minimum brightness

#define ILI9341_IFMODE  0xB0    // RGB interface signal control
#define ILI9341_FRMCTR1 0xB1    // Frame rate control (normal/full colours)
#define ILI9341_FRMCTR2 0xB2    // Frame rate control (idle/8 colours)
#define ILI9341_FRMCTR3 0xB3    // Frame rate control (partial/full colours)
#define ILI9341_INVCTR  0xB4    // Display inversion control
#define ILI9341_PRCTR   0xB5    // Blanking porch control
#define ILI9341_DISCTRL 0xB6    // Display function control
#define ILI9341_ETMOD   0x87    // Entry mode set
#define ILI9341_BKLIT1  0xB8    // Backlight control 1
#define ILI9341_BKLIT2  0xB9    // Backlight control 2
#define ILI9341_BKLIT3  0xBA    // Backlight control 3
#define ILI9341_BKLIT4  0xBB    // Backlight control 4
#define ILI9341_BKLIT5  0xBC    // Backlight control 5
#define ILI9341_BKLIT7  0xBE    // Backlight control 7
#define ILI9341_BKLIT8  0xBF    // Backlight control 8

#define ILI9341_PWCTR1  0xC0    // Power control 1
#define ILI9341_PWCTR2  0xC1    // Power control 2
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5    // VCOM control 1
#define ILI9341_VMCTR2  0xC7    // VCOM control 2
#define ILI9341_PWRCTRLA 0xCB   // Power Control A
#define ILI9341_PWRCTRLB 0xCF   // Power Control B

#define ILI9341_NVMWR   0xD0    // NV memory write
#define ILI9341_NVMPKEY 0xD1    // NV memory protection key
#define ILI9341_RDNVM   0xD2    // NV memory status read
#define ILI9341_RDID4   0xD3    // Read ID 4
#define ILI9341_RDID1   0xDA    // Read ID 1
#define ILI9341_RDID2   0xDB    // Read ID 2
#define ILI9341_RDID3   0xDC    // Read ID 3
//#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0    // Positive gamma correction
#define ILI9341_GMCTRN1 0xE1    // Negative gamma correction
#define ILI9341_DGAMCTRL1 0xE2  // Digital gamma control 1
#define ILI9341_DGAMCTRL2 0xE3  // Digital gamma control 2
#define ILI9341_DRVTCTRLA 0xE8  // Driver Timing Control A
#define ILI9341_DRVTCTRLAA 0xE9 // Driver Timing Control AA
#define ILI9341_DRVTCTRLB 0xEA  // Driver Timing Control B
#define ILI9341_PWRSEQ  0xED    // Power on sequence control

#define ILI9341_EN3GAMMA   0xF2 // Enable 3 Gamma control
#define ILI9341_IFCTL   0xF6    // Interface control
#define ILI9341_PUMPRAT 0xF7    // Pump ratio control

/*
#define ILI9341_PWCTR6  0xFC

*/

// Color definitions
#define ILI9341_BLACK   0x0000
#define ILI9341_BLUE    0x001F
#define ILI9341_RED     0xF800
#define ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0  
#define ILI9341_WHITE   0xFFFF

// Usual 7-segment names for SAA1064
#define A (1 << 0)
#define B (1 << 1)
#define C (1 << 2)
#define D (1 << 3)
#define E (1 << 4)
#define F (1 << 5)
#define G (1 << 6)
#define DP (1 << 7)

// Additional dot names for HDSP0761
#define H (1 << 8)
#define I (1 << 9)
#define J (1 << 10)
#define K (1 << 11)
#define L (1 << 12)
#define M (1 << 13)

//     H C C I
//     B     D
//     B     D
//     J G G K
//     A     E
//     A     E
//     L F F M

// Table of 14-segment digits 0-9 and A-F
const unsigned int HDSPsegtab[16] = {
  A | B | C | D | E | F | J | K,         // 0
  D | E | I | K | M,                     // 1
  A | C | D | F | G | H | L | M,         // 2
  C | D | E | F | G | H | L,             // 3
  B | D | E | G | H | J | K | M,         // 4
  B | C | E | F | G | H | I | J | L,     // 5
  A | B | C | E | F | G | J,             // 6
  C | D | E | H | I | K | M,             // 7
  A | B | C | D | E | F | G,             // 8
  B | C | D | E | F | G | K,             // 9
  A | B | C | D | E | G | J | K | L | M, // A
  A | B | C | D | E | F | G | H | J | L, // B
  A | B | C | F | I | J | M,             // C
  A | B | C | D | E | F | H | J | K | L, // D
  A | B | C | F | G | H | I | J | L | M, // E
  A | B | C | G | H | I | J | L          // F
};

#if 0
// Table of ordinary seven-segment digits 0-9 for SAA1064
const unsigned char Segtab[10] = {
  A | B | C | D | E | F,     // 0
  D | E,                     // 1
  A | C | D | F | G,         // 2
  C | D | E | F | G,         // 3
  B | D | E | G,             // 4
  B | C | E | F | G,         // 5
  A | B | C | E | F | G,     // 6
  C | D | E,                 // 7
  A | B | C | D | E | F | G, // 8
  B | C | D | E | F | G      // 9
};

unsigned short int Tile[8][8];
#endif

const int DotSpacing = 16;
const int DotSize = 13;
int DotTilt = 1;
unsigned short int DotColr[7][5 * 4];

void setup (void)
{
// int i;
   int x, y;
   int w;
// unsigned short int colr;
// char msg[] = "Hello, world";

   Serial.begin (38400);
   
   Serial.println ("Hello, world\n");
   
   pinMode (LED_PIN, OUTPUT);
   pinMode (PC13, OUTPUT);
  
   ili9341_begin ();
   
   drawRect (0, 0, MAXX, MAXY, ILI9341_BLUE);

// for (y = 0; y < MAXY; y++)
//    for (x = 0; x < MAXX; x++)
//       drawPixel (x, y, 0);
         
// for (i = 0; i < MAXX; i++)
//    drawPixel (i, i, ILI9341_MAGENTA);

// for (x = 0; x < 8; x++)
//    for (y = 0; y < 8; y++)
//       drawPixel (x, y, ILI9341_YELLOW);
         
// delay_centi (50);
   
// for (x = 1; x < 21; x++)
//    for (y = 1; y < 31; y++) {
//       makeTile (  0 + x, ILI9341_RED, Tile);
//       drawTile (x, y, Tile);
//    }
      
// for (i = 0; i < strlen (msg); i++) {
//    makeTile (msg[i], ILI9341_WHITE, Tile);
//    drawTile (i, 0, Tile);
// }
         
// delay_centi (100);
   
#ifdef PORTRAIT
   drawRect (0, 0, DotSpacing * 4 * 5, DotSpacing * 7, ILI9341_BLACK);
#else
   y = 319 - DotSpacing * 4 * 5;
   
   if (y < 0)
      y = 0;
      
   w = DotSpacing * 4 * 5;

   if (w > 320)
      w = 320;

   drawRect (0, y, DotSpacing * 7, w, ILI9341_BLACK);
#endif

   // Initialise dot colour array to black
   for (y = 0; y < 7; y++)
      for (x = 0; x < (4 * 5); x++)
         DotColr[y][x] = ILI9341_BLACK;
}


void loop (void)
{
   int i;
   int r, g, b;
   unsigned char hue;
   unsigned int display;
   //unsigned long int before, after;
   unsigned short int colr;
   
   display = 0xc0de;
   hue = 0;
   
   for (;;) {
      hsvtorgb (&r, &g, &b, hue, MAXPRI, MAXPRI);

      colr = rgbtoili (r, g, b);

      for (i = 0; i < 16; i++) {
         //before = T1TC;
         show_clock (display++, colr);
         //after = T1TC;
         
         //putdec ("show_clock: ", ticks_to_usec (after - before), "us\n");
// 782ms with drawPixel and all dots drawn every time, for 4 digits
//  80ms with drawRect and all dots drawn every time, for 4 digits
//  20ms with drawRect and show_digit holding digit state
//  3 to 14ms with drawRect and show_dot holding dot state
//       delay_centi (5);
      }

      // Blink two LEDs
      if (digitalRead (LED_PIN) == LOW) {
         digitalWrite (PC13, HIGH);
         digitalWrite (LED_PIN, HIGH);
      }
      else {
         digitalWrite (PC13, LOW);
         digitalWrite (LED_PIN, LOW);
      }
         
      hue++;
   }
}


/* show_clock --- display four digits of hex from 16-bit unsigned integer */

void show_clock (const unsigned int display, const unsigned short int colr)
{
#ifdef SHOW_DOT_GRID
  show_dot_grid (0, colr);
  show_dot_grid (1, colr);
  show_dot_grid (2, colr);
  show_dot_grid (3, colr);
#else
  show_digit (0, (display >> 12) & 0x0f, colr);
  show_digit (1, (display >>  8) & 0x0f, colr);
  show_digit (2, (display >>  4) & 0x0f, colr);
  show_digit (3, (display >>  0) & 0x0f, colr);
#endif 
}


/* show_dot_grid --- for debugging, show the full 4x7 grid of dots that form the display */

void show_dot_grid (int pos, const unsigned short int colr)
{
  int i, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 7; j++)
      show_dot ((pos * 5) + i, j, 1, colr);
}


/* show_digit --- display one hex digit in a specified colour */

void show_digit (int pos, const int digit, const unsigned short int colr)
{
//     H C C I    6
//     B     D    5
//     B     D    4
//     J G G K    3
//     A     E    2
//     A     E    1
//     L F F M DP 0
//     0 1 2 3 4
   unsigned int segs;
   static int prevDigit[4] = {-1, -1, -1, -1};
   static int prevColr[4] = {-1, -1, -1, -1};

   // Save time by doing nothing if the digit is unchanged from last time
   if ((prevDigit[pos] == digit) && (prevColr[pos] == colr))
      return;
      
   prevDigit[pos] = digit;
   prevColr[pos] = colr;
   
   pos *= 5;
   segs = HDSPsegtab[digit];

   show_dot (pos + 0, 1, segs & A, colr);
   show_dot (pos + 0, 2, segs & A, colr);
    
   show_dot (pos + 0, 4, segs & B, colr);
   show_dot (pos + 0, 5, segs & B, colr);
    
   show_dot (pos + 1, 6, segs & C, colr);
   show_dot (pos + 2, 6, segs & C, colr);
    
   show_dot (pos + 3, 4, segs & D, colr);
   show_dot (pos + 3, 5, segs & D, colr);
    
   show_dot (pos + 3, 1, segs & E, colr);
   show_dot (pos + 3, 2, segs & E, colr);
    
   show_dot (pos + 1, 0, segs & F, colr);
   show_dot (pos + 2, 0, segs & F, colr);
    
   show_dot (pos + 1, 3, segs & G, colr);
   show_dot (pos + 2, 3, segs & G, colr);
    
   show_dot (pos + 0, 6, segs & H, colr);
   
   show_dot (pos + 3, 6, segs & I, colr);
   
   show_dot (pos + 0, 3, segs & J, colr);
   
   show_dot (pos + 3, 3, segs & K, colr);
   
   show_dot (pos + 0, 0, segs & L, colr);
   
   show_dot (pos + 3, 0, segs & M, colr);
   
   show_dot (pos + 4, 0, segs & DP, colr);
}


/* show_dot --- draw one dot that makes up the grid */

void show_dot (const int dx, const int dy, const int on, unsigned short int colr)
{
#ifdef USE_SLOW_DRAWPIXEL
   int i, j;
#endif
   int x, y;
   
   if (!on)
      colr = ILI9341_BLACK;
      
   if (DotColr[dy][dx] == colr)
      return;
   
   DotColr[dy][dx] = colr;

#ifdef PORTRAIT
   x = dx * DotSpacing;
   y = (6 - dy) * DotSpacing;
#else
   x = ((6 - dy) * DotSpacing);
   y = (319 - DotSpacing) - (dx * DotSpacing) - (dy * DotTilt);
#endif

#ifdef USE_SLOW_DRAWPIXEL
   for (i = 0; i < DotSize; i++)
      for (j = 0; j < DotSize; j++)
         drawPixel (x + i, y + j, colr);
#else
   drawRect (x, y, DotSize, DotSize, colr);
#endif
}


/* setVline --- draw vertical line */

void setVline (unsigned int x, unsigned int y1, unsigned int y2)
{
  unsigned int y;
  
  for (y = y1; y <= y2; y++)
    ;
}


/* clrVline --- draw vertical line */

void clrVline (unsigned int x, unsigned int y1, unsigned int y2)
{
  unsigned int y;
  
  for (y = y1; y <= y2; y++)
    ;
}


/* setHline --- set pixels in a horizontal line */

void setHline (unsigned int x1, unsigned int x2, unsigned int y)
{
  unsigned int x;
  
  for (x = x1; x <= x2; x++)
    ;
}


/* clrHline --- clear pixels in a horizontal line */

void clrHline (unsigned int x1, unsigned int x2, unsigned int y)
{
  unsigned int x;

  if (y < MAXY) {
    for (x = x1; x <= x2; x++)
      ;
  }
}


/* setRect --- set pixels in a (non-filled) rectangle */

void setRect (int x1, int y1, int x2, int y2)
{
  setHline (x1, x2, y1);
  setVline (x2, y1, y2);
  setHline (x1, x2, y2);
  setVline (x1, y1, y2);
}


/* fillRect --- set pixels in a filled rectangle */

void fillRect (int x1, int y1, int x2, int y2, int ec, int fc)
{
  int y;
  
  for (y = y1; y <= y2; y++)
    if (fc == 0)
      clrHline (x1, x2, y);
    else if (fc == 1)
      setHline (x1, x2, y);
  
  if (ec == 1) {
    setHline (x1, x2, y1);
    setVline (x2, y1, y2);
    setHline (x1, x2, y2);
    setVline (x1, y1, y2);
  }
  else if (ec == 0) {
    clrHline (x1, x2, y1);
    clrVline (x2, y1, y2);
    clrHline (x1, x2, y2);
    clrVline (x1, y1, y2);
  }
}


/* ili9341write --- write a byte to the ILI9341 */

void ili9341write (unsigned char cmd, unsigned char val)
{
   volatile int __attribute__ ((unused)) junk;
   
   if (cmd)
      digitalWrite (DC_PIN, LOW);
   else
      digitalWrite (DC_PIN, HIGH);

   digitalWrite (SS_PIN, LOW);
   junk = SPI.transfer (val);
#if 0
   S0SPDR = val;
   
   while ((S0SPSR & 0x80) == 0)
      ;
      
   junk = S0SPDR;
#endif
   digitalWrite (SS_PIN, HIGH);
}

void writecommand (unsigned char cmd)
{
   ili9341write (1, cmd);
}

void writedata (unsigned char val)
{
   ili9341write (0, val);
}

void spiwrite (unsigned char val)
{
   volatile int __attribute__ ((unused)) junk;

   junk = SPI.transfer (val);
#if 0
   S0SPDR = val;
   
   while ((S0SPSR & 0x80) == 0)
      ;
      
   junk = S0SPDR;
#endif
}


/* ili9341_begin --- initialise the ILI9341 LCD controller */

void ili9341_begin (void)
{
  /* Configure I/O pins on LPC2119 */
  pinMode (SS_PIN, OUTPUT);
  pinMode (DC_PIN, OUTPUT);
  pinMode (RST_PIN, OUTPUT);
  
  digitalWrite (SS_PIN, HIGH);
  digitalWrite (DC_PIN, HIGH);
  digitalWrite (RST_PIN, HIGH);

  SPI.begin ();
  SPI.setBitOrder (MSBFIRST);
  SPI.setDataMode (SPI_MODE0);
  SPI.setClockDivider (SPI_CLOCK_DIV2);
  //(0, MSBFIRST, 6, 8);
  delay (20);
  digitalWrite (RST_PIN, LOW);
  delay (20);
  digitalWrite (RST_PIN, HIGH);
  delay (150);
  
  writecommand(0xEF); // Unknown command
  writedata(0x03);
  writedata(0x80);
  writedata(0x02);

  writecommand(ILI9341_PWRCTRLB);   // Power Control B
  writedata(0x00); 
  writedata(0XC1);   // PCEQ Enabled for power saving
  writedata(0X30); 

  writecommand(ILI9341_PWRSEQ);   // Power on sequence control
  writedata(0x64); 
  writedata(0x03); 
  writedata(0X12); 
  writedata(0X81); 
 
  writecommand(ILI9341_DRVTCTRLA);   // Driver Timing Control A
  writedata(0x85);  // Gate driver non-overlap timing control, default + 1 unit
  writedata(0x00);  // EQ and CR timing control
  writedata(0x78);  // pre-charge timing control, default - 2 unit

  writecommand(ILI9341_PWRCTRLA);  // Power Control A
  writedata(0x39); 
  writedata(0x2C); 
  writedata(0x00); 
  writedata(0x34);     // REG_VD, Vcore control, 1.6V
  writedata(0x02);     // VBC, ddvdh control, 5.6V
 
  writecommand(ILI9341_PUMPRAT);   // Pump ratio control
  writedata(0x20);     // DDVDH=2xVCI

  writecommand(ILI9341_DRVTCTRLB); // Driver Timing Control B
  writedata(0x00); 
  writedata(0x00); 
 
  writecommand(ILI9341_PWCTR1);    // Power control 1
  writedata(0x23);                 // VRH[5:0] 4.50V (default 4.60V 0x21)
 
  writecommand(ILI9341_PWCTR2);    // Power control 2
  writedata(0x10);                 // SAP[2:0];BT[3:0]
 
  writecommand(ILI9341_VMCTR1);    // VCM control 
  writedata(0x3e);
  writedata(0x28); 
  
  writecommand(ILI9341_VMCTR2);    // VCM control2 
  writedata(0x86);
 
  writecommand(ILI9341_MADCTL);    // Memory Access Control 
  writedata(0x48);

  writecommand(ILI9341_PIXSET);    // Pixel format set
  writedata(0x55);                 // 16 bits/pixel for both RGB and MCU
  
  writecommand(ILI9341_FRMCTR1);   // Frame rate control
  writedata(0x00);                 // Division ratio (fosc)
  writedata(0x18);                 // Clocks per line 79hz (default 1B: 70Hz)
 
  writecommand(ILI9341_DISCTRL);   // Display Function Control 
  writedata(0x08);                 // (default 0x0A)
  writedata(0x82);
  writedata(0x27);  
 
  writecommand(ILI9341_EN3GAMMA);  // 3Gamma Function Disable
  writedata(0x00);

  writecommand(ILI9341_GAMMASET);  // Gamma curve select
  writedata(0x01);                 // Gamma curve 1 (the only valid one)

  writecommand(ILI9341_GMCTRP1);   // Set Gamma 
  writedata(0x0F); 
  writedata(0x31); 
  writedata(0x2B); 
  writedata(0x0C); 
  writedata(0x0E); 
  writedata(0x08); 
  writedata(0x4E); 
  writedata(0xF1); 
  writedata(0x37); 
  writedata(0x07); 
  writedata(0x10); 
  writedata(0x03); 
  writedata(0x0E); 
  writedata(0x09); 
  writedata(0x00); 
  
  writecommand(ILI9341_GMCTRN1);    // Set Gamma 
  writedata(0x00); 
  writedata(0x0E); 
  writedata(0x14); 
  writedata(0x03); 
  writedata(0x11); 
  writedata(0x07); 
  writedata(0x31); 
  writedata(0xC1); 
  writedata(0x48); 
  writedata(0x08); 
  writedata(0x0F); 
  writedata(0x0C); 
  writedata(0x31); 
  writedata(0x36); 
  writedata(0x0F); 

  writecommand(ILI9341_SLPOUT);    // Exit Sleep 

  delay (120);

  writecommand(ILI9341_DISPON);    // Display on 
}


/* setAddrWindow --- set the address window, the rectangular region we can write into */

void setAddrWindow(const unsigned short int x0, const unsigned short int y0, const unsigned short int x1, const unsigned short int y1)
{
  writecommand(ILI9341_CASET); // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART 
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND

  writecommand(ILI9341_PASET); // Row addr set
  writedata(y0 >> 8);
  writedata(y0 & 0xFF);     // YSTART
  writedata(y1 >> 8);
  writedata(y1 & 0xFF);     // YEND

  writecommand(ILI9341_RAMWR); // Enable writing to RAM
}


void drawPixel(short int x, short int y, unsigned short int color)
{
  setAddrWindow (x, y, x + 1, y + 1);

  digitalWrite (DC_PIN, HIGH);
  digitalWrite (SS_PIN, LOW);

  spiwrite(color >> 8);
  spiwrite(color);

  digitalWrite (SS_PIN, HIGH);
}


/* drawRect --- draw a rectangle in the specified colour */

void drawRect (const int x1, const int y1, const int w, const int h, const unsigned short int colr)
{
   unsigned char hi, lo;
   int i;
   
   hi = colr >> 8;
   lo = colr & 0xff;
   
   setAddrWindow (x1, y1, x1 + w - 1, y1 + h - 1);
   
   digitalWrite (DC_PIN, HIGH);
   digitalWrite (SS_PIN, LOW);

   for (i = 0; i < (h * w); i++) {
      spiwrite (hi);
      spiwrite (lo);
   }

   digitalWrite (SS_PIN, HIGH);
}


#if 0
void makeTile (int asc, unsigned short int c, unsigned short int tile[8][8])
{
   int x, y;
   
   for (y = 0; y < 8; y++) {
      for (x = 0; x < 8; x++) {
         if (TileGlyph[asc][x] & (1 << y))
            tile[y][x] = c;
         else
            tile[y][x] = 0;
      }
   }
}


void drawTile (int x0, int y0, unsigned short int tile[8][8])
{
   int x, y;
   
   x0 *= 8;
   y0 *= 8;
   
   for (y = 0; y < 8; y++)
      for (x = 0; x < 8; x++)
         drawPixel (x0 + x, y0 + y, tile[y][x]);
}
#endif


/* hsvtorgb --- convert HSV colour to RGB */

void hsvtorgb (int *ir, int *ig, int *ib, int ih, int is, int iv)
{
   int i;
   double p, q, t;
   double f;
   double h, s, v;
   double r = 0.0, g = 0.0, b = 0.0;

   h = (double)ih * (360.0 / (double)MAXPRI);
   s = is / (double)MAXPRI;
   v = iv / (double)MAXPRI;

   if (is == 0) {  /* No saturation so make a grey */
      *ir = iv;
      *ig = iv;
      *ib = iv;
   }
   else {
      if (h >= 359.0)
         h = 0.0;

      h /= 60.0;
      i = (int)h;
      f = h - i;

      p = v * (1.0 - s);
      q = v * (1.0 - (s * f));
      t = v * (1.0 - (s * (1.0 - f)));

      switch (i) {
         case 0:
            r = v;
            g = t;
            b = p;
            break;
         case 1:
            r = q;
            g = v;
            b = p;
            break;
         case 2:
            r = p;
            g = v;
            b = t;
            break;
         case 3:
            r = p;
            g = q;
            b = v;
            break;
         case 4:
            r = t;
            g = p;
            b = v;
            break;
         case 5:
            r = v;
            g = p;
            b = q;
            break;
      }

      *ir = r * MAXPRI;
      *ig = g * MAXPRI;
      *ib = b * MAXPRI;
   }
}


/* rgbtoili --- convert RGB colour to packed 16-bit format */

unsigned short int rgbtoili (const int r, const int g, const int b)
{
  // ILI941 uses 5 bit red, 6 bit green, 5 bit blue
  return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
}

