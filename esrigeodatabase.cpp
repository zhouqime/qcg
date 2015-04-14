#include "esrigeodatabase.h"

#import "C:\Program Files (x86)\ArcGIS\Desktop10.1\com\esriSystem.olb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "VARTYPE","XMLSerializer")
#import "C:\Program Files (x86)\ArcGIS\Desktop10.1\com\esriGeometry.olb" raw_interfaces_only raw_native_types no_namespace named_guids

struct ESRIGeometry :public QCG::IGeometry
{
public:
	ESRIGeometry(IGeometryPtr g):geo(g) {}
	ESRIGeometry(GUID guid) :geo(guid){}
	virtual ~ESRIGeometry(){}
	IGeometryPtr geo;
    // IGeometry interface
public:
    QCG::IGeometryFactory *factory();
    QCG::HGEOMETRY handle();
    QCG::GeometryType geometryType();

    int partCount();
    QCG::HGEOMETRY part(int index);
    void removePart(int index);
    void insertPart(int before_pos, QCG::IGeometry *geo);

    int pointCount();
    QCG::Point2D point(int index);
    void updatePoint(int index, QCG::Point2D pt);
    void removePoint(int index);
    void insertPoint(int before_pos, QCG::Point2D geo);

};

ESRIGeometryFactory *ESRIGeometryFactory::factory()
{
	static ESRIGeometryFactory f;

	return &f;
}

std::unique_ptr<QCG::IGeometry> ESRIGeometryFactory::createGeometry(QCG::GeometryType tp)
{
	QCG::IGeometry *geo;
	switch (tp)
	{
	case QCG::GT_Point:
		geo = new ESRIGeometry(CLSID_Point);
        break;
	case QCG::GT_Path:
		geo = new ESRIGeometry(CLSID_Path);
		break;
	case QCG::GT_Ring:
		geo = new ESRIGeometry(CLSID_Ring);
		break;
	case QCG::GT_Polyline:
		geo = new ESRIGeometry(CLSID_Polyline);
		break;
	case QCG::GT_Polygon:
		geo = new ESRIGeometry(CLSID_Polygon);
		break;
	case QCG::GT_MultiGeometry:
		geo = new ESRIGeometry(CLSID_GeometryBag);
		break;
	default:
		break;
	}

	return std::unique_ptr<QCG::IGeometry>(geo);
}

std::unique_ptr<QCG::IGeometry> ESRIGeometryFactory::cloneGeometry(QCG::IGeometry *g)
{
	if (g->factory() == factory())
	{
		ESRIGeometry* eg = reinterpret_cast<ESRIGeometry*>(g);
		IClonePtr c(eg->geo);
		IClonePtr cp;
		c->Clone(&cp);
		IGeometryPtr geo(cp);
		return  std::unique_ptr<QCG::IGeometry>(new ESRIGeometry(geo));
	}
	else
	{
		return std::unique_ptr<QCG::IGeometry>(nullptr);
    }
}

std::unique_ptr<QCG::IGeometry> ESRIGeometryFactory::fromHandle(QCG::HGEOMETRY h)
{
	IGeometryPtr g((IGeometry *)h);
	return std::unique_ptr<QCG::IGeometry>(new ESRIGeometry(g));
}


QCG::IGeometryFactory *ESRIGeometry::factory()
{
    return ESRIGeometryFactory::factory();
}

QCG::HGEOMETRY ESRIGeometry::handle()
{
	return reinterpret_cast<QCG::HGEOMETRY>(geo.GetInterfacePtr());
}

QCG::GeometryType ESRIGeometry::geometryType()
{
	esriGeometryType tp;
	geo->get_GeometryType(&tp);
	switch (tp)
	{
	case esriGeometryNull:
		break;
	case esriGeometryPoint:
		return QCG::GT_Point;
	case esriGeometryMultipoint:
		break;
	case esriGeometryLine:
		break;
	case esriGeometryCircularArc:
		break;
	case esriGeometryEllipticArc:
		break;
	case esriGeometryBezier3Curve:
		break;
	case esriGeometryPath:
		return QCG::GT_Path;
	case esriGeometryPolyline:
		return QCG::GT_Polyline;
	case esriGeometryRing:
		return QCG::GT_Ring;
	case esriGeometryPolygon:
		return QCG::GT_Polygon;
	case esriGeometryEnvelope:
		break;
	case esriGeometryAny:
		break;
	case esriGeometryBag:
		return QCG::GT_MultiGeometry;
	case esriGeometryMultiPatch:
		break;
	case esriGeometryTriangleStrip:
		break;
	case esriGeometryTriangleFan:
		break;
	case esriGeometryRay:
		break;
	case esriGeometrySphere:
		break;
	case esriGeometryTriangles:
		break;
	default:
		break;
	}
	return QCG::GT_Unknow;
}

int ESRIGeometry::partCount()
{
	auto tp = geometryType();
	switch (tp)
	{
	case QCG::GT_Point:
	case QCG::GT_Path:
	case QCG::GT_Ring:
		return 1;

	case QCG::GT_Polyline:
		break;
	case QCG::GT_Polygon:
		break;
	case QCG::GT_MultiGeometry:
		break;
	case QCG::GT_Unknow:
		return 0;
	default:
		break;
	}
	IGeometryCollectionPtr gc = geo;
	long c;
	gc->get_GeometryCount(&c);
	return c;
}

QCG::HGEOMETRY ESRIGeometry::part(int index)
{
	auto tp = geometryType();
	switch (tp)
	{
	case QCG::GT_Point:
	case QCG::GT_Path:
	case QCG::GT_Ring:
        return this->handle();
	case QCG::GT_Unknow:
		return 0;
	default:
		break;
	}
	IGeometryCollectionPtr gc = geo;
	IGeometryPtr part;
	gc->get_Geometry(index, &part);
	return (QCG::HGEOMETRY)part.GetInterfacePtr();
}

void ESRIGeometry::removePart(int index)
{
	assert(geometryType() >= QCG::GT_Polyline);

	IGeometryCollectionPtr gc = geo;
	gc->RemoveGeometries(index, 1);
}

void ESRIGeometry::insertPart(int before_pos, QCG::IGeometry *g)
{
	assert(g != nullptr);
	assert(g->factory() == this->factory());
	assert(geometryType() >= QCG::GT_Polyline);
	ESRIGeometry *eg = reinterpret_cast<ESRIGeometry*>(g);
	IGeometryCollectionPtr gc(geo);
	if (before_pos >= 0 && before_pos < partCount())
	{
		gc->InsertGeometries(before_pos, 1, &(eg->geo));
	}
	else
	{
		gc->AddGeometry(eg->geo);
	}
}

int ESRIGeometry::pointCount()
{
	auto t = geometryType();
	switch (t)
	{
	case QCG::GT_Point:
		return 1;		
	case QCG::GT_Unknow:
		return 0;
	default:
		break;
	}
	IPointCollectionPtr pc(geo);
	long c;
	pc->get_PointCount(&c);
	return c;
}

QCG::Point2D ESRIGeometry::point(int index)
{
	if (geometryType() == QCG::GT_Point)
	{
		assert(index == 0);
		IPointPtr pt(geo);
		double x, y;
		pt->QueryCoords(&x, &y);
		return QCG::Point2D{ x, y };
	}
	else
	{
		IPointCollectionPtr pc(geo);
		IPointPtr pt;
		pc->get_Point(index, &pt);
		double x, y;
		pt->QueryCoords(&x, &y);
		return QCG::Point2D{ x, y };
	}
}

void ESRIGeometry::updatePoint(int index, QCG::Point2D pt)
{
	if (geometryType() == QCG::GT_Point)
	{
		assert(index == 0);
		IPointPtr p(geo);
		p->PutCoords(pt.x, pt.y);
	}
	else
	{
		IPointCollectionPtr pc(geo);
		IPointPtr p;
		pc->get_Point(index, &p);
		p->PutCoords(pt.x, pt.y);
		pc->UpdatePoint(index, p);
	}
}

void ESRIGeometry::removePoint(int index)
{
	assert(geometryType() != QCG::GT_Point);
	IPointCollectionPtr pc(geo);
	pc->RemovePoints(index, 1);
}

void ESRIGeometry::insertPoint(int before_pos, QCG::Point2D pt)
{
	assert(geometryType() != QCG::GT_Point);
	IPointCollectionPtr pc(geo);
	
	IPointPtr p(CLSID_Point);
	p->PutCoords(pt.x, pt.y);
	if (before_pos >= 0 && before_pos < pointCount())
	{
		pc->InsertPoints(before_pos, 1, &p);
	}
	else
	{
		pc->AddPoint(p);
	}
}

