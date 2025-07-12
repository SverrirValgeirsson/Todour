/********************************************************************************
** Form generated from reading UI file 'aboutbox.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTBOX_H
#define UI_ABOUTBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_AboutBox
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_4;
    QLabel *lblCurrentVersion;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *AboutBox)
    {
        if (AboutBox->objectName().isEmpty())
            AboutBox->setObjectName(QString::fromUtf8("AboutBox"));
        AboutBox->resize(400, 303);
        buttonBox = new QDialogButtonBox(AboutBox);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 270, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        label = new QLabel(AboutBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 0, 61, 16));
        label_4 = new QLabel(AboutBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(120, 50, 161, 16));
        label_4->setTextFormat(Qt::RichText);
        label_4->setOpenExternalLinks(true);
        label_4->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        lblCurrentVersion = new QLabel(AboutBox);
        lblCurrentVersion->setObjectName(QString::fromUtf8("lblCurrentVersion"));
        lblCurrentVersion->setGeometry(QRect(140, 20, 211, 20));
        textBrowser = new QTextBrowser(AboutBox);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(30, 80, 341, 181));

        retranslateUi(AboutBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), AboutBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AboutBox, SLOT(reject()));

        QMetaObject::connectSlotsByName(AboutBox);
    } // setupUi

    void retranslateUi(QDialog *AboutBox)
    {
        AboutBox->setWindowTitle(QCoreApplication::translate("AboutBox", "About Todour", nullptr));
        label->setText(QCoreApplication::translate("AboutBox", "Todour", nullptr));
        label_4->setText(QCoreApplication::translate("AboutBox", "<html><head/><body><p><a href=\"http://nerdur.com/todour\"><span style=\" text-decoration: underline; color:#0000ff;\">https://nerdur.com/todour</span></a></p></body></html>", nullptr));
        lblCurrentVersion->setText(QString());
        textBrowser->setHtml(QCoreApplication::translate("AboutBox", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">Copyright 2012-2018 Sverrir Valgeirsson</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Monaco'; font-size:9pt; color:#5c5855;\">THE SOFTWARE IS PROVIDED &quot;AS IS&quot;, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0p"
                        "x; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Monaco'; font-size:9pt; color:#5c5855;\">IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.</span><span style=\" font-size:9pt;\"> </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:9pt;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutBox: public Ui_AboutBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTBOX_H
