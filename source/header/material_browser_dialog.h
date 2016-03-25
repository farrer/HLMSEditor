/****************************************************************************
**
** Copyright (C) 2014
**
** This file is generated by the Magus toolkit
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
****************************************************************************/

#ifndef MATERIAL_BROWSER_DIALOG_H
#define MATERIAL_BROWSER_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include "constants.h"
#include "material_browser.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

/****************************************************************************
 This is the material browser
 ***************************************************************************/
class MaterialBrowserDialog : public QDialog
{
	Q_OBJECT

	public:
        MaterialBrowserDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
        ~MaterialBrowserDialog(void);
        void addMaterial(const QString& baseNameJson,
                         const QString& fullNameJson,
                         const QString& baseNameThumb,
                         EditorHlmsTypes type);
        QVector<Magus::QtResourceInfo*>& getResources (void);
        void setResources (const QVector<Magus::QtResourceInfo*>& resources);
        void initResourceTree(void);
        const QString& getSelectedJsonFileName(void) const {return mSelectedFileName;}
        void clearResources(void);

    private slots:
        void handleJsonFileSelectedToProcess(const QString& fullNameJson);
        void handleOkAndAccept(void);

    private:
        MaterialBrowserWidget* mResourceWidget;
        QString mSelectedFileName;
};

#endif

