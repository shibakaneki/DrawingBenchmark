#include "SDocumentManager.h"
#include "workspace/SDrawingView.h"
#include "workspace/SMainWnd.h"

SDocumentManager* SDocumentManager::mpInstance = NULL;

SDocumentManager::SDocumentManager() : mNextLayerID(0),
		mpCurrentLayer(NULL){

}

SDocumentManager::~SDocumentManager(){

}

SDrawingView* SDocumentManager::addLayer(const QString& name, QWidget* parent){
	SLayer* layer = new SLayer();
	layer->id = mNextLayerID;
	mNextLayerID++;
	layer->name = name;
	layer->visible = true;
	layer->layerWidget = new SDrawingView(parent);
	if(NULL != parent){
		layer->layerWidget->resize(parent->size());
		if(layer->id == 0){
			layer->layerWidget->drawBackgroundLeaf(640, 480);
		}
	}
	mLayers << layer;
	mpCurrentLayer = layer;
	return layer->layerWidget;
}

SDocumentManager* SDocumentManager::documentManager(){
	if(NULL == mpInstance){
		mpInstance = new SDocumentManager();
	}
	return mpInstance;
}

void SDocumentManager::onClear(){
	if(NULL != mpCurrentLayer){
		mpCurrentLayer->layerWidget->clearPage();
	}
}
