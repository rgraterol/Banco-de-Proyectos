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

#include "pdfprinter_db.h"
#include <QtPrintSupport/qprinter.h>
#include <qpainter.h>
#include <QSqlQuery>
#include <qsqlrecord.h>
#include <QVariant>
#include <QTextDocument>
#include <iostream>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTextStream>
#include <QMessageBox>

pdfprinter_db::pdfprinter_db()
{
}

void pdfprinter_db::imprimir_pdf(QString nombreArchivo, QString filtro, QString parametro_filtro, bool filtro_estricto,QString msjIntro, QString msjFin, QString tituloPdf){
    /*Creacion de la impresion*/
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QSizeF(8.5, 13), QPrinter::Inch);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(true);
    printer.setOutputFileName(nombreArchivo);

    /*Especializacion del filtro*/
    if(!filtro_estricto){
        parametro_filtro.prepend("%");
        parametro_filtro.append("%");
    }
    /*Llamada a la tabla bajo parametros de filtro*/
    QSqlQuery query(QString("SELECT CodigoUnico,Nombre,InstitucionEjecuta,AmbitoProyecto,InstitucionFinancia,InstitucionFinanciaDos,"
                            "PorcentajeAporteInstitucion,PorcentajeAporteInstitucionDos,"
                            "MontoAprobado,MontoSolicitado,FechaInicio,FechaFin,Objetivos,Comunidad,NumeroBeneficiarios,"
                            "Estatus,Contacto,Indicador,Comentario,Insercion FROM Proyecto WHERE %1 LIKE '%2'").arg(filtro).arg(parametro_filtro));

    //QSqlQuery query(QString("SELECT Nombre,InstitucionEjecuta,InstitucionFinancia,Monto,AnoVigencia,Comunidad,Objetivos,Indicadores,Comentario,Insercion FROM Proyecto WHERE %1 LIKE '%2'").arg(filtro).arg(parametro_filtro));
    /*Modelo auxiliar para SQLquery*/

    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery(query);
    QList<QVariant> list = query.boundValues().values();
    for(int i=0;i<list.size();i++){
        model->setHeaderData(i,Qt::Horizontal,list.at(i).data());
    }

    /*for(int i=0; i<=8;i++){
        model->setHeaderData(i, Qt::Horizontal,query.value(i));
    }*/

    QTableView *view = new QTableView;
    /*Impresion lineal consecutiva de tabla en PDF*/
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = model->rowCount();
    const int columnCount = model->columnCount();

        out <<  "<html>\n"
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
            <<  QString("<title>%1</title>\n").arg(tituloPdf)
            << "<style type=text/css>"
            << "font {size:5}"
            << "</style>"
            <<  "</head>\n"
            <<"<body bgcolor=#ffffff link=#5000A0 >\n"
            << QString("<h1>%1</h1>").arg(tituloPdf)
            <<  QString("<p>%1</p>").arg(msjIntro)
            <<"<table border=1 cellspacing=0 cellpadding=2 width=98%>\n";

        // headers
        out << "<thead><tr bgcolor=#f0f0f0>";
        for (int column = 0; column < columnCount; column++)
            if (!view->isColumnHidden(column)){
                out << QString("<th>%1</th>").arg(model->headerData(column, Qt::Horizontal).toString());
            }
        out << "</tr></thead>\n";

        // data table
        for (int row = 0; row < rowCount; row++) {
            out << "<tr>";
            for (int column = 0; column < columnCount; column++) {
                if (!view->isColumnHidden(column)) {
                    QString data = model->data(model->index(row, column)).toString().simplified();
                    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
            }
            out << "</tr>\n";
        }
        out <<  "</table>\n"
            <<QString("<p>%1</p>").arg(msjFin)
            <<"</body>\n"
            <<"</html>\n";

        QTextDocument *document = new QTextDocument();
        document->setHtml(strStream);
        document->adjustSize();
        document->print(&printer);
        delete document;
        msj_error_pdf("Se ha realizado la tarea con exito");
}

void pdfprinter_db::imprimir_pdf_detalles(QString nombreArchivo, QString filtro, QString parametro_filtro, bool filtro_estricto, QString msjIntro, QString msjFin, QString tituloPdf)
{
    /*Creacion de la impresion*/
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
   // printer.setPaperSize(QSizeF(8.5, 13), QPrinter::Inch);
    printer.setPaperSize(QPrinter::Letter);
    printer.setFullPage(true);
    printer.setOutputFileName(nombreArchivo);

    /*Especializacion del filtro*/
    if(!filtro_estricto){
        parametro_filtro.prepend("%");
        parametro_filtro.append("%");
    }
    /*Llamada a la tabla bajo parametros de filtro*/
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
    QString qEstatus_aux;


        out <<  "<html>\n"
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
            <<  QString("<title>%1</title>\n").arg(tituloPdf)
            << "<style type=text/css>"
            << "font {size:5}"
            << "</style>"
            <<  "</head>\n"
            <<"<body bgcolor=#ffffff link=#5000A0 >\n"
            << QString("<h1>%1</h1>").arg(tituloPdf)
            <<  QString("<p>%1</p>").arg(msjIntro);


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
            qObjetivos = model->data(model->index(row, 12)).toString().simplified();
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
            qContacto = model->data(model->index(row, 16)).toString().simplified();
            qIndicador = model->data(model->index(row, 17)).toString().simplified();
            qComentario = model->data(model->index(row, 18)).toString().simplified();
            qInsersion = model->data(model->index(row, 19)).toString().simplified();
            out << QString("<p><b>Código del Proyecto: </b> %1</p>").arg((!qCodigo.isEmpty()) ? qCodigo : QString("&nbsp;"));
            out << QString("<p><b>Nombre del Proyecto: </b> %1</p>").arg((!qNombre.isEmpty()) ? qNombre : QString("&nbsp;"));
            out << QString("<p><b>Institución que Ejecuta el Proyecto: </p></b> %1").arg((!qInstitucionEjecuta.isEmpty()) ? qInstitucionEjecuta : QString("&nbsp;"));
            out << QString("<p><b>Ámbito del Proyecto: </b></p> %1").arg((!qAmbitoProyecto.isEmpty()) ? qAmbitoProyecto : QString("&nbsp;"));
            out << QString("<p><b>Institución que financia el Proyecto: </b> %1</p>").arg((!qInstitucionFinancia.isEmpty()) ? qInstitucionFinancia : QString("&nbsp;"));
            out << QString("<p><b>Segunda institución que financia el Proyecto: </b> %1</p>").arg((!qInstitucionFinanciaDos.isEmpty()) ? qInstitucionFinanciaDos : QString("&nbsp;"));
            out << QString("<p><b>Porcentaje de aporte de primera institución: </b> %1</p>").arg((!qPorcentajeInstitucionFinancia.isEmpty()) ? qPorcentajeInstitucionFinancia : QString("&nbsp;"));
            out << QString("<p><b>Porcentaje de aporte de segunda institución: </b> %1</p>").arg((!qPorcentajeInstitucionFinanciaDos.isEmpty()) ? qPorcentajeInstitucionFinanciaDos : QString("&nbsp;"));
            out << QString("<p><b>Monto aprobado (en Bolivares): </b> %1</p>").arg((!qMontoAprobado.isEmpty()) ? qMontoAprobado : QString("&nbsp;"));
            out << QString("<p><b>Monto solicitado (en Bolivares): </b> %1</p>").arg((!qMontoSolicitado.isEmpty()) ? qMontoSolicitado : QString("&nbsp;"));
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

        out <<QString("<p>%1</p>").arg(msjFin)
            <<"</body>\n"
            <<"</html>\n";

        QTextDocument *document = new QTextDocument();
        document->setHtml(strStream);
        document->adjustSize();
        document->print(&printer);
        delete document;
        msj_error_pdf("Se ha realizado la tarea con exito");
}

void pdfprinter_db::msj_error_pdf(QString error){
    QMessageBox msgBox;
    msgBox.setText( error);
    msgBox.exec();
}
