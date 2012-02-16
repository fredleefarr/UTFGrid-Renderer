//#include <msclr\marshal_cppstd.h>
#include "stdafx.h"
#include <string>
#include <stdlib.h>
#include <msclr\marshal.h>

#include "wrapper.h"

using namespace wrapper;
using namespace System;
using namespace msclr::interop;
using namespace System::Collections::Generic;
using namespace System::Drawing;
using namespace System::Text;
using namespace model::types;
using namespace model::enums;
using namespace System::Diagnostics;

#include <stdio.h>
#include <math.h>
#include "grid.h"
#include "renderer.h"

#include <iostream>
#include <sstream>

using namespace std;

  
enum
{
    width  = 256,
    height = 256
};

void draw_ellipse(agg::grid_rasterizer& ras,
                  double x,  double y,
                  double rx, double ry)
{
    int i;
    ras.move_to_d(x + rx, y);
    for(i = 1; i < 360; i++)
    {
        double a = double(i) * 3.1415926 / 180.0;
        ras.line_to_d(x + cos(a) * rx, y + sin(a) * ry);
    }
}


void draw_line(agg::grid_rasterizer& ras,
               double x1, double y1, 
               double x2, double y2,
               double width)
{

    double dx = x2 - x1;
    double dy = y2 - y1;
    double d = sqrt(dx*dx + dy*dy);
    
    dx = width * (y2 - y1) / d;
    dy = width * (x2 - x1) / d;

    ras.move_to_d(x1 - dx,  y1 + dy);
    ras.line_to_d(x2 - dx,  y2 + dy);
    ras.line_to_d(x2 + dx,  y2 - dy);
    ras.line_to_d(x1 + dx,  y1 - dy);
}


std::wstring buf2grid_as_string(int step, agg::grid_rendering_buffer& buf) {
    std::wostringstream s(L"");
	
	s << "\"grid\":[";
    for (unsigned y = 0; y < buf.height(); y=y+step)
    {
        agg::grid_value* row = buf.row(y);
        s << "\"";
        for (unsigned x = 0; x < buf.width(); x=x+step)	
        {
			agg::grid_value val = (agg::grid_value)row[x];
			if (val < 0) {
				s << " ";}
			else {
				s << val;
			}
        }

         s << "\"";
		if(y < buf.height()-step) {
			s << ",";
		}
    }
	s << "]";
	
	return  s.str();
}


char16_t get_key(int index) {

	// https://github.com/mapbox/mbtiles-spec/blob/master/1.1/utfgrid.md

	// Get passes zero
	index++;

	// Add 3210.
    index += 32;

    // If the result is >= 34x10, add 1.
    if (index >= 34) index++;

    // If the result is >= 92x10, add 1.
    if (index >= 92) index++;

	char16_t st = (char16_t)index;
	return st;
	
}

UtfGrid^ GridRenderer::GetUtfGrid(TileUtfGridCollection ^utfGridCollection) {
 
	agg::grid_value *buf = new agg::grid_value[width * height];
    agg::grid_rendering_buffer rbuf2(buf, width, height, width);

    agg::grid_renderer<agg::span_grid> ren_grid(rbuf2);
    agg::grid_rasterizer ras_grid;

    ren_grid.clear(' ');

	UtfGrid^ utfGridResult = gcnew UtfGrid();

	System::String^ keyString("");
	System::String^ dataString("");

	keyString += "\"keys\":[\"\",";
	dataString += "\"data\":{";
	List<TileUtfGridCoords^> coordCollection = utfGridCollection->Collection;

	int c;
	 for(c = 0; c < coordCollection.Count; c++)
	{
				char32_t key = get_key(c);
				TileUtfGridCoords ^tileUtfGridCoords = coordCollection[c];

				keyString += "\"" + tileUtfGridCoords->Id.ToString() + "\"";
				dataString += "\"" + tileUtfGridCoords->Id.ToString() + "\":{\"NAME\":\"" + tileUtfGridCoords->Name + "\"";

				if (c < coordCollection.Count - 1) {
					keyString += ",";
					dataString += ",";
				}

				List<PointF> pointList = tileUtfGridCoords->PointList;

				UtfShapeType shapeType = tileUtfGridCoords->UtfShapeType;

				if(shapeType == UtfShapeType::Line) {
	
					for(int p = 0; p < pointList.Count-1; p++)
					{

						PointF pnt = pointList[p];
						PointF nxtPnt = pointList[p+1];

						draw_line(	ras_grid, 
									pnt.X,
									pnt.Y,
									nxtPnt.X,
									nxtPnt.Y,
									tileUtfGridCoords->LineWidth
								);

					}


				}
				else if (shapeType == UtfShapeType::Polygon) {
					for(int p = 0; p < pointList.Count; p++)
					{

						PointF pnt = pointList[p];

						if(p == 0) {
							ras_grid.move_to_d(pnt.X,pnt.Y);
						} else {
							ras_grid.line_to_d(pnt.X,pnt.Y);
						}
					}
		
				}
				else
				{
					throw gcnew Exception("Unsupported type");
				}

				ras_grid.render(ren_grid, key);
			
	}

	keyString += "]";
	dataString += "}";

	std::wstring gridString = buf2grid_as_string(4,rbuf2);
	System::String^ gridSystemString = gcnew System::String(gridString.c_str());
	System::String^ finalJson = "{" + gridSystemString + "," + keyString + "," + dataString + "}";

	utfGridResult->JsonString = finalJson;

    return utfGridResult;

 }