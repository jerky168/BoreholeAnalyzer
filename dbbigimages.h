#ifndef DBBIGIMAGES_H
#define DBBIGIMAGES_H

#include "precompiled.h"

class DbBigImages
{
public:
    long height;
    QByteArray imgData;

    DbBigImages() : height(0) {;}
    virtual ~DbBigImages() {;}
};

QX_REGISTER_HPP(DbBigImages, qx::trait::no_base_class_defined, 1)


#endif // DBBIGIMAGES_H
