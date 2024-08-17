add_compile_options(
    -Wall
    -Wextra                         # Reasonable and standard
    -Wshadow                        # Warns the user if a variable declaration shadows one from a parent context
    -Wnon-virtual-dtor              # Warns the user if a class with virtual functions has a non-virtual destructor. This helps
    # catch hard to track down memory errors
    -Wold-style-cast                # Warns for c-style casts
    -Wcast-align                    # Warns for potential performance problem casts
    -Wunused                        # Warns on anything being unused
    -Woverloaded-virtual            # Warns if you overload (not override) a virtual function
    -Wpedantic                      # Warns if non-standard C++ is used
    -Wconversion                    # Warns on type conversions that may lose data
    -Wsign-conversion               # Warns on sign conversions
    -Wnull-dereference              # Warns if a null dereference is detected
    -Wdouble-promotion              # Warns if float is implicit promoted to double
    -Wformat=2                      # Warns on security issues around functions that format output (ie printf)
    -Wimplicit-fallthrough          # Warns on statements that fallthrough without an explicit annotation
    -Wmisleading-indentation        # Warns if indentation implies blocks where blocks do not exist
    -Wduplicated-cond               # Warns if if / else chain has duplicated conditions
    -Wduplicated-branches           # Warns if if / else branches have duplicated code
    -Wlogical-op                    # Warns about logical operations being used where bitwise were probably wanted
    -Wuseless-cast                  # Warns if you perform a cast to the same type
    -Wsuggest-override
)