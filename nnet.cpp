#include "nnet.h"

double NNet::m_recentAverageSmoothingFactor = 100.0;

NNet::NNet(const QVector<unsigned int> &topology)
{
    if (topology == QVector<unsigned int>{0, 1, 0}) return;
    unsigned layersNum = topology.size();
    for (unsigned iLayer = 0; iLayer < layersNum; ++iLayer) {
        m_layers.push_back(Layer());
        unsigned numOutputs = iLayer == topology.size()-1 ? 0 : topology[iLayer+1];

        for (unsigned iNeuron = 0; iNeuron < topology[iLayer]; ++iNeuron) {
            m_layers.back().push_back(Neuron(numOutputs, iNeuron));
            m_layers.back().back().setOutputVal(1.0);
        }
    }
    qDebug() << "NNet::NNet() | neural network init " << topology;
}

void NNet::feedForward(const QVector<double> &inputValues)
{
    Q_ASSERT(inputValues.size() == m_layers[0].size());

    for (unsigned i = 0; i < inputValues.size(); ++i) {
        m_layers[0][i].setOutputVal(inputValues[i]);
    }

    for (unsigned iLayer = 1; iLayer < m_layers.size(); ++iLayer) {
        Layer &prevLayer = m_layers[iLayer-1];
        for (unsigned n = 0; n < m_layers[iLayer].size(); ++n) {
            m_layers[iLayer][n].feedForward(prevLayer);
        }
    }
}

void NNet::backProp(const QVector<double> &targetValues)
{
    Layer &outputLayer = m_layers.back();
    m_error = 0.0;
    for (unsigned n = 0; n < outputLayer.size(); ++n) {
        double delta = targetValues[n] - outputLayer[n].getOutputVal();
        m_error += delta * delta;
    }
    m_error /= outputLayer.size();
    m_error = sqrt(m_error);

    m_recentAverageError =
        (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
        / (m_recentAverageSmoothingFactor + 1.0);

    for (unsigned n = 0; n < outputLayer.size(); ++n) {
        outputLayer[n].calcOutputGradients(targetValues[n]);
    }

    for (unsigned iLayer = m_layers.size() - 2; iLayer > 0; --iLayer) {
        Layer &hiddenLayer = m_layers[iLayer];
        Layer &nextLayer = m_layers[iLayer+1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }

    for (unsigned iLayer = m_layers.size() - 1; iLayer > 0; --iLayer) {
        Layer &layer = m_layers[iLayer];
        Layer &prevLayer = m_layers[iLayer-1];

        for (unsigned n = 0; n < layer.size(); ++n) {
            layer[n].updateInputWeights(prevLayer);
        }
    }
}

void NNet::getResults(QVector<double> &resultValues) const
{
    resultValues.clear();

    for (unsigned n = 0; n < m_layers.back().size(); ++n) {
        resultValues.push_back(m_layers.back()[n].getOutputVal());
    }
}
