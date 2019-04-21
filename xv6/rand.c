// a Par-Miller random number generator
// reference: https://www.sanfoundry.com/c-program-implement-park-miller-random-number-generation-algorithm/

#define RNG_M   0xffffffff
#define RNG_A   16807L
#define RNG_Q   127773L
#define RNG_R   2836L

static long rnd_seed;

unsigned long fastrand(unsigned long seedval) {
    register long low, high, tmp;
    rnd_seed = (seedval % (RNG_M - 1)) + 1;
    high = rnd_seed / RNG_Q;
    low = rnd_seed % RNG_Q;
    tmp = RNG_A * low - RNG_R * high;
    rnd_seed = (tmp > 0)?tmp:(tmp + RNG_M);

    return rnd_seed;
}