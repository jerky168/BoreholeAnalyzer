#pragma once

#include <QObject>
#include <QAxObject>
#include <QFile>
#include <QDebug>

class QExcel : public QObject
{
    Q_OBJECT
public:
    explicit QExcel(QObject *parent = Q_NULLPTR);
    ~QExcel();

public:
    bool createNewExcel();

    bool Open(quint32 nSheet = 1, bool visible = false);//打开xls文件
    bool Open(QString xlsFile, quint32 nSheet = 1, bool visible = false);
    void Save();                //保存xls报表
    void Close();               //关闭xls报表

    void setCellString(const QString& cell, const QString& value);
    void setCellString(int row, int column, const QString& value);
    void mergeCells(const QString& cell);
    void mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn);
    void setColumnWidth(int column, int width);
    void setRowHeight(int row, int height);
    void setRowColumnAuto();
    bool IsOpen();
    bool IsValid();

    

    QAxObject* getWorkbooks(){return pWorkbooks;}
    QAxObject* getWorkbook(){return pWorkbook;}
    QAxObject* getExcelApp(){return pExcel;}
    QString GetExcelVersion();

protected:
    void Clear();

private:
    QAxObject *pExcel;      //指向整个excel应用程序
    QAxObject *pWorkbooks;  //指向工作簿集,excel有很多工作簿
    QAxObject *pWorkbook;   //指向sXlsFile对应的工作簿
    QAxObject *pWorkSheets; //指向工作簿的所有sheet表单
    QAxObject *pWorksheet;  //指向工作簿中的某个sheet表单

    QString   sXlsFile;     //xls文件路径
    quint32   nCurrSheet;   //当前打开的第几个sheet
    bool      bIsVisible;   //excel是否可见
    int       nRowCount;    //行数
    int       nColumnCount; //列数
    int       nStartRow;    //开始有数据的行下标值
    int       nStartColumn; //开始有数据的列下标值

    bool      bIsOpen;      //是否已打开
    bool      bIsValid;     //是否有效
    bool      bIsANewFile;  //是否是一个新建xls文件，用来区分打开的excel是已存在文件还是有本类新建的
    bool      bIsSaveAlready;//防止重复保存

    QString   m_strError;   //错误信息
};

