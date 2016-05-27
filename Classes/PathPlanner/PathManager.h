#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   PathManager.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   a template class to manage a number of graph searches, and to 
//          distribute the calculation of each search over several update-steps
//-----------------------------------------------------------------------------

#include <list>
#include <cassert>

template <class PathPlanner>
class PathManager
{
public:

	explicit PathManager(unsigned int num_cycles_per_update)
		:
		_num_search_cycles_per_update(num_cycles_per_update)
	{}

	~PathManager()
	{
#ifdef SHOW_DTOR_INFO
		CCLOG("~PathManager()");
#endif
	}

	//every time this is called the total amount of search cycles available will
	//be shared out equally between all the ACTIVE path requests. If a search
	//completes successfully or fails the method will notify the relevant bot
	void updateSearches();

	//a path planner should call this method to register a search with the 
	//manager. (The method checks to ensure the path planner is only registered
	//once)
	inline void registerPath(PathPlanner* const path_planner);

	inline void unRegisterPath(PathPlanner* const path_planner);

	//returns the amount of path requests currently ACTIVE.
	int  getNumActiveSearches()const { return _search_requests.size(); }

private:

	//a container of all the ACTIVE search requests
	std::list<PathPlanner*>		_search_requests;

	//this is the total number of search cycles allocated to the manager. 
	//Each update-step these are divided equally amongst all registered path requests.
	unsigned int				_num_search_cycles_per_update;
};

//------------------------- updateSearches ------------------------------------
//
//  This method iterates through all the ACTIVE path planning requests 
//  updating their searches until the user specified total number of search
//  cycles has been satisfied.
//
//  If a path is found or the search is unsuccessful the relevant agent is
//  notified accordingly by Telegram
//-----------------------------------------------------------------------------
template <class PathPlanner>
void PathManager<PathPlanner>::updateSearches()
{
	int num_cycles_remaining = _num_search_cycles_per_update;

	//iterate through the search requests until either all requests have been
	//fulfilled or there are no search cycles remaining for this update-step.
	std::list<PathPlanner*>::iterator iter = _search_requests.begin();

	while (num_cycles_remaining-- && !_search_requests.empty())
	{
		//make one search cycle of this path request
		int result = (*iter)->cycleOnce();

		//if the search has terminated remove from the list
		if ((result == TARGET_FOUND) || (result == TARGET_NOT_FOUND))
		{
			//remove this path from the path list
			iter = _search_requests.erase(iter);
		}
		//move on to the next
		else
		{
			++iter;
		}

		//the iterator may now be pointing to the end of the list. If this is so,
		// it must be reset to the beginning.
		if (iter == _search_requests.end())
		{
			iter = _search_requests.begin();
		}
	}//end while
}

//--------------------------- registerPath ----------------------------------------
//
//  this is called to register a search with the manager.
//-----------------------------------------------------------------------------
template <class PathPlanner>
void PathManager<PathPlanner>::registerPath(PathPlanner* const path_planner)
{
	//make sure the bot does not already have a current search in the queue
	if (std::find(_search_requests.begin(),
		_search_requests.end(),
		path_planner) == _search_requests.end())
	{
		//add to the list
		_search_requests.push_back(path_planner);
	}
}

//----------------------------- unRegisterPath ------------------------------------
//-----------------------------------------------------------------------------
template <class PathPlanner>
void PathManager<PathPlanner>::unRegisterPath(PathPlanner* const path_planner)
{
	_search_requests.remove(path_planner);
}



#endif