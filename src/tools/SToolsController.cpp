#include "tools/SToolsController.h"

SToolsController* SToolsController::mpInstance = NULL;

SToolsController::SToolsController(){
	mCurrentTool = eTool_Pen;
}

SToolsController::~SToolsController(){

}

SToolsController* SToolsController::toolsController(){
	if(NULL == mpInstance){
		mpInstance = new SToolsController();
	}
	return mpInstance;
}

void SToolsController::onSetCurrentTool(eTool tool){
	mCurrentTool = tool;
}

eTool SToolsController::currentTool(){
	return mCurrentTool;
}
