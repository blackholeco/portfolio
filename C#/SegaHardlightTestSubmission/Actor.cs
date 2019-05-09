using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Input;

namespace GameWorld
{
	/// <summary>
	/// Represents a moving character within the game world. It is rendered as a simple coloured polygon.
	/// </summary>
	public abstract class Actor
	{
		protected Point m_dimensions; // Size of the play area - prevents Actor from leaving the screen

		public Point Position { get; private set; } // Current position of the actor.
		public float Rotation { get; private set; } // Current rotation of the actor.
		public SolidColorBrush Colour { get; protected set; } // Colour to render the actor.
		protected readonly Controller m_controller = new Controller(); // Controller used to update the position and rotation of the actor.

		/// <summary>
		/// Construct the actor, initialising its position, rotation and colour.
		/// </summary>
		public Actor()
		{
			Position = new Point(400, 200);
			Rotation = 0.0f;
			Colour = Brushes.LightSeaGreen;
		}

		/// <summary>
		/// Set up the Actor's initial position and rotation.  Allows for randomisation of
		/// the initial game window
		/// </summary>
		/// <param name="position">Position to place the Actor</param>
		/// <param name="rotation">Actor's initial rotation</param>
		/// <param name="dimensions">Size of the play area</param>
		public void Initialise(Point position, float rotation, Point dimensions)
		{
			Position = position;
			Rotation = rotation;
			m_dimensions = dimensions;
		}

		/// <summary>
		/// Update the position and rotation of the actor - advancing by deltaT.
		/// </summary>
		/// <param name="deltaT"></param>
		/// 
		/// (have made this virtual)
		public virtual void Update(float deltaT)
		{
			m_controller.UpdateSpeed(deltaT);

			// Apply current rotation to actors heading.
			Rotation += m_controller.AngularSpeed;

			// Update the position of the actor taking into account its current heading.
			var movement = new Point
			{
				Y = m_controller.Speed * 100
			};

			var rotatedPoint = new Point(
				movement.X * Math.Cos(Rotation) - movement.Y * Math.Sin(Rotation),
				movement.Y * Math.Cos(Rotation) + movement.X * Math.Sin(Rotation)
			);
			var point = Position;
			point.X += rotatedPoint.X;
			point.Y += rotatedPoint.Y;

			// Checking that the Actor stays within the boundaries
			if (point.X < 10 && rotatedPoint.X < 0)
				point.X = 10;
			else if (point.X > m_dimensions.X && rotatedPoint.X > 0)
				point.X = m_dimensions.X;

			if (point.Y < 10 && rotatedPoint.Y < 0)
				point.Y = 10;
			else if (point.Y > m_dimensions.Y && rotatedPoint.Y > 0)
				point.Y = m_dimensions.Y;

			Position = point;
		}

		/// <summary>
		/// Add representation of the actor into the renderables collection.
		/// </summary>
		/// <param name="renderables"></param>
		public void Render(UIElementCollection renderables)
		{
			renderables.Add(RenderUtils.CreateTriangle(Position, Rotation, Colour));
		}
	}

	/// <summary>
	/// Implementation of the Actor class, which moves the Actor autonomously.
	/// 
	/// Every second, the direction of movement and rotation is randomised.
	/// </summary>
	public class AutoActor : Actor
	{
		protected float m_timer; // Records time between changes in autonomous movement

		public AutoActor() :
			base()
		{
			m_timer = 0.0f;

			Colour = Brushes.LightSeaGreen;
		}

		public override void Update(float deltaT)
		{
			UpdateMovement(deltaT);

			base.Update(deltaT);
		}

		/// <summary>
		/// Update the AutoActor's movement state, if needed
		/// </summary>
		/// <param name="deltaT">Time since last update in seconds</param>
		protected virtual void UpdateMovement(float deltaT)
		{
			m_timer += deltaT;

			if (m_timer > 1.0f)
			{
				m_controller.AutoMove();
				m_controller.AutoRotate();

				// Trying to stop Actors from spinning for long periods
				if (m_controller.IsRotating() != Controller.Rotating.Stationary)
					m_timer -= 0.5f;
				else
					m_timer -= 1.0f;
			}
		}
	}

	/// <summary>
	/// Implementation of the Actor class, which moves the Actor based on user input.
	/// </summary>
	public class ManualActor : AutoActor
	{
		private bool manual = true; // Toggles control method between manual and automatic

		public enum Player
		{
			First,
			Second
		}

		/// <summary>
		/// Defines the controls for this ManualActor.
		/// 
		/// Controls should be different for each Player
		/// </summary>
		public struct Controls
		{
			public Key forward; // Key to move the Actor forward
			public Key backward; // Key to move the Actor backwards
			public Key clockwise; // Key to rotate the Actor clockwise
			public Key anticlockwise; // Key to rotate the Actor anti-clockwise
			public Key toggle; // Key to toggle between manual and automatic control
		};

		private Controls m_controls; // This Actor's controls

		private readonly Player m_player; // Player numer

		public ManualActor(Player player)
			: base()
		{
			m_player = player;

			Colour = Brushes.Aquamarine;

			SetupControls();
		}

		/// <summary>
		/// Check keyboard input and change movement state as required
		/// </summary>
		/// <param name="deltaT">Time since last update</param>
		protected override void UpdateMovement(float deltaT)
		{
			if (manual)
			{
				if (Keyboard.IsKeyDown(m_controls.forward))
					m_controller.Move(Controller.Moving.Forward);
				else if (Keyboard.IsKeyDown(m_controls.backward))
					m_controller.Move(Controller.Moving.Backward);
				else
					m_controller.Move(Controller.Moving.Stationary);

				if (Keyboard.IsKeyDown(m_controls.anticlockwise))
					m_controller.Rotate(Controller.Rotating.AntiClockwise);
				else if (Keyboard.IsKeyDown(m_controls.clockwise))
					m_controller.Rotate(Controller.Rotating.Clockwise);
				else
					m_controller.Rotate(Controller.Rotating.Stationary);
			}
			else
				base.UpdateMovement(deltaT);
		}

		/// <summary>
		/// Handle a key down event.  Used to switch between automatic and manual controls
		/// </summary>
		/// <param name="args">Key Event arguments</param>
		/// <returns>True if the event is handled by this instance, false if not</returns>
		public bool HandleKey(KeyEventArgs args)
		{
			if (args.Key == m_controls.toggle)
			{
				manual = !manual;
				return true;
			}

			return false;
		}

		/// <summary>
		/// Set up this Actor's controls based on player number.
		/// 
		/// Player 1 uses WASD, Player 2 uses Arrow Keys
		/// </summary>
		private void SetupControls()
		{
			m_controls = new Controls();

			switch (m_player)
			{
				case Player.First:
					m_controls.forward = Key.W;
					m_controls.backward = Key.S;
					m_controls.clockwise = Key.D;
					m_controls.anticlockwise = Key.A;
					m_controls.toggle = Key.Tab;
					break;
				case Player.Second:
					m_controls.forward = Key.Up;
					m_controls.backward = Key.Down;
					m_controls.clockwise = Key.Right;
					m_controls.anticlockwise = Key.Left;
					m_controls.toggle = Key.Enter;
					break;
			}
		}
	}

	/// <summary>
	/// Subclass of the AutoActor, which attempts to escape from another Actor (normally Player 1)
	/// 
	/// PreyActor will act like a normal AutoActor, until its predator comes within a certain distance.
	/// It will then change try to find the best escape route and change its movement state accordingly.
	/// </summary>
	class PreyActor : AutoActor
	{
		private readonly Actor m_predator;

		public PreyActor(Actor predator)
			: base()
		{
			m_predator = predator;
		}


		protected override void UpdateMovement(float deltaT)
		{
			// Get the distance to the predator

			Point toPredator = new Point(m_predator.Position.X - Position.X, m_predator.Position.Y - Position.Y);

			// Avoiding square root calculation
			double distance = (toPredator.X * toPredator.X) + (toPredator.Y * toPredator.Y);

			// Run away if predator is too close
			if (distance < 100 * 100)
			{
				// Have the prey stop completely (perhaps not realistic, but makes the change in movement to escape more visible)
				if (m_timer != 0)
					m_controller.AllStop();

				m_timer = 0;

				// Change colour to indicate danger state
				Colour = Brushes.OrangeRed;

				// Find angle to predator, and figure out best route of escape
				double angle = System.Math.Atan2(toPredator.Y, toPredator.X);

				angle *= (180.0f / System.Math.PI);

				// Convert to correct orientation
				angle += 90;

				// Stay in positive angles
				if (angle < 0)
					angle += 360;

				// This Actor's current heading, as an angle.  Used to decide the best direction to go
				double heading = Rotation * (180.0f / System.Math.PI);

				// Keep heading positive
				if (heading > 0)
				{
					while (heading > 360.0f)
						heading -= 360.0f;
				}
				else
				{
					while (heading < 0)
						heading += 360.0f;
				}

				// Convert angle to Predator to Prey's local orientation
				double result = System.Math.Abs(angle - heading);

				if (result >= 335 || result < 45)
				{
					// Predator in front - reverse
					m_controller.Move(Controller.Moving.Backward);
					m_controller.Rotate(Controller.Rotating.Stationary);
				}
				else if (result >= 135 && result < 225)
				{
					// Predator behind - go forward
					m_controller.Move(Controller.Moving.Forward);
					m_controller.Rotate(Controller.Rotating.Stationary);
				}
				else if (result >= 45 && result < 135)
				{
					// Predator on the right - turn left
					m_controller.Move(Controller.Moving.Forward);
					m_controller.Rotate(Controller.Rotating.AntiClockwise);
				}
				else if (result >= 225 && result < 335)
				{
					// Predator on the left - turn right
					m_controller.Move(Controller.Moving.Forward);
					m_controller.Rotate(Controller.Rotating.Clockwise);
				}
			}
			else
			{
				// Operate normally
				base.UpdateMovement(deltaT);
				Colour = Brushes.PaleGoldenrod;
			}
		}
	}
}