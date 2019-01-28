This is an experiment in developing a 2D engine + game using data-oriented-design principles.
The core of the engine is based on an Entity-Component-System architecture.
Only a small subset of non-object-oriented C++ is used.

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

## STYLE GUIDELINES
* create as few separate functions as possible

![alt text](https://raw.githubusercontent.com/zmeadows/aerocore/master/screenshot.png)
