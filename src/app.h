#pragma once

#include <QObject>
#include <QString>

#define     Major_Version_Number    "0"
#define     Minor_Version_Number    "4"
#define     Revision_Number         "0"
#define     Build_Number            "0104"

#define     App_Name        QObject::tr("BoreholeAnalyzer")
#define     App_Version     QString(Major_Version_Number) + "." + Minor_Version_Number \
                            + "." + Revision_Number + " build-" + Build_Number
#define     Company_Name    QObject::tr("Y-Link")
#define     Company_Url     "http://www.y-link.cn/"

