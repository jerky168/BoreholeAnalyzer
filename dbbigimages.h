#ifndef DBBIGIMAGES_H
#define DBBIGIMAGES_H


class DbBigImages
{
public:
    long height;
    QByteArray imgData;

    DbBigImages() {;}
    virtual ~DbBigImages() {;}
};

#endif // DBBIGIMAGES_H
