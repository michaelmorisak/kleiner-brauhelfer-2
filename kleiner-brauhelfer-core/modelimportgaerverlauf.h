#ifndef MODELIMPORTGAERVERLAUF_H
#define MODELIMPORTGAERVERLAUF_H

#include <QDateTime>
#include <QVariant>

class SudObject;

class GaerverlaufMesswert
{
public:
    typedef QMap<int, QVariant> DBValue;

public:
    GaerverlaufMesswert(const QDateTime & timestamp);

    const QDateTime &getTimestamp() const;
    void setTemp(double temp);
    void setSre(double sre);
    void setBemerkung(double bemerkung);
    bool hasValues() const;

    bool update(const GaerverlaufMesswert & other);
    DBValue dbValues(const SudObject* sud) const;

private:
    QDateTime timestamp;
    QVariant temp;
    QVariant sre;
    QString bemerkung;
};

class GaerverlaufTable
{
public:
public:
    typedef QMap<QDateTime, GaerverlaufMesswert> GaerverlaufMap;
    typedef QList<GaerverlaufMesswert::DBValue> DBValueList;

public:
    GaerverlaufTable();
    bool addMesswert(const GaerverlaufMesswert & value);

    DBValueList getDBValues(const SudObject* sud) const;
private:
    GaerverlaufMap values;


};


#endif // MODELIMPORTGAERVERLAUF_H
