#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pnglite.h"
#include "cs220_paint.h"

// This is just a utility to convert a file in our textual format
// back to truecolor PNG. You should not need to modify this code.

int main(int argc, char *argv[]) {
  png_init(0, 0);

  if (argc != 3) {
    printf("Error: Usage: txt2png <input txt file> <output png file>\n");
    return 1;
  }

  const char *in_filename = argv[1];
  const char *out_filename = argv[2];

  // read pixel data from input txt file
  int width, height;
  unsigned char *pixel_data = read_image(in_filename, &width, &height);
  if (pixel_data == NULL) {
    printf("Error: Could not read data from input file\n");
    return 1;
  }

  // write to PNG file
  png_t png;
  if (png_open_file_write(&png, out_filename) != PNG_NO_ERROR) {
    printf("Error: Could not open PNG output file\n");
    free(pixel_data);
    png_close_file(&png);
    return 1;
  }

  if (png_set_data(&png, width, height, 8, PNG_TRUECOLOR, pixel_data) != PNG_NO_ERROR) {
    printf("Error: Could not set PNG data\n");
    free(pixel_data);
    png_close_file(&png);
    return 1;
  }

  // Success
  free(pixel_data);
  png_close_file(&png);

  printf("Success\n");

  return 0;
}
