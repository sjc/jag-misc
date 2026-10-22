//
// draw.c
//

#include "draw.h"
#include "font.h"

unsigned char *jagscreen;

void SetScreenAddress(unsigned char *ptr) {

    jagscreen = ptr;

    long w;
    for(w=0;w<64000;w++) jagscreen[w]=0;
}

void DrawCharLine (int charloc, int screenloc)
{
  static unsigned char shift[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
  int xcnt;

  for (xcnt = 0; xcnt < F_WIDTH; xcnt++) 
  {
    if (((textfont[charloc] >> shift[xcnt]) & 0x1)) 
      jagscreen[screenloc++] = 1;
    else
      jagscreen[screenloc++] = 0;
  }
}

void DrawChar (int x, int y, char ch)
{
  int ycnt, charloc, screenloc;

  charloc = ch * F_CHARSIZE;

  screenloc = (y * T_XREZ) + x ;

  for (ycnt = 0; ycnt < F_HEIGHT; ycnt++) 
  {
    DrawCharLine (charloc, screenloc);
    screenloc += T_XREZ;
    charloc++;
  }
}

void DrawString (int x, int y, char *str)
{
  int cnt;

  for (cnt = 0; str[cnt] != 0; cnt++) 
  {
    DrawChar (x, y, str[cnt]);
    x += F_WIDTH;
  }
}
