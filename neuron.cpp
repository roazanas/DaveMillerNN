#include "neuron.h"


double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;


Neuron::Neuron(unsigned int numOutputs, unsigned int myIndex)
{
    for (unsigned c = 0; c < numOutputs; ++c) {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randWeight();
    }

    m_myIdx = myIndex;
}

void Neuron::feedForward(Layer &prevLayer)
{
    double sum = 0.0;

    for (unsigned n = 0; n < prevLayer.size(); ++n) {
        sum += prevLayer[n].m_outputVal * prevLayer[n].m_outputWeights[m_myIdx].weight;
    }

    m_outputVal = Neuron::activationFunc(sum);
}

void Neuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - m_outputVal;
    m_gradient = delta * Neuron::activationFunc(m_outputVal, true);
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
    double DOW = sumDOW(nextLayer);
    m_gradient = DOW * Neuron::activationFunc(m_outputVal, true);
}

void Neuron::updateInputWeights(Layer &prevLayer)
{
    for (unsigned n = 0; n < prevLayer.size(); ++n) {
        Neuron &neuron = prevLayer[n];
        double oldDeltaWeight = neuron.m_outputWeights[m_myIdx].deltaWeight;

        double newDeltaWeight = eta
                                * neuron.getOutputVal()
                                * m_gradient
                                + alpha
                                * oldDeltaWeight;

        neuron.m_outputWeights[m_myIdx].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIdx].weight += newDeltaWeight;
    }
}

double Neuron::activationFunc(double x, bool isDerivative)
{
    if (!isDerivative) { return tanh(x); }
    else               { return 1.0 - x*x; }
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
    double sum = 0.0;

    for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
    }

    return sum;
}
