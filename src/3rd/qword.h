#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QAxObject>
#include <QAxWidget>
#include "qt_windows.h"

class QWord : public QObject
{
    Q_OBJECT

public:
    explicit QWord(QObject *parent = Q_NULLPTR);
    ~QWord();

public:
    QAxObject* getDocuments(){return m_documents;}
    QAxObject* getDocument(){return m_document;}
    QAxObject* getWordApp(){return m_word;}

    //获得word的版本号
    QString GetWordVersion();
public:
    /**************************************************************************/
    /* 文件   操作                                                            */
    /**************************************************************************/
    //保存操作内容
    void save();
    //关闭 退出 析构时候也会自动调用一次
    void close();
    //新建Word另存为
    void saveAs();
    //创建一个新的Word
    bool createNewWord();
    //打开一个现有的Word
    bool openWord(QString &filepath);
    //创建一个新的Word
    bool createNewWord(const QString& filePath );

    /**************************************************************************/
    /* 操作                                                            */
    /**************************************************************************/
    //表格自动拉伸列 0固定  1根据内容调整  2 根据窗口调整
    void setTableAutoFitBehavior(int flag);
    //设置页面0为纵向wdOrientPortrait  1为横向wdOrientLandscape
    void setPageOrientation(int flag);
    //设置页面视图，是web视图wdWebView 6还是页面视图wdPrintView 3 或者阅读视图wdReadingView 7
    void setWordPageView(int flag);
    //设置字体大小
    void setFontSize(int fontsize);
    //设置字体	比如 “宋体”
    void setFontName(QString& fontName);
    //字体加粗
    void setFontBold(bool flag);
    //设置选中位置文字居中 0 ,居左 1,居右 2
    void setParagraphAlignment(int flag);
    //设置列宽
    void setColumnWidth(int column, int width);
    //设置单元格文本
    void setCellString(int row, int column, const QString& text);
    //设置单元格字体内容粗体  isBold控制是否粗体
    void setCellFontBold(int row, int column, bool isBold);
    //设置单元格文字大小
    void setCellFontSize(int row, int column, int size);
    //设置审阅的拼写检查  true开启检查  false 取消检查
    void setOptionCheckSpell(bool flags);

    //获取内容
    QString GetText();
    void getUsedRange(int *topLeftRow, int *topLeftColumn, int *bottomRightRow, int *bottomRightColumn);
    //"SetRange(1, 9)"第1个字符后开始，到第9个字符结束范围
    void setSelectionRange(int start,int end);
    //获取单元格内容
    QVariant getCellValue(int row, int column);
    //获取word中表格总数
    int getTableCount();


    //获取代码中出现的错误信息可以用QMessageBox::information打印 在cpp不用QMessageBox 是怕你们在线程中调导出报表
    QString getStrErrorInfo(){return m_strError;}
    //删除指定的列
    void deleteSelectColumn(int column);
    //移动选定对象到文档末尾
    void moveForEnd();

    //单元格插入图片
    void insertCellPic(int row,int column,const QString& picPath);
    //插入一个几行几列表格
    void insertTable(int row,int column);
    //插入回车
    void insertMoveDown();
    //插入文字
    void insertText(const QString& text);

    void insertPic(QString& picPath);
    void setMargin(int top, int bottom, int left, int right);


private:
    QAxObject* m_word;
    QAxObject* m_documents;
    QAxObject* m_document;
    QString  m_fileName;
    QString  m_saveName;
    QString  m_strError;

};

