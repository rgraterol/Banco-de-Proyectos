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

#ifndef TOOLS_H
#define TOOLS_H
#include <QWidget>
#include <QString>
#include "csv_manejador_db.h"

namespace Ui {
class tools;
}

class tools : public QWidget
{
    Q_OBJECT

public:
    explicit tools(QWidget *parent = 0);
    ~tools();

private slots:
    void on_pushButtonExportarPDF_clicked();

    void msj_error(QString error);

    void on_pushButtonClean_clicked();

    void on_pushButtonToolPdf_clicked();

    void on_pushButtonToolCsv_clicked();

    void on_pushButtonExportarDetallePdf_clicked();

    void on_pushButtonExportarCsv_clicked();

    void on_pushButtonSeleccioneCsv_clicked();

    void on_pushButtonImport_clicked();

    void on_pushButtonClean_2_clicked();

    void on_pushButtonExportarPDF_2_clicked();

    void on_pushButtonGoExportarCSV_clicked();

    void on_pushButtonHacerExport_clicked();

private:
    Ui::tools *ui;
    csv_manejador_DB *csv;
    QString direccionGloval;
    QString direccionExportCSV;
};

#endif // TOOLS_H
