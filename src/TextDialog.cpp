#include "TextDialog.h"

TextDialog::TextDialog(QFont font, QWidget *parent) :
    QDialog(parent),
    closeFlag(false),
    curFont(font)
{
    resize(250, 100);
    textEdit = new QTextEdit;
    textEdit->setMinimumSize(250, 100);
    okBtn = new QPushButton("确认");
    okBtn->setMinimumSize(50, 30);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(okBtnClicked()));
    cancelBtn = new QPushButton("取消");
    cancelBtn->setMinimumSize(50, 30);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
    selectFontBtn = new QPushButton(tr("选择文本"));
    selectFontBtn->setMinimumSize(50, 30);
    connect(selectFontBtn, SIGNAL(clicked()), this, SLOT(selectFont()));

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(selectFontBtn);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textEdit);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
}

TextDialog::~TextDialog()
{

}

void TextDialog::okBtnClicked()
{
    closeFlag = true;
    this->close();
}

void TextDialog::selectFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, curFont, this);
    if (ok)
    {
        curFont = font;
        textEdit->setFocus();
    }
}

void TextDialog::setText(const QString& text)
{
    textEdit->setText(text);
}
