#include "Settings.h"
#include "ui_Settings.h"
#include "ViewWebPage.h"
#include <QMessageBox>
#include <QWebView>

Settings::Settings(QWidget *parent) :
QDialog(parent),
ui(new Ui::Settings)
{
	ui->setupUi(this);
	this->setWindowTitle(tr("Настройки"));

	ui->frame->hide();

	ownerModel = new QSqlTableModel(this,QSqlDatabase::database());
	ownerModel->setTable("owners");
	qDebug()<< ownerModel->select();
	QSqlRecord rec = ownerModel->record();
    ownerModel->setHeaderData(rec.indexOf("name"), Qt::Horizontal, tr("Название"));
    ownerModel->setHeaderData(rec.indexOf("owner"), Qt::Horizontal, tr("ID страницы"));
	ui->ownerView->setModel(ownerModel);
	ui->ownerView->setColumnHidden(rec.indexOf("id"), true);
	ui->ownerView->setCurrentIndex(ownerModel->index(0,0));
	ui->ownerView->resizeColumnsToContents();

	lastPostModel = new QSqlTableModel(this,QSqlDatabase::database());
    lastPostModel->setTable("lastmedia");
	qDebug()<< lastPostModel->select();
    rec = lastPostModel->record();
    lastPostModel->setHeaderData(rec.indexOf("ownerid"), Qt::Horizontal, tr("ID страницы"));
    lastPostModel->setHeaderData(rec.indexOf("media"), Qt::Horizontal, tr("ID изображения"));
    ui->lastPostView->setModel(lastPostModel);
    ui->lastPostView->setColumnHidden(rec.indexOf("id"), true);
    ui->lastPostView->setCurrentIndex(lastPostModel->index(0,1));
    ui->lastPostView->resizeColumnsToContents();
}

Settings::~Settings()
{
	delete ui;
}

void Settings::on_insertOwner_clicked()
{
	ui->ownerOKButton->setText(tr("Добавить"));
	ui->frame->show();
    ui->ownerID->clear();
    ui->ownerName->clear();
}

void Settings::on_updateOwner_clicked()
{
	ui->ownerOKButton->setText(tr("Изменить"));
	ui->frame->show();
    ui->ownerID->clear();
    ui->ownerName->clear();
}

void Settings::on_deleteOwner_clicked()
{
	ownerModel->removeRow(ui->ownerView->currentIndex().row());
	QModelIndex ind = ui->ownerView->currentIndex();
	ownerModel->select();
	ui->ownerView->setCurrentIndex(ind);
}

void Settings::on_ownerCancelButton_clicked()
{
	ui->frame->hide();
	ui->ownerID->clear();
	ui->ownerName->clear();
}

void Settings::on_ownerOKButton_clicked()
{
	int ind = 0;
	if(ui->ownerOKButton->text() == "Добавить")
		ownerModel->insertRow(ind);
	else
		ind = ui->ownerView->currentIndex().row();

	QSqlRecord rec = ownerModel->record();
	ownerModel->setData(ownerModel->index(ind, rec.indexOf("name")),\
		ui->ownerName->text());
	ownerModel->setData(ownerModel->index(ind, rec.indexOf("owner")),\
		ui->ownerID->text());
	if (!ownerModel->submitAll()) {
		qDebug() << "Insertion error!";
	}
	ui->frame->hide();

	QModelIndex index= ui->ownerView->currentIndex();
	ownerModel->select();
	ui->ownerView->setCurrentIndex(index);
	ui->ownerID->clear();
	ui->ownerName->clear();
}

void Settings::setCookie(QNetworkCookieJar*jar)
{
    this->jar = jar;
}

void Settings::on_viewImageButton_clicked()
{
    QUrl imageURL;
    QSqlRecord rec = lastPostModel->record();
    imageURL.setUrl("https://m.vk.com/"+\
        lastPostModel->record(ui->lastPostView->currentIndex().row())\
            .value("media").toString());

    ViewWebPage wp;
    wp.setCookie(this->jar);
    wp.setURL(imageURL);
    wp.exec();
}
