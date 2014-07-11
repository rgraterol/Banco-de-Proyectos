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

#include "ayuda.h"
#include "ui_ayuda.h"

ayuda::ayuda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ayuda)
{   
    ui->setupUi(this);
    this->setWindowTitle("Ayuda del Banco de Proyectos");
    ui->stackedWidgetAyuda->setCurrentIndex(0);
}

ayuda::~ayuda()
{
    delete ui;
}

void ayuda::on_pushButtonSiguiente_clicked()
{
    int aux=ui->stackedWidgetAyuda->currentIndex();
    ui->stackedWidgetAyuda->setCurrentIndex(aux+1);
}

void ayuda::on_pushButtonAtras_clicked()
{
    int aux=ui->stackedWidgetAyuda->currentIndex();
    ui->stackedWidgetAyuda->setCurrentIndex(aux-1);
}
