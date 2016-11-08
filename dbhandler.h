#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>

#include "precompiled.h"
#include "dbbigimages.h"
#include <QxMemLeak.h>


class DbHandler : public QObject
{
    Q_OBJECT
public:
    explicit DbHandler(QObject *parent = 0);

signals:

public slots:
};

#endif // DBHANDLER_H
