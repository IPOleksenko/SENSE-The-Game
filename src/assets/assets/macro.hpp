#pragma once

#ifdef __ALLOW_ASSET_MACRO_INCLUDE__

    #include <incbin.h>

    #define INCBIN_MULTIPLE(seq) __INCBIN_END(__INCBIN_A seq)
    #define __INCBIN_SINGLE(x, y) INCBIN(x, y);
    #define __INCBIN_A(x, y) __INCBIN_SINGLE(x, y) __INCBIN_B
    #define __INCBIN_B(x, y) __INCBIN_SINGLE(x, y) __INCBIN_A
    #define __INCBIN_A_END
    #define __INCBIN_B_END
    #define __INCBIN_END(...) __INCBIN_END_(__VA_ARGS__)
    #define __INCBIN_END_(...) __VA_ARGS__##_END

    #define INCBIN_EXTERN_MULTIPLE(seq) __INCBIN_EXTERN_END(__INCBIN_EXTERN_A seq)
    #define __INCBIN_EXTERN_SINGLE(x) INCBIN_EXTERN(x);
    #define __INCBIN_EXTERN_A(x) __INCBIN_EXTERN_SINGLE(x) __INCBIN_EXTERN_B
    #define __INCBIN_EXTERN_B(x) __INCBIN_EXTERN_SINGLE(x) __INCBIN_EXTERN_A
    #define __INCBIN_EXTERN_A_END
    #define __INCBIN_EXTERN_B_END
    #define __INCBIN_EXTERN_END(...) __INCBIN_EXTERN_END_(__VA_ARGS__)
    #define __INCBIN_EXTERN_END_(...) __VA_ARGS__##_END

    #define SDL_Incbin(NAME) \
        SDL_RWFromConstMem( \
            INCBIN_CONCATENATE( \
                INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
                INCBIN_STYLE_IDENT(DATA) \
            ), \
            INCBIN_CONCATENATE( \
                INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
                INCBIN_STYLE_IDENT(SIZE) \
            ) \
        )
#else
    #error "Direct asset macro include is forbidden!"
#endif
