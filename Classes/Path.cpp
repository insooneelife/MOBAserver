#include <algorithm>
#include "Path.h"
#include "Geometry\Transformations.h"
#include "Util.h"

USING_NS_CC;


Path::Path()
	:
	_looped(false),
	_cur_way_point(std::list<cocos2d::Vec2>::iterator())
{}


//constructor for creating a path with initial random waypoints. MinX/Y
//& MaxX/Y define the bounding box of the path.
Path::Path(
	int    num_waypoints,
	double minX,
	double minY,
	double maxX,
	double maxY,
	bool   looped)
	:
	_looped(looped)
{
	createRandomPath(num_waypoints, minX, minY, maxX, maxY);
	_cur_way_point = _way_points.begin();
}


//returns the current waypoint
const cocos2d::Vec2& Path::currentWaypoint() const
{
	return *_cur_way_point;
}

//returns true if the end of the list has been reached
bool Path::finished()
{
	return !(_cur_way_point != _way_points.end());
}

void Path::setNextWaypoint() {
	assert(_way_points.size() > 0);

	if (++_cur_way_point == _way_points.end())
	{
		if (_looped)
		{
			_cur_way_point = _way_points.begin();
		}
	}
}

void Path::addPath(const cocos2d::Vec2& pos)
{
	_way_points.push_back(pos);
}

void Path::loopOn()
{
	_looped = true;
}

void Path::loopOff() {
	_looped = false;
}

//methods for setting the path with either another Path or a list of vectors
void Path::setPath(const std::list<cocos2d::Vec2>& new_path)
{
	_way_points = new_path;
	_cur_way_point = _way_points.begin();
}

void Path::setPath(const Path& path)
{
	_way_points = path.getPath();
	_cur_way_point = _way_points.begin();
}

void Path::clear()
{
	_way_points.clear();
}

const std::list<cocos2d::Vec2>& Path::getPath() const
{
	return _way_points;
}


const std::list<Vec2>& Path::createRandomPath(
	int		num_waypoints,
	double	minX,
	double	minY,
	double	maxX,
	double	maxY)
{
	_way_points.clear();

	double midX = (maxX + minX) / 2.0;
	double midY = (maxY + minY) / 2.0;

	double smaller = std::min(midX, midY);

	double spacing = M_PI * 2 / (double)num_waypoints;

	for (int i = 0; i < num_waypoints; ++i)
	{
		double radial_dist = smaller*0.2f + util::genRand<0, 1>() * (smaller - smaller * 0.2f);
		Vec2 temp(radial_dist, 0.0f);

		Vec2DRotateAroundOrigin(temp, i*spacing);

		temp.x += midX; temp.y += midY;

		_way_points.push_back(temp);
	}

	_cur_way_point = _way_points.begin();
	return _way_points;
}
