/*--- Banco de Proyectos Versión 1.0
 *
 * Copyright 2014 Jesús Salcedo - Ricardo Graterol.

 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QFileDialog>
#include <QtPrintSupport/qprinter.h>
#include <qpainter.h>
#include "pdfprinter_db.h"
#include "ayuda.h"
#include "tools.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButtonConsult_clicked();

    void on_pushButtonInsert_clicked();

    void on_pushButtonList_clicked();

    void on_pushButtonSaveInsertar_clicked();

    void on_pushButtonSerchConsult_clicked();

    void showError(const QSqlError &err);

    void msj_error(QString);

    void on_pushButtonNotFound_clicked();

    void on_pushButtonRegresar_clicked();

    void editSlot(const QModelIndex &);

    void on_pushButtonResetConsult_clicked();

    void on_pushButtonBackInsertar_clicked();

    void on_pushButtonResetInsert_clicked();

    void on_pushButton_4_clicked();

    void about();

    void aboutSqlite();

    void importDB();

    bool exportDB();

    //void on_pushButtonIE_clicked();

    void on_pushButtonModificar_clicked();

    //void on_pushButtonResetInsert_2_clicked();

    void on_pushButtonBackModificar_clicked();

    void on_pushButtonEliminarProyecto_clicked();

    void on_pushButtonReturnConsulta_clicked();

    void on_pushButtonGuardarModificar_clicked();

    void on_pushButton_clicked();

    void on_pushButtonListarLista_clicked();

    void ayuda_clicked();

    void on_pushButtonPdfProyecto_clicked();

    void on_pushButtonCSVProyecto_clicked();

    void importar_exportar();

    void on_pushButton_2_clicked();

    void on_pushButtonBuscarEstats_clicked();

private:
    Ui::MainWindow *ui;

    QSqlRelationalTableModel *model;
    int institucionEjecutaIdx, institucionFinanciaIdx;

    void clearInsert();

    ayuda * help;
    tools * herramientas;

    QString aux_q;

    QString pdf_proyecto;

    QString proyect_a_modificar;

};

#endif // MAINWINDOW_H
