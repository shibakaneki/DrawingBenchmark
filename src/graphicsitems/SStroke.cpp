#include <math.h>

#include "SStroke.h"
#include "maths/SGeometryHelper.h"

SStroke::SStroke(QPen p, QGraphicsScene* s) : mpPath(NULL)
{
	mPen = p;
	mpScene = s;
	mpDrawingController = SDrawingController::drawingController();
}

SStroke::~SStroke(){

}

void SStroke::addPoint(sPoint* p){
    mPoints <<p;
}

void SStroke::generatePath(){
    if(NULL !=mpPath){
        mpScene->removeItem(mpPath);
    }
    //mpPath = new QGraphicsPathItem();
    //mpPath->setPen(mPen);
    QPainterPath path;
    if(!mPoints.empty()){
        if(mPoints.size() > 1){
            for(int i=mPoints.size()-2; i<mPoints.size(); i++){
                if(i>=0){
                    if(i==0){
                        mPoints.at(i)->dx = ((mPoints.at(i+1)->x - mPoints.at(i)->x) / SMOOTHING);
                        mPoints.at(i)->dy = ((mPoints.at(i+1)->y - mPoints.at(i)->y) / SMOOTHING);
                    }else if(i == mPoints.size()-1){
                        mPoints.at(i)->dx = ((mPoints.at(i)->x - mPoints.at(i-1)->x) / SMOOTHING);
                        mPoints.at(i)->dy = ((mPoints.at(i)->y - mPoints.at(i-1)->y) / SMOOTHING);
                    }else{
                        mPoints.at(i)->dx = ((mPoints.at(i + 1)->x - mPoints.at(i-1)->x) / SMOOTHING);
                        mPoints.at(i)->dy = ((mPoints.at(i + 1)->y - mPoints.at(i-1)->y) / SMOOTHING);
                    }
                }
            }
        }
    }

    bool first = true;
    for(int i=0; i<mPoints.size(); i++){
        sPoint pt = *mPoints.at(i);
        if(first){
            first = false;
            path.moveTo(pt.x, pt.y);
        }else{
            sPoint prev = *mPoints.at(i-1);
            sPoint c0, c1;

            c0.x = prev.x + prev.dx;
            c0.y = prev.y + prev.dy;
            c1.x = pt.x - pt.dx;
            c1.y = pt.y - pt.dy;
            path.cubicTo(c0.x, c0.y, c1.x, c1.y, pt.x, pt.y);
        }
    }
    //mpPath->setPath(path);
    mpPath = mpDrawingController->currentBrush()->renderPath(path);
    mpScene->addItem(mpPath);
}

QList<qreal> SStroke::generatePressures(qreal before, qreal next, int step){
	QList<qreal> pressures;
	qreal delta = (next-before)/step;

	for(int i=0; i<step; i++){
		pressures << before + i*delta;
	}

	return pressures;
}
