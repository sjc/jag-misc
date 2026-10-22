//
// draw.h
//

#ifndef DRAW_H
#define DRAW_H

void SetScreenAddress(unsigned char *ptr);

void DrawCharLine (int charloc, int screenloc);
void DrawChar (int x, int y, char ch);
void DrawString (int x, int y, char *str);

#endif // DRAW_H
