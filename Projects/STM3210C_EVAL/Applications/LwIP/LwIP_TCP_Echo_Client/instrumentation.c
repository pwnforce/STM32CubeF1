#include <stdint.h>
#include <string.h>
// #include <stdio.h>

// magic address of one page size (4k) mapped r+w to share data between
// loader and fimrware constructors
#define CONSTRUCTOR_MAGIC_ADDRESS 0x13370000


static const size_t AFL_MAP_SIZE = 1 << 16;
uint8_t *AFL_MAP = NULL;

const char fuzz_evalue_base = 0;
const char fuzz_evalue_prng_seed = 0;
const char fuzz_evalue_inst_ratio = 100;

__attribute__((destructor)) void instr_return_shm (void);
void __sanitizer_cov_trace_pc_guard(uint32_t*);
void __sanitizer_cov_trace_pc_guard_init(uint32_t*, uint32_t*);

extern __attribute__((constructor)) void __sancov_ctor(void);

/* Parameters of the linear congruential PRNG. Taken from
 * https://en.wikipedia.org/wiki/Linear_congruential_generator#Parameters_in_common_use
 * */
static const uint64_t PRNG_MULT = 0x5DEECE66D;
static const uint64_t PRNG_INC = 11;
static const uint64_t PRNG_SEED_MASK = ((1ll << 48) - 1ll);
static const uint32_t PRNG_OUT_FIRST_BIT = 16;
static const uint32_t PRNG_OUT_MASK = ((1ll << 32) - 1ll);
static uint64_t sancov_prng_seed;

// internal state of the coverage sanitizer
static uint32_t prev_location = 0;
static uint32_t prev_location_init = 0;

// deterministic PRNG:
static void initrand(void) {
    sancov_prng_seed = (uint64_t)(fuzz_evalue_prng_seed - fuzz_evalue_base);
}

static uint32_t randint(uint32_t max) {
    uint32_t rand;

    sancov_prng_seed = (PRNG_MULT*sancov_prng_seed + PRNG_INC) & PRNG_SEED_MASK;
    rand = (sancov_prng_seed >> PRNG_OUT_FIRST_BIT) & PRNG_OUT_MASK;

    return rand % max;
}


void __sanitizer_cov_trace_pc_guard(uint32_t* guard) {
  // called from firmware context (all object files related to CNC firmware)

  if (AFL_MAP != NULL && *guard != 0) {
    AFL_MAP[*guard ^ prev_location]++;
    prev_location = *guard >> 1;
  }
}

void __sanitizer_cov_trace_pc_guard_init(uint32_t* start, uint32_t* stop) {
  // called from loader context (~constructor)

  unsigned long inst_ratio;

  if (start == stop || *start) {
      //printf("__sanitizer_cov_trace_pc_guard_init returning");
      return;
  }

  //printf("Init afl instrumentation!\n");


  uint32_t *GLOBAL_DATA = CONSTRUCTOR_MAGIC_ADDRESS;
  AFL_MAP = (uint8_t*) GLOBAL_DATA;  // obtain reference to AFL_MAP
  if (AFL_MAP == NULL) {
    //fprintf(stderr, "error obtaining AFL_MAP\n");
    return;
  }

  // TODO: mimic afl behavior here
  inst_ratio = (unsigned long)(fuzz_evalue_inst_ratio - fuzz_evalue_base);
  inst_ratio = (unsigned long)(100 - 0);
  initrand();

  /* Initialize prev_location with a random value to mimic the behavior of
   * afl-as, which defines the variable as .comm prev_loc, 8. */
  prev_location_init = randint(AFL_MAP_SIZE - 1) + 1;
  prev_location = prev_location_init;

  /* Always set the value of the very first guard, as it is done in
   * afl-llvm-rt.c. */
  *start = randint(AFL_MAP_SIZE - 1) + 1;
  start++;

  for (; start < stop; start++) {
      if (randint(100) < inst_ratio) {
          *start = randint(AFL_MAP_SIZE - 1) + 1;
      } else {
          *start = 0;
      }
  }
}
