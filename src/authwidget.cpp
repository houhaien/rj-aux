#include "authwidget.h"

#include <iostream>
#include <QString>

#include <DApplication>

authWidget::authWidget(DMainWindow *parent) : QWidget(parent)
{

    /* data */


    this->account = new QString;
    this->password = new QString;
    this->sudo_pass = new QString;
    this->process = nullptr;
    this->command = new QString("./rj");
    this->cmd_args = new QStringList();
    this->netcard = new QString;



    /* get network card name  */
    this->network_list = new QStringList;
    foreach (QNetworkInterface i, QNetworkInterface::allInterfaces()) {
        if (i.flags().testFlag(QNetworkInterface::IsLoopBack)) continue;
        QString cardname = i.name();

        if (!cardname.isEmpty() &&
            cardname.startsWith("e", Qt::CaseInsensitive)) {
            this->netcard->append(cardname);
            continue;
        }
        this->network_list->append(cardname);
    }
    this->network_list->insert(0, *(this->netcard));



    /* control  */



    /* QTextEdit : show information */
    this->show_info_edit = new QTextEdit(this);
    this->show_info_edit->setReadOnly(true);
    this->show_info_edit->resize(640, 120);
    this->show_info_edit->move(0, 0);



    /* account label */
    this->label_acco_num = new DLabel(this);
    this->label_acco_num->setText("账号");
    /* Position */
    this->label_acco_num->move(200, 150);



    /* account line edit  */
    this->lineedit_acco_num = new DLineEdit(this);
    // this->lineedit_acco_num->setPlaceholderText("please input account");
    this->lineedit_acco_num->move(260, 145);
    /* connect signal to slot */



    /* password label */
    this->label_pass_num = new DLabel(this);
    this->label_pass_num->setText("密码");
    /* position */
    this->label_pass_num->move(200, 210);



    /* password line edit */
    this->passwordedit_pass_num = new DPasswordEdit(this);
    // this->passwordedit_pass_num->setPlaceholderText("please input password");
    this->passwordedit_pass_num->move(260, 205);



    /* sudo pass label */
    this->label_sudo_pass = new DLabel(this);
    this->label_sudo_pass->setText("sudo");
    this->label_sudo_pass->move(200, 270);



    /* sudo pass line edit */
    this->passwordedit_sudo_pass = new DPasswordEdit(this);
    this->passwordedit_sudo_pass->setText("123");
    this->passwordedit_sudo_pass->move(260, 265);



    /* net card label */
    this->label_choose_card = new DLabel(this);
    this->label_choose_card->setText("网卡");
    this->label_choose_card->move(200, 330);



    /* choose net card QComboBox */
    this->combox = new QComboBox(this);
    this->combox->move(260, 325);
    // this->combox->addItem("默认");
    this->combox->addItems(*(this->network_list));



    /* confirm button */
    this->button_confirm = new QPushButton(this);
    this->button_confirm->setText("认证");
    this->button_confirm->move(260, 430);



    /*  password checkbox  */
    this->checkbox = new QCheckBox(this);
    this->checkbox->move(200, 385);



    /* password checkbox label */
    this->label_chbox = new DLabel(this);
    this->label_chbox->setText("记住信息");
    this->label_chbox->move(230, 387);



    /* connect signal with slot */


    QWidget::connect(this->lineedit_acco_num, SIGNAL(textChanged(QString)),
                     this, SLOT(getLineEditText(QString)),
                     Qt::AutoConnection);
    QWidget::connect(this->passwordedit_pass_num, SIGNAL(textChanged(QString)),
                     this, SLOT(getPasswdText(QString)),
                     Qt::AutoConnection);
    QWidget::connect(this->passwordedit_sudo_pass, SIGNAL(textChanged(QString)),
                     this, SLOT(getSudopassText(QString)),
                     Qt::AutoConnection);
    QWidget::connect(this->combox, SIGNAL(activated(const QString&)),
                     this, SLOT(getNetCardText(QString)),
                     Qt::AutoConnection);
    QWidget::connect(this->button_confirm, SIGNAL(released()),
                     this, SLOT(triggerauthen()),
                     Qt::AutoConnection);

}

authWidget::~authWidget() {

    if (this->process != nullptr) {
        QProcess quit;
        quit.setWorkingDirectory(DApplication::applicationDirPath());
        quit.start("./rj", QStringList() << *(this->sudo_pass) << "-q");
        quit.waitForFinished();
        this->process->kill();
    }
}


/* slop */

void authWidget::getLineEditText(const QString& inputstr) {
    *(this->account) = inputstr;


}

void authWidget::getPasswdText(const QString& inputstr) {
    *(this->password) = inputstr;
}

void authWidget::getSudopassText(const QString& inputstr) {
    *(this->sudo_pass) = inputstr;
}

void authWidget::getNetCardText(const QString& card_name) {
    if (this->netcard == nullptr) this->netcard = new QString(card_name);
    else *(this->netcard) = card_name;
}



void authWidget::triggerauthen() {

    if (this->process != nullptr) {
        QProcess quit;
        quit.start("./rj", QStringList() << *(this->sudo_pass) << "-q");
        quit.waitForFinished();
        this->process->kill();
        delete this->process;
    }
    this->process = new QProcess(this);

    this->process->setWorkingDirectory(DApplication::applicationDirPath());

    this->cmd_args->clear();
    this->cmd_args->append(QStringList()
                           << *(this->sudo_pass)
                           // << "-S"
                           // << "./rjsupplicant"
                           << "-a"
                           << "1"
                           << "-d"
                           << "0"
                           << "-u"
                           << *(this->account)
                           << "-p"
                           << *(this->password)
                           << "-n"
                           << *(this->netcard));


    this->process->start(*(this->command), *(this->cmd_args));
    // qDebug() << this->process->waitForStarted(4000);
    // qDebug() << this->process->write(this->sudo_pass->toStdString().c_str());

    qDebug() << process->waitForStarted();

    QObject::connect(this->process, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(cmd_output()));
    QObject::connect(this->process, SIGNAL(readyReadStandardError()),
                     this, SLOT(cmd_errout()));
}


void authWidget::cmd_output() {
    qDebug() << "get command output";
    this->show_info_edit->append(QString::fromLocal8Bit(process->readAllStandardOutput()));
}

void authWidget::cmd_errout() {
    qDebug() << "get command out error";
    QString errStr = process->readAllStandardError().data();
    if (!QRegExp("\\s*").exactMatch(errStr) &&
        !errStr.contains("sysctl") &&
        !errStr.contains("write error") &&
        !errStr.contains("错误的文件描述符") &&
        !errStr.contains("[sudo]")) {
        QMessageBox::information(nullptr, "Error", errStr);
    }
}

