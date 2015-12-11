#include "mainwindow.h"
#include <vector>
using namespace std;

vector<pair<float,float>> MainWindow::loadData(string strFile)
{
	 vector<pair<float,float>> data_xy;
	 QFile file(QString::fromStdString(strFile));
	 if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return data_xy;
	 }

	 while (!file.atEnd()) {
        
		QString strLine = file.readLine();
		QStringList lineStrList = strLine.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
		
		if(lineStrList.size()>0)
		{
			QStringList strNumbers = lineStrList.at(0).split(',');
			if(strNumbers.size()>=2)
			{
				float xx = strNumbers.at(0).toFloat();
				float yy = strNumbers.at(1).toFloat();
				data_xy.push_back(pair<float,float>(xx,yy));
			}
		}
    }
	return data_xy;
}
