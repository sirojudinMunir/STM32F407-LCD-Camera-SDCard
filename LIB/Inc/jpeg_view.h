#ifndef INC_JPEG_VIEW_H_
#define INC_JPEG_VIEW_H_

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

uint8_t jpeg_decode(JFILE *file, uint8_t *rowBuff, int posx, int posy, UINT *iw, UINT *ih);

void read_jpg (char *fn);

void jpeg_screen_view(char* fn, int px, int py, UINT *iw, UINT *ih);


#endif /* INC_JPEG_VIEW_H_ */
