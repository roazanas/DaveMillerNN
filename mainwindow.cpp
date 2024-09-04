#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filepathvalidator.h"

QVector<unsigned> MainWindow::topology{0, 1, 0};
QString MainWindow::header = "No header";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    neuralNetwork(topology),
    ui(new Ui::MainWindow)
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
                                                    "*.*");
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
    updateStatusBar();
    ui->initButton->setEnabled(true);
}

void MainWindow::updateStatusBar()
{
    QString message;
    if (header == "No header") {
        message = tr("Input neurons: %1 | Hidden neurons: %2 | Output neurons: %3 | %4 train values: %5/%6");
        ui->statusbar->showMessage(
            QString(message)
                .arg(topology[0])
                .arg(topology[1])
                .arg(topology[2])
                .arg(inputValues.size())
                .arg(inputValues.size() * (topology[0])/(topology[0]+topology[2]))
                .arg(inputValues.size() * (topology[2])/(topology[0]+topology[2])));
    } else {
        message = tr("Input neurons: %1 | Hidden neurons: %2 | Output neurons: %3 | (%4): %5 train values: %6/%7");
        ui->statusbar->showMessage(
            QString(message)
                .arg(topology[0])
                .arg(topology[1])
                .arg(topology[2])
                .arg(header)
                .arg(inputValues.size())
                .arg(inputValues.size() * (topology[0])/(topology[0]+topology[2]))
                .arg(inputValues.size() * (topology[2])/(topology[0]+topology[2])));
    }

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
    updateStatusBar();
}

void MainWindow::on_initButton_clicked()
{
    neuralNetwork = NNet(topology);
    ui->initButton->setEnabled(false);
    ui->trainButton->setEnabled(true);
    ui->NNinputs->setEnabled(false);
    ui->IOfields->setEnabled(true);

    updateInputList();
    updateOutputList(QVector<double>(topology[2], 0.0));

    connect(ui->inputList, &QListWidget::itemChanged, this, &MainWindow::onInputItemChanged);
}

void MainWindow::on_trainButton_clicked()
{
    trainer = new Trainer(neuralNetwork, inputValues, topology, this);

    connect(trainer, &Trainer::progressUpdated, this, &MainWindow::onProgressUpdated);

    ui->NNactions->setEnabled(false);
    trainer->startTraining(maxEpochs);
}

void MainWindow::onProgressUpdated(int value)
{
    ui->trainProcess->setValue(value);
    QCoreApplication::processEvents();
}

void MainWindow::updateInputList()
{
    ui->inputList->clear();
    for (unsigned i = 0; i < topology[0]; ++i) {
        QListWidgetItem *item = new QListWidgetItem(QString::number(0.0), ui->inputList);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->inputList->addItem(item);
    }
}

void MainWindow::updateOutputList(const QVector<double> &outputValues)
{
    ui->outputList->clear();
    for (double value : outputValues) {
        QListWidgetItem *item = new QListWidgetItem(QString::number(value), ui->outputList);
        ui->outputList->addItem(item);
    }
}

void MainWindow::onInputItemChanged(QListWidgetItem *item)
{
    QVector<double> inputValues;
    for (int i = 0; i < ui->inputList->count(); ++i) {
        bool ok;
        double value = ui->inputList->item(i)->text().toDouble(&ok);
        if (ok) {
            inputValues.push_back(value);
        } else {
            inputValues.push_back(0.0);
            item->setText("0.0");
        }
    }

    neuralNetwork.feedForward(inputValues);

    QVector<double> outputValues;
    neuralNetwork.getResults(outputValues);
    updateOutputList(outputValues);
}

void MainWindow::on_epochInput_valueChanged(int value)
{
    maxEpochs = value;
}
