#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pnglite.h"
#include "cs220_paint.h"

// This is just a utility to convert a truecolor PNG file into our
// textual format. You should not need to modify this code.

int main(int argc, char *argv[]) {
  png_init(0, 0);

  if (argc != 3) {
    printf("Error: Usage: png2txt <png file> <output txt file>\n");
    return 1;
  }

  const char *in_filename = argv[1];
  const char *out_filename = argv[2];

  png_t png;
  if (png_open_file_read(&png, in_filename) != PNG_NO_ERROR) {
    printf("Error: Couldn't open PNG file\n");
    return 1;
  }

  // only allow truecolor 8bpp images
  if (!(png.color_type == PNG_TRUECOLOR && png.bpp == 3) &&
      !(png.color_type == PNG_TRUECOLOR_ALPHA && png.bpp == 4)) {
    png_close_file(&png);
    printf("Error: PNG is not truecolor\n");
    return 1;
  }

  int num_pixels = png.width * png.height;

  // allocate buffer for pixel data
  unsigned char *pixel_data = malloc(num_pixels * png.bpp);
  if (!pixel_data) {
    png_close_file(&png);
    printf("Error: failed to allocate PNG pixel data buffer\n");
    return 1;
  }

  // read pixel data
  if (png_get_data(&png, pixel_data) != PNG_NO_ERROR) {
    free(pixel_data);
    png_close_file(&png);
    return 1;
  }

  // if pixel data is RGBA rather than RGB, convert to RGB
  if (png.color_type == PNG_TRUECOLOR_ALPHA) {
    assert(png.bpp == 3);

    unsigned char *compacted = malloc(num_pixels * 3);
    if (compacted == NULL) {
      png_close_file(&png);
      free(pixel_data);
      printf("Error: failed to allocate PNG pixel data buffer\n");
      return 1;
    }

    for (int i = 0; i < num_pixels; i++) {
      // note that we're just discarding the alpha value

      unsigned char r = pixel_data[i*4 + 0];
      unsigned char g = pixel_data[i*4 + 1];
      unsigned char b = pixel_data[i*4 + 2];

      compacted[i*3 + 0] = r;
      compacted[i*3 + 1] = g;
      compacted[i*3 + 2] = b;
    }

    free(pixel_data);
    pixel_data = compacted;
  }

  // write to text-based format
  int rc = write_image(out_filename, png.width, png.height, pixel_data);
  png_close_file(&png);
  free(pixel_data);

  printf("%s\n", (rc != 0) ? "Success" : "Error: Failed to write output");

  return (rc != 0) ? 0 : 1;
}
