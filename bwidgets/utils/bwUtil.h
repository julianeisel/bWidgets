#pragma once


/**
 * Portable version of __PRETTY_FUNCTION__.
 */
#ifdef _MSC_VER
#  define PRETTY_FUNCTION __FUNCSIG__
#else
#  define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif


/* Include util headers. */
#include "bwOptional.h"
#include "bwPointer.h"
