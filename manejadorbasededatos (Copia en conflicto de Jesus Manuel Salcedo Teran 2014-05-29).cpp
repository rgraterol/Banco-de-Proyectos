#include "manejadorbasededatos.h"
#include "proyecto.h"
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

ManejadorBasedeDatos::ManejadorBasedeDatos()
{

}

void ManejadorBasedeDatos::Write_BasedeDatos(linea_texto){
    QFile file("bdd.prstt");
    if(!file.open(QFile::Append | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("No se pudo abrir la base de datos");
        msgBox.exec();
    }

}
void ManejadorBasedeDatos::Read_BasedeDatos(){
    QFile file(filename);
    QString linea;

    if(!file.open(QFile::ReadOnly | QFile::Text)
    {
    return;
    }
    QTextStream in(&file);
}
