#ifndef CS220_PAINT_H
#define CS220_PAINT_H

// image I/O functions
unsigned char *read_image(const char *filename, int *width, int *height);
int write_image(const char *filename, int width, int height, const unsigned char *buf);


// suggested helper functions for doing drawing operations
double geom_dist(double x1, double y1, double x2, double y2);
void render_rectangle(unsigned char *pixel_data, int img_width, int img_height,
                      int rect_x, int rect_y, int rect_w, int rect_h,
                      int r, int g, int b);
void render_ellipse(unsigned char *pixel_data, int img_width, int img_height,
                      double x1, double y1, double x2, double y2, double len,
                      int r, int g, int b);
void flood_fill(unsigned char *pixel_data, int img_width, int img_height,
                int x, int y, int r, int g, int b);
void rec_flood_fill(unsigned char *pixel_data, int img_width, int img_height,
                    int x, int y,
                    int orig_r, int orig_g, int orig_b,
                    int r, int g, int b,
                    int dir);


#endif // CS220_PAINT_H
