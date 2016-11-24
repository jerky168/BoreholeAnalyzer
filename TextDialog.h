#pragma once

#include <QDialog>
#include <QTextEdit>
#include <QFontDialog>
#include <QPushButton>
#include <QHBoxLayout>

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    TextDialog(QFont font, QWidget *parent = Q_NULLPTR);
    ~TextDialog();

    void setText(const QString &text);
    QString getText() {return textEdit->toPlainText();}
    QFont getFont() {return curFont;}
    bool getCloseFlag() {return closeFlag;}

private:
    QFont curFont;
    QTextEdit *textEdit;
    QPushButton *selectFontBtn;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    bool closeFlag;

private slots:
    void okBtnClicked();
    void selectFont();

};
