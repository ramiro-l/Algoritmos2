/*
@file array_helpers.c
@brief Array Helpers method implementation
*/
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array_helpers.h"

/**
 * @brief returns true when reach last entry in flight table
 * @return True when is the last entry of the flight table, False otherwise
 */
static bool is_last_line(unsigned int hour, unsigned int type)
{
  return hour == HOURS - 1u && type == TYPE - 1u;
}

void array_dump(LayoverTable a)
{
  for (unsigned int hour = 0u; hour < HOURS; ++hour)
  {
    for (unsigned int type = 0u; type < TYPE; ++type)
    {
      Flight f = a[hour][type];
      fprintf(stdout, "%c: %s at %u:00 with %u passengers", f.code, f.type == 0 ? "arrives" : "departs", f.hour - 1, f.passengers_amount);
      if (!is_last_line(hour, type))
      {
        fprintf(stdout, "\n");
      }
    }
  }
}

unsigned int passengers_amount_in_airport(LayoverTable a, unsigned int h)
{
  unsigned int count = 0;
  for (size_t i = 0; i <= h; i++)
  {
    count = count + a[i][0].passengers_amount - a[i][1].passengers_amount;
  }
  count = count + a[h][0].passengers_amount;

  return count;
}

void array_from_file(LayoverTable array, const char *filepath)
{
  FILE *file = NULL;

  file = fopen(filepath, "r");
  if (file == NULL)
  {
    fprintf(stderr, "File does not exist.\n");
    exit(EXIT_FAILURE);
  }

  char code;
  int i = 0;
  while (!feof(file))
  {
    int res = fscanf(file, " _%c_ ", &code);
    if (res != 1)
    {
      fprintf(stderr, "Invalid file.\n");
      exit(EXIT_FAILURE);
    }
    Flight flight_arrival = flight_from_file(file, code);
    Flight flight_departure = flight_from_file(file, code);

    if (flight_departure.hour < flight_arrival.hour)
    {
      fprintf(stderr, "Invalid hours.\n");
      exit(EXIT_FAILURE);
    }
    array[flight_arrival.hour - 1][0] = flight_arrival;
    array[flight_departure.hour - 1][1] = flight_departure;
    i = i + 1;
  }
  if (i > 24)
  {
    fprintf(stderr, "Invalid data.\n");
    exit(EXIT_FAILURE);
  }
  fclose(file);
}
