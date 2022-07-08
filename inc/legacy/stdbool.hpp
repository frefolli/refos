#ifndef __STDBOOL_HPP__
#define __STDBOOL_HPP__

#ifndef __cplusplus

#define bool        _Bool
#define true        1
#define false        0

#else /* __cplusplus */

#define _Bool       bool
#define bool        bool
#define false       false
#define true        true

#endif /* __cplusplus */

#define __bool_true_false_are_defined        1

#endif        /* stdbool.hpp */
