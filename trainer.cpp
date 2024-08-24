#include "trainer.h"

Trainer::Trainer(NNet &network, const QVector<double> &inputValues, QVector<unsigned> topology, QObject *parent)
    : QObject(parent), neuralNetwork(network), inputValues(inputValues), topology(topology)
{}

void Trainer::startTraining(int maxEpochs)
{
    for (int epoch = 0; epoch < maxEpochs; ++epoch) {
        for (int i = 0; i < inputValues.size(); i += topology[0] + topology[2]) {
            QVector<double> dataPoint = inputValues.mid(i, topology[0] + topology[2]);
            QVector<double> input = dataPoint.mid(0, topology[0]);
            QVector<double> target = dataPoint.mid(topology[0], topology[2]);
            neuralNetwork.feedForward(input);
            neuralNetwork.backProp(target);
        }
        emit progressUpdated((epoch + 1) * 100 / maxEpochs);
    }
    emit trainingFinished();
}
