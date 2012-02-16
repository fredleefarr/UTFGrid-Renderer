using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using model.enums;

namespace model.types
{
    public class TileUtfGridCoords
    {
        public List<PointF> PointList = new List<PointF>();
        public UtfShapeType UtfShapeType;
        public char ShapeRepresentation;
        public int LineWidth = 0;
        public int Id;
        
        public String Name;

    }
}
