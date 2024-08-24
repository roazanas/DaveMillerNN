#ifndef FILEPATHVALIDATOR_H
#define FILEPATHVALIDATOR_H

#include <QValidator>
#include <QList>

class FilePathValidator : public QValidator
{
    Q_OBJECT
public:
    explicit FilePathValidator(QObject *parent = nullptr);

    State validate(QString &input, int &pos) const override;

signals:
    void valuesValidated(const QList<double> &values) const;
};

#endif // FILEPATHVALIDATOR_H
