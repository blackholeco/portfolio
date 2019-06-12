using System;
using System.Windows;
using System.Windows.Shapes;
using System.Windows.Media;

namespace GameWorld
{
    /// <summary>
    /// A collection of utility functions to help with rendering the game world.
    /// </summary>
    public class RenderUtils
    {
        /// <summary>
        /// Create and return a single triangle given the center point, rotation and colour.
        /// </summary>
        /// <param name="center"></param>
        /// <param name="rotation"></param>
        /// <param name="colour"></param>
        /// <returns></returns>
        public static Polygon CreateTriangle(Point center, float rotation, SolidColorBrush colour)
        {
            Polygon triangle = new Polygon();
            triangle.Stroke = Brushes.Black;
            triangle.Fill = colour;
            triangle.StrokeThickness = 2;
            triangle.HorizontalAlignment = HorizontalAlignment.Left;
            triangle.VerticalAlignment = VerticalAlignment.Top;
            Point[] originalPoints = new Point[]{new Point(0, -10), new Point(-6, 10), new Point(6, 10)};
            PointCollection myPointCollection = new PointCollection();
            for (var i = 0; i < 3; ++i)
            {
                Point rotatedPoint = new Point(
                    (originalPoints[i].X * Math.Cos(rotation)) - (originalPoints[i].Y * Math.Sin(rotation)),
                    (originalPoints[i].Y * Math.Cos(rotation)) + (originalPoints[i].X * Math.Sin(rotation))
                );
                Point finalPoint = new Point(rotatedPoint.X + center.X, rotatedPoint.Y + center.Y);
                myPointCollection.Add(finalPoint);
            }
            triangle.Points = myPointCollection;
            return triangle;
        }
    }
}