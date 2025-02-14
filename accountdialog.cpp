﻿/*
 * Copyright (C) 2016 - 2021 Andy Weiss, Adi Hilber
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "accountdialog.h"
#include "ui_accountdialog.h"
#include <QFileDialog>


AccountDialog::AccountDialog(QWidget *parent, s_account *account, CmdFacade *lib) :
    QDialog(parent),
    ui(new Ui::AccountDialog),
    m_s_account(account), m_lib(lib)
{
    for (auto && buddy : m_lib->getBuddies()) {
        s_account* buddyaccount = m_lib->getAccountByUID(buddy.accUid);
        if(buddyaccount != nullptr && buddyaccount->serverURI == account->serverURI){                     // only append buddys that are on the same server
            m_buddies.append(buddy);
        }
    }
    ui->setupUi(this);
    editedaccount = *m_s_account;
    ui->lineEdit_Name->setText(m_s_account->name);
    ui->lineEdit_server->setText(m_s_account->serverURI);
    ui->lineEdit_user->setText(m_s_account->user);
    ui->lineEdit_password->setText("******");
    ui->spinBox_JB_FixedDel->setSuffix("ms");
    ui->spinBox_JB_FixedDel->setMinimum(0);
    ui->spinBox_JB_FixedDel->setMaximum(100000);
    ui->spinBox_JB_FixedDel->setValue(m_s_account->fixedJitterBufferValue);
    ui->comboBox_JBType->addItem("adaptive");
    ui->comboBox_JBType->addItem("fixed");
    ui->comboBox_JBType->setCurrentIndex(m_s_account->fixedJitterBuffer);
    for (auto && buddy : m_buddies) {
     ui->comboBox_AutoconBuddy->addItem(buddy.Name);
    }
    ui->comboBox_AutoconBuddy->addItem("no autoconnect");
    ui->comboBox_AutoconBuddy->setCurrentIndex(ui->comboBox_AutoconBuddy->count()-1);       // select no autoconnect item if no buddy is assigned
    for (auto && buddy : m_buddies) {
     if(buddy.uid == m_s_account->autoconnectToBuddyUID){
         int index = ui->comboBox_AutoconBuddy->findText(buddy.Name);
         if ( index != -1 ) { // -1 for not found
            ui->comboBox_AutoconBuddy->setCurrentIndex(index);
            break;
         }
     }
    }
    if(!m_s_account->FileRecordPath.isEmpty()){
        QString path = m_s_account->FileRecordPath;
        int pos = path.lastIndexOf(QChar('/'));
        path = path.left(pos);
        QString templname = m_s_account->FileRecordPath;
        templname = templname.right(templname.length() - pos-1);
        ui->lineEdit_rec_template->setText(templname);
        ui->lineEdit_recdir->setText(path);
        ui->checkBox_enablerec->setChecked(1);
    }
    else{
        ui->lineEdit_recdir->setHidden(1);
        ui->lineEdit_rec_template->setHidden(1);
        ui->label_rectemplate->setHidden(1);
        ui->label_recorddir->setHidden(1);
        ui->label_tempdef1->setHidden(1);
        ui->label_tempdef2->setHidden(1);
        ui->label_tempdef3->setHidden(1);
    }

   if(!m_s_account->FilePlayPath.isEmpty()){
        ui->lineEdit_playpath->setText(m_s_account->FilePlayPath);
        ui->checkBox_enableplayer->setChecked(1);
   }
   else{
       ui->lineEdit_playpath->setHidden(1);
       ui->label_filepah->setHidden(1);
   }





}

AccountDialog::~AccountDialog()
{
    delete ui;
}

s_account AccountDialog::getAccount()
{
    return *m_s_account;
}

void AccountDialog::on_buttonBox_accepted()
{
    if(ui->checkBox_enablerec->isChecked()){
        editedaccount.FileRecordPath.append("/");
        editedaccount.FileRecordPath.append(ui->lineEdit_rec_template->text());
    }
    else editedaccount.FileRecordPath.clear();

    m_s_account = &editedaccount;
    AccountDialog::close();
}

void AccountDialog::on_buttonBox_rejected()
{
   AccountDialog::close();
}

void AccountDialog::on_lineEdit_Name_textChanged(const QString &arg1)
{
    editedaccount.name = arg1;
}

void AccountDialog::on_lineEdit_server_textChanged(const QString &arg1)
{
    editedaccount.serverURI = arg1;
}


void AccountDialog::on_lineEdit_user_textChanged(const QString &arg1)
{
    editedaccount.user = arg1;
}

void AccountDialog::on_lineEdit_password_textEdited(const QString &arg1)
{
    editedaccount.password = arg1;
}

void AccountDialog::on_checkBox_enablerec_stateChanged(int arg1)
{
    if(arg1){
        ui->lineEdit_recdir->setHidden(0);
        ui->label_recorddir->setHidden(0);
        ui->lineEdit_rec_template->setHidden(0);
        ui->label_rectemplate->setHidden(0);
        ui->label_tempdef1->setHidden(0);
        ui->label_tempdef2->setHidden(0);
        ui->label_tempdef3->setHidden(0);
    }
    else{
        ui->lineEdit_recdir->setHidden(1);
        ui->label_recorddir->setHidden(1);
        ui->lineEdit_rec_template->setHidden(1);
        ui->label_rectemplate->setHidden(1);
        ui->label_tempdef1->setHidden(1);
        ui->label_tempdef2->setHidden(1);
        ui->label_tempdef3->setHidden(1);
        editedaccount.FileRecordPath.clear();
   }
}

void AccountDialog::on_checkBox_enableplayer_stateChanged(int arg1)
{
    if(arg1){
        ui->lineEdit_playpath->setHidden(0);
        ui->label_filepah->setHidden(0);
    }
    else{
        ui->lineEdit_playpath->setHidden(1);
        ui->label_filepah->setHidden(1);
        editedaccount.FilePlayPath.clear();
    }
}

void AccountDialog::on_lineEdit_recdir_textChanged(const QString &arg1)
{
    editedaccount.FileRecordPath = arg1;
}

void AccountDialog::on_lineEdit_playpath_textChanged(const QString &arg1)
{
    editedaccount.FilePlayPath = arg1;
}

void AccountDialog::on_comboBox_JBType_currentIndexChanged(int index)
{
    if(index){
        ui->spinBox_JB_FixedDel->show();
        editedaccount.fixedJitterBuffer = true;
    }
    else{
        ui->spinBox_JB_FixedDel->hide();
        editedaccount.fixedJitterBuffer = false;
    }
}

void AccountDialog::on_spinBox_JB_FixedDel_valueChanged(int arg1)
{
    editedaccount.fixedJitterBufferValue = arg1;
}

void AccountDialog::on_comboBox_AutoconBuddy_currentIndexChanged(int index)
{
    if(index < m_buddies.count()){
        editedaccount.autoconnectToBuddyUID = m_buddies.at(index).uid;
    }
    else{
        editedaccount.autoconnectToBuddyUID = "";
    }
}

