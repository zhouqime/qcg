#ifndef ESRIGEODATABASE_H
#define ESRIGEODATABASE_H

#include "qgeodatabase.h"

class ESRIGeometryFactory :public QCG::IGeometryFactory
{
public:
	static ESRIGeometryFactory* factory();
    // IGeometryFactory interface
public:
    std::unique_ptr<QCG::IGeometry> createGeometry(QCG::GeometryType);
    std::unique_ptr<QCG::IGeometry> cloneGeometry(QCG::IGeometry *);
    std::unique_ptr<QCG::IGeometry> fromHandle(QCG::HGEOMETRY);
private:
	ESRIGeometryFactory(){}
	virtual ~ESRIGeometryFactory(){}

};

#endif // ESRIGEODATABASE_H
