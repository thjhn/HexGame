#include "hexfield.h"
#include <iostream>

/**
 * The constructor creates an empty field without any neighbours.
 */
hexfield::hexfield():color(0)
{}

/**
 * Change the color of a field. This is only possible, if the field was previously empty.
 *
 * @param color the colot to set.
 * @return -1 if the field was not empty, 0 otherwise
 */
int hexfield::setColor(int color)
{
    if(this->color != 0) return -1; // you can only change color if the field is empty

    this->color = color;
    return 0;
}

/**
 * Get the color of the field.
 *
 * @return the color 0 (empty), 1 (red), 2 (blue)
 */
int hexfield::getColor()
{
    return this->color;
}

/**
 * Introduce a new neighbour.
 *
 * @param neighbour a pointer to the new neighbour.
 */
void hexfield::addNeighbour(hexfield* neighbour)
{
    this->neighbours.push_back(neighbour);
}

/**
 * Get a vector of pointers to all neighbours.
 *
 * @return a vector of pointers of the neighbours.
 */
std::vector<hexfield*> hexfield::getNeighbours()
{
    return this->neighbours;
}
