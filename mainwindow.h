#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "nnet.h"
#include <QMainWindow>
#include <QVector>
#include <QFileDialog>
#include <QValidator>
#include <QFileInfo>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void setTopology(QVector<unsigned> top) { topology = top; };
    static QVector<unsigned> getTopology() { return topology; };
    static QString header;

private slots:
    void on_trainDataPathButton_clicked();
    void on_trainDataPathEdit_returnPressed();
    void on_hiddenNeuronCount_valueChanged(int count);
    void onValuesValidated(const QList<double> &values);
    void on_initButton_clicked();
    void on_trainButton_clicked();

private:
    static QVector<unsigned> topology;
    QVector<double> inputValues;
    QVector<double> targetValues;
    QVector<double> resultValues;
    NNet neuralNetwork;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
