/*
 * Cell3D.h
 *
 *  Created on: Apr 21, 2009
 *      Author: David Berrios
 */

#ifndef CELL3D_H_
#define CELL3D_H_

#ifdef __cplusplus
#include "Point.h"
#include <vector>
#include <deque>
#include "Vector.h"

namespace ccmc
{
	/**
	 * @class Cell3D
	 * @brief TODO: brief description of Cell3D class
	 *
	 * TODO: full description of Cell3D class
	 */
	//P=positions, D=data
	template<class P, class D>
	class Cell3D
	{
		public:
			Cell3D();
			Cell3D(Point<P> * positions, D * vectors);
			void setData(D * vectors);
			void setPositions(Point<P> * positions);
			D* getData();
			D * getData(int i);
			Vector<D> getDeltas();
			Point<P> * getPositions();
			Point<P> * getPosition(int i);
			std::vector<int> getDataForFace(int i);
			std::vector<std::vector<int> > getTriangles();
			std::string toString();
			Point<P> getCenter();
			std::deque<Cell3D<P, D> *> getChildren();
			D interpolateData(Point<P>& point);
			D interpolateData(const P& c0, const P& c1, const P& c2);
			~Cell3D();

		private:
			Point<P> positions[8];
			D data[8];
			int level;
			float dx;
			float dy;
			float dz;
	};

	template<class P, class D>
	Cell3D<P, D>::Cell3D()
	{
		level = 0;
	}

	//    6_____7
	//   /|    /|
	//  4_|___5 |       4
	//  | |   | |    0 1 2 3
	//  | 2 - - 3       5
	//  |/    |/
	//  0 - - 1
	template<class P, class D>
	Cell3D<P, D>::Cell3D(Point<P> positions[], D data[])
	{
		for (int i = 0; i < 8; i++)
		{
			this->data[i] = data[i];
			this->positions[i] = positions[i];
		}

		dx = abs(positions[0].c0() - positions[1].c0());
		dy = abs(positions[0].c1() - positions[4].c1());
		dz = abs(positions[0].c2() - positions[2].c2());
	}

	template<class P, class D>
	void Cell3D<P, D>::setData(D data[])
	{
		for (int i = 0; i < 8; i++)
		{
			this->data[i] = data[i];
		}
	}

	template<class P, class D>
	void Cell3D<P, D>::setPositions(Point<P> positions[])
	{
		for (int i = 0; i < 8; i++)
		{
			this->positions[i] = positions[i];
		}

		dx = abs(positions[0].c0() - positions[1].c0());
		dy = abs(positions[0].c1() - positions[4].c1());
		dz = abs(positions[0].c2() - positions[2].c2());
	}

	template<class P, class D>
	D* Cell3D<P, D>::getData()
	{
		return data;
	}

	template<class P, class D>
	D* Cell3D<P, D>::getData(int i)
	{
		return &data[i];
	}

	//    6_____7
	//   /|    /|
	//  4_|___5 |       4
	//  | |   | |    0 1 2 3
	//  | 2 - - 3       5
	//  |/    |/
	//  0 - - 1
	template<class P, class D>
	std::vector<int> Cell3D<P, D>::getDataForFace(int i)
	{
		std::vector<int> dataOnFace = std::vector<int>(4);

		switch (i)
		{
			case 0:
				dataOnFace[0] = 0;
				dataOnFace[1] = 1;
				dataOnFace[2] = 4;
				dataOnFace[3] = 5;
				break;
			case 1:
				dataOnFace[0] = 1;
				dataOnFace[1] = 3;
				dataOnFace[2] = 5;
				dataOnFace[3] = 7;
				break;
			case 2:
				dataOnFace[0] = 3;
				dataOnFace[1] = 2;
				dataOnFace[2] = 7;
				dataOnFace[3] = 6;
				break;
			case 3:
				dataOnFace[0] = 2;
				dataOnFace[1] = 0;
				dataOnFace[2] = 6;
				dataOnFace[3] = 4;
				break;
			case 4:
				dataOnFace[0] = 4;
				dataOnFace[1] = 5;
				dataOnFace[2] = 6;
				dataOnFace[3] = 7;
				break;
			case 5:
				dataOnFace[0] = 2;
				dataOnFace[1] = 3;
				dataOnFace[2] = 0;
				dataOnFace[3] = 1;
				break;

		}
		return dataOnFace;
	}

	/**
	 * Fetches the 12 triangles to calculate the solid angles for Greene's bisection method
	 * @return
	 */
	template<class P, class D>
	std::vector<std::vector<int> > Cell3D<P, D>::getTriangles()
	{
		// 2______3
		// |     /|
		// |    / |
		// |   /  |
		// |  /   |
		// | /    |
		// |/_____|
		// 0      1

		// get vectors B1, B2, B3, and B4 - b1, b2, b3 and B2, B4, B3
		// B1 = v[1]
		// B2 = v[3]
		// B3 = v[0]
		// B4 = v[2]
		std::vector<std::vector<int> > triangles = std::vector<std::vector<int> >(12);
		for (int i = 0; i < 12; i++)
		{
			triangles[i] = std::vector<int>(3);
		}
		//[12][3];

		for (int i = 0; i < 6; i++)
		{
			std::vector<int> dataOnFace = getDataForFace(i);

			int i2 = i * 2;
			int i21 = i2 + 1;
			triangles[i2][0] = dataOnFace[1];
			triangles[i2][1] = dataOnFace[3];
			triangles[i2][2] = dataOnFace[0];

			triangles[i21][0] = dataOnFace[3];
			triangles[i21][1] = dataOnFace[2];
			triangles[i21][2] = dataOnFace[0];
		}
		return triangles;

	}

	/*template<class P, class D>
	 Cell3D_struct Cell3D<P, D>::getStruct()
	 {
	 Cell3D_struct cStruct;
	 for (int i = 0; i < 8; i++)
	 {
	 cStruct.positions[i].x = positions[i].c0();
	 cStruct.positions[i].y = positions[i].c1();
	 cStruct.positions[i].z = positions[i].c2();

	 cStruct.vectors[i].x = vectors[i].c0();
	 cStruct.vectors[i].y = vectors[i].c1();
	 cStruct.vectors[i].z = vectors[i].c2();

	 }

	 std::vector<std::vector<int> > triangles = std::vector<std::vector<int> >(12);
	 triangles = getTriangles();

	 for (int i = 0; i < triangles.size(); i++)
	 {
	 cStruct.triangles[i].x = triangles[i][0];
	 cStruct.triangles[i].y = triangles[i][1];
	 cStruct.triangles[i].z = triangles[i][2];
	 }

	 return cStruct;
	 }*/

	//    6_____7
	//   /|    /|
	//  4_|___5 |       4
	//  | |   | |    0 1 2 3
	//  | 2 - - 3       5
	//  |/    |/
	//  0 - - 1

	template<class P, class D>
	std::string Cell3D<P, D>::toString()
	{
		Point<P> center = getCenter();
		return center.toString();

	}

	template<class P, class D>
	Point<P> Cell3D<P, D>::getCenter()
	{
		Point<P> center;
		float x = (positions[0].c0() + positions[1].c0()) / 2.0f;
		float y = (positions[0].c1() + positions[4].c1()) / 2.0f;
		float z = (positions[0].c2() + positions[2].c2()) / 2.0f;
		center.setComponents(x, y, z);
		return center;
	}

	template<class P, class D>
	std::deque<Cell3D<P, D> *> Cell3D<P, D>::getChildren()
	{
		std::deque<Cell3D<P, D> *> cells = std::deque<Cell3D<P, D> *>(8);
		;

		Cell3D<P, D> * c0 = new Cell3D<P, D> ();
		Cell3D<P, D> * c1 = new Cell3D<P, D> ();
		Cell3D<P, D> * c2 = new Cell3D<P, D> ();
		Cell3D<P, D> * c3 = new Cell3D<P, D> ();
		Cell3D<P, D> * c4 = new Cell3D<P, D> ();
		Cell3D<P, D> * c5 = new Cell3D<P, D> ();
		Cell3D<P, D> * c6 = new Cell3D<P, D> ();
		Cell3D<P, D> * c7 = new Cell3D<P, D> ();

		//        float dx = Math.abs(positions[0].c0() - positions[1].c0());
		//        float dy = Math.abs(positions[0].c1() - positions[4].c1());
		//        float dz = Math.abs(positions[0].c2() - positions[2].c2());

		Point<D> p = positions[0];
		Point<D> cPositions0[] = { Point<D> (p.c0(), p.c1(), p.c2()), Point<D> (p.c0() + dx * .5f, p.c1(), p.c2()),
				Point<D> (p.c0(), p.c1(), p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1(), p.c2() + dz * .5f),
				Point<D> (p.c0(), p.c1() + .5f * dy, p.c2()), Point<D> (p.c0() + .5f * dx, p.c1() + .5f * dy, p.c2()),
				Point<D> (p.c0(), p.c1() + .5f * dy, p.c2() + dz * .5f), Point<D> (p.c0() + .5f * dx,
						p.c1() + .5f * dy, p.c2() + dz * .5f) };
		c0->setPositions(cPositions0);

		Point<D> cPositions1[] = { Point<D> (p.c0() + dx * .5f, p.c1(), p.c2()),
				Point<D> (p.c0() + dx, p.c1(), p.c2()), Point<D> (p.c0() + dx * .5f, p.c1(), p.c2() + dz * .5f), Point<
						D> (p.c0() + dx, p.c1(), p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f,
						p.c2()), Point<D> (p.c0() + dx, p.c1() + dy * .5f, p.c2()), Point<D> (p.c0() + dx * .5f, p.c1()
						+ dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0() + dx, p.c1() + dy * .5f, p.c2() + dz * .5f) };
		c1->setPositions(cPositions1);

		Point<D> cPositions2[] = { Point<D> (p.c0() + dx * .5f, p.c1(), p.c2() + dz * .5f), Point<D> (p.c0() + dx,
				p.c1(), p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1(), p.c2() + dz), Point<D> (p.c0() + dx,
				p.c1(), p.c2() + dz), Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (
				p.c0() + dx, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f,
				p.c2() + dz), Point<D> (p.c0() + dx, p.c1() + dy * .5f, p.c2() + dz) };
		c2->setPositions(cPositions2);

		Point<D> cPositions3[] = { Point<D> (p.c0(), p.c1(), p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1(),
				p.c2() + dz * .5f), Point<D> (p.c0(), p.c1(), p.c2() + dz), Point<D> (p.c0() + dx * .5f, p.c1(), p.c2()
				+ dz), Point<D> (p.c0(), p.c1() + .5f * dy, p.c2() + dz * .5f), Point<D> (p.c0() + .5f * dx, p.c1()
				+ .5f * dy, p.c2() + dz * .5f), Point<D> (p.c0(), p.c1() + .5f * dy, p.c2() + dz), Point<D> (p.c0()
				+ .5f * dx, p.c1() + .5f * dy, p.c2() + dz) };
		c3->setPositions(cPositions3);

		Point<D> cPositions4[] = { Point<D> (p.c0(), p.c1() + dy * .5f, p.c2()), Point<D> (p.c0() + dx * .5f, p.c1()
				+ dy * .5f, p.c2()), Point<D> (p.c0(), p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0() + dx
				* .5f, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0(), p.c1() + dy, p.c2()), Point<D> (p.c0()
				+ .5f * dx, p.c1() + dy, p.c2()), Point<D> (p.c0(), p.c1() + dy, p.c2() + dz * .5f), Point<D> (p.c0()
				+ .5f * dx, p.c1() + dy, p.c2() + dz * .5f) };
		c4->setPositions(cPositions4);

		Point<D> cPositions5[] = { Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2()), Point<D> (p.c0() + dx,
				p.c1() + dy * .5f, p.c2()), Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2() + dz * .5f),
				Point<D> (p.c0() + dx, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1() + dy,
						p.c2()), Point<D> (p.c0() + dx, p.c1() + dy, p.c2()), Point<D> (p.c0() + dx * .5f, p.c1() + dy,
						p.c2() + dz * .5f), Point<D> (p.c0() + dx, p.c1() + dy, p.c2() + dz * .5f) };
		c5->setPositions(cPositions5);

		Point<D> cPositions6[] = { Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0()
				+ dx, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2()
				+ dz), Point<D> (p.c0() + dx, p.c1() + dy * .5f, p.c2() + dz), Point<D> (p.c0() + dx * .5f,
				p.c1() + dy, p.c2() + dz * .5f), Point<D> (p.c0() + dx, p.c1() + dy, p.c2() + dz * .5f), Point<D> (
				p.c0() + dx * .5f, p.c1() + dy, p.c2() + dz), Point<D> (p.c0() + dx, p.c1() + dy, p.c2() + dz) };
		c6->setPositions(cPositions6);

		Point<D> cPositions7[] = { Point<D> (p.c0(), p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (
				p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2() + dz * .5f), Point<D> (p.c0(), p.c1() + dy * .5f, p.c2()
				+ dz), Point<D> (p.c0() + dx * .5f, p.c1() + dy * .5f, p.c2() + dz), Point<D> (p.c0(), p.c1() + dy,
				p.c2() + dz * .5f), Point<D> (p.c0() + .5f * dx, p.c1() + dy, p.c2() + dz * .5f), Point<D> (p.c0(),
				p.c1() + dy, p.c2() + dz), Point<D> (p.c0() + .5f * dx, p.c1() + dy, p.c2() + dz) };
		c7->setPositions(cPositions7);

		D cData0[] = { interpolateData(c0->getPositions()[0]), interpolateData(c0->getPositions()[1]), interpolateData(
				c0->getPositions()[2]), interpolateData(c0->getPositions()[3]), interpolateData(c0->getPositions()[4]),
				interpolateData(c0->getPositions()[5]), interpolateData(c0->getPositions()[6]), interpolateData(
						c0->getPositions()[7]) };
		c0->setData(cData0);

		//        derived.loadVectorVariable("b");
		//        for (int i = 0; i < positions.length; i++)
		//        {
		//
		//            Vector3f kameleonValue = new Vector3f(
		//                    derived.interpolate("bx", c1.getPositions()[i].c0(), c1.getPositions()[i].c1(), c1.getPositions()[i].c2()),
		//                    derived.interpolate("by", c1.getPositions()[i].c0(), c1.getPositions()[i].c1(), c1.getPositions()[i].c2()),
		//                    derived.interpolate("bz", c1.getPositions()[i].c0(), c1.getPositions()[i].c1(), c1.getPositions()[i].c2()));
		//            //if (Math.abs(kameleonValue.length() - c1.getVectors()[i].length()) >.001)
		//                System.out.println("cposition: " + c1.getPositions()[i] + " vectors: " + c1.getVectors()[i] + " kameleonv: " + kameleonValue);
		//        }

		D cData1[] = { interpolateData(c1->getPositions()[0]), interpolateData(c1->getPositions()[1]), interpolateData(
				c1->getPositions()[2]), interpolateData(c1->getPositions()[3]), interpolateData(c1->getPositions()[4]),
				interpolateData(c1->getPositions()[5]), interpolateData(c1->getPositions()[6]), interpolateData(
						c1->getPositions()[7]) };
		c1->setData(cData1);

		D cData2[] = { interpolateData(c2->getPositions()[0]), interpolateData(c2->getPositions()[1]), interpolateData(
				c2->getPositions()[2]), interpolateData(c2->getPositions()[3]), interpolateData(c2->getPositions()[4]),
				interpolateData(c2->getPositions()[5]), interpolateData(c2->getPositions()[6]), interpolateData(
						c2->getPositions()[7]) };
		c2->setData(cData2);

		D cData3[] = { interpolateData(c3->getPositions()[0]), interpolateData(c3->getPositions()[1]), interpolateData(
				c3->getPositions()[2]), interpolateData(c3->getPositions()[3]), interpolateData(c3->getPositions()[4]),
				interpolateData(c3->getPositions()[5]), interpolateData(c3->getPositions()[6]), interpolateData(
						c3->getPositions()[7]) };
		c3->setData(cData3);

		D cData4[] = { interpolateData(c4->getPositions()[0]), interpolateData(c4->getPositions()[1]), interpolateData(
				c4->getPositions()[2]), interpolateData(c4->getPositions()[3]), interpolateData(c4->getPositions()[4]),
				interpolateData(c4->getPositions()[5]), interpolateData(c4->getPositions()[6]), interpolateData(
						c4->getPositions()[7]) };
		c4->setData(cData4);

		D cData5[] = { interpolateData(c5->getPositions()[0]), interpolateData(c5->getPositions()[1]), interpolateData(
				c5->getPositions()[2]), interpolateData(c5->getPositions()[3]), interpolateData(c5->getPositions()[4]),
				interpolateData(c5->getPositions()[5]), interpolateData(c5->getPositions()[6]), interpolateData(
						c5->getPositions()[7]) };
		c5->setData(cData5);

		D cData6[] = { interpolateData(c6->getPositions()[0]), interpolateData(c6->getPositions()[1]), interpolateData(
				c6->getPositions()[2]), interpolateData(c6->getPositions()[3]), interpolateData(c6->getPositions()[4]),
				interpolateData(c6->getPositions()[5]), interpolateData(c6->getPositions()[6]), interpolateData(
						c6->getPositions()[7]) };
		c6->setData(cData6);

		D cData7[] = { interpolateData(c7->getPositions()[0]), interpolateData(c7->getPositions()[1]), interpolateData(
				c7->getPositions()[2]), interpolateData(c7->getPositions()[3]), interpolateData(c7->getPositions()[4]),
				interpolateData(c7->getPositions()[5]), interpolateData(c7->getPositions()[6]), interpolateData(
						c7->getPositions()[7]) };
		c7->setData(cData7);

		cells[0] = c0;
		cells[1] = c1;
		cells[2] = c2;
		cells[3] = c3;
		cells[4] = c4;
		cells[5] = c5;
		cells[6] = c6;
		cells[7] = c7;

		//        System.out.println("children.");
		//        for (int i = 0; i < children.length; i++)
		//        {
		//            System.out.println("deltas: " + children[i].getDeltas().length());
		//        }


		return cells;
	}

	template<class P, class D>
	D Cell3D<P, D>::interpolateData(Point<P>& point)
	{
		return interpolateData(point.c0(), point.c1(), point.c2());
	}

	template<class P, class D>
	D Cell3D<P, D>::interpolateData(const P& c0, const P& c1, const P& c2)
	{
		D m_x = (c0 - positions[0].c0()) / dx;
		D m_y = (c1 - positions[0].c1()) / dy;
		D m_z = (c2 - positions[0].c2()) / dz;
		//System.out.println("m_x: " + m_x + " m_y: " + m_y + " m_z: " + m_z);

		D vc1 = (1 - m_x) * data[0] + m_x * data[1];
		D vc2 = (1 - m_x) * data[4] + m_x * data[5];
		D vc3 = (1 - m_x) * data[2] + m_x * data[3];
		D vc4 = (1 - m_x) * data[6] + m_x * data[7];
		D vc5 = (1 - m_z) * vc1 + m_z * vc3;
		D vc6 = (1 - m_z) * vc2 + m_z * vc4;
		D xc = (1 - m_y) * vc5 + m_y * vc6;

		return xc;
	}

	template<class P, class D>
	Point<P> * Cell3D<P, D>::getPositions()
	{
		return positions;
	}

	template<class P, class D>
	Point<P> * Cell3D<P, D>::getPosition(int i)
	{
		return &(positions[i]);
	}

	template<class P, class D>
	Vector<D> Cell3D<P, D>::getDeltas()
	{
		return Vector<D> (dx, dy, dz);
	}

	template<class P, class D>
	Cell3D<P, D>::~Cell3D()
	{
	}

}
#endif /* __cplusplus */
#endif /* CELL3D_H_ */
