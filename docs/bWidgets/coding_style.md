Coding Style
------------

In short: Be considerate, be reasonable!

Although bWidgets is written in C++, it's used rather conservatively. Crazy template usage a la type-traits is avoided. Same goes for operator overloading for complex data-types, lambdas, . Also note that we stick to using C++11 (because of Blender).<br/>
In fact, if possible we try to stick to a pretty C-like C++ usage. It's more like we use C++ features to compensate C weaknesses.

Some features from C++ that make sense for us and that we do use: namespaces, constructors/destructors, smart pointers (`unique_ptr()` mainly), references, callable objects (functors), simple inheritance, standard library, aliases, `constexpr`.<br/>
We are a bit wary of using templates and operator overloading. They are used, but only for very simple cases where they make sense. `auto` can be used for variables with obvious type.

One thing we are unsure about is exception handling. Should we use C++ exceptions? Are alternatives preferable (return codes, `std::optional`, etc)?
