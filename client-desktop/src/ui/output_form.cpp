#include "output_form.h"
#include <QVBoxLayout>

OutputForm::OutputForm(QWidget *parent) : QWidget(parent){
    setup_ui();
}

OutputForm::~OutputForm(){
    if(this->edit_info_){
        delete this->edit_info_;
    }
}

void OutputForm::setup_ui(){
    this->edit_info_ = new QTextEdit;
    this->edit_info_->setHtml(
        "<h2>数据导出</h2>"
        "<p>功能尚未实现</p>"
        );
    this->edit_info_->setReadOnly(true);

    QVBoxLayout* lyt_main = new QVBoxLayout(this);
    lyt_main->addWidget(this->edit_info_);
}
