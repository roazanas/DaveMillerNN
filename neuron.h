#ifndef NEURON_H
#define NEURON_H

#include <QVector>
#include <QRandomGenerator>

struct Connection {
    double weight;
    double deltaWeight;
};

class Neuron;
typedef QVector<Neuron> Layer;

class Neuron
{
public:
    Neuron(unsigned numOutputs, unsigned myIndex);
    void setOutputVal(double val) { m_outputVal = val; }
    double getOutputVal() const { return m_outputVal; }
    void feedForward(Layer &prevLayer);

    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

private:
    static double eta;
    static double alpha;
    unsigned m_myIdx;
    double m_gradient;
    double m_outputVal;
    QVector<Connection> m_outputWeights;
    static double randWeight() { return QRandomGenerator::global()->generateDouble(); };
    static double activationFunc(double x, bool isDerivative=false);
    double sumDOW(const Layer &nextLayer) const;
};

#endif // NEURON_H
