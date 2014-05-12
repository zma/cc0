#ifndef __L0_TYPES_H__
#define __L0_TYPES_H__

#include <stdint.h>

enum {
  SUCCESS = 1,
  FAIL_RETRY    = 127,
  FAIL    = 255,
};

typedef uint32_t vpc_id_t;
typedef uint8_t l0_reval_t;

#endif /* __TYPES_H__ */
