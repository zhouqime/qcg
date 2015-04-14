#ifndef QGEODATABASE
#define QGEODATABASE

#include <memory>
#include <QtGlobal>
#include <QString>
#include <QVariant>

#include "qcommon.h"

namespace QCG {

    struct Point2D
	{
        double x,y;
	};

	enum GeometryType
	{
		GT_Unknow = 0,
		GT_Point = 1,
		GT_Path = 2,
		GT_Ring = 3,
		GT_Polyline = 4,
		GT_Polygon = 5,
		GT_MultiGeometry = 6		
	};

	struct IGeometryFactory;
    typedef struct HGEOMETRY__ * HGEOMETRY;


	struct IGeometry :public Interface
	{
		virtual IGeometryFactory* factory() = 0;
        virtual HGEOMETRY handle()=0;

		virtual GeometryType geometryType() = 0;
		virtual int partCount() = 0; //for GT_Point\GT_Path\GT_Ring,return 1
        virtual HGEOMETRY part(int index) = 0; //ref of the part, for GT_Point\GT_Path\GT_Ring,return handle of self.
		virtual void removePart(int index) = 0;
		virtual void insertPart(int before_pos, IGeometry* geo) = 0;

		virtual int pointCount() = 0; //only used in GT_Point\GT_Path\GT_Ring ,for GT_Point, return 1
        virtual Point2D point(int index) = 0; //copy of the Point, for GT_Point,return self,only used in GT_Point\GT_Path\GT_Ring.
        virtual void updatePoint(int index, Point2D pt) = 0;
		virtual void removePoint(int index) = 0;
        virtual void insertPoint(int before_pos, Point2D geo) = 0;
	};

	struct IGeometryFactory : public Interface
	{
		virtual std::unique_ptr<IGeometry> createGeometry(GeometryType) = 0;
        virtual std::unique_ptr<IGeometry> fromHandle(HGEOMETRY) = 0;
		virtual std::unique_ptr<IGeometry> cloneGeometry(IGeometry *) = 0;
	};

}


//enum FieldValueType
//{
//    FVT_Interger,
//    FVT_Float,
//    FVT_String,
//    FVT_Blob,
//    FVT_Geometry
//};


//struct IField:public Interface
//{
//    virtual QString Name() = 0;
//    virtual FieldValueType FieldValueType() = 0;
//};

//struct IFeature:public Interface
//{
//    virtual int featureID() = 0;
//    virtual int fieldCount() = 0;
//    virtual IField * field(int index) = 0;
//    virtual QVariant value(int index) = 0;
//};

#endif // QGEODATABASE

