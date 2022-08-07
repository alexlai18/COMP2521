// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Place.h"
#include "PQ.h"

double dist (Place p1, Place p2);
////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid1(Place cities[], int numCities, Place powerPlant,
              PowerLine powerLines[]) {
    // We can refer to the powerplant by a number corresponding to number of cities
    int power_plant = numCities;

    Graph g = GraphNew(numCities + 1);

    for (int i = 0; i < numCities; i++) {
        // Adding the distance between the cities and the powerplant into the graph as an edge
        double distance = dist(cities[i], powerPlant);
        Edge new_edge = {i, power_plant, distance};
        GraphInsertEdge(g, new_edge);

        // Looking at other cities
        for (int j = i + 1; j < numCities; j++) {
            // Seeing the distance between 2 different cities
            distance = dist(cities[i], cities[j]);
            Edge new_edge = {i, j, distance};
            GraphInsertEdge(g, new_edge);
        }
    }

    // Creating MST from the new graph made for cities
    Graph MST = GraphMST(g);

    // Doing same thing as above but filtering out non-adjacent cities
    // Keeps count of lines to return
    int num_lines = 0;
    for (int i = 0; i < numCities; i++) {
        if (GraphIsAdjacent(MST, i, power_plant) != 0) {
            num_lines++;
            PowerLine new_line = {cities[i], powerPlant};
            powerLines[num_lines - 1] = new_line;
        }
        //num_lines--;

        for (int j = i + 1; j < numCities; j++) {
            if (GraphIsAdjacent(MST, i, j) != 0) {
                num_lines++;
                PowerLine new_line = {cities[i], cities[j]};
                powerLines[num_lines - 1] = new_line;
            }
            //num_lines--;
        }
    }
    GraphFree(MST);
    GraphFree(g);
    return num_lines;
}

double dist (Place p1, Place p2) {
    int dist_x = p1.x - p2.x;
    int dist_y = p1.y - p2.y;
    return sqrt((dist_x * dist_x) + (dist_y * dist_y));
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(Place cities[], int numCities,
              Place powerPlants[], int numPowerPlants,
              PowerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
