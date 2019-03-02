#ifndef ROHSTOFFAUSWAHLPROXYMODEL_H
#define ROHSTOFFAUSWAHLPROXYMODEL_H

#include "proxymodel.h"

class RohstoffAuswahlProxyModel : public ProxyModel
{
public:
    RohstoffAuswahlProxyModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    void setColumnMenge(int col);
    void setIndexMengeBenoetigt(const QModelIndex &index);
private:
    int mColMenge;
    QModelIndex mIndexMengeBenoetigt;
};

#endif // ROHSTOFFAUSWAHLPROXYMODEL_H
