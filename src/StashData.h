/*
	Cole Clark's Fractal Suite

	StashData.h
	Header defining stashed data.
 */

#pragma once

#include "typedefs.h"

#include <OP/OP_Node.h>

namespace CC
{
	/// Base class for stash data defining abstract methods.
	class StashData
	{
	public:
		virtual void evalArgs(const OP_Node* node, fpreal t) = 0;
	};


	/// Struct meant to simplify the stashing of xform parm data.
	struct XformStashData : public StashData
	{
		double offset_x, offset_y{ 0 };
		double rotate{ 0 };
		double scale{ 1 };
		RSTORDER xord{ RSTORDER::TRS };

		XformStashData(
			double offset_x = 0,
			double offset_y = 0,
			double rotate = 0,
			double scale = 0,
			RSTORDER xord = RSTORDER::RST);

		void evalArgs(const OP_Node* node, fpreal t);
	};

	/// Struct meant to simplify the stashing of Mandelbrot parm data
	struct MandelbrotStashData : public StashData
	{
		int iters{ 50 };
		double power{ 0 };
		double bailout{ 2 };
		int jdepth{ 0 };
		COMPLEX joffset{ 0.0f, 0.0f };
		bool blackhole{ false };

		MandelbrotStashData(
			int iters = 50,
			double power = 2,
			double bailout = 2,
			int jdepth = 0,
			COMPLEX joffset = (0.0f, 0.0f),
			bool blackhole = false);

		void evalArgs(const OP_Node* node, fpreal t);
	};

	struct PickoverStashData : public MandelbrotStashData
	{
		COMPLEX popoint{ 0, 0 };
		double porotate{ 0 };
		bool pomode{ false };
		bool poref{ true };
		fpreal porefsize{ 10.0 };

		PickoverStashData(
			int iters = 50,
			double power = 2.0,
			double bailout = 2.0,
			int jdepth = 0,
			COMPLEX joffset = (0.0, 0.0),
			bool blackhole = false,
			COMPLEX popoint = (0.0, 0.0),
			double porotate = 0.0,
			bool pomode = false,
			bool poref = true,
			fpreal porefsize = 10.0);

		void evalArgs(const OP_Node* node, fpreal t);
	};

	struct LyapunovStashData : public StashData
	{
		int iters{ 50 };
		double start{ 0.5 };
		double maxval{5.0f};
		bool invertnegative{ true };
		std::vector<double> seq;

		void evalArgs(const OP_Node* node, fpreal t);

		virtual ~LyapunovStashData();
	};
}
