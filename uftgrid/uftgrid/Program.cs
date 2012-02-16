using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using model.enums;
using model.types;
using wrapper;

namespace uftgrid
{
    class Program
    {
        static void Main(string[] args)
        {

            var gridCollection = new TileUtfGridCollection();

            // Add a polygon 
            var polygon = new TileUtfGridCoords();

            polygon.PointList.Add(new PointF(50, 50));
            polygon.PointList.Add(new PointF(50, 100));
            polygon.PointList.Add(new PointF(100, 100));
            polygon.PointList.Add(new PointF(100, 50));
            
            polygon.UtfShapeType = UtfShapeType.Polygon;
            polygon.LineWidth = 10;
            polygon.Id = 1;
            polygon.Name = "Eranu";

            gridCollection.Collection.Add(polygon);

            // Add a line
            var line = new TileUtfGridCoords();

            line.PointList.Add(new PointF(200, 200));
            line.PointList.Add(new PointF(200, 300));
            line.PointList.Add(new PointF(300, 300));
            line.PointList.Add(new PointF(300, 200));

            line.UtfShapeType = UtfShapeType.Line;
            line.LineWidth = 5;
            line.Id = 2;
            line.Name = "Oovavu";

            gridCollection.Collection.Add(line);

            // Render the shapes
            var gridRenderer = new GridRenderer();

            var json = gridRenderer.GetUtfGrid(gridCollection).JsonString;

            
            Console.WriteLine(json);
            Console.ReadLine();

            // System.IO.File.WriteAllText(@"C:\temp\grid.json", json);

        }
    }
}
