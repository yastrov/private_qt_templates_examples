/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include "mainwindow.h"
#include "textedit.h"

//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), completer(nullptr)
{
    createMenu();

    completingTextEdit = new TextEdit(this);
    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    completingTextEdit->setCompleter(completer);

    setCentralWidget(completingTextEdit);
    resize(500, 300);
    setWindowTitle(tr("Completer"));
}
//! [0]

//! [1]
void MainWindow::createMenu()
{
    QAction *exitAction = new QAction(tr("Exit"), this);
    QAction *saveAction = new QAction(tr("Save"), this);
    QAction *loadAction = new QAction(tr("Load"), this);
    QAction *aboutAct = new QAction(tr("About"), this);
    QAction *aboutQtAct = new QAction(tr("About Qt"), this);

    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    connect(loadAction, &QAction::triggered, this, &MainWindow::load);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

    QAction *undoAction = new QAction(tr("Undo"), this);
    QAction *boldAction = new QAction(tr("Bold"), this);
    QAction *italicAction = new QAction(tr("Italic"), this);

//    connect(undoAction, &QAction::triggered, completingTextEdit, &TextEdit::undo);
    connect(boldAction, &QAction::triggered, this, &MainWindow::on_bold);
//    connect(italicAction, &QAction::triggered, completingTextEdit, &TextEdit::on_italic);

    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(exitAction);

    QMenu* editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(boldAction);
    editMenu->addAction(italicAction);

    QMenu* helpMenu = menuBar()->addMenu(tr("About"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
//! [1]

//! [2]
QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}
//! [2]

//! [3]
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"), tr("This example demonstrates the "
                       "different features of the QCompleter class."));
}
//! [3]

void MainWindow::on_bold(bool triggered)
{
    Q_UNUSED(triggered)
    completingTextEdit->on_bold();
}

void MainWindow::save()
{
    const QString filter = "HTML files (*.html);;ODF files (*.odt)";
    QString cfilter = "HTML files (*.html)";
    const QString fileName = QFileDialog::getSaveFileName(this,
                                                          tr("Save File"),
                                                          QDir::homePath(),
                                                          filter,
                                                          &cfilter);
    if(!fileName.isNull() && !fileName.isEmpty())
    {
        const QFileInfo qF(fileName);
        if(qF.exists())
        {
            const QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                            windowTitle(),
                                                                            tr("File already exists!\nOverwrite?"),
                                                                            QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No)
            {
                return;
            }
        }
        QString ext = qF.suffix().toLower();
        if(ext.isNull() || ext.isEmpty())
            ext = "odt";
        if(ext == "html") {
            completingTextEdit->exportToHTML(fileName);
        } else
            if(ext == "odt") {
                completingTextEdit->exportToODF(fileName);
            }
    }
}

void MainWindow::load()
{
    const QString filter = "HTML files (*.html)";
    QString cfilter = "HTML files (*.html)";
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open File"),
                                                          QDir::homePath(),
                                                          filter,
                                                          &cfilter);
    if(!fileName.isNull() && !fileName.isEmpty())
    {
        const QFileInfo qF(fileName);
        if(qF.exists())
        {
            completingTextEdit->importFromHTML(fileName);
        }
    }
}
