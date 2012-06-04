#include "SDocumentManager.h"
#include "workspace/SDrawingView.h"

static int mNextLayerID = 0;
static QVector<SLayer*> mLayers;

SDocumentManager::SDocumentManager(){

}

SDocumentManager::~SDocumentManager(){

}

void SDocumentManager::addLayer(const QString& name, QWidget* parent){
	SLayer* layer = new SLayer();
	layer->id = mNextLayerID;
	mNextLayerID++;
	layer->name = name;
	layer->visible = true;
	layer->layerWidget = new SDrawingView(parent);
	if(NULL != parent){
		layer->layerWidget->resize(parent->size());
		if(layer->id == 0){
			layer->layerWidget->drawBackgroundLeaf(1024, 768);
		}
	}
	mLayers << layer;
}
