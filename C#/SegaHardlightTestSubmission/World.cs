using System.Windows.Controls;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;

namespace GameWorld
{
	/// <summary>
	/// Main game world that contains a single actor.
	/// </summary>
	public class World
	{
		private readonly List<ManualActor> m_manuals; // References to player controlled actors in the world

		private readonly List<Actor> m_actors; // World's contents

		private readonly int m_capacity = 9; // Maximum number of Actors to be in the world

		private Point m_dimensions; // Size of the play area

		/// <summary>
		/// Construct the world and a single actor.
		/// 
		/// Added width and height parameters to this, so that different sized worlds can be used,
		/// and so the play area is known.
		/// </summary>
		public World(int width = 800, int height = 400)
		{
			System.Random random = new System.Random();

			// Reducing actual window size to prevent objects going offscreen
			m_dimensions = new Point(width - 20, height - 40);

			m_manuals = new List<ManualActor>();

			// Generate a list of Actors who will appear, then randomly position them
			List<Actor> temp = new List<Actor>(m_capacity)
			{
				new ManualActor(ManualActor.Player.First),
				new ManualActor(ManualActor.Player.Second)
			};

			// Creating a reference to the player Actors for easier input resolution
			m_manuals.Add((ManualActor)temp[0]);
			m_manuals.Add((ManualActor)temp[1]);

			while (temp.Count < m_capacity)
				temp.Add(new PreyActor(temp[0]));

			m_actors = new List<Actor>(m_capacity);

			// Spawn area (smaller than the window size to ensure everything starts on screen)
			float w = (width * 0.75f) / 2.0f;
			float h = (height * 0.75f) / 2.0f;

			for (int i = 0; i < m_capacity; i++)
			{
				// Setting all Actors out into a 3 * 3 grid
				float x = (w * (i % 3));
				float y = (h * (i / 3));

				// Have all Actors point to the center of the screen at start (dot product)
				double angle = System.Math.Atan2(h - y, w - x);

				Actor actor = temp[random.Next(temp.Count)];

				actor.Initialise(new Point(x + 100, y + 50), (float)(angle + (System.Math.PI / 2.0)), m_dimensions);

				m_actors.Add(actor);

				temp.Remove(actor);
			}
		}

		/// <summary>
		/// Handle a key down event on the keyboard.  This is used for changing the control method
		/// of the ManualActors
		/// </summary>
		/// <param name="args">Arguments of the key event</param>
		public void HandleKey(KeyEventArgs args)
		{
			foreach (ManualActor actor in m_manuals)
			{
				if (actor.HandleKey(args))
					break;
			}
		}


		/// <summary>
		/// Update the world - advancing the simulation by deltaT.
		/// </summary>
		/// <param name="deltaT"></param>
		public void Update(float deltaT)
		{
			foreach (Actor actor in m_actors)
				actor.Update(deltaT);
		}

		/// <summary>
		/// Render the world.
		/// </summary>
		/// <param name="renderables"></param>
		public void Render(UIElementCollection renderables)
		{
			foreach (Actor actor in m_actors)
				actor.Render(renderables);
		}
	}
}