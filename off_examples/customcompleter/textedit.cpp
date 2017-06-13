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
** This is Custom Completer Example from Qt
**
****************************************************************************/

#include "textedit.h"

//! [0]
TextEdit::TextEdit(QWidget *parent)
: QTextEdit(parent), c(0)
{
    setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
                    " 3 characters. You can trigger autocompletion using ") +
                    QKeySequence("Ctrl+E").toString(QKeySequence::NativeText));
}
//! [0]

//! [1]
TextEdit::~TextEdit()
{
}
//! [1]

//! [2]
void TextEdit::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    connect(c, static_cast<void(QCompleter::*)(const QString &)>(&QCompleter::activated),
            this, &TextEdit::insertCompletion);
}
//! [2]

//! [3]
QCompleter *TextEdit::completer() const
{
    return c;
}
//! [3]

//! [4]
void TextEdit::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    const int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}
//! [4]

//! [5]
QString TextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
//! [5]

//! [6]
void TextEdit::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QTextEdit::focusInEvent(e);
}
//! [6]

//! [7]
void TextEdit::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    const bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
//! [7]

//! [8]
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    const static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    const QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
//! [8]

bool TextEdit::exportToODF(const QString &fileName)
{
    QTextDocumentWriter writer(fileName);
    writer.setFormat("odf");
    return writer.write(document());
}

bool TextEdit::exportToHTML(const QString &fileName)
{
    QTextDocumentWriter writer(fileName);
    writer.setFormat("HTML");
    return writer.write(document());
}

void TextEdit::setFontBoldS()
{
    QTextCursor cursor = textCursor();
    QTextCharFormat prev_format = cursor.charFormat();
    QTextCharFormat new_format;
    if(prev_format.fontWeight() == QFont::Bold) {
        new_format.setFontWeight(QFont::Normal);
    } else {
        new_format.setFontWeight(QFont::Bold);
    }
    cursor.mergeCharFormat(new_format);
    setTextCursor(cursor);
}

void TextEdit::setFontItalicS()
{
    QTextCursor cursor = textCursor();
    QTextCharFormat prev_format = cursor.charFormat();
    QTextCharFormat new_format;
    new_format.setFontItalic(!prev_format.fontItalic());
    cursor.mergeCharFormat(new_format);
    setTextCursor(cursor);
}

void TextEdit::on_bold(bool triggered)
{
    Q_UNUSED(triggered)
    setFontBoldS();
}

void TextEdit::on_italic(bool triggered)
{
    Q_UNUSED(triggered)
    setFontItalicS();
}

void TextEdit::dropEvent(QDropEvent *event)
{
    QTextEdit::dropEvent(event);
}

// Вставка MIME данных
void TextEdit::insertFromMimeData(const QMimeData *source)
{
    //    qDebug() << "TextEdit::insertFromMimeData";
    //    for(const QString &format: source->formats() ) {
    //        qDebug() << format;
    //    }
    //    for(const QUrl &url: source->urls() ) {
    //        qDebug() << url;
    //    }
    if(source->hasImage())
    {
        // Принятые данные преобразуются в тип QImage
        // QImage image = source->imageData().value<QImage>();
        const QImage image = qvariant_cast<QImage>(source->imageData());
        if(!image.isNull()){
            // Генерируется имя ресурса
            const QString image_name = QString("image%1").arg(qrand());
            addImageToResAndInsert(image_name, image);
        } else {
            emit error(tr("Can't read image data from MimeData!"));
        }
        return;
    }

    QStringList formats;
    formats << "UsingDefaultDragImage" << "DragImageBits";
    for(const QString &format: formats) {
        QByteArray data = source->data(format);
        if(!data.isNull() && !data.isEmpty()) {
            QBuffer buffer(&data);
            const QImage image = getImageFromQIODevice(&buffer);
            if(!image.isNull()) {
                // Генерируется имя ресурса
                const QString image_name = QString("image%1").arg(qrand());
                addImageToResAndInsert(image_name, image);
                return;
            }
        }
    }

    for(const QUrl &url: source->urls() ) {
        if(!url.isLocalFile()) {
            emit error(tr("Can't insert image from non-local!"));
//            QTextCursor cursor = this->textCursor();
//            cursor.insertText(url.toString());
            addLink(url.toString());
            continue;
        }
        const QString path = url.toLocalFile();
        const QImage image = getImageFromLocalDrive(path);
        QString image_name = QString("image%1").arg(qrand());
        addImageToResAndInsert(image_name, image);
    }
}

QImage TextEdit::getImageFromLocalDrive(const QString &path)
{
    if(path.isNull() || path.isEmpty())
        return QImage();
    QImageReader reader(path);
    if(!reader.autoDetectImageFormat())
        reader.setAutoDetectImageFormat(true);
    if(!reader.canRead()) {
        emit error(reader.errorString());
        return QImage();
    }
    const QImage image = reader.read();
    if(!image.isNull()) {
        emit error(reader.errorString());
    }
    return image;
}

QImage TextEdit::getImageFromQIODevice(QIODevice *device)
{
    QImageReader reader(device);
    if(!reader.autoDetectImageFormat())
        reader.setAutoDetectImageFormat(true);
    if(!reader.canRead()) {
        emit error(reader.errorString());
        return QImage();
    }
    const QImage image = reader.read();
    if(!image.isNull()) {
        emit error(reader.errorString());
    }
    return image;
}

void TextEdit::addImageToResAndInsert(const QString &name, const QImage &image)
{
    if(image.isNull()) return;
    QTextCursor cursor = this->textCursor();
    QTextDocument *document = this->document();
    // Ресурс добавляется в хранилище ресурсов документа
    document->addResource(QTextDocument::ImageResource, QUrl(name), image);
    // Картинка вставляется в текст
    cursor.insertImage(name);
}

