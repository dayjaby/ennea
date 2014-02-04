#ifndef ENNOVIA_NEURAL_SIGMOID_HPP
#define ENNOVIA_NEURAL_SIGMOID_HPP

#include <cmath>

namespace Ennovia {
namespace Neural {

template <class Float>
class logistic {
public:
    static const Float lower = 0.0;
    static const Float upper = 1.0;
    static Float f(Float x) {
        return 1.0/(1.0+std::exp(-x));
    }
    static Float f_derived(Float x) {
        Float y = f(x);
        return y*(1-y);
    }
}

template <class Float>
class tanh {
public:
    static const Float lower = -1.0;
    static const Float upper = 1.0;
    static Float f(Float x) {
        return std::tanh(x);
    }

    static Float f_derived(Float x) {
        Float y = f(x);
        return 1-y*y;
    }
};

}
}

#endif
