#include "modelimportgaerverlauf.h"
#include "modelhauptgaerverlauf.h"
#include "biercalc.h"
#include "brauhelfer.h"

GaerverlaufMesswert::GaerverlaufMesswert(const QDateTime & itimestamp)
    : timestamp(itimestamp)
{
}

const QDateTime & GaerverlaufMesswert::getTimestamp() const
{
    return this->timestamp;
}

void GaerverlaufMesswert::setTemp(double temp)
{
    this->temp = temp;
}

void GaerverlaufMesswert::setSre(double sre)
{
    this->sre = sre;
}

void GaerverlaufMesswert::setBemerkung(double bemerkung)
{
    this->bemerkung = bemerkung;
}

bool GaerverlaufMesswert::hasValues() const
{
    return !(this->temp.isNull() && this->sre.isNull() && this->bemerkung.length() == 0);
}

bool GaerverlaufMesswert::update(const GaerverlaufMesswert & other)
{
    bool changed = false;
    if (!other.temp.isNull())
    {
       this->temp = (this->temp.toDouble() + other.temp.toDouble()) / 2.;
       changed = true;
    }

    if (!other.sre.isNull())
    {
       this->sre = (this->sre.toDouble() + other.sre.toDouble()) / 2.;
       changed = true;
    }

    if (other.bemerkung.length() != 0 and other.bemerkung != this->bemerkung)
    {
        this->bemerkung.append(" ");
        this->bemerkung.append(other.bemerkung);
        changed = true;
    }

    return changed;
}

GaerverlaufMesswert::DBValue GaerverlaufMesswert::dbValues(const SudObject * sud) const
{
   DBValue values = {{ModelHauptgaerverlauf::ColSudID, sud->id()},
                     {ModelHauptgaerverlauf::ColZeitstempel, this->timestamp}};

   if (!this->sre.isNull())
   {
       values[ModelHauptgaerverlauf::ColRestextrakt] = this->sre;
       values[ModelHauptgaerverlauf::ColAlc] = BierCalc::alkohol(sud->getSWIst(), this->sre.toDouble());
   }

   if (!this->temp.isNull())
   {
       values[ModelHauptgaerverlauf::ColTemp] = this->temp;
   }

   if (this->bemerkung.isEmpty())
   {
       values[ModelHauptgaerverlauf::ColBemerkung] = this->bemerkung;
   }
   return values;
}

GaerverlaufTable::GaerverlaufTable()
{
}

bool GaerverlaufTable::addMesswert(const GaerverlaufMesswert &value)
{
    bool changed = false;
    GaerverlaufMap::Iterator iter = this->values.find(value.getTimestamp());
    if (iter == this->values.end()) {
        this->values.insert(value.getTimestamp(), value);
        changed = true;
    } else {
        GaerverlaufMesswert & value = iter.value();
        if (value.update(value)) {
            changed = true;
        }

    }
    return changed;
}

GaerverlaufTable::DBValueList GaerverlaufTable::getDBValues(const SudObject * sud) const
{
    GaerverlaufTable::DBValueList result;

    for (GaerverlaufMesswert & value : this->values.values()) {
        if (value.hasValues())
        {
            result.append(value.dbValues(sud));
        }
    }
    return result;
}
