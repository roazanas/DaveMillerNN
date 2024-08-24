#include "filepathvalidator.h"
#include "mainwindow.h"

FilePathValidator::FilePathValidator(QObject *parent)
    : QValidator(parent)
{}

QValidator::State FilePathValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);
    QFileInfo fileInfo(input);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        return Intermediate;
    }

    QFile file(input);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return Intermediate;
    }

    QTextStream in(&file);
    QString firstLine = in.readLine();

    while (firstLine.isEmpty() || !firstLine.startsWith("Inputs: ")) {
        if (!firstLine.isEmpty() && firstLine[0] == '#') MainWindow::header = firstLine.removeFirst().trimmed();
        firstLine = in.readLine();
    }

    QRegularExpression firstLineRegex("^Inputs:\\s+(\\d+)\\s+\\|\\s+Outputs:\\s+(\\d+)$");
    QRegularExpressionMatch match = firstLineRegex.match(firstLine);
    if (!match.hasMatch()) {
        return Intermediate;
    }

    int inputs = match.captured(1).toInt();
    int outputs = match.captured(2).toInt();

    QVector<unsigned> top = MainWindow::getTopology();
    top[0] = inputs; top[2] = outputs;
    MainWindow::setTopology(top);

    QString secondLine = in.readLine();
    QStringList values = secondLine.split(' ', Qt::SkipEmptyParts);
    if (values.size() % (inputs + outputs) != 0) {
        return Intermediate;
    }

    bool ok;
    QList<double> dValues;
    foreach (const QString &value, values) {
        dValues.append(value.toDouble(&ok));
        if (!ok) {
            return Intermediate;
        }
    }
    emit valuesValidated(dValues);
    return Acceptable;
}
