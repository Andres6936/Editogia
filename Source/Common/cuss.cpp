#include "Editogia/cuss.h"
#include "Editogia/stringfunc.h"
#include <sstream>
#include <fstream>

using namespace cuss;

void print_scrollbar(Window* win, int posx, int posy, int length, int offset,
		int size, bool selected);


#define SELECTCOLOR c_blue

#define PREP_ELEMENT(ele) \
  (ele)->name = name;\
  (ele)->posx = posx;\
  (ele)->posy = posy;\
  (ele)->sizex = szx;\
  (ele)->sizey = szy;\
  (ele)->selected = false;\
  (ele)->selectable = selectable;\
  (ele)->align = ALIGN_LEFT












