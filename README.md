# LP2 - Geographic Map and Pathfinding Project

This repository contains a C++ project focused on handling geographic data structures, points of interest, roads, and shortest-path logic. The code is organized around map objects, route structures, and spatial indexing utilities.

## Project purpose

The project models a simplified map with:

- points of interest (POIs)
- routes/paths between locations
- spatial distance calculations using geographic coordinates
- path search between two locations
- spatial indexing with a BallTree-style structure

The implementation is oriented toward a university assignment or laboratory exercise in data structures and algorithms, especially in graph/map representation and proximity search.

## Repository structure

```text
LP2/
├── README.md
└── LP_segundaParte/
    ├── BallTree.h
    ├── BallTree.cpp
    ├── CamiBase.h
    ├── CamiSolucio.h
    ├── Common.h
    ├── GrafSolucio.h
    ├── GrafSolucio.cpp
    ├── MapaBase.h
    ├── MapaSolucio.h
    ├── MapaSolucio.cpp
    ├── PuntDeInteresBase.h
    ├── PuntDeInteresBase.cpp
    ├── PuntDeInteresBotigaSolucio.h
    ├── PuntDeInteresBotigaSolucio.cpp
    ├── PuntDeInteresRestaurantSolucio.h
    ├── PuntDeInteresRestaurantSolucio.cpp
    ├── Util.h
    ├── Util.cpp
    └── ...
```

## Main components

### BallTree
The `BallTree` structure is used for spatial searching and nearest-neighbor-like operations. It stores points and partitions them hierarchically using a center pivot and radius. This is useful for efficiently identifying nearby geographic positions.

Key responsibilities:

- building the tree from a set of coordinates
- searching for the nearest node to a target point
- traversing the tree in ordered ways for debugging or visualization

### Map representation
The map is modeled through abstract base classes and concrete solution classes:

- `MapaBase.h`: generic interface for map functionality
- `MapaSolucio.h`: concrete implementation
- `CamiBase.h`: base type for roads/paths
- `GrafSolucio.*`: graph-based solution for route-related structures

These classes define the overall representation of a network of interest points connected by paths.

### Points of interest
The project includes different POI subclasses, such as:

- `PuntDeInteresBase`
- `PuntDeInteresBotigaSolucio`
- `PuntDeInteresRestaurantSolucio`

These represent specific map entities, such as shops or restaurants, with location and metadata.

### Utilities and geometry
The `Util` module includes helper functions for:

- converting degrees to radians and vice versa
- calculating geographic distances using the Haversine formula
- computing central points from groups of coordinates

This is essential for path calculations and spatial analysis.

## Core functionality

The repository is designed around common map operations such as:

- parsing XML geographic data
- constructing a route graph or map from the parsed elements
- computing shortest paths between points of interest
- retrieving all POIs or roads in the map
- performing spatial searches or nearest-point operations

## Typical domain model

The project represents a map as an abstraction of:

- geographic coordinates (`Coordinate`)
- roads or paths (`CamiBase`)
- interest points (`PuntDeInteresBase`)
- graph or path search logic (`GrafSolucio`, `MapaSolucio`)

This is a classic structure for route-planning or geographic navigation problems.

## Build and execution

This repository appears to be a C++ laboratory project, likely meant to be compiled in an IDE or with a C++ compiler. There are no project files such as a CMakeLists.txt or Visual Studio solution visible in the folder listing, so it is likely intended to be built in the same environment used by the course assignment.

Typical compilation would require:

- a C++ compiler supporting modern C++ standards
- the project headers and source files included in the build
- any required external dependencies defined by the course framework

## Notes

This repository is not a standalone end-user application; rather, it is a data-structure and algorithm implementation focused on geographic networks and path computation. It is best understood as a framework for map modeling and route-related logic.

## Summary

The repository implements a lightweight geographic map system with road/path representation, point-of-interest entities, an indexing structure for spatial queries, and utilities to compute distances and shortest routes. It is a solid example of applying C++ object-oriented design to a real-world map-navigation problem.
