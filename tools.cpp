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

#include "tools.h"
#include "ui_tools.h"
#include "pdfprinter_db.h"
#include "csv_manejador_db.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QSql>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <iostream>
#include <QImage>
#include <QDesktopServices>
#include <QDir>
#include <QIcon>
#include <QTextStream>
#include <QSqlQueryModel>
#include <QProgressBar>


tools::tools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tools)
{
    ui->setupUi(this);
    QImage image("images/imagenexamplepdf.jpg");
    ui->labelImagePdf->setPixmap(QPixmap::fromImage(image));
    QImage image2("images/imagenexampledetallepdf.jpg");
    ui->labelImagePdf_2->setPixmap(QPixmap::fromImage(image2));
    ui->pushButtonImport->setIcon(QIcon("images/import.png"));
    QImage image4("images/pdficon.png");
    ui->labelIcon->setPixmap(QPixmap::fromImage(image4));
    ui->labelIcon_2->setPixmap(QPixmap::fromImage(image4));
    ui->pushButtonHacerExport->setIcon(QIcon("images/import.png"));
}

tools::~tools()
{
    delete ui;
}

void tools::on_pushButtonExportarPDF_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Proyecto a Guardar en PDF"), QString(),
                tr("PDF(*.pdf)"));
    if(!fileName.isEmpty()){
        if(!fileName.contains(".pdf")){
            fileName.append("X.pdf");
        }
        QString comentarioIntro = ui->plainTextEditComentarioStart->toPlainText();
        QString comentarioFin = ui->plainTextEditComentarioEnd->toPlainText();


        if(comentarioIntro.isEmpty()){
            comentarioIntro = "Este documento fue exportado del Banco de Proyectos";
        }
        if(comentarioFin.isEmpty()){
            comentarioFin = "Datos exportados a pdf satisfactoriamente";
        }
        bool estricto;
        estricto = ui->checkBoxFiltroEstricto->isTristate();
        QString titulo = ui->lineEditTituloPdf->text();
        QString filtro = ui->lineEditFiltroParametro->text();
        int combobox = ui->comboBoxFiltro->currentIndex();
        QString atributoFiltro;

        switch (combobox) {
        case 0:
            atributoFiltro = "CodigoUnico";
            break;
        case 1:
            atributoFiltro = "Nombre";
            break;
        case 2:
            atributoFiltro = "InstitucionEjecuta";
            break;
        case 3:
            atributoFiltro = "AmbitoProyecto";
            break;
        case 4:
            atributoFiltro = "InstitucionFinancia";
            break;
        case 5:
            atributoFiltro = "InstitucionFinanciaDos";
            break;
        case 6:
            atributoFiltro = "MontoAprobado";
            break;
        case 7:
            atributoFiltro = "MontoSolicitado";
            break;
        case 8:
            atributoFiltro = "Comunidad";
            break;
        case 9:
            atributoFiltro = "Estatus";
            break;
        case 10:
            atributoFiltro = "Indicador";
            break;
        default:
            break;
        }
        pdfprinter_db * r = new pdfprinter_db();
        r->imprimir_pdf(fileName,atributoFiltro,filtro,estricto,comentarioIntro,comentarioFin,titulo);
    }else{
        //msj_error("no puede ser un nombre vacio");
        QMessageBox::critical(this,tr("Error en la Herramienta"),tr("No puede ser un nombre vacio") );
    }
}

void tools::msj_error(QString error){

    QMessageBox msgBox;
    msgBox.setText(error);
    msgBox.exec();
}


void tools::on_pushButtonClean_clicked()
{
    ui->plainTextEditComentarioStart->clear();
    ui->plainTextEditComentarioEnd->clear();
    ui->lineEditTituloPdf->clear();
    ui->lineEditFiltroParametro->clear();
}

void tools::on_pushButtonToolPdf_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void tools::on_pushButtonToolCsv_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void tools::on_pushButtonExportarDetallePdf_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void tools::on_pushButtonExportarCsv_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void tools::on_pushButtonSeleccioneCsv_clicked()
{
    QString csvDireccion = QFileDialog::getOpenFileName(this, tr("Importar a Archivo CSV"), QString(),
               tr("Hoja de calculo (*.csv);;"));
    direccionGloval = csvDireccion;
    ui->lineEditArchivoImportarCsv->setText(direccionGloval);
}

void tools::on_pushButtonImport_clicked()
{
    //ESTA ES LA FUNCION DE CSV
    QString csvDireccion = direccionGloval;
    if(!csvDireccion.isEmpty()){
        //    //preparar la tabla
        //    QDir dir(QStandardPaths::standardLocations(QStandardPaths::DataLocation)[2]);
        //    QString dbName = dir.filePath("AuxProDB.sqlite");

        //    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        //    db.setDatabaseName(dbName);

        //    if (!db.open()) {
        //        QMessageBox msgBox;
        //        msgBox.setText("Error al crear la base de datos auxiliar. No se pudo realizar la improtación");
        //        msgBox.exec();
        //    }

        QSqlQuery query;
        //    query.exec("CREATE TABLE AuxProyecto(CodigoUnico char(20),Nombre char(50) UNIQUE,InstitucionEjecuta char(50),AmbitoProyecto char(20),"
        //               "InstitucionFinancia char(50),InstitucionFinanciaDos char(50),"
        //               "PorcentajeAporteInstitucion char(10),PorcentajeAporteInstitucionDos char(10),"
        //               "MontoAprobado char(10),MontoSolicitado char(10),FechaInicio datetime,FechaFin datetime,"
        //               "Objetivos char(60),Comunidad char(50),NumeroBeneficiarios char(10),Estatus char(10),Contacto char(25),"
        //               "Indicador char(10),Comentario char(50),Insercion datetime,PRIMARY KEY(CodigoUnico))");

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

        //
        QFile archivo(csvDireccion);

        if(archivo.open( QIODevice::ReadWrite )){
            //QProgressDialog progress(QObject::tr("Decoding CSV file..."), QObject::tr("Cancel"), 0, archivo.size());
            //progress.setWindowModality(Qt::ApplicationModal);

            //QByteArray line, ext;
            QStringList list;
            QString line;
            int i=0;
            bool t = true;
            line = archivo.readLine();
            while(!archivo.atEnd()){
                line = archivo.readLine();
                //progress.setValue(i);

                list = line.split(";");
                qCodigo = list.value(0);
                qNombre = list.value(1);
                qInstitucionEjecuta= list.value(2);
                qAmbitoProyecto= list.value(3);
                qInstitucionFinancia = list.value(4);
                qInstitucionFinanciaDos = list.value(5);
                qPorcentajeInstitucionFinancia = list.value(6);
                qPorcentajeInstitucionFinanciaDos = list.value(7);
                qMontoAprobado = list.value(8);
                qMontoSolicitado = list.value(9);
                qFechaInicio = list.value(10);
                qFechaFin = list.value(11);
                qObjetivos = list.value(12);
                qComunidad = list.value(13);
                qNumeroBeneficiarios = list.value(14);
                qEstatus = list.value(15);
                qContacto = list.value(16);
                qIndicador = list.value(17);
                qComentario = list.value(18);
                qInsersion = list.value(19);
                i++;

                query.prepare("INSERT OR IGNORE INTO Proyecto (Nombre, InstitucionFinancia, InstitucionFinanciaDos, PorcentajeAporteInstitucion,"
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

                if((!query.exec()) && t){
                    msj_error("Error al importar alguno de los atributos del archivo CSV");
                    t=false;
                }

            }
            msj_error("Se ha exportado satisfactoriamente");
            //query.exec(QString("ATTACH DATABASE 'C:/Users/Raul/compilador borrable de qt/debug/AuxProDB.sqlite' AS toMerge"));
            //query.exec(QString("INSERT OR IGNORE INTO Proyecto SELECT * FROM toMerge.AuxProyecto"));
            //query.exec(QString("DETACH DATABASE toMerge"));

        }else{

            msj_error(QString("No se pudo abrir el archivo %1. Cierre cualquier programa que lo este usando").arg(csvDireccion));
        }
    }else{
        QMessageBox::critical(this,tr("Error en la Herramienta"),tr("Debe seleccionar un archivo CSV primero") );
         //msj_error("Debe seleccionar un archivo CSV primero");
    }
}

void tools::on_pushButtonClean_2_clicked()
{
    ui->plainTextEditComentarioStart_2->clear();
    ui->plainTextEditComentarioEnd_2->clear();
    ui->lineEditTituloPdf_2->clear();
    ui->lineEditFiltroParametro_2->clear();
}

void tools::on_pushButtonExportarPDF_2_clicked()
{

    QString comentarioIntro = ui->plainTextEditComentarioStart_2->toPlainText();
    QString comentarioFin = ui->plainTextEditComentarioEnd_2->toPlainText();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Proyecto a Guardar en PDF"), QString(),
                tr("PDF(*.pdf)"));
    if(!fileName.isEmpty()){
        if(!fileName.contains(".pdf")){
            fileName.append("X.pdf");
        }

        if(comentarioIntro.isEmpty()){
            comentarioIntro = "Este documento fue exportado de su base de datos";
        }
        if(comentarioFin.isEmpty()){
            comentarioFin = "Datos exportados a pdf satisfactoriamente";
        }
        bool estricto;
        estricto = ui->checkBoxFiltroEstricto_2->isTristate();
        QString titulo = ui->lineEditTituloPdf_2->text();
        QString filtro = ui->lineEditFiltroParametro_2->text();

        int combobox = ui->comboBoxFiltro_2->currentIndex();
        QString atributoFiltro;
        /*CodigoUnico,Nombre,InstitucionEjecuta,AmbitoProyecto,InstitucionFinancia,InstitucionFinanciaDos,"
                            "PorcentajeAporteInstitucion,PorcentajeAporteInstitucionDos,"
                            "MontoAprobado,MontoSolicitado,FechaInicio,FechaFin,Objetivos,Comunidad,NumeroBeneficiarios,"
                            "Estatus,Contacto,Indicador,Comentario,Insercion*/
        switch (combobox) {
        case 0:
            atributoFiltro = "CodigoUnico";
            break;
        case 1:
            atributoFiltro = "Nombre";
            break;
        case 2:
            atributoFiltro = "InstitucionEjecuta";
            break;
        case 3:
            atributoFiltro = "AmbitoProyecto";
            break;
        case 4:
            atributoFiltro = "InstitucionFinancia";
            break;
        case 5:
            atributoFiltro = "InstitucionFinanciaDos";
            break;
        case 6:
            atributoFiltro = "MontoAprobado";
            break;
        case 7:
            atributoFiltro = "MontoSolicitado";
            break;
        case 8:
            atributoFiltro = "Comunidad";
            break;
        case 9:
            atributoFiltro = "Estatus";
            break;
        case 10:
            atributoFiltro = "Indicador";
            break;
        default:
            break;
        }

        pdfprinter_db * r = new pdfprinter_db();
        r->imprimir_pdf_detalles(fileName,atributoFiltro,filtro,estricto,comentarioIntro,comentarioFin,titulo);
    }else{
        QMessageBox::critical(this,tr("Error en la Herramienta"),tr("no puede ser un nombre vacio"));
        //msj_error("no puede ser un nombre vacio");
    }
}

void tools::on_pushButtonGoExportarCSV_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Proyecto a exportar a CSV"), QString(),
                tr("CSV(*.CSV)"));
    direccionExportCSV = fileName;
    ui->lineEditDireccionExportarCSV->setText(fileName);
}

void tools::on_pushButtonHacerExport_clicked()
{\
    QString fileName = direccionExportCSV;
    if(!fileName.isEmpty()){
        if(!fileName.contains(".CSV")){
            fileName.append("X.CSV");
        }
        QString filtro = "Nombre";
        QString parametro_filtro = "%%";
        QSqlQuery query(QString("SELECT CodigoUnico,Nombre,InstitucionEjecuta,AmbitoProyecto,InstitucionFinancia,InstitucionFinanciaDos,"
                                "PorcentajeAporteInstitucion,PorcentajeAporteInstitucionDos,"
                                "MontoAprobado,MontoSolicitado,FechaInicio,FechaFin,Objetivos,Comunidad,NumeroBeneficiarios,"
                                "Estatus,Contacto,Indicador,Comentario,Insercion FROM Proyecto WHERE %1 LIKE '%2'").arg(filtro).arg(parametro_filtro));

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


            int i = 0;
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
                qObjetivos = model->data(model->index(row, 12)).toString().simplified().replace(QString(";"),QString(",")).replace(QString("\n"),QString("CHAR(10)"));
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
            msj_error("Se ha exportado satisfactoriamente");
            //archivo.write(finalText);

        }else{
            msj_error(QString("No se puedo abrir el archivo %1").arg(fileName));
        }

    }else{
        msj_error("No puede ser un nombre vacio");
    }
}
