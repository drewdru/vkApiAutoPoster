#ifndef VIEWWEBPAGE_H
#define VIEWWEBPAGE_H

#include <QDialog>
#include <QNetworkCookieJar>

namespace Ui {
class ViewWebPage;
}

class ViewWebPage : public QDialog
{
    Q_OBJECT

public:
    explicit ViewWebPage(QWidget *parent = 0);
    ~ViewWebPage();

    void setCookie(QNetworkCookieJar*jar);
    void setURL(QUrl url);
private:
    Ui::ViewWebPage *ui;
};

#endif // VIEWWEBPAGE_H
