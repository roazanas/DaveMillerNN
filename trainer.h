#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QVector>
#include "nnet.h"

class Trainer : public QObject
{
    Q_OBJECT

public:
    Trainer(NNet &network, const QVector<double> &inputValues, QVector<unsigned> topology, QObject *parent = nullptr);

public slots:
    void startTraining(int maxEpochs);

signals:
    void progressUpdated(int progress);
    void trainingFinished();

private:
    NNet &neuralNetwork;
    const QVector<double> &inputValues;
    QVector<unsigned> topology;
};

#endif // TRAINER_H
