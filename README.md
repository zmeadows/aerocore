This is an experiment in developing a 2D engine + game using data-oriented-design principles.
The core of the engine is based on an Entity-Component-System architecture.
Only a small subset of non-object-oriented C++ is used.

## FEATURES
* Component storage/retrieval with custom high-performance contiguous data structures
    * Stable chunked dynamic bucket arrays with no re-allocation/copying
    * Entity ID <--> Component ID mapping via custom hash table with open addressing and robin hood hashing
    * Numerous alternatives to STL containers which perform better in the game development context (ArraySet, SmallString, etc)
* Straight forward creation of systems to subscribe to component sets and process entities in only a few lines of code.
* Fast 2D collision for arbitrary polygons using quad-tree + separating axis theorem
* Custom (simple) creation/loading of sprite, animation, and collision geometry from GIMP (GNU Image Manipulation Program) output.
* Implementations of many common components/systems
* Camull-Rom splines for smooth movement/animation
* Basic 2D linear algebra
* Simple and fast sprite graphics manipulation

## EXAMPLE
![alt text](https://raw.githubusercontent.com/zmeadows/aerocore/master/screenshot.PNG)

## ALLOWED C++
* lambdas
* operator overloading
* function overloading
* range-based for loops
* auto
* nullptr
* references
* move semantics (mostly for constructing in-place within containers)
* templates (minimally, and only where they are the only/best solution)

## FORBIDDEN C++
* STL containers
* RAII (constructors/destructors)
* unique_ptr and friends
* inheritance
* member functions
* iterators
* non-public member variables


