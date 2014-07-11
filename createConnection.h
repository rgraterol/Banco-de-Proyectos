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

#ifndef CREATECONNECTION_H
#define CREATECONNECTION_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <iostream>
#include <QDesktopServices>
#include <QDir>

// Crea la conexión a la BD
static bool createConnection()
{

  //Nuevas líneas
  //Obtenemos el directorio en el que se almacenan los datos de las aplicaciones
  //QString dataDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  QDir dir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0]);
  std::cout<<QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0].toStdString()<<std::endl;
   // QDir dir("");
  //Creamos la ruta al fichero de la base de datos
  QString dbName = dir.filePath("Banco_de_Proyectos.sqlite");

 // std::cout << QStandardPaths::standardLocations(QStandardPaths::DataLocation)[2].toLocal8Bit().constData() <<endl;

  //Instanciamos QSqlDatabase y utilizamos dbName como ruta
  //al archivo de la base de datos
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbName);

  //Abre la base de datos y muestra un aviso en caso de error
  if (!db.open()) {
      QMessageBox msgBox;
      msgBox.setText(QString("No se pudo abrir la base de datos de %1").arg(dbName));
      msgBox.exec();
    return false;
  }
/*
  QString qNombre;
  QString qInstitucionFinancia;
  QString qInstitucionFinanciaDos;
  QString qPorcentajeInstitucionFinancia;
  QString qPorcentajeInstitucionFinanciaDos;
  QString qInstitucionEjecuta;
  QString qMontoAprobado;
  QString qMontoSolicitado;
  QString qFechaInicio;
  QString qFechaFin;
  QString qObjetivos;
  QString qComunidad;
  int qNumeroBeneficiarios;
  QString qAmbitoProyecto;
  QString qEstatus;
  QString qContacto;
  int qIndicador;
  QString qCodigo
  QString qComentario;
  datetime insercion
*/

  //Crea la tabla prueba
  QSqlQuery query;
 // query.addBindValue(7);
  if(query.exec("CREATE TABLE Proyecto(CodigoUnico char(20),Nombre char(100) UNIQUE,InstitucionEjecuta char(150),AmbitoProyecto char(50),InstitucionFinancia char(150),InstitucionFinanciaDos char(150),"
             "PorcentajeAporteInstitucion char(10),PorcentajeAporteInstitucionDos char(10),"
             "MontoAprobado char(20),MontoSolicitado char(20),FechaInicio datetime,FechaFin datetime,"
             "Objetivos varchar(5000),Comunidad char(150),NumeroBeneficiarios char(20),Estatus char(10),Contacto varchar(4000),"
             "Indicador char(10),Comentario varchar(5000),Insercion datetime,PRIMARY KEY(CodigoUnico))")){
      qDebug()<<query.lastError();
  }
  else
      //qDebug()<<query.lastError();




  return true;
}

#endif // CREATECONNECTION_H
