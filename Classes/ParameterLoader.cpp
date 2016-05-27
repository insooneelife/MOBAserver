#include "ParameterLoader.h"
#include "Util.h"

ParameterLoader::ParameterLoader() {
	rapidjson::Document json;
	util::readJSON("params.json", json);

	handleInt(json);
	handleDouble(json);
	handleBool(json);
	handleString(json);
	handleFromResource();
}

void ParameterLoader::handleInt(rapidjson::Document& json) {
	for (rapidjson::Value::MemberIterator iter = json["int"].MemberBegin();
	iter != json["int"].MemberEnd(); iter++)
	{
		std::string name = iter->name.GetString();
		int value = iter->value.GetInt();

		if ("NumCellsX" == name) {
			NumCellsX = value;
		}
		else if ("NumCellsY" == name) {
			NumCellsY = value;
		}
		else if ("NumSamplesForSmoothing" == name) {
			NumSamplesForSmoothing = value;
		}
		else if ("NumMaxAgentCellSpace" == name) {
			NumMaxAgentCellSpace = value;
		}
		else if ("NumMaxClients" == name) {
			NumMaxClients = value;
		}

		
	}
}

void ParameterLoader::handleDouble(rapidjson::Document& json) {
	for (rapidjson::Value::MemberIterator iter = json["double"].MemberBegin();
	iter != json["double"].MemberEnd(); iter++)
	{
		std::string name = iter->name.GetString();
		double value = iter->value.GetDouble();

		if ("SteeringForceTweaker" == name) {
			SteeringForceTweaker = value;
		}
		else if ("SteeringForce" == name) {
			SteeringForce = value;
		}
		else if ("MaxSpeed" == name) {
			MaxSpeed = value;
		}
		else if ("EntityMass" == name) {
			EntityMass = value;
		}
		else if ("EntityRadius" == name) {
			EntityRadius = value;
		}
		else if ("SeparationWeight" == name) {
			SeparationWeight = value;
		}
		else if ("AlignmentWeight" == name) {
			AlignmentWeight = value;
		}
		else if ("CohesionWeight" == name) {
			CohesionWeight = value;
		}
		else if ("ObstacleAvoidanceWeight" == name) {
			ObstacleAvoidanceWeight = value;
		}
		else if ("WallAvoidanceWeight" == name) {
			WallAvoidanceWeight = value;
		}
		else if ("WanderWeight" == name) {
			WanderWeight = value;
		}
		else if ("SeekWeight" == name) {
			SeekWeight = value;
		}
		else if ("FleeWeight" == name) {
			FleeWeight = value;
		}
		else if ("ArriveWeight" == name) {
			ArriveWeight = value;
		}
		else if ("PursuitWeight" == name) {
			PursuitWeight = value;
		}
		else if ("OffsetPursuitWeight" == name) {
			OffsetPursuitWeight = value;
		}
		else if ("InterposeWeight" == name) {
			InterposeWeight = value;
		}
		else if ("HideWeight" == name) {
			HideWeight = value;
		}
		else if ("EvadeWeight" == name) {
			EvadeWeight = value;
		}
		else if ("FollowPathWeight" == name) {
			FollowPathWeight = value;
		}
		else if ("ViewDistance" == name) {
			ViewDistance = value;
		}
		else if ("MinDetectionBoxLength" == name) {
			MinDetectionBoxLength = value;
		}
		else if ("WallDetectionFeelerLength" == name) {
			WallDetectionFeelerLength = value;
		}
		else if ("WanderRad" == name) {
			WanderRad = value;
		}
		else if ("WanderDist" == name) {
			WanderDist = value;
		}
		else if ("WanderJitterPerSec" == name) {
			WanderJitterPerSec = value;
		}
		else if ("WaypointSeekDist" == name) {
			WaypointSeekDist = value;
		}
		else if ("StartViewX" == name) {
			StartViewX = value;
		}
		else if ("StartViewY" == name) {
			StartViewY = value;
		}
		else if ("BlueStartX" == name) {
			BlueStartX = value;
		}
		else if ("BlueStartY" == name) {
			BlueStartY = value;
		}
		else if ("RedStartX" == name) {
			RedStartX = value;
		}
		else if ("RedStartY" == name) {
			RedStartY = value;
		}
		else if ("CameraDistance" == name) {
			CameraDistance = value;
		}
		else if ("GameSpeedInterval" == name) {
			GameSpeedInterval = value;
		}
		else if ("BorderSize" == name) {
			BorderSize = value;
		}
		else if ("MouseMoveVelocity" == name) {
			MouseMoveVelocity = value;
		}
		else if ("MinObstacleRadius" == name) {
			MinObstacleRadius = value;
		}
		else if ("MaxObstacleRadius" == name) {
			MaxObstacleRadius = value;
		}
		else if ("ResolutionSizeX" == name) {
			ResolutionSizeX = value;
		}
		else if ("ResolutionSizeY" == name) {
			ResolutionSizeY = value;
		}
		else if ("FrameSizeX" == name) {
			FrameSizeX = value;
		}
		else if ("FrameSizeY" == name) {
			FrameSizeY = value;
		}
		else if ("CharacterBaseAttackRange" == name) {
			CharacterBaseAttackRange = value;
		}
		else if ("ExpGiveRange" == name) {
			ExpGiveRange = value;
		}
	}
}

void ParameterLoader::handleBool(rapidjson::Document& json) {
	for (rapidjson::Value::MemberIterator iter = json["bool"].MemberBegin();
	iter != json["bool"].MemberEnd(); iter++)
	{
		std::string name = iter->name.GetString();
		bool value = iter->value.GetBool();

		if ("RenderBoundingBox" == name) {
			RenderBoundingBox = value;
		}
		else if ("RenderBoundingCircle" == name) {
			RenderBoundingCircle = value; 
		}
		else if ("RenderBoundingShape" == name) {
			RenderBoundingShape = value; 
		}
		else if ("RenderViewCircle" == name) {
			RenderViewCircle = value;
		}
		else if ("RenderShadow" == name) {
			RenderShadow = value;
		}
		else if ("RenderMainLabel" == name) {
			RenderMainLabel = value;
		}
		else if ("RenderGoalLabel" == name) {
			RenderGoalLabel = value;
		}
		else if ("RenderHealthLabel" == name) {
			RenderHealthLabel = value;
		}
		else if ("RenderMemoryLabel" == name) {
			RenderMemoryLabel = value;
		}
		else if ("RenderCellSpace" == name) {
			RenderCellSpace = value;
		}
		else if ("RenderGraph" == name) {
			RenderGraph = value;
		}
		else if ("RenderPathLine" == name) {
			RenderPathLine = value;
		}
	}
}

void ParameterLoader::handleString(rapidjson::Document& json) {
	for (rapidjson::Value::MemberIterator iter = json["string"].MemberBegin();
	iter != json["string"].MemberEnd(); iter++)
	{
		std::string name = iter->name.GetString();
		std::string value = iter->value.GetString();

		if ("CharacterFolderName" == name) {
			CharacterFolderName = value;
		}else if ("StructureFolderName" == name) {
			StructureFolderName = value;
		}
		else if ("FontFolderName" == name) {
			FontFolderName = value;
		}
		else if ("ParticleFolderName" == name)
		{
			ParticleFolderName = value;
		}
		else if ("FontName" == name) {
			FontName = value;
		}
		else if ("DefaultCharacter" == name) {
			DefaultCharacter = value;
		}
		else if ("RenderCellSpaceType" == name) {
			RenderCellSpaceType = value;
		}
	}
}

void ParameterLoader::handleFromResource()
{
}