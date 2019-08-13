/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "securitychunk.h"
#include "ui_securitychunk.h"

#include <QSslCertificate>
#include <QSslCertificateExtension>
#include <QPainter>
#include <QIcon>
#include <the-libs_global.h>

void tintImage(QImage &image, QColor tint) {
    int failNum = 0;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor pixelCol = image.pixelColor(x, y);
            if ((pixelCol.blue() > pixelCol.green() - 10 && pixelCol.blue() < pixelCol.green() + 10) &&
                    (pixelCol.green() > pixelCol.red() - 10 && pixelCol.green() < pixelCol.red() + 10)) {
            } else {
                failNum++;
            }
        }
    }

    if (failNum < (image.size().width() * image.size().height()) / 8) {
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.fillRect(0, 0, image.width(), image.height(), tint);
        painter.end();
    }
}

struct SecurityChunkPrivate {
    QPalette defaultPalette;
    QPalette evPalette;

    const QMap<QString, QString> EVOids = {
        {"Actalis", "1.3.159.1.17.1"},
        {"AffirmTrust", "1.3.6.1.4.1.34697.2.1"},
        {"AffirmTrust", "1.3.6.1.4.1.34697.2.2"},
        {"AffirmTrust", "1.3.6.1.4.1.34697.2.3"},
        {"AffirmTrust", "1.3.6.1.4.1.34697.2.4"},
        {"A-Trust", "1.2.40.0.17.1.22"},
        {"Buypass Class 3 Root CA", "2.16.578.1.26.1.3.3"},
        {"AC Camerfirma S.A. Chambers of Commerce Root - 2008", "1.3.6.1.4.1.17326.10.14.2.1.2"},
        {"AC Camerfirma S.A. Global Chambersign Root - 2008", "1.3.6.1.4.1.17326.10.8.12.1.2"},
        {"COMODO SECURE™", "1.3.6.1.4.1.6449.1.2.1.5.1"},
        {"DigiCert", "2.16.840.1.114412.2.1"},
        {"Entrust.net", "2.16.840.1.114028.10.1.2"},
        {"GeoTrust Primary Certificate Authority - G3", "1.3.6.1.4.1.14370.1.6"},
        {"GlobalSign", "1.3.6.1.4.1.4146.1.1"},
        {"Go Daddy Root Certificate Authority – G2", "2.16.840.1.114413.1.7.23.3"},
        {"Izenpe", "1.3.6.1.4.1.14777.6.1.1"},
        {"Izenpe", "1.3.6.1.4.1.14777.6.1.2"},
        {"Keynectis", "1.3.6.1.4.1.22234.2.5.2.3.1"},
        {"Network Solutions", "1.3.6.1.4.1.782.1.2.1.8.1"},
        {"QuoVadis Root CA 2", "1.3.6.1.4.1.8024.0.2.100.1.2"},
        {"SECOM Trust Systems", "1.2.392.200091.100.721.1"},
        {"Starfield Root Certificate Authority – G2", "2.16.840.1.114414.1.7.23.3"},
        {"StartCom Certification Authority", "1.3.6.1.4.1.23223.2"},
        {"StartCom Certification Authority", "1.3.6.1.4.1.23223.1.1.1"},
        {"SwissSign", "2.16.756.1.89.1.2.1.1"},
        {"thawte Primary Root CA - G3", "2.16.840.1.113733.1.7.48.1"},
        {"Trustwave", "2.16.840.1.114404.1.1.2.4.1"},
        {"VeriSign Class 3 Public Primary Certification Authority - G5", "2.16.840.1.113733.1.7.23.6"},
        {"Verizon Business", "1.3.6.1.4.1.6334.1.100.1"}
    };

    QImage getTintedImage(QString path, QColor tint) {
        QImage image = QIcon(path).pixmap(SC_DPI_T(QSize(16, 16), QSize)).toImage();
        tintImage(image, tint);
        return image;
    }

    bool isEvCertificate(QSslCertificate certificate) {
        if (certificate.subjectInfo(QSslCertificate::Organization).count() == 0) return false;
        if (certificate.subjectInfo(QSslCertificate::CountryName).count() == 0) return false;

        for (QSslCertificateExtension ext : certificate.extensions()) {
            if (ext.oid() == "2.5.29.32") { //OID for certificate policies
                for (QString oid : EVOids.values()) {
                    if (ext.value().toString().contains(oid)) return true;
                }
            }
        }

        return false;
    }
};

SecurityChunk::SecurityChunk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecurityChunk)
{
    ui->setupUi(this);

    d = new SecurityChunkPrivate();
    d->defaultPalette = this->palette();
    d->evPalette = this->palette();
    d->evPalette.setColor(QPalette::WindowText, QColor(0, 200, 0));
    d->evPalette.setColor(QPalette::Text, QColor(0, 200, 0));

    ui->text->setVisible(false);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

SecurityChunk::~SecurityChunk()
{
    delete ui;
    delete d;
}

void SecurityChunk::setCurrentCertificate(QSslCertificate certificate)
{
    if (certificate.isNull()) {
        this->setPalette(d->defaultPalette);
        ui->text->setVisible(true);
        ui->text->setText(tr("Insecure"));

        ui->securityIcon->setPixmap(QPixmap::fromImage(d->getTintedImage(":/icons/insecure.svg", this->palette().color(QPalette::WindowText))));
    } else if (d->isEvCertificate(certificate)) {
        this->setPalette(d->evPalette);

        if (certificate.subjectInfo(QSslCertificate::Organization).count() != 0 && certificate.subjectInfo(QSslCertificate::CountryName).count() != 0) {
            ui->text->setVisible(true);
            ui->text->setText(QStringLiteral("%1 [%2]").arg(certificate.subjectInfo(QSslCertificate::Organization).first())
                                                       .arg(certificate.subjectInfo(QSslCertificate::CountryName).first()));
        } else {
            ui->text->setVisible(false);
        }

        ui->securityIcon->setPixmap(QPixmap::fromImage(d->getTintedImage(":/icons/secure.svg", this->palette().color(QPalette::WindowText))));
    } else {
        this->setPalette(d->defaultPalette);
        ui->text->setVisible(false);
        ui->securityIcon->setPixmap(QPixmap::fromImage(d->getTintedImage(":/icons/secure.svg", this->palette().color(QPalette::WindowText))));
    }
}

void SecurityChunk::resizeEvent(QResizeEvent*event)
{
    emit resized();
}
