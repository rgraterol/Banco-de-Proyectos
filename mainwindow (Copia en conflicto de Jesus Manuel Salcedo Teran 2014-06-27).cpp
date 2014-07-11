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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "manejadorbasededatos.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <QTextStream>
#include <QMessageBox>
#include <QtSql>
#include <QImage>
#include <QDate>
#include <QIcon>
#include <QSortFilterProxyModel>
#include "createConnection.h"
#include "pdfprinter_db.h"



MainWindow::MainWindow(QWidget *parent) :    QMainWindow(parent),    ui(new Ui::MainWindow)
{
    //this->parentWidget()->showFullScreen();
    ui->setupUi(this);
    this->setWindowTitle("Banco de Proyectos V1.0");
    createConnection();
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(this, "Unable to load database", "This demo needs the SQLITE driver");
    ui->stackedWidget->setCurrentIndex(5);
    this->setWindowState(Qt::WindowMaximized);
    ui->pushButtonInsert->setIcon(QIcon("images/insert.png"));
    ui->pushButtonConsult->setIcon(QIcon("images/consult.png"));
    ui->pushButtonList->setIcon(QIcon("images/list.png"));
    ui->pushButton_4->setIcon(QIcon("images/exit.png"));
    QImage image("images/bancomain.jpg");
    ui->label_12->setPixmap(QPixmap::fromImage(image));
    ui->pushButtonSaveInsertar->setIcon(QIcon("images/save.png"));
    ui->pushButtonBackInsertar->setIcon(QIcon("images/back.png"));
    ui->pushButtonBackModificar->setIcon(QIcon("images/back.png"));

    help = new ayuda();
    herramientas = new tools();

    connect (ui->actionAcerca_de, SIGNAL(triggered()),this, SLOT(about()));
    connect (ui->actionAcerca_de_Qt, SIGNAL(triggered()),qApp, SLOT(aboutQt()));
    connect (ui->actionAcerca_de_Sqlite, SIGNAL(triggered()), this, SLOT(aboutSqlite()));
    connect (ui->actionImportar_Banco_de_Datos_2, SIGNAL(triggered()), this, SLOT(importDB()));
    connect (ui->lineEditVariableConsult, SIGNAL(returnPressed()), this, SLOT(on_pushButtonSerchConsult_clicked()));
    connect (ui->tableViewConsulta, SIGNAL(clicked(QModelIndex)), this, SLOT(editSlot(QModelIndex)));
    connect (ui->actionExportar_a_PDF_y_Hoja_de_Calculo, SIGNAL(triggered()), this, SLOT(on_pushButtonIE_clicked()));
    connect (ui->actionExportar_Banco_de_Datos_2, SIGNAL(triggered()), this, SLOT(exportDB()));
    connect (ui->actionSalir, SIGNAL(triggered()), this, SLOT(on_pushButton_4_clicked()));
    connect (ui->actionAyuda_2, SIGNAL(triggered()), this, SLOT(ayuda_clicked()));

    ui->dateEditFechaInicio->setDate(QDate::currentDate());
    ui->dateEditFechaFin->setDate(QDate::currentDate());
    ui->dateEditFechaInicio->setCalendarPopup(true);
    ui->dateEditFechaFin->setCalendarPopup(true);
    ui->dateEditFechaInicio_2->setCalendarPopup(true);
    ui->dateEditFechaFin_2->setCalendarPopup(true);

    QIntValidator *v = new QIntValidator(00000,100000);
    QIntValidator *percent = new QIntValidator(0,100);
    ui->lineEditCodigo->setValidator(v);
    ui->lineEditCodigo_2->setValidator(v);
    ui->lineEditPorcentajeInstitucionFinancia->setValidator(percent);
    ui->lineEditPorcentajeInstitucionFinancia2->setValidator(percent);
    ui->lineEditPorcentajeInstitucionFinancia_2->setValidator(percent);
    ui->lineEditPorcentajeInstitucionFinancia2_2->setValidator(percent);

    QRegExp re("[0-9]+");
    QRegExpValidator *regV = new QRegExpValidator(re);
    ui->lineEditNumeroBeneficiarios->setValidator(regV);
    ui->lineEditNumeroBeneficiarios_2->setValidator(regV);
    ui->lineEditMontoAprobado->setValidator(regV);
    ui->lineEditMontoAprobado_2->setValidator(regV);
    ui->lineEditMontoSolicitado->setValidator(regV);
    ui->lineEditMontoSolicitado_2->setValidator(regV);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete help;
    delete herramientas;
}


void MainWindow::on_pushButtonInsert_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButtonConsult_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButtonList_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButtonSaveInsertar_clicked()
{
    QDate date = QDate::currentDate();

    QString qNombre = ui->lineEditNombre->text();
    QString qInstitucionFinancia = ui->lineEditInstitucionFinancia->text();
    QString qInstitucionFinanciaDos = ui->lineEditInstitucionFinancia2->text();
    QString qPorcentajeInstitucionFinancia = ui->lineEditPorcentajeInstitucionFinancia->text();
    QString qPorcentajeInstitucionFinanciaDos = ui->lineEditPorcentajeInstitucionFinancia2->text();
    QString qInstitucionEjecuta = ui->lineEditInstitucionEjecuta->text();
    QString qMontoAprobado = ui->lineEditMontoAprobado->text();
    QString qMontoSolicitado = ui->lineEditMontoSolicitado->text();
    QString qFechaInicio = ui->dateEditFechaInicio->text();
    QString qFechaFin = ui->dateEditFechaFin->text();
    QString qObjetivos = ui->plainTextEditObjetivos->toPlainText();
    QString qComunidad = ui->lineEditComunidadDestino->text();
    QString qNumeroBeneficiarios = ui->lineEditNumeroBeneficiarios->text();
    QString qAmbitoProyecto = ui->lineEditAmbito->text();

    int qEstatus = ui->comboBoxEstatus->currentIndex();
    QString qEstatusString = QString::number(qEstatus);

    QString qContacto = ui->plainTextEditContacto ->toPlainText();
    QString qComentario = ui->plainTextEditComentario->toPlainText();

    int qIndicador = ui->horizontalSlider->value();
    QString qIndicadorString = QString::number(qIndicador);

    QString qCodigo = ui->lineEditCodigo->text();
    QString qInsersion = date.toString();
    QString validacion="";
    QString aux="";
    bool ok=true;

    if(qMontoAprobado>qMontoSolicitado) {
        aux="-  El Monto Aprobado no puede ser mayor al Monto Solicitado\n";
        validacion=validacion+aux;
        ok=false;
    }

    if(qPorcentajeInstitucionFinancia.toInt()>100 || qPorcentajeInstitucionFinanciaDos.toInt()>100 || qPorcentajeInstitucionFinancia.toInt()+qPorcentajeInstitucionFinanciaDos.toInt()>100) {
        aux="-  Los Porcentajes de Financiamiento no deben ser mayores a 100% en su totalidad\n";
        validacion=validacion+aux;
        ok=false;
    }

    if(ui->dateEditFechaInicio->date()>ui->dateEditFechaFin->date()){
        aux="-  La fecha de fin no puede ocurrir primero que la fecha de inicio\n";
        validacion=validacion+aux;
        ok=false;
    }

    if(validacion!="")
        msj_error(validacion);


    if(qNombre.isEmpty() || qInstitucionEjecuta.isEmpty()
            ||  qFechaInicio.isEmpty() || qComunidad.isEmpty() || qObjetivos.isEmpty()
            || qFechaFin.isEmpty() || qCodigo.isEmpty() ){
        msj_error("Estos campos No pueden quedar Vacíos: Código, Nombre, Institución que Ejecuta, Comunidad u Objetivos");
    }else{
        if(ok) {
            QSqlQuery query;
            query.exec(QString("SELECT * FROM Proyecto WHERE Nombre LIKE '%%1%' ").arg(qNombre));
            if(!query.next()){
                query.prepare("INSERT INTO Proyecto (Nombre, InstitucionFinancia, InstitucionFinanciaDos, PorcentajeAporteInstitucion,"
                              "PorcentajeAporteInstitucionDos, InstitucionEjecuta, "
                              "MontoAprobado, MontoSolicitado, FechaInicio, FechaFin, Objetivos, Comunidad, NumeroBeneficiarios,"
                              "AmbitoProyecto, Estatus, Contacto, Indicador, CodigoUnico, Comentario, Insercion)"
                              "VALUES (:Nombre, :InstitucionFinancia, :InstitucionFinanciaDos, :PorcentajeAporteInstitucion, :PorcentajeAporteInstitucionDos, :InstitucionEjecuta,"
                              ":MontoAprobado, :MontoSolicitado, :FechaInicio, :FechaFin, :Objetivos, :Comunidad, :NumeroBeneficiarios,"
                              ":AmbitoProyecto, :Estatus, :Contacto, :Indicador, :CodigoUnico, :Comentario, :Insercion)");

                query.bindValue(":Nombre",qNombre);
                query.bindValue(":InstitucionFinancia",qInstitucionFinancia);
                query.bindValue(":InstitucionFinanciaDos",qInstitucionFinanciaDos);
                query.bindValue(":PorcentajeAporteInstitucion",qPorcentajeInstitucionFinancia);
                query.bindValue(":PorcentajeAporteInstitucionDos",qPorcentajeInstitucionFinanciaDos);
                query.bindValue(":InstitucionEjecuta",qInstitucionEjecuta);
                query.bindValue(":MontoAprobado",qMontoAprobado);
                query.bindValue(":MontoSolicitado",qMontoSolicitado);
                query.bindValue(":FechaInicio",qFechaInicio);
                query.bindValue(":FechaFin",qFechaFin);
                query.bindValue(":Objetivos",qObjetivos);
                query.bindValue(":Comunidad",qComunidad);
                query.bindValue(":NumeroBeneficiarios",qNumeroBeneficiarios);
                query.bindValue(":AmbitoProyecto",qAmbitoProyecto);
                query.bindValue(":Estatus",qEstatus);
                query.bindValue(":Contacto",qContacto);
                query.bindValue(":Indicador",qIndicador);
                query.bindValue(":CodigoUnico",qCodigo);
                query.bindValue(":Comentario",qComentario);
                query.bindValue(":Insercion",qInsersion);

                //qDebug() << query.lastError();

                if(!query.exec()){
                    msj_error("El código del proyecto ya esta en uso");
                }else{
                     ui->stackedWidget->setCurrentIndex(6);
                }
            }
            else{
                msj_error("El Proyecto con ese nombre ya existe");

            }
        }
    }
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

//CONSULTA
int clickedFlag=0;
void MainWindow::on_pushButtonSerchConsult_clicked()
{
    clickedFlag=0;
    int comboFlag=0;
    int FoundFlag=0;
    QString value;
    QString filtro;
    QString sRec = "Problemas con el Combo Box";
    if(ui->comboBox->currentIndex() == 0) {
        filtro="Nombre";
        comboFlag=1;
    }
    if(ui->comboBox->currentIndex() == 1 ) {
        filtro="CodigoUnico";
        comboFlag=1;
    }
    if(ui->comboBox->currentIndex() == 2 ){
        value = ui->lineEditVariableConsult->text();
        filtro="InstitucionEjecuta";
        comboFlag=1;
    }
    if(ui->comboBox->currentIndex() == 3){
        filtro="InstitucionFinancia";
        comboFlag=1;
    }

    if(ui->comboBox->currentIndex() == 4){
        filtro="Comunidad";
        comboFlag=1;
    }


    if(ui->comboBox->currentIndex() == 5){
        filtro="AmbitoProyecto";
        comboFlag=1;
    }

    if(comboFlag==1) {
        QSqlQuery query;
        if(filtro=="InstitucionFinancia")  {
            value = ui->lineEditVariableConsult->text();
            query.exec(QString("SELECT * FROM Proyecto WHERE InstitucionFinancia OR InstitucionFinanciaDos LIKE '%%1%' ").arg(value));

        }

        else {
            value = ui->lineEditVariableConsult->text();
            query.exec(QString("SELECT * FROM Proyecto WHERE %1 LIKE '%%2%' ORDER BY %1 ").arg(filtro).arg(value));

        }

        QSqlQueryModel * model = new QSqlQueryModel;

        if(query.next()) {

             model->setQuery(query);

             if (model->lastError().type() != QSqlError::NoError)
                    msj_error(model->lastError().text());
             else {
                QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel;
                sqlproxy->setSourceModel(model);
                ui->tableViewConsulta->setSortingEnabled(false);
                ui->tableViewConsulta->setModel(sqlproxy);
                ui->tableViewConsulta->setColumnWidth(1,300);
                ui->tableViewConsulta->setColumnWidth(2,150);
                ui->tableViewConsulta->setColumnWidth(3,150);
                ui->tableViewConsulta->setColumnWidth(4,150);
                ui->tableViewConsulta->setColumnWidth(5,150);
                ui->tableViewConsulta->horizontalHeader()->setSortIndicatorShown(true);
                ui->tableViewConsulta->horizontalHeader()->setSortIndicator(0,Qt::DescendingOrder);
                ui->tableViewConsulta->setSortingEnabled(true);
                ui->tableViewConsulta->show();

                FoundFlag=1;
             }
         }

        if(FoundFlag==0)
            ui->stackedWidget->setCurrentIndex(2);
    }

    else
         msj_error(sRec);

}

void MainWindow::editSlot(const QModelIndex &QMI) {

    QString sRec = "Columna no válida, haga click en el Nombre o Código del Proyecto para ver su información mas detallada.";

    QVariant value = ui->tableViewConsulta->model()->data(QMI,0);
    QString aux = value.toString();
    QString filtro;
    QString estatus;
    proyect_a_modificar=aux;

    if(value.isValid()) {

        if(QMI.column()==1 || QMI.column()==0){

                if(QMI.column()==1) {
                    filtro = "Nombre";
                    aux_q = QString("DELETE FROM Proyecto WHERE %1 = '%2'").arg(filtro).arg(aux);//ESTO ES PARA ELIMINAR
                }
                if(QMI.column()==0) {
                    filtro = "CodigoUnico";
                    aux_q = QString("DELETE FROM Proyecto WHERE %1 = '%2'").arg(filtro).arg(aux);//ESTO ES PARA ELIMINAR
                }

                QSqlQuery query(QString("SELECT * FROM Proyecto WHERE %1 LIKE '%2'").arg(filtro).arg(aux));
                pdf_proyecto=QString("SELECT * FROM Proyecto WHERE %1 LIKE '%2'").arg(filtro).arg(aux);



                int nombreNum = query.record().indexOf("Nombre");
                int institucionFinanciaNum = query.record().indexOf("InstitucionFinancia");
                int institucionFinanciaDosNum = query.record().indexOf("InstitucionFinanciaDos");
                int porcentajeInstitucionFinanciaNum = query.record().indexOf("PorcentajeAporteInstitucion");
                int porcentajeInstitucionFinanciaDosNum = query.record().indexOf("PorcentajeAporteInstitucionDos");
                int institucionEjecutaNum = query.record().indexOf("InstitucionEjecuta");
                int montoAprobadoNum = query.record().indexOf("MontoAprobado");
                int montoSolicitadoNum = query.record().indexOf("MontoSolicitado");
                int fechaInicioNum = query.record().indexOf("FechaInicio");
                int fechaFinNum = query.record().indexOf("FechaFin");
                int objetivosNum = query.record().indexOf("Objetivos");
                int comunidadNum = query.record().indexOf("Comunidad");
                int numeroBeneficiariosNum = query.record().indexOf("NumeroBeneficiarios");
                int ambitoProyectoNum = query.record().indexOf("AmbitoProyecto");
                int estatusNum = query.record().indexOf("Estatus");
                int contactoNum = query.record().indexOf("Contacto");
                int comentarioNum = query.record().indexOf("Comentario");
                int indicadorNum = query.record().indexOf("Indicador");
                int codigoNum = query.record().indexOf("CodigoUnico");
                int fechaInsercion = query.record().indexOf("Insercion");


                if(query.next()) {
                    ui->textBrowserNombre->setText(query.value(nombreNum).toString());
                    ui->lineEditNombre_2->setText(query.value(nombreNum).toString());

                    ui->textBrowserInstitucionFinancia->setText(query.value(institucionFinanciaNum).toString());
                    ui->lineEditInstitucionFinancia_2->setText(query.value(institucionFinanciaNum).toString());

                    ui->textBrowserInstitucionFinancia2->setText(query.value(institucionFinanciaDosNum).toString());
                    ui->lineEditInstitucionFinancia2_2->setText(query.value(institucionFinanciaDosNum).toString());

                    ui->textBrowserPorcentajeInstitucionFinancia->setText(query.value(porcentajeInstitucionFinanciaNum).toString());
                    ui->lineEditPorcentajeInstitucionFinancia_2->setText(query.value(porcentajeInstitucionFinanciaNum).toString());

                    ui->textBrowserPorcentajeInstitucionFinancia2->setText(query.value(porcentajeInstitucionFinanciaDosNum).toString());
                    ui->lineEditPorcentajeInstitucionFinancia2_2->setText(query.value(porcentajeInstitucionFinanciaDosNum).toString());

                    ui->textBrowserInstitucionEjecuta->setText(query.value(institucionEjecutaNum).toString());
                    ui->lineEditInstitucionEjecuta_2->setText(query.value(institucionEjecutaNum).toString());

                    ui->textBrowserMontoAprobado->setText(query.value(montoAprobadoNum).toString());
                    ui->lineEditMontoAprobado_2->setText(query.value(montoAprobadoNum).toString());

                    ui->textBrowserMontoSolicitado->setText(query.value(montoSolicitadoNum).toString());
                    ui->lineEditMontoSolicitado_2->setText(query.value(montoSolicitadoNum).toString());

                    ui->dateEditFechaInicioConsulta->setDate(query.value(fechaInicioNum).toDate());
                    ui->dateEditFechaInicio_2->setDate(query.value(fechaInicioNum).toDate());

                    ui->dateEditFechaFinConsulta->setDate(query.value(fechaFinNum).toDate());
                    ui->dateEditFechaFin_2->setDate(query.value(fechaFinNum).toDate());

                    ui->textBrowserObjetivos->setText(query.value(objetivosNum).toString());
                    ui->plainTextEditObjetivos_2->clear();
                    ui->plainTextEditObjetivos_2->insertPlainText(query.value(objetivosNum).toString());

                    ui->textBrowserComunidad->setText(query.value(comunidadNum).toString());
                    ui->lineEditComunidadDestino_2->setText(query.value(comunidadNum).toString());

                    ui->textBrowserNumeroBeneficiarios->setText(query.value(numeroBeneficiariosNum).toString());
                    ui->lineEditNumeroBeneficiarios_2->setText(query.value(numeroBeneficiariosNum).toString());

                    ui->textBrowserAmbito->setText(query.value(ambitoProyectoNum).toString());
                    ui->lineEditAmbito_2->setText(query.value(ambitoProyectoNum).toString());

                    if(query.value(estatusNum).toInt()==0)
                        estatus="En Trámite";
                    if(query.value(estatusNum).toInt()==1)
                        estatus="En Ejecución";
                    if(query.value(estatusNum).toInt()==2)
                        estatus="Negado";
                    if(query.value(estatusNum).toInt()==3)
                        estatus="Completado";

                    ui->textBrowserEstatus->setText(estatus);
                    ui->comboBoxEstatus_2->setCurrentIndex(query.value(estatusNum).toInt());

                    ui->textBrowserContacto->setText(query.value(contactoNum).toString());
                    ui->plainTextEditContacto_2->clear();
                    ui->plainTextEditContacto_2->insertPlainText(query.value(contactoNum).toString());

                    ui->textBrowserComentarios->setText(query.value(comentarioNum).toString());
                    ui->plainTextEditComentario_2->clear();
                    ui->plainTextEditComentario_2->insertPlainText(query.value(comentarioNum).toString());

                    ui->textBrowserCodigo->setText(query.value(codigoNum).toString());
                    ui->lineEditCodigo_2->setText(query.value(codigoNum).toString());

                    ui->progressBar->setValue(query.value(indicadorNum).toInt());
                    ui->horizontalSlider_2->setValue(query.value(indicadorNum).toInt());

                    ui->textBrowserInsercion->setText(query.value(fechaInsercion).toString());

                    ui->stackedWidget->setCurrentIndex(3);
                }

        }

        if((QMI.column()!=1 && QMI.column()!=0) && clickedFlag==0) {
            msj_error(sRec);
            clickedFlag=1;
        }
    }
}

void MainWindow::msj_error(QString error){
    QMessageBox msgBox;
    msgBox.setText( error);
    msgBox.exec();
}

void MainWindow::on_pushButtonNotFound_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButtonRegresar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButtonResetConsult_clicked()
{
    ui->lineEditVariableConsult->setText("");
}

void MainWindow::on_pushButtonBackInsertar_clicked()
{
    clearInsert();
    on_pushButtonInsert_clicked();

}

void MainWindow::on_pushButtonResetInsert_clicked()
{
    clearInsert();

}

void MainWindow::on_pushButton_4_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Salir de la Aplicación","Esta seguro que desea salir del Banco de Proyectos?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qApp->quit();
    }

}

void MainWindow::clearInsert() {
    ui->lineEditNombre->setText("");
    ui->lineEditInstitucionEjecuta->setText("");
    ui->lineEditInstitucionFinancia->setText("");
    ui->lineEditInstitucionFinancia2->setText("");
    ui->lineEditPorcentajeInstitucionFinancia->setText("");
    ui->lineEditPorcentajeInstitucionFinancia2->setText("");
    ui->lineEditMontoAprobado->setText("");
    ui->lineEditMontoSolicitado->setText("");
    //ui->dateEditFechaFin->clear();
    //ui->dateEditFechaInicio->clear();
    //ui->comboBoxEstatus->clear();
    ui->lineEditCodigo->setText("");
    ui->lineEditComunidadDestino->setText("");
    ui->plainTextEditObjetivos->clear();
    ui->plainTextEditComentario->clear();
    ui->plainTextEditContacto->clear();
    ui->lineEditAmbito->clear();
    ui->lineEditNumeroBeneficiarios->setText("");

}


void MainWindow::about() {
        QString title = "<H2> Banco de Proyectos </H2>";
      QString version = "<H5> Versión 1.0 <br> svn checkout https://code.google.com/p/proyectos-merida/ </H5> <br>";
      QString description;
      description.append("<b>Proyecto de construcción de un banco de datos ");
      description.append("sobre proyectos turísticos del municipio Libertador en el estado Mérida, Venezuela.</b> <br>");
      description.append("<ol><li>Creado bajo la licencia de software libre Apache 2.0</li>");
      description.append("<li>Sistema de Manejo de Base de Datos: SQLite</li>");
      description.append("<li>Desarrollado bajo la Interfaz Grafica QTCreator</li></ol><br>");
      QString titleProgramer = "<b>Desarrollado por:</b> <br>";
      QString programer = "Jesús Manuel Salcedo  <br>";
      QString programer2 ="Ricardo Xavier Graterol <br><br>";
      QString titleEmail = "<b>Dirección electronica:</b><br>";
      QString email = "jesus_salteran@hotmail.com<br>";
      QString email2 = "xavier.graterol@gmail.com<br><br>";



      QMessageBox::about(this, tr("Acerca de Banco de Proyectos V1.0"), "<p align=\"center\">" + title +
                         version +"</p>"+ "<p align=\"justify\">"+description +"</p>"+"<p align=\"center\">"+ titleProgramer + programer
                         + programer2 + titleEmail + email + email2 + "<font color = #FF8000>" +
                         "</font>" + "</p>");

}

void MainWindow::aboutSqlite() {

    QString title = "<h1><font color=#76B900>About SQLITE</font></h1>";

    QString p1 ="SQLite is a relational database management system contained in a C programming library. In contrast to other database management systems, SQLite is not a separate process that is accessed from the client application, but an integral part of it.<br><br>";

    QString p2 ="SQLite is ACID-compliant and implements most of the SQL standard, using a dynamically and weakly typed SQL syntax that does not guarantee the domain integrity.<br><br>";

    QString p3 ="SQLite is a popular choice as embedded database for local/client storage in application software such as web browsers. It is arguably the most widely deployed database engine, as it is used today by several widespread browsers, operating systems, and embedded systems, among others.[5] SQLite has bindings to many programming languages.";

    p3.append("<br><br>The source code for SQLite is in the public domain.");

    QString sqlitelink = "http://www.sqlite.org/";

    QMessageBox messageAbout;

    messageAbout.setWindowTitle("Acerca de SQLite");
    messageAbout.setText("<p align=\"center\">" + title +
                         "</p>"+ "<p align=\"justify\">"+p1+ p2+ p3+ "</p>" + "<p align=\"center\">" + sqlitelink +"</p>");
    messageAbout.exec();
}



void MainWindow::importDB(){

    QSqlQuery query;

    QString dbName = QFileDialog::getOpenFileName(this, tr("Importar Banco de Datos"), QString(),
               tr("Bases de Datos SQLite (*.sqlite)"));
    if(!dbName.isEmpty()) {
        query.exec(QString("ATTACH DATABASE '%1' AS toMerge").arg(dbName));
        query.exec(QString("INSERT OR IGNORE INTO Proyecto SELECT * FROM toMerge.Proyecto"));
        query.exec(QString("DETACH DATABASE toMerge"));
        ui->stackedWidget->setCurrentIndex(7);
    }
}

bool MainWindow::exportDB()
{
    bool r_dumpingSuccessfull = false;
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Nombre del Banco de Proyectos a Exportar"),
                QStandardPaths::standardLocations(QStandardPaths::DataLocation)[2],
                tr("Sqlite Files(*.sqlite)"));

    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly);

    if(!outputFile.isOpen()) {
       //qDebug<< "- Error, imposible de abrir '" << fileName << "' Para exportar la base de Datos SQL";
       return r_dumpingSuccessfull;
    }

    QTextStream outStream(&outputFile);

    QSqlQuery query;

    if( !query.exec( "SELECT * FROM Proyecto" ) )
    {
       //Something with the SQL-Query or the DB-connection is wrong.
       QString lastError = query.lastError().text();
       qDebug() << lastError;
       return r_dumpingSuccessfull;
    }

    else {
        do {
           QString tableName = query.value("Nombre").toString();

           if(tableName!="sqlite_sequence") {
               outStream << query.value(0).toString();
               outStream << "\n";
               outStream << "--;;";
               outStream << "\n";
           }
        } while(query.next());
    }

    outputFile.close();

    return r_dumpingSuccessfull;

}

void MainWindow::ayuda_clicked(){
    help->show();
}

void MainWindow::on_pushButtonModificar_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_pushButtonRegresarConsulta_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButtonBackModificar_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButtonEliminarProyecto_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Eliminar Proyecto","Esta Seguro que desea eliminar el proyecto?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QSqlQuery query(aux_q);
        ui->stackedWidget->setCurrentIndex(9);
        if(!query.exec()){
            msj_error("Error al tratar de eliminar el proyecto");
        }else{

        }
    } else {
        qDebug() << "Yes was *not* clicked";
    }


}

void MainWindow::on_pushButtonReturnConsulta_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tableViewConsulta->hide();//modificar esto
}

void MainWindow::on_pushButtonGuardarModificar_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Modificar Proyecto","Esta seguro que desea guardar los cambios?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        QString nombre_edit = ui->lineEditNombre_2->text();
        QString institucionFinancia_edit = ui->lineEditInstitucionFinancia_2->text();
        QString institucionFinancia2_edit = ui->lineEditInstitucionFinancia2_2->text();
        QString porcentajeInstitucionFinancia_edit = ui->lineEditPorcentajeInstitucionFinancia_2->text();
        QString porcentajeInstitucionFinancia2_edit = ui->lineEditInstitucionFinancia2_2->text();
        QString institucionEjecuta_edit = ui->lineEditInstitucionEjecuta_2->text();
        QString montoAprobado_edit = ui->lineEditMontoAprobado_2->text();
        QString montoSolicitado_edit = ui->lineEditMontoSolicitado_2->text();
        QString fechaInicio_edit = ui->dateEditFechaInicio_2->text();
        QString fechaFin_edit = ui->dateEditFechaFin_2->text();
        QString objetivos_edit = ui->plainTextEditObjetivos_2->toPlainText();
        QString comunidad_edit = ui->lineEditComunidadDestino_2->text();
        QString numeroBeneficiarios_edit = ui->lineEditNumeroBeneficiarios_2->text();
        QString ambitoProyecto_edit = ui->lineEditAmbito_2->text();

        int estatus_edit_int = ui->comboBoxEstatus_2->currentIndex();
        QString estatus_edit = QString::number(estatus_edit_int);

        QString contacto_edit = ui->plainTextEditContacto_2->toPlainText();
        QString comentario_edit = ui->plainTextEditComentario_2->toPlainText();

        int indicador_edit_int = ui->horizontalSlider_2->value();
        QString indicador_edit = QString::number(indicador_edit_int);


       // QSqlQuery query(QString("UPDATE Proyecto SET Nombre='"+nombre_edit+"',InstitucionEjecuta='"+institucionEjecuta_edit+"',AmbitoProyecto='"+ambitoProyecto_edit+"',InstitucionFinancia='"+institucionFinancia_edit+"',InstitucionFinanciaDos='"+institucionFinancia2_edit+"',PorcentajeAporteInstitucion='"+porcentajeInstitucionFinancia_edit+"',PorcentajeAporteInstitucionDos='"+porcentajeInstitucionFinancia2_edit+"',MontoAprobado='"+montoAprobado_edit+"',MontoSolicitado='"+montoSolicitado_edit+"',FechaInicio='"+fechaInicio_edit+"',FechaFin='"+fechaFin_edit+"',Objetivos='"+objetivos_edit+"',Comunidad='"+comunidad_edit+"',NumeroBeneficiarios='"+numeroBeneficiarios_edit+"',Estatus='"+estatus_edit+"',Contacto='"+contacto_edit+"',Indicador='"+indicador_edit+"',Comentario='"+comentario_edit+"' WHERE Nombre LIKE '%1'").arg(proyect_a_modificar));
        QSqlQuery query;
        query.prepare(QString("UPDATE Proyecto SET Nombre=:Nombre,InstitucionEjecuta=:InstitucionEjecuta,AmbitoProyecto=:AmbitoProyecto,InstitucionFinancia=:InstitucionFinancia,InstitucionFinanciaDos=:InstitucionFinanciaDos,PorcentajeAporteInstitucion=:PorcentajeAporteInstitucion,PorcentajeAporteInstitucionDos=:PorcentajeAporteInstitucionDos,MontoAprobado=:MontoAprobado,MontoSolicitado=:MontoSolicitado,FechaInicio=:FechaInicio,FechaFin=:FechaFin,Objetivos=:Objetivos,Comunidad=:Comunidad,NumeroBeneficiarios=:NumeroBeneficiarios,Estatus=:Estatus,Contacto=:Contacto,Indicador=:Indicador,Comentario=:Comentario WHERE Nombre LIKE '%1'").arg(proyect_a_modificar));
        query.bindValue(":Nombre",nombre_edit);
        query.bindValue(":InstitucionFinancia",institucionFinancia_edit);
        query.bindValue(":InstitucionFinanciaDos",institucionFinancia2_edit);
        query.bindValue(":PorcentajeAporteInstitucion",porcentajeInstitucionFinancia_edit);
        query.bindValue(":PorcentajeAporteInstitucionDos",porcentajeInstitucionFinancia2_edit);
        query.bindValue(":InstitucionEjecuta",institucionEjecuta_edit);
        query.bindValue(":MontoAprobado",montoAprobado_edit);
        query.bindValue(":MontoSolicitado",montoSolicitado_edit);
        query.bindValue(":FechaInicio",fechaInicio_edit);
        query.bindValue(":FechaFin",fechaFin_edit);
        query.bindValue(":Objetivos",objetivos_edit);
        query.bindValue(":Comunidad",comunidad_edit);
        query.bindValue(":NumeroBeneficiarios",numeroBeneficiarios_edit);
        query.bindValue(":AmbitoProyecto",ambitoProyecto_edit);
        query.bindValue(":Estatus",estatus_edit);
        query.bindValue(":Contacto",contacto_edit);
        query.bindValue(":Indicador",indicador_edit);
        query.bindValue(":Comentario",comentario_edit);


        if(query.exec())
            ui->stackedWidget->setCurrentIndex(10);

        else
            QMessageBox::critical(this,tr("Error"),query.lastError().text());
    }
}



void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButtonListarLista_clicked()
{
    QSqlQuery query;

    if(!query.exec(QString("SELECT * FROM Proyecto"))) {
        msj_error("Error al leer de la base de datos");
    }

    else {
        QSqlQueryModel * model = new QSqlQueryModel;
        while(query.next()) {
             model->setQuery(query);

             if (model->lastError().type() != QSqlError::NoError)
                    msj_error(model->lastError().text());
             else {
                QSortFilterProxyModel *sqlproxy = new QSortFilterProxyModel;
                sqlproxy->setSourceModel(model);
                ui->tableViewListar->setSortingEnabled(false);
                ui->tableViewListar->setModel(sqlproxy);
                ui->tableViewListar->setColumnWidth(1,300);
                ui->tableViewListar->setColumnWidth(2,150);
                ui->tableViewListar->setColumnWidth(3,150);
                ui->tableViewListar->setColumnWidth(4,150);
                ui->tableViewListar->setColumnWidth(5,150);
                ui->tableViewListar->horizontalHeader()->setSortIndicatorShown(true);
                ui->tableViewListar->horizontalHeader()->setSortIndicator(0,Qt::DescendingOrder);
                ui->tableViewListar->setSortingEnabled(true);
                ui->tableViewListar->show();
             }
         }
    }
}

void MainWindow::on_pushButtonPdfProyecto_clicked()
{
    QSqlQuery query(pdf_proyecto);
    if(query.next()) {
        QString fileName = QFileDialog::getSaveFileName(this,
                    tr("Proyecto a Guardar en PDF"),
                    query.value("Nombre").toString(),
                    tr("PDF(*.pdf)"));

        if(fileName!="") {
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPaperSize(QPrinter::Letter);
            printer.setFullPage(true);
            printer.setOutputFileName(fileName);


            QSqlQueryModel *model = new QSqlQueryModel;

            model->setQuery(query);
            QList<QVariant> list = query.boundValues().values();
            for(int i=0;i<list.size();i++){
                model->setHeaderData(i,Qt::Horizontal,list.at(i).data());
            }

            QTableView *view = new QTableView;
            /*Impresion lineal consecutiva de tabla en PDF*/
            QString strStream;
            QTextStream out(&strStream);

            const int rowCount = model->rowCount();

            QString qCodigo;
            QString qNombre;
            QString qInstitucionEjecuta;
            QString qAmbitoProyecto;
            QString qInstitucionFinancia;
            QString qInstitucionFinanciaDos;
            QString qPorcentajeInstitucionFinancia;
            QString qPorcentajeInstitucionFinanciaDos;
            QString qMontoAprobado;
            QString qMontoSolicitado;
            QString qFechaInicio;
            QString qFechaFin;
            QString qObjetivos;
            QString qComunidad;
            QString qNumeroBeneficiarios;
            QString qEstatus;
            QString qContacto;
            QString qIndicador;
            QString qComentario;
            QString qInsersion;
            QString nombre_aux=model->data(model->index(0, 1)).toString().simplified();
            QString qEstatus_aux;

                out <<  "<html>\n"
                    "<head>\n"
                    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                    <<  QString("<title>%1</title>\n").arg(nombre_aux)
                    << "<style type=text/css>"
                    << "font {size:5}"
                    << "</style>"
                    <<  "</head>\n"
                    <<"<body bgcolor=#ffffff link=#5000A0 >\n"
                    << QString("<h1>%1</h1>").arg(nombre_aux)
                    <<  QString("<p>%1</p>").arg("");


                // data table
                for (int row = 0; row < rowCount; row++) { //inicio for
                    qCodigo = model->data(model->index(row, 0)).toString().simplified();
                    qNombre = model->data(model->index(row, 1)).toString().simplified();
                    qInstitucionEjecuta = model->data(model->index(row, 2)).toString().simplified();
                    qAmbitoProyecto = model->data(model->index(row, 3)).toString().simplified();
                    qInstitucionFinancia = model->data(model->index(row, 4)).toString().simplified();
                    qInstitucionFinanciaDos = model->data(model->index(row, 5)).toString().simplified();
                    qPorcentajeInstitucionFinancia = model->data(model->index(row, 6)).toString().simplified();
                    qPorcentajeInstitucionFinanciaDos = model->data(model->index(row, 7)).toString().simplified();
                    qMontoAprobado = model->data(model->index(row, 8)).toString().simplified();
                    qMontoSolicitado = model->data(model->index(row, 9)).toString().simplified();
                    qFechaInicio = model->data(model->index(row, 10)).toString().simplified();
                    qFechaFin = model->data(model->index(row, 11)).toString().simplified();
                    qObjetivos = model->data(model->index(row, 12)).toString();
                    qComunidad = model->data(model->index(row, 13)).toString().simplified();
                    qNumeroBeneficiarios = model->data(model->index(row, 14)).toString().simplified();
                    qEstatus_aux = model->data(model->index(row, 15)).toString().simplified();
                    if(qEstatus_aux=="0")
                        qEstatus="En Trámite";
                    if(qEstatus_aux=="1")
                        qEstatus="En Ejecución";
                    if(qEstatus_aux=="2")
                        qEstatus="Negado";
                    if(qEstatus_aux=="3")
                        qEstatus="Completado";
                    qContacto = model->data(model->index(row, 16)).toString();
                    qIndicador = model->data(model->index(row, 17)).toString().simplified();
                    qComentario = model->data(model->index(row, 18)).toString();
                    qInsersion = model->data(model->index(row, 19)).toString().simplified();
                    out << QString("<p><b>Código del Proyecto: </b> %1</p>").arg((!qCodigo.isEmpty()) ? qCodigo : QString("&nbsp;"));
                    out << QString("<p><b>Nombre del Proyecto: </b> %1</p>").arg((!qNombre.isEmpty()) ? qNombre : QString("&nbsp;"));
                    out << QString("<p><b>Institución que Ejecuta el Proyecto: </p></b> %1").arg((!qInstitucionEjecuta.isEmpty()) ? qInstitucionEjecuta : QString("&nbsp;"));
                    out << QString("<p><b>Ámbito del Proyecto: </b></p> %1").arg((!qAmbitoProyecto.isEmpty()) ? qAmbitoProyecto : QString("&nbsp;"));
                    out << QString("<p><b>Institución que financia el Proyecto: </b> %1</p>").arg((!qInstitucionFinancia.isEmpty()) ? qInstitucionFinancia : QString("&nbsp;"));
                    out << QString("<p><b>Segunda institución que financia el Proyecto: </b> %1</p>").arg((!qInstitucionFinanciaDos.isEmpty()) ? qInstitucionFinanciaDos : QString("&nbsp;"));
                    out << QString("<p><b>Porcentaje de aporte de primera institución: </b> %1</p>").arg((!qPorcentajeInstitucionFinancia.isEmpty()) ? qPorcentajeInstitucionFinancia : QString("&nbsp;"));
                    out << QString("<p><b>Porcentaje de aporte de segunda institución: </b> %1</p>").arg((!qPorcentajeInstitucionFinanciaDos.isEmpty()) ? qPorcentajeInstitucionFinanciaDos : QString("&nbsp;"));
                    out << QString("<p><b>Monto aprobado (en Bolívares): </b> %1</p>").arg((!qMontoAprobado.isEmpty()) ? qMontoAprobado : QString("&nbsp;"));
                    out << QString("<p><b>Monto solicitado (en Bolívares): </b> %1</p>").arg((!qMontoSolicitado.isEmpty()) ? qMontoSolicitado : QString("&nbsp;"));
                    out << QString("<p><b>Fecha de inicio: </b> %1</p>").arg((!qFechaInicio.isEmpty()) ? qFechaInicio : QString("&nbsp;"));
                    out << QString("<p><b>Fecha de fin: </b> %1</p>").arg((!qFechaFin.isEmpty()) ? qFechaFin : QString("&nbsp;"));
                    out << QString("<p><b>Objetivos: </b> %1</p>").arg((!qObjetivos.isEmpty()) ? qObjetivos.replace(QString("\n"),QString("<br>")) : QString("&nbsp;"));
                    out << QString("<p><b>Comunidad: </b> %1</p>").arg((!qComunidad.isEmpty()) ? qComunidad : QString("&nbsp;"));
                    out << QString("<p><b>Número de personas beneficiadas: </b> %1 </p>").arg((!qNumeroBeneficiarios.isEmpty()) ? qNumeroBeneficiarios : QString("&nbsp;"));
                    out << QString("<p><b>Estatus del proyecto: </b> %1 </p>").arg((!qEstatus.isEmpty()) ? qEstatus : QString("&nbsp;"));
                    out << QString("<p><b>Contacto: </b> %1</p> ").arg((!qContacto.isEmpty()) ? qContacto.replace(QString("\n"),QString("<br>")) : QString("&nbsp;"));
                    out << QString("<p><b>Indicador de progreso: </b> %1 %</p>").arg((!qIndicador.isEmpty()) ? qIndicador : QString("&nbsp;"));
                    out << QString("<p><b>Comentario: </b> %1 </p>").arg((!qComentario.isEmpty()) ? qComentario.replace(QString("\n"),QString("<br>")) : QString("&nbsp;"));
                    out << QString("<p><b>Inserción en el Banco de Proyectos: </b> %1</p> ").arg((!qInsersion.isEmpty()) ? qInsersion : QString("&nbsp;"));
                    out << "<br>------------------------------------------------------------------------------------------<br>";
                }//FIN DEL FOR for (int row = 0; row < rowCount; row++)

                out <<QString("<p>%1</p>").arg("Banco de Proyectos V1.0")
                    <<"</body>\n"
                    <<"</html>\n";

                QTextDocument *document = new QTextDocument();
                document->setHtml(strStream);
                document->adjustSize();
                document->print(&printer);
                delete document;

                msj_error("Proyecto Guardado en PDF Correctamente");
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{

}



void MainWindow::on_pushButtonCSVProyecto_clicked()
{
    QSqlQuery query(pdf_proyecto);
    if(query.next()) {
        QString fileName = QFileDialog::getSaveFileName(this,
                    tr("Proyecto a Guardar en Hoja de Cálculo CSV"),
                    query.value("Nombre").toString(),
                    tr("CSV(*.csv)"));

        if(fileName!="") {
            QSqlQueryModel *model = new QSqlQueryModel;

            model->setQuery(query);
            QList<QVariant> list = query.boundValues().values();
            for(int i=0;i<list.size();i++){
                model->setHeaderData(i,Qt::Horizontal,list.at(i).data());
            }

            QString out;//strStream;
           // QTextStream out(&strStream);
            const int rowCount = model->rowCount();
            QFile archivo(fileName);
            //QByteArray textTemp,finalText;

            if(archivo.open( QIODevice::WriteOnly )){
                QTextStream out(&archivo);
                QString qCodigo;
                QString qNombre;
                QString qInstitucionEjecuta;
                QString qAmbitoProyecto;
                QString qInstitucionFinancia;
                QString qInstitucionFinanciaDos;
                QString qPorcentajeInstitucionFinancia;
                QString qPorcentajeInstitucionFinanciaDos;
                QString qMontoAprobado;
                QString qMontoSolicitado;
                QString qFechaInicio;
                QString qFechaFin;
                QString qObjetivos;
                QString qComunidad;
                QString qNumeroBeneficiarios;
                QString qEstatus;
                QString qContacto;
                QString qIndicador;
                QString qComentario;
                QString qInsersion;
                QString newlineChar = "\n";
                //int i = 0;
                out << "Codigo;Nombre;InstitucionEjecuta;AmbitoProyecto;InstitucionFinancia;InstitucionFinanciaDos;"
                    << "PorcentajeInstitucionFinancia;PorcentajeInstitucionFinanciaDos;MontoAprobado;MontoSolicitado;"
                    << "FechaInicio;FechaFin;Objetivos;Comunidad;NumeroBeneficiarios;Estatus;Contacto;Indicador;"
                    << "Comentario;Insersion;";
                out << newlineChar;

                for (int row = 0; row < rowCount; row++){
                    qCodigo = model->data(model->index(row, 0)).toString().simplified().replace(QString(";"),QString(","));
                    qNombre = model->data(model->index(row, 1)).toString().simplified().replace(QString(";"),QString(","));
                    qInstitucionEjecuta = model->data(model->index(row, 2)).toString().simplified().replace(QString(";"),QString(","));
                    qAmbitoProyecto = model->data(model->index(row, 3)).toString().simplified().replace(QString(";"),QString(","));
                    qInstitucionFinancia = model->data(model->index(row, 4)).toString().simplified().replace(QString(";"),QString(","));
                    qInstitucionFinanciaDos = model->data(model->index(row, 5)).toString().simplified().replace(QString(";"),QString(","));
                    qPorcentajeInstitucionFinancia = model->data(model->index(row, 6)).toString().simplified().replace(QString(";"),QString(","));
                    qPorcentajeInstitucionFinanciaDos = model->data(model->index(row, 7)).toString().simplified().replace(QString(";"),QString(","));
                    qMontoAprobado = model->data(model->index(row, 8)).toString().simplified().replace(QString(";"),QString(","));
                    qMontoSolicitado = model->data(model->index(row, 9)).toString().simplified().replace(QString(";"),QString(","));
                    qFechaInicio = model->data(model->index(row, 10)).toString().simplified().replace(QString(";"),QString(","));
                    qFechaFin = model->data(model->index(row, 11)).toString().simplified().replace(QString(";"),QString(","));
                    qObjetivos = model->data(model->index(row, 12)).toString().simplified().replace(QString(";"),QString(","));
                    qComunidad = model->data(model->index(row, 13)).toString().simplified().replace(QString(";"),QString(","));
                    qNumeroBeneficiarios = model->data(model->index(row, 14)).toString().simplified().replace(QString(";"),QString(","));
                    qEstatus = model->data(model->index(row, 15)).toString().simplified().replace(QString(";"),QString(","));
                    qContacto = model->data(model->index(row, 16)).toString().simplified().replace(QString(";"),QString(","));
                    qIndicador = model->data(model->index(row, 17)).toString().simplified().replace(QString(";"),QString(","));
                    qComentario = model->data(model->index(row, 18)).toString().simplified().replace(QString(";"),QString(","));
                    qInsersion = model->data(model->index(row, 19)).toString().simplified().replace(QString(";"),QString(","));
                    out << (QString("%1;").arg((!qCodigo.isEmpty()) ? qCodigo : QString(" ")));
                    out << (QString("%1;").arg((!qNombre.isEmpty()) ? qNombre : QString(" ")));
                    out << (QString("%1;").arg((!qInstitucionEjecuta.isEmpty()) ? qInstitucionEjecuta : QString(" ")));
                    out << (QString("%1;").arg((!qAmbitoProyecto.isEmpty()) ? qAmbitoProyecto : QString(" ")));
                    out << (QString("%1;").arg((!qInstitucionFinancia.isEmpty()) ? qInstitucionFinancia : QString(" ")));
                    out << (QString("%1;").arg((!qInstitucionFinanciaDos.isEmpty()) ? qInstitucionFinanciaDos : QString(" ")));
                    out << (QString("%1;").arg((!qPorcentajeInstitucionFinancia.isEmpty()) ? qPorcentajeInstitucionFinancia : QString(" ")));
                    out << (QString("%1;").arg((!qPorcentajeInstitucionFinanciaDos.isEmpty()) ? qPorcentajeInstitucionFinanciaDos : QString(" ")));
                    out << (QString("%1;").arg((!qMontoAprobado.isEmpty()) ? qMontoAprobado : QString(" ")));
                    out << (QString("%1;").arg((!qMontoSolicitado.isEmpty()) ? qMontoSolicitado : QString(" ")));
                    out << (QString("%1;").arg((!qFechaInicio.isEmpty()) ? qFechaInicio : QString(" ")));
                    out << (QString("%1;").arg((!qFechaFin.isEmpty()) ? qFechaFin : QString(" ")));
                    out << (QString("%1;").arg((!qObjetivos.isEmpty()) ? qObjetivos : QString(" ")));
                    out << (QString("%1;").arg((!qComunidad.isEmpty()) ? qComunidad : QString(" ")));
                    out << (QString("%1;").arg((!qNumeroBeneficiarios.isEmpty()) ? qNumeroBeneficiarios : QString(" ")));
                    out << (QString("%1;").arg((!qEstatus.isEmpty()) ? qEstatus : QString(" ")));
                    out << (QString("%1;").arg((!qContacto.isEmpty()) ? qContacto : QString(" ")));
                    out << (QString("%1;").arg((!qIndicador.isEmpty()) ? qIndicador : QString(" ")));
                    out << (QString("%1;").arg((!qComentario.isEmpty()) ? qComentario : QString(" ")));
                    out << (QString("%1;").arg((!qInsersion.isEmpty()) ? qInsersion : QString(" ")));
                    out << newlineChar;
                }
                archivo.close();

                //archivo.write(finalText);

            }else{
                msj_error(QString("No se puedo abrir el archivo %1").arg(fileName));
            }

        }
   }
}
