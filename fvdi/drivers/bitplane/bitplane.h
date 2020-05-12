extern short fix_shape;
extern short no_restore;

long CDECL x_get_colour(Workstation *wk, long colour);
void CDECL x_get_colours(Workstation *wk, long colour, short *foreground, short *background);

long CDECL c_get_colour(Virtual *vwk, long colour);
void CDECL c_get_colours(Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background);
void CDECL c_set_colours(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);

long CDECL c_write_pixel(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
long CDECL c_read_pixel(Virtual *vwk, MFDB *mfdb, long x, long y);
long CDECL c_line_draw(Virtual *vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode);
long CDECL c_expand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
long CDECL c_fill_area(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
long CDECL c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
long CDECL c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse);
