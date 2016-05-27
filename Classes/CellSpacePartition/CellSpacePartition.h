#ifndef CELLSPACEPARTITION_H
#define CELLSPACEPARTITION_H
//-----------------------------------------------------------------------------
//
//  Name:   CellSpacePartition\CellSpacePartition.h
//
//  Desc:   class to divide a 2D space into a grid of cells each of which
//          may contain a number of entities. Once created and initialized 
//          with entities, fast proximity querys can be made by calling the
//          calculateNeighborsForSmall method with a position and proximity radius.
//
//          If an Entity is capable of moving, and therefore capable of moving
//          between cells, the update method should be called each update-cycle
//          to sychronize the Entity and the cell space it occupies
//
//  Author: Insub Im (insooneelife@naver.com)
//
//-----------------------------------------------------------------------------
#pragma warning (disable:4786)

#include <vector>
#include <list>
#include <memory>
#include <sstream>

#include "AABBox2D.h"
#include "cocos2d.h"

#include "../Geometry/Geometry.h"
#include "../Util.h"


//-------------------------------- Cell ----------------------------------
//
//  defines a cell containing a list of pointers to entities
//------------------------------------------------------------------------
template <class Entity>
struct Cell
{
	//all the entities inhabiting this cell
	std::list<Entity>	members;

	//the cell's bounding box (it's inverted because the Window's default
	//co-ordinate system has a y axis that increases as it descends)
	AABBox2D			bounding_box;

	Cell(cocos2d::Vec2 botleft,
		cocos2d::Vec2 topright)
		:
		bounding_box(AABBox2D(botleft, topright))
	{}

	~Cell()
	{}
};

//------------------------------CellSpacePartition-------------------------------
//
//  defines a cell space containing a vector of cells
//-------------------------------------------------------------------------------

template <class Entity>
class CellSpacePartition
{
public:
	typedef std::vector<std::unique_ptr<Cell<Entity> > > CellVector;
	typedef Entity entity_type;

	CellSpacePartition(
		double	width,			//width of the environment
		double	height,			//height ...
		int		cellsX,			//number of cells horizontally
		int		cellsY,			//number of cells vertically
		int		max_entitys);	//maximum number of entities to add

	//the getter of cells
	const CellVector& getCells() const;

	//show cell_space_line and other data
	void render();

	//adds entities to the class by allocating them to the appropriate cell
	inline void addEntity(const Entity& ent);

	//if some entity has spacious bounding area, it could be included in
	//may cell spaces. This function checks the overlap of bounding circle
	//and AABB box, and adds the entity to every area it overlaps.
	inline void addEntityByArea(const Entity& ent, geometry::Shape* bounding_shape);

	//removes entitiy from the cell
	inline void removeEntity(const Entity& ent);

	//update an Entity's cell by calling this from your Entity's update method 
	inline void updateEntity(const Entity& ent, cocos2d::Vec2 old_pos);

	//this method calculates all a target's neighbors and stores them in
	//the neighbor vector. After you have called this method use the begin, 
	//next and end methods to iterate through the vector.
	//calculates neighbors with bounding shape
	inline void	calculateNeighbors(geometry::Shape* bounding_shape);

	inline void	calculateNeighborsForSmall(
		cocos2d::Vec2 pos,
		geometry::Shape* bounding_shape);

	//returns a reference to the Entity at the front of the neighbor vector
	inline Entity&	begin();

	//this returns the next Entity in the neighbor vector
	inline Entity&	next();

	//returns true if the end of the vector is found (a zero value marks the end)
	inline bool		end();

	//empties the cells of entities
	void			clearCells();

private:

	//given a position in the game space this method determines the           
	//relevant cell's index
	inline int  positionToIndex(const cocos2d::Vec2& pos)const;

	//the required amount of cells in the space
	CellVector								_cells;

	//this is used to store any valid neighbors when an agent searches
	//its neighboring space
	std::vector<Entity>						_neighbors;

	//this iterator will be used by the methods next and begin to traverse
	//through the above vector of neighbors
	typename std::vector<Entity>::iterator	_cur_neighbor;

	//the width and height of the world space the entities inhabit
	double  _space_width;
	double  _space_height;

	//the number of cells the space is going to be divided up into
	int    _num_cellsX;
	int    _num_cellsY;

	double  _cell_sizeX;
	double  _cell_sizeY;
};


//----------------------------- ctor ---------------------------------------
//--------------------------------------------------------------------------
template<class Entity>
CellSpacePartition<Entity>::CellSpacePartition(
	double width,			//width of 2D space
	double height,			//height...
	int cellsX,				//number of divisions horizontally
	int cellsY,				//and vertically
	int max_entitys)		//maximum number of entities to partition 
	:	
	_space_width(width),
	_space_height(height),
	_num_cellsX(cellsX),
	_num_cellsY(cellsY),
	_neighbors(max_entitys, Entity())
{
	//calculate bounds of each cell
	_cell_sizeX = width / cellsX;
	_cell_sizeY = height / cellsY;

	//create the cells
	for (int y = 0; y < _num_cellsY; ++y)
	{
		for (int x = 0; x < _num_cellsX; ++x)
		{
			double left	= x * _cell_sizeX;
			double right = left + _cell_sizeX;
			double bot = y * _cell_sizeY;
			double top = bot + _cell_sizeY;

			_cells.emplace_back(
				new Cell<Entity>(
				cocos2d::Vec2(left, bot),
				cocos2d::Vec2(right, top)));
		}
	}
}

template<class Entity>
const typename CellSpacePartition<Entity>::CellVector& 
CellSpacePartition<Entity>::getCells() const
{
	return _cells;
}

template<class Entity>
void CellSpacePartition<Entity>::render()
{
	if (!Prm.RenderCellSpace)return;

	for (CellVector::iterator iter = _cells.begin();
		iter != _cells.end(); iter++) 
	{
		if (!(*iter)->members.empty()) 
		{
			//(*iter)->renderEntityOn();
		}
		else 
		{
			//(*iter)->renderEntityOff();
		}
	}
}

//----------------------- calculateNeighbors ----------------------------
//
//  This must be called to create the vector of neighbors.This method 
//  examines each cell within range of the target, If the 
//  cells contain entities then they are tested to see if they are situated
//  within the target's neighborhood region. If they are they are added to
//  neighbor list
//------------------------------------------------------------------------
template<class Entity>
void CellSpacePartition<Entity>::calculateNeighbors(geometry::Shape* bounding_shape)
{
	//create an iterator and set it to the beginning of the neighbor vector
	std::vector<Entity>::iterator cur_nbor = _neighbors.begin();

	for (auto cur_cell = std::begin(_cells); cur_cell != std::end(_cells); ++cur_cell)
	{
		if (bounding_shape->overlap(
			&geometry::Rect(
			(*cur_cell)->bounding_box.getBottomLeft(),
			(*cur_cell)->bounding_box.getTopRight()))
			&&
			!(*cur_cell)->members.empty())
		{
			//add any entities found within query radius to the neighbor list
			std::list<Entity>::iterator it = (*cur_cell)->members.begin();
			for (it; it != (*cur_cell)->members.end(); ++it)
			{
				*cur_nbor++ = *it;
			}
		}
	}//next cell

	 //mark the end of the list with a zero.
	*cur_nbor = 0;
}


//----------------------- calculateNeighborsForSmall ----------------------------
//
// If an entity is not too big, then it doesn't have to loop all the cells to
// find the intersecting cells.
// Check neighbors for only 3x3 cells including current cell.
//-------------------------------------------------------------------------------
template<class Entity>
void CellSpacePartition<Entity>::calculateNeighborsForSmall(
	cocos2d::Vec2 pos,
	geometry::Shape* bounding_shape)
{
	//create an iterator and set it to the beginning of the neighbor vector
	std::vector<Entity>::iterator cur_nbor = _neighbors.begin();

	int idx = positionToIndex(pos);

	int mj = idx % _num_cellsX;
	int mi = idx / _num_cellsX;

	for (int i = std::max(0, mi - 1); i <= std::min(_num_cellsY - 1, mi + 1); i++)
	{
		for (int j = std::max(0, mj - 1); j <= std::min(_num_cellsX - 1, mj + 1); j++)
		{
			int ridx = i * _num_cellsX + j;
			
			if (bounding_shape->overlap(
				&geometry::Rect(
					_cells[ridx]->bounding_box.getBottomLeft(),
					_cells[ridx]->bounding_box.getTopRight()))
				&&
				!_cells[ridx]->members.empty())
			{
				//add any entities found within query radius to the neighbor list
				
				for (auto iter = std::begin(_cells[ridx]->members); 
					iter != std::end(_cells[ridx]->members); ++iter)
				{
					*cur_nbor++ = *iter;
				}
			}
		}
	}

	 //mark the end of the list with a zero.
	*cur_nbor = 0;
}

template<class Entity>
Entity&	CellSpacePartition<Entity>::begin()
{
	_cur_neighbor = _neighbors.begin();
	return *_cur_neighbor;
}

template<class Entity>
Entity&	CellSpacePartition<Entity>::next()
{
	++_cur_neighbor;
	return *_cur_neighbor;
}

template<class Entity>
bool CellSpacePartition<Entity>::end()
{
	return (_cur_neighbor == _neighbors.end()) || (*_cur_neighbor == 0);
}

//--------------------------- clearCells --------------------------------------
//
//  clears the cells of all entities
//------------------------------------------------------------------------
template<class Entity>
void CellSpacePartition<Entity>::clearCells()
{
	CellVector::iterator it = _cells.begin();

	for (it; it != _cells.end(); ++it)
	{
		(*it)->members.clear();
	}
}

//--------------------- positionToIndex ----------------------------------
//
//  Given a 2D vector representing a position within the game world, this
//  method calculates an index into its appropriate cell
//------------------------------------------------------------------------
template<class Entity>
int CellSpacePartition<Entity>::positionToIndex(const cocos2d::Vec2& pos)const
{
	CCASSERT(0 <= pos.x && pos.x <= _space_width &&
		0 <= pos.y && pos.y <= _space_height, "position out of range!");

	int idx = (int)(_num_cellsX * pos.x / _space_width) +
		((int)((_num_cellsY)* pos.y / _space_height) * _num_cellsX);

	//if the Entity's position is equal to vector2d(_space_width, _space_height)
	//then the index will overshoot. We need to check for this and adjust
	if (idx > _cells.size() - 1) idx = _cells.size() - 1;

	return idx;
}

//----------------------- addEntity --------------------------------------
//
//  Used to add the entitys to the data structure
//------------------------------------------------------------------------
template<class Entity>
inline void CellSpacePartition<Entity>::addEntity(const Entity& ent)
{
	CCASSERT(ent, "entity is null!");
	int idx = positionToIndex(ent->getPos());

	_cells[idx]->members.push_back(ent);
}

//----------------------- addEntityByArea --------------------------------------
//
//  Used to add the entitys to the data structure
//------------------------------------------------------------------------
template<class Entity>
inline void CellSpacePartition<Entity>::addEntityByArea(
	const Entity& ent, 
	geometry::Shape* bounding_shape)
{
	CCASSERT(ent, "entity is null!");
	
	for (auto cur_cell = std::begin(_cells); cur_cell != std::end(_cells); ++cur_cell)
	{
		if (bounding_shape->overlap(
			&geometry::Rect(
				(*cur_cell)->bounding_box.getBottomLeft(),
				(*cur_cell)->bounding_box.getTopRight())))
		{
			(*cur_cell)->members.push_back(ent);
		}
	}
}


//----------------------- removeEntity --------------------------------------
//
//  Removes the erased entity from cellspace.
//------------------------------------------------------------------------
template<class Entity>
inline void CellSpacePartition<Entity>::removeEntity(const Entity& ent)
{
	CCASSERT(ent, "entity is null!");
	int idx = positionToIndex(ent->getPos());

	_cells[idx]->members.remove(ent);
}

//----------------------- updateEntity -----------------------------------
//
//  Checks to see if an Entity has moved cells. If so the data structure
//  is updated accordingly
//------------------------------------------------------------------------
template<class Entity>
inline void CellSpacePartition<Entity>::updateEntity(
	const Entity&	entity,
	cocos2d::Vec2	old_pos)
{
	//if the index for the old pos and the new pos are not equal then
	//the Entity has moved to another cell.
	int old_idx = positionToIndex(old_pos);
	int new_idx = positionToIndex(entity->getPos());

	if (new_idx == old_idx) return;

	//the Entity has moved into another cell so delete from current cell
	//and add to new one
	_cells[old_idx]->members.remove(entity);
	_cells[new_idx]->members.push_back(entity);
}

#endif