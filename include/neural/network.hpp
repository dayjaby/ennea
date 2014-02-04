#ifndef ENNOVIA_NEURAL_NETWORK_HPP
#define ENNOVIA_NEURAL_NETWORK_HPP

#include <boost/graph/adjacency_list.hpp>

#include "neural/layer.hpp"

using namespace boost;

namespace Ennovia {
namespace Neural {

template <class Float, template <typename> class Sigmoid>
class Network
public:
    typedef adjacency_list<boost::vecS,boost::vecS,boost::bidirectionalS> Graph;
    Network(LayerConnection connectons[],int conn_count, int layers) :
        graph(connections,conn_count,layers) {

    }

    void backpropagate(LayerConnection& lc) {
        Layer& out = lc.first();
        Layer& target = lc.second();
    }
private:
    void calculateLayer(Layer layer) {
        int n = layerSize[layer];
        for(int i=0;i<n;i++) {
            neuron(layer,i)(value) =
            value[layer][i] = Sigmoid<Float>::f()
        }
    }
    struct neuron {
        neuron(Layer layer_, int i_) :
            layer(layer_), i(i_) {}

        template <class Map>
        void operator()(Map& map) {
            return map[layer][i];
        }
        Layer layer;
        int i;
    };

    Float weight(neuron& a, neuron& b) {
        return weight(a.layer,b.layer)(a.i,b.i);
    }

    Matrix weight(Layer in, Layer out) {
        return weight[std::make_pair(a.layer,b.layer)];
    }

    void getErrorSignal
    Graph graph;
}
}

#endif
