#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cs220_paint.h"

int main(int argc, char *argv[]) {

  // TODO: check that correct number of command line arguments were passed (report error if not)
  if (argc != 3) {
    printf("Error: expected three arguments");
    return 1;
  }

  const char *in_filename, *out_filename;

  // TODO: get command line arguments
  in_filename = argv[1];
  out_filename = argv[2];

  // TODO: open input file
  FILE *in = fopen(in_filename, "r"); 
  if (in == NULL) {
     printf("Error: the input file was unable to be read");
     return 1;
  }

  // TODO: read header (width and height) from input file
  int width = 0;
  int height = 0;
  int nums = fscanf(in, "%d %d", &width, &height);

  if (nums != 2) {
    printf("Error: Not able to find width and height of image\n");
    fclose(in);
    return 1;
  }

  // TODO: allocate pixel buffer, and set every pixel to be black
  int num_vals = width * height * 3;
  unsigned char *buf = (unsigned char *) calloc(num_vals, sizeof(unsigned char));
  
  if (buf == NULL) {
    printf("Error: Pointer is a null pointer\n");
    fclose(in);
    free(buf);
    return 1;
  }

  // TODO: process commands, call helper functions to perform drawing operations 
  int error = 0;
  char command;

  int rc = fscanf(in, " %c", &command); // read first command

  // color values
  int r = 0;
  int g = 0;
  int b = 0;

  // rectangle data
  int rect_x = 0;
  int rect_y = 0;
  int rect_w = 0;
  int rect_h = 0;

  // ellipse data
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  double len = 0;

  // flood fill data
  int x = 0;
  int y = 0;

  // number of inputs
  int val = 0;

  while (!error && rc != EOF) {
    switch (command) {
    case 'c':   // set current color
      {
        val = fscanf(in, " %d %d %d", &r, &g, &b);
        if (val != 3) {
          error = 1;
          break;
        }
      }
      break;

    case 'r':   // rectangle
      {
        val = fscanf(in, " %d %d %d %d", &rect_x, &rect_y, &rect_w, &rect_h);
        if (val != 4) {
          error = 1;
          break;
        }
        render_rectangle(buf, width, height, rect_x, rect_y, rect_w, rect_h, r, g, b);
      }
      break;

    case 'e':   // ellipse
      {
        val = fscanf(in, " %lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &len);
        if (val != 5) {
          error = 1;
          break;
        }
        render_ellipse(buf, width, height, x1, y1, x2, y2, len, r, g, b);
      }
      break;

    case 'f':   // flood fill
      {
        val = fscanf(in, " %d %d", &x, &y);
        if (val != 2) {
          error = 1;
          break;
        }
        flood_fill(buf, width, height, x, y, r, g, b);
      }
      break;

    default:    // unknown command
      printf("Error: Unknown command '%c'\n", command);
      error = 1;
      break;
    }

    if (!error) {
      rc = fscanf(in, " %c", &command); // read next command
    }
  }
  
  // TODO: close input file
  fclose(in);

  // TODO: if an error occurred while processing commands, report it
  if (error != 0) {
    printf("Error: Unable to process commands");
    return 1;
  }

  // TODO: try to write output file by calling write_image
  write_image(out_filename, width, height, buf);

  // TODO: if output couldn't be written, report error
  if (out_filename == NULL) {
    printf("Error: Output couldn't be written to file");
    return 1;
  }

  // free dynamic memory allocation for pixel buffer if no errors
  free(buf);

  // avoid accidently attempting to use pointer later
  // buf = NULL;

  printf("Success\n");

  return 0;

}
