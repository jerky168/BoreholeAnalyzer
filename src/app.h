#pragma once

#include <QObject>
#include <QString>
#include <QDir>

#define     Major_Version_Number    "1"
#define     Minor_Version_Number    "1"
#define     Revision_Number         "2"
#define     Build_Number            "20170109"

#define     App_Name_CN         QObject::tr("BoreholeAnalyzer")
#define     App_Name_EN         "BoreholeAnalyzer"
#define     App_Version         QString(Major_Version_Number) + "." + Minor_Version_Number \
                                + "." + Revision_Number + " build-" + Build_Number
#define     Company_Name_CN     QObject::tr("Y-Link")
#define     Company_Name_EN     "Y-Link"
#define     Company_Url         "http://www.y-link.cn/"


#define     Default_Folder  QDir::homePath() + "/" + App_Name_EN
