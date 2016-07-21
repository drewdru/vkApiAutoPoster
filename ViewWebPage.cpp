#include "ViewWebPage.h"
#include "ui_ViewWebPage.h"

ViewWebPage::ViewWebPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewWebPage)
{
    ui->setupUi(this);
}

ViewWebPage::~ViewWebPage()
{
    delete ui;
}
void ViewWebPage::setCookie(QNetworkCookieJar*jar)
{
    ui->webView->page()->networkAccessManager()->setCookieJar(jar);
}

void ViewWebPage::setURL(QUrl url)
{
    ui->webView->setUrl(url);
}
