#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <random>
#include <vector>
#include <tuple>
#include "json\reader.h"
#include "json\document.h"
#include "Singleton.h"
#include "cocos2d.h"
#include "GlobalValues.h"
#include "ParameterLoader.h"

//------------------------------------------------------------------------
//
//  Name:   Util.h
//
//  Desc:   Sets of global functions which could be called from anywhere.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

//전역함수를 담기위한 namespace

namespace util {
	//json 파일을 읽고 parsing한 결과를 document에 담는다.
	static inline void readJSON(std::string filename, rapidjson::Document& json) {
		std::ifstream fin(filename);
		char ch;
		std::string data;

		while (1) {
			fin.get(ch);
			if (fin.eof())break;
			data = data + ch;
		}
		fin.close();
		json.Parse(data.c_str());
	}

	//ZOrder를 위한 값 생성 함수
	template <typename T, typename U>
	static inline auto zOrdering(T x, U y) -> decltype(x * y) {
		return ((1999999999) - (y * (18000) + x));
	}

	//random number 생성기
	template<unsigned int T = 0, unsigned int U = 1>
	static inline auto genRand() {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		static std::uniform_real_distribution<double> dist(T, std::nextafter(U, DBL_MAX));

		return dist(mt);
	}

	//random number 생성기 (구간)  [T , U)  test
	template<typename T, typename U>
	static inline decltype(auto) genRand(T begin, U end) {
		double rand = genRand<0, 1>();
		return ((end - begin) * rand + begin);
	}

	//두 실수 값이 같은지?
	template <typename T, typename U>
	static inline bool isEqual(T a, U b){
		if (fabs(a - b) < 1E-12){
			return true;
		}
		return false;
	}

	//generic square 함수
	template <typename T>
	static inline auto square(T a) -> decltype(a * a) {
		return a * a;
	}

	//generic distance^2 함수
	template<typename X1, typename Y1, typename X2, typename Y2>
	static inline auto dis2(X1 x1, Y1 y1, X2 x2, Y2 y2) -> decltype(square(x2 - x1) + square(y2 - y1)) {
		return square(x2 - x1) + square(y2 - y1);
	}

	//generic range 함수, (x1, y1)으로부터 range 내에 (x2, y2)가 있는지 검사
	template<typename X1, typename Y1, typename X2, typename Y2, typename R>
	static inline bool inRange(X1 x1, Y1 y1, X2 x2, Y2 y2, R range) {
		return dis2(x1, y1, x2, y2) <= square(range);
	}

	//generic range 함수, Vec2(x1, y1)으로부터 range 내에 Vec2(x2, y2)가 있는지 검사
	template<typename T, typename R>
	static inline bool inRange(const T& p1, const T& p2, R range) {
		return dis2(p1.x, p1.y, p2.x, p2.y) <= square(range);
	}

	template<typename T>
	static inline int sign(const T& v1, const T& v2) {
		if (v1.y * v2.x > v1.x * v2.y)
			return 1;
		else
			return -1;
	}

	//clamps the first argument between the second two
	template <class T, class U, class V>
	static inline void clamp(T& arg, const U& minVal, const V& maxVal) {
		CCASSERT((minVal < maxVal), "<Clamp>MaxVal < MinVal!");

		if (arg < (T)minVal)
			arg = (T)minVal;
		if (arg > (T)maxVal)
			arg = (T)maxVal;
	}

	//returns a random double in the range -1 < n < 1
	static inline double randomClamped() { 
		double temp =  genRand<0, 1>() - genRand<0, 1>();
		return temp;
	}


	//Returns true if the two circles overlap
	template <typename T>
	static inline bool TwoCirclesOverlapped(const T& c1, double r1, const T& c2, double r2) {
		double DistBetweenCenters = sqrt((c1.x - c2.x) * (c1.x - c2.x) +
			(c1.y - c2.y) * (c1.y - c2.y));

		if ((DistBetweenCenters < (r1 + r2)) || (DistBetweenCenters < fabs(r1 - r2))) {
			return true;
		}
		return false;
	}

	inline bool TwoCirclesOverlapped(
		double x1, double y1, double r1,
		double x2, double y2, double r2)
	{
		double DistBetweenCenters = sqrt((x1 - x2) * (x1 - x2) +
			(y1 - y2) * (y1 - y2));

		if ((DistBetweenCenters < (r1 + r2)) || (DistBetweenCenters < fabs(r1 - r2)))
		{
			return true;
		}
		return false;
	}


	//S > 0 : 반시계 방향
	//S = 0 : 일직선
	//S < 0 : 시계 방향
	static inline double ccw(double x1, double y1, double x2, double y2, double x3, double y3) {
		double temp = x1 * y2 + x2 * y3 + x3 * y1;
		temp = temp - y1 * x2 - y2 * x3 - y3 * x1;
		if (temp > 0)
			return 1;
		else if (temp < 0)
			return -1;
		else
			return 0;
	}

	static inline double ccw(cocos2d::Vec2 start, cocos2d::Vec2 mid, cocos2d::Vec2 end) {
		return ccw(start.x, start.y, mid.x, mid.y, end.x, end.y);
	}

	//벡터를 인자로 받아서 (1, 0)벡터로부터 회전한 각도를 return한다.
	static inline float getAngleFromZero(const cocos2d::Vec2 &point)
	{
		//CCASSERT(!isEqual(point.x, 0) || !isEqual(point.y, 0), "heading can't be zero!");
		
		if (isEqual(point.x, 0) && isEqual(point.y, 0))
			return 3;
		//	CCLOG("heading is zero!");

		cocos2d::Vec2 a = { 0.0f, 1.0f };
		cocos2d::Vec2 b = { 1.0f, 0.0f };
		
		float angle = MATH_RAD_TO_DEG(acos(a.dot(point) / (a.getLength() * point.getLength())));

		if (angle > 90.0f) return 360.0f - MATH_RAD_TO_DEG(acos(b.dot(point) / (b.getLength() * point.getLength())));
		else return MATH_RAD_TO_DEG(acos(b.dot(point) / (b.getLength() * point.getLength())));
	}

	//container generator
	template <typename Container, typename How>
	static inline decltype(auto) genContainer(size_t size, const How& h) {
		Container v(size);
		generate(v.begin(), v.end(), h);
		return v;
	}

	//시간 측정을 위한 inner class
	class Timer{
		int count;
	public:
		Timer() { count = clock(); }
		~Timer(){
			int n = clock();
			CCLOG("time : %d", n - count);
		}
	};

	//시간을 측정하는 함수 wrapper
	template<typename F, typename ... Types>
	static inline decltype(auto) HowLong(F f, Types&& ... args){
		Timer tm;
		return f(std::forward<Types&&>(args)...);
	}

	//heading 벡터를 인자로 받아서 8, 16방향에 대한 적합한 방향을 생성한다.
	//is_eight가 true이면 8방향
	//false이면 16방향
	static inline int make_direction(cocos2d::Vec2 heading, bool is_eight) {

		static std::vector<int> degree_map_8 = genContainer<std::vector<int>>(361, []() {
			static int i = 0;
			return (14 - (((i++ + 22) % 360) / 45)) % 8;
		});

		static std::vector<int> degree_map_16 = genContainer<std::vector<int>>(361, []() {
			static int i = 0;
			return (45 - (((i++ + 23) % 360) / 23)) % 16;
		});
		degree_map_16[360] = 13;

		int degree = static_cast<int>(getAngleFromZero(heading));

		CCASSERT(degree >= 0, "degree_map index can't be minus!");

		if (is_eight)
			return degree_map_8[degree];
		else
			return degree_map_16[degree];
	}

	//x,y가 화면 크기보다 border_size만큼 떨어진 곳과 직사각형 형태로 닿는지 검사한다. 
	template<typename A, typename B, typename C>
	static inline bool isBorderRect(A x, B y, C border_size) {
		return (x <= border_size ||
			Prm.FrameSizeX - border_size <= x ||
				y <= border_size ||
			Prm.FrameSizeY - border_size <= y);
	}

	enum
	{ 
		BORDER_LEFT, BORDER_RIGHT, BORDER_UP, BORDER_DOWN,
		BORDER_LEFT_UP, BORDER_LEFT_DOWN, BORDER_RIGHT_UP, BORDER_RIGHT_DOWN
	};

	template<typename A, typename B, typename C>
	static inline cocos2d::Vec2 borderDirection(A x, B y, C border_size)
	{
		if (x <= border_size)
			if (y <= border_size)
				return cocos2d::Vec2(-1, -1);
			else if (Prm.FrameSizeY - border_size <= y)
				return cocos2d::Vec2(-1, 1);
			else
				return cocos2d::Vec2(-1, 0);

		else if (Prm.FrameSizeX - border_size <= x)
			if (y <= border_size)
				return cocos2d::Vec2(1, -1);
			else if (Prm.FrameSizeY - border_size <= y)
				return cocos2d::Vec2(1, 1);
			else
				return cocos2d::Vec2(1, 0);

		else if (y <= border_size)
			if (x <= border_size)
				return cocos2d::Vec2(-1, -1);
			else if (Prm.FrameSizeX - border_size <= x)
				return cocos2d::Vec2(1, -1);
			else
				return cocos2d::Vec2(0, -1);
		
		else if (Prm.FrameSizeY - border_size <= y)
			if (x <= border_size)
				return cocos2d::Vec2(-1, 1);
			else if (Prm.FrameSizeX - border_size <= x)
				return cocos2d::Vec2(1, 1);
			else
				return cocos2d::Vec2(0, 1);
		else
			return cocos2d::Vec2(0, 0);
	}

	//x,y가 화면 크기보다 border_size만큼 떨어진 곳과 타원 형태로 닿는지 검사한다. 
	template<typename A, typename B, typename C>
	static inline bool isBorderCircle(A x, B y, C border_size) {
		double a = Prm.FrameSizeX / 2;
		double b = Prm.FrameSizeY / 2;
		if (square(x - a) / square(a - border_size) 
			+ square(y - b) / square(b - border_size) <= 1) {
			return false;
		}
		return true;
	}

	static cocos2d::Vec2 makeRandomPosition(double rand_min, double rand_max) {
		double x = util::genRand(rand_min, rand_max);
		double y = util::genRand(rand_min, rand_max);
		double opx, opy;
		if (util::genRand(0, 2) < 1)
			opx = -1;
		else
			opx = 1;
		if (util::genRand(0, 2) < 1)
			opy = -1;
		else
			opy = 1;

		return cocos2d::Vec2(x * opx, y * opy);
	}

	static cocos2d::Vec2 makeRandomPosInRange(
		double map_begin_x,
		double map_begin_y,
		double map_end_x,
		double map_end_y,
		double now_x,
		double now_y,
		double rand_min,
		double rand_max,
		double border_x = 100,
		double border_y = 100)
	{
		cocos2d::Vec2 pos = 
			cocos2d::Vec2(now_x, now_y) + makeRandomPosition(rand_min, rand_max);
		if (pos.x < map_begin_x + border_x)
			pos.x = map_begin_x + border_x;
		else if (pos.x > map_end_x - border_x)
			pos.x = map_end_x - border_x;

		if (pos.y < map_begin_y + border_y)
			pos.y = map_begin_y + border_y;
		else if (pos.y > map_end_y - border_y)
			pos.y = map_end_y - border_y;

		return pos;
	}

	template <class T, class Container>
	bool isCircleOverlapOthers(const T& c, const Container& circles, double min_dist_between_circles = 40.0)
	{
		typename Container::const_iterator it;

		for (it = circles.begin(); it != circles.end(); ++it)
		{
			if (TwoCirclesOverlapped(
				std::get<0>(c), std::get<1>(c), std::get<2>(c) + min_dist_between_circles,
				std::get<0>(*it), std::get<1>(*it), std::get<2>(*it)))
			{
				return true;
			}
		}
		return false;
	}

	static void generateRandomCircle(
		float num_obstacle,
		float begin_x,
		float begin_y,
		float end_x,
		float end_y,
		float border,
		float min_gap_between_circles,
		float radius,
		std::vector<std::tuple<float, float, float> >& circles,
		int num_allowed = 200)
	{
		assert(end_x - begin_x > 2 * border && end_y - begin_y > 2 * border);
		for (int i = 0; i<num_obstacle; ++i)
		{
			bool overlap = true;
			int num_trys = 0;

			while (overlap)
			{
				num_trys++;

				if (num_trys > num_allowed) return;

				float x = genRand(begin_x + border, end_x - border);
				float y = genRand(begin_y + border, end_y - border);

				std::tuple<float, float, float> temp = std::make_tuple(x, y, radius);

				if (!isCircleOverlapOthers(temp, circles, min_gap_between_circles))
				{
					circles.emplace_back(temp);
					overlap = false;
				}
			}
		}
	}

	//------------------ isSecondInFOVOfFirst -------------------------------------
	//
	//  returns true if the target position is in the field of view of the entity
	//  positioned at posFirst facing in facingFirst
	//-----------------------------------------------------------------------------
	static inline bool isSecondInFOVOfFirst(
		cocos2d::Vec2 posFirst,
		cocos2d::Vec2 facingFirst,
		cocos2d::Vec2 posSecond,
		double    fov)
	{
		cocos2d::Vec2 toTarget = (posSecond - posFirst).getNormalized();

		return facingFirst.dot(toTarget) >= cos(fov / 2.0);
	}

	static inline cocos2d::Vec2 convertMousePosToWorldPos(
		cocos2d::Vec2 mouse,
		cocos2d::Vec2 camera)
	{
		//it is important to know how this ratio made
		//I(Insub Im) just tested with the size of window(750)
		//and the size in game world(575). 575/750 = 0.7666666667f
		const float ratio = 0.7666666667f;
		return cocos2d::Vec2(
			(mouse.x - Prm.FrameSizeX / 2)*ratio + camera.x,
			(mouse.y - Prm.FrameSizeY / 2)*ratio + camera.y);
	}

	static void extractNumber(std::string str, std::vector<int>& number)
	{
		size_t start = 0;
		size_t i = 0;
		std::string num = "";
		while (i < str.size())
		{
			for (; i < str.size(); i++)
			{
				if ('0' <= str[i] && str[i] <= '9')
					num.push_back(str[i]);
				else
					break;
			}
			if (num.size() > 0)
				number.push_back(atoi(num.c_str()));
			num = "";
			i++;
		}
	}
};