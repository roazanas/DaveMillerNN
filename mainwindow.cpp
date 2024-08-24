#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filepathvalidator.h"

QVector<unsigned> MainWindow::topology{0, 1, 0};
QString MainWindow::header = "No header";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    neuralNetwork(topology)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FilePathValidator* validator = new FilePathValidator(ui->trainDataPathEdit);
    ui->trainDataPathEdit->setValidator(validator);
    connect(validator, &FilePathValidator::valuesValidated, this, &MainWindow::onValuesValidated);

}

void MainWindow::on_trainDataPathButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose text file"),
                                                    QString(),
                                                    tr("*.*"));
    if (!fileName.isEmpty())
    {
        ui->trainDataPathEdit->setText(fileName);
        on_trainDataPathEdit_returnPressed();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_trainDataPathEdit_returnPressed()
{
    ui->statusbar->showMessage(
        QString("Input neurons: %1 | Hidden neurons: %2 | Output neurons: %3 | (%4): %5 train values: %6/%7")
            .arg(topology[0])
            .arg(topology[1])
            .arg(topology[2])
            .arg(header)
            .arg(inputValues.size())
            .arg(inputValues.size() * (topology[0])/(topology[0]+topology[2]))
            .arg(inputValues.size() * (topology[2])/(topology[0]+topology[2])));
    ui->NNactions->setEnabled(true);
}

void MainWindow::on_hiddenNeuronCount_valueChanged(int count)
{
    topology[1] = count;
    if (topology[0] != 0 && topology[2] != 0)
        on_trainDataPathEdit_returnPressed();
}

void MainWindow::onValuesValidated(const QList<double>& values)
{
    inputValues = values;
}

void MainWindow::on_initButton_clicked()
{
    neuralNetwork = NNet(topology);
    ui->initButton->setEnabled(false);
    ui->NNinputs->setEnabled(false);
}

void MainWindow::on_trainButton_clicked()
{
    int maxEpochs = 10000;
    trainer = new Trainer(neuralNetwork, inputValues, topology, this);

    connect(trainer, &Trainer::progressUpdated, this, &MainWindow::onProgressUpdated);
    connect(trainer, &Trainer::trainingFinished, this, &MainWindow::onTrainingFinished);

    trainer->startTraining(maxEpochs);
}

void MainWindow::onProgressUpdated(int value)
{
    ui->trainProcess->setValue(value);
}

void MainWindow::onTrainingFinished()
{
    ui->trainProcess->setValue(100);
    qDebug() << "Training completed!";

    QVector<double> resultValues;
    neuralNetwork.feedForward({0, 0});
    neuralNetwork.getResults(resultValues);
    qDebug() << resultValues;

    neuralNetwork.feedForward({0, 1});
    neuralNetwork.getResults(resultValues);
    qDebug() << resultValues;

    neuralNetwork.feedForward({1, 0});
    neuralNetwork.getResults(resultValues);
    qDebug() << resultValues;

    neuralNetwork.feedForward({1, 1});
    neuralNetwork.getResults(resultValues);
    qDebug() << resultValues;

    trainer->deleteLater();
}
