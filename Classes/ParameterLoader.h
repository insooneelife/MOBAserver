#pragma once

#include <map>
#include <string>
#include "Singleton.h"
#include "json\reader.h"
#include "json\document.h"

//provide easy access
#define Prm ParameterLoader::getInstance()

class ParameterLoader : public Singleton<ParameterLoader>
{
public:
	//--------------------------Int Types-----------------------------//
	int NumCellsX;
	int NumCellsY;
	int NumSamplesForSmoothing;
	int NumMaxAgentCellSpace;
	int NumMaxClients;

	//--------------------------Double Types-----------------------------//
	double SteeringForceTweaker;
	double SteeringForce;
	double MaxSpeed;
	double EntityMass;
	double EntityRadius;
	double SeparationWeight;
	double AlignmentWeight;
	double CohesionWeight;
	double ObstacleAvoidanceWeight;
	double WallAvoidanceWeight;
	double WanderWeight;
	double SeekWeight;
	double FleeWeight;
	double ArriveWeight;
	double PursuitWeight;
	double OffsetPursuitWeight;
	double InterposeWeight;
	double HideWeight;
	double EvadeWeight;
	double FollowPathWeight;
	double ViewDistance;
	double MinDetectionBoxLength;
	double WallDetectionFeelerLength;
	double WanderRad;
	double WanderDist;
	double WanderJitterPerSec;
	double WaypointSeekDist;
	double StartViewX;
	double StartViewY;
	double BlueStartX;
	double BlueStartY;
	double RedStartX;
	double RedStartY;
	double CameraDistance;
	double GameSpeedInterval;
	double BorderSize;
	double MouseMoveVelocity;
	double MinObstacleRadius;
	double MaxObstacleRadius;
	double ResolutionSizeX;
	double ResolutionSizeY;
	double FrameSizeX;
	double FrameSizeY;
	double TileMapSizeX;
	double TileMapSizeY;
	double CharacterBaseAttackRange;
	double ExpGiveRange;

	//--------------------------Bool Types-----------------------------//
	bool RenderBoundingBox;
	bool RenderBoundingCircle;
	bool RenderBoundingShape;
	bool RenderViewCircle;
	bool RenderShadow;
	bool RenderMainLabel;
	bool RenderGoalLabel;
	bool RenderHealthLabel;
	bool RenderMemoryLabel;
	bool RenderCellSpace;
	bool RenderGraph;
	bool RenderPathLine;

	//--------------------------String Types-----------------------------//
	std::string CharacterFolderName;
	std::string StructureFolderName;
	std::string FontFolderName;
	std::string ParticleFolderName;
	std::string FontName;
	std::string DefaultCharacter;
	std::string RenderCellSpaceType;	//"collision", "agent", "graph"


	//for debug
	std::string global_str;

private:
	ParameterLoader();
	ParameterLoader(const ParameterLoader&) = delete; // no copies
	ParameterLoader& operator=(const ParameterLoader&) = delete; // no self-assignments
	friend class Singleton<ParameterLoader>;

	void handleInt(rapidjson::Document& json);

	void handleDouble(rapidjson::Document& json);

	void handleBool(rapidjson::Document& json);

	void handleString(rapidjson::Document& json);

	void handleFromResource();
};