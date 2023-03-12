#include <QMessageBox>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "dlgimportgaerverlauf.h"
#include "ui_dlgimportgaerverlauf.h"


DlgImportGaerverlauf::DlgImportGaerverlauf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgImportGaerverlauf)
{
    ui->setupUi(this);
}

DlgImportGaerverlauf::~DlgImportGaerverlauf()
{
    delete ui;
}

void DlgImportGaerverlauf::on_btnDownload_clicked()
{
    QUrl url = ui->tbUrl->text();
    QString path = url.path();
    QStringList parts = path.split("/");
    QStringList newPathParts;
    for (QString part : parts) {
        if (part != "edit") {
            newPathParts.append(part);
        } else {
            break;
        }
    }

    newPathParts.append("export");
    QString newPath = newPathParts.join("/");
    url.setPath(newPath);
    QUrlQuery newQuery;
    newQuery.addQueryItem("format", "csv");
    newQuery.addQueryItem("gid", "0");
    url.setQuery(newQuery);

    QString content;
    QEventLoop loop;
    QNetworkRequest request(url);
    QNetworkAccessManager mNetManager;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    mNetManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
#endif

    QPointer<QNetworkReply> reply = mNetManager.get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    data.clear();
    if (reply->error() == QNetworkReply::NoError)
    {
        QString line = reply->readLine();
        while (!line.isEmpty()) {
            this->data.append(line);
            line = reply->readLine();
        }
        ui->importPreview->document()->setPlainText(data.join(""));
    }
}


