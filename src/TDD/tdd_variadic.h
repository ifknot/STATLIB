#ifndef TDD_VARIADIC_H
#define TDD_VARIADIC_H

/**
 * @brief      Internal helper macro for COUNT_ARGS. Do not use directly.
 * @param      _1,_2,_3,_4,_5  Argument slots (unused, for position matching)
 * @param      N               The count to return (selected by position)
 * @param      ...             Ignored (varargs placeholder)
 * @return     The argument count from position N
 * @warning    Implementation detail - use COUNT_ARGS instead
 * @private
 */
#define COUNT_ARGS_(_1,_2,_3,_4,_5,N,...) N

/**
 * @brief      Counts the number of arguments passed to a variadic macro
 * @param      ...  The arguments to count (0-5 supported)
 * @return     The number of arguments (0 if empty)
 * @note       Supports up to 5 arguments. Returns 0 for empty argument list.
 * @note       Works by argument position matching through COUNT_ARGS_
 * @example    Basic usage:
 *             @code
 *             COUNT_ARGS()      // → 0
 *             COUNT_ARGS(a)     // → 1
 *             COUNT_ARGS(a,b,c) // → 3
 *             @endcode
 * @example    Usage in conditional compilation:
 *             @code
 *             #if COUNT_ARGS(__VA_ARGS__) > 2
 *             // Handle case with >2 arguments
 *             #endif
 *             @endcode
 */
#define COUNT_ARGS(...) COUNT_ARGS_(__VA_ARGS__, 5,4,3,2,1,0)

/**
 * @brief      Macro to demonstrate COUNT_ARGS usage
 * @param      ...  Arguments to count and print
 * @example    PRINT_ARG_COUNT(1, "two", 3.0); // Prints "3 arguments"
 */
#define PRINT_ARG_COUNT(...) \
    printf("%d arguments\n", COUNT_ARGS(__VA_ARGS__))

/**
    * @brief      Extracts the first argument from a variadic macro.
    * @param      ...  Variadic arguments (must contain at least one argument).
    * @return     The first argument.
    * @warning    If no arguments are provided, behavior is undefined (may trigger compiler warnings).
    * @note       Uses a two-layer indirection to ensure proper expansion.
    * @example    Basic usage:
    *             @code
    *             int x = FIRST_ARG(42, 100, 200);  // x = 42
    *             @endcode
    * @example    Edge case (avoid):
    *             @code
    *             FIRST_ARG();  // Dangerous! No arguments provided.
    *             @endcode
    */
#define FIRST_ARG(...)        FIRST_ARG_IMPL(__VA_ARGS__)

/**
    * @brief      Implementation helper for FIRST_ARG. Do not use directly.
    * @private
    */
#define FIRST_ARG_IMPL(X, ...) X

/**
    * @brief      Extracts all arguments except the first from a variadic macro.
    * @param      ...  Variadic arguments (must contain at least one argument).
    * @return     A comma-separated list of remaining arguments (empty if only one argument provided).
    * @warning    If no arguments are provided, behavior is undefined.
    * @note       Useful for recursive macro patterns or argument forwarding.
    * @example    Basic usage:
    *             @code
    *             REST_ARGS(10, 20, 30);  // Expands to: 20, 30
    *             @endcode
    * @example    Edge case (single argument):
    *             @code
    *             REST_ARGS(42);  // Expands to nothing (empty).
    *             @endcode
    */
#define REST_ARGS(...)        REST_ARGS_IMPL(__VA_ARGS__)

/**
    * @brief      Implementation helper for REST_ARGS. Do not use directly.
    * @private
    */
#define REST_ARGS_IMPL(X, ...) __VA_ARGS__

/**
 * @brief      Safe version of FIRST_ARG with a default value.
 * @param      ...       Variadic arguments.
 * @param      default   Value returned if no arguments provided.
 * @example    SAFE_FIRST_ARG(, 0) → 0
 */
#define SAFE_FIRST_ARG(...) \
    (COUNT_ARGS(__VA_ARGS__) >= 1 ? FIRST_ARG(__VA_ARGS__) : "")

#endif

#define STRINGIFY(x) #x
#define EXPAND_AND_STRINGIFY(x) STRINGIFY(x)
