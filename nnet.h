#ifndef NNET_H
#define NNET_H

#include <QVector>
#include <QDebug>
#include "neuron.h"

class NNet
{
public:
    NNet(const QVector<unsigned> &topology);
    void feedForward(const QVector<double> &inputValues);
    void backProp(const QVector<double> &targetValues);
    void getResults(QVector<double> &resultValues) const;

private:
    QVector<Layer> m_layers;
    double m_error;

    double m_recentAverageError = 0.0;
    static double m_recentAverageSmoothingFactor;
};

#endif // NNET_H
