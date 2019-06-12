using System;

namespace GameWorld
{
	/// <summary>
	/// Input control for an actor. 
	/// </summary>
	public class Controller
	{

		public enum Rotating
		{
			Stationary,
			Clockwise,
			AntiClockwise
		}

		public enum Moving
		{
			Stationary,
			Forward,
			Backward
		}


		public float Speed { get; private set; } = 0.0f; // Movement speed
		public float AngularSpeed { get; private set; } = 0.0f; // Rotation speed

		private Rotating m_rotating = Rotating.Stationary;

		private Moving m_moving = Moving.Stationary;

		private static readonly Random m_random = new Random((int)DateTime.Now.Ticks & 0x0000FFFF); // Randomiser used to automatically pick states

		/// <summary>
		///  Automatically choose a movement state
		/// </summary>
		public void AutoMove()
		{
			m_moving = (Moving)m_random.Next(0, 3);
		}

		/// <summary>
		///  Automatically choose a rotation state
		/// </summary>
		public void AutoRotate()
		{
			m_rotating = (Rotating)m_random.Next(0, 3);
		}

		/// <summary>
		/// Stops all movement and rotation
		/// </summary>
		public void AllStop()
		{
			Speed = 0;
			AngularSpeed = 0;

			m_moving = Moving.Stationary;
			m_rotating = Rotating.Stationary;
		}

		/// <summary>
		/// Update the speed of movement and rotation
		/// </summary>
		/// <param name="deltaT">Time since last update</param>
		public void UpdateSpeed(float deltaT)
		{
			var acc = deltaT / 10;

			if (m_moving != Moving.Stationary)
			{
				if (m_moving == Moving.Forward)
					Speed -= acc;
				else
					Speed += acc;
			}
			else
			{
				// Decrease speed
				if (Speed < 0)
					Speed += acc * 2.5f;
				else if (Speed > 0)
					Speed -= acc * 2.5f;

				if (System.Math.Abs(Speed) < acc * 2.5)
					Speed = 0;
			}

			// Enforce a max speed
			if (Speed > 1)
				Speed = 1;
			else if (Speed < -1)
				Speed = -1;


			if (m_rotating != Rotating.Stationary)
			{
				if (m_rotating == Rotating.AntiClockwise)
					AngularSpeed -= acc;
				else
					AngularSpeed += acc;
			}
			else // Having angular speed decrease faster than it increases to improve control
			{
				if (AngularSpeed < 0)
					AngularSpeed += acc * 2;
				else if (AngularSpeed > 0)
					AngularSpeed -= acc * 2;

				if (System.Math.Abs(AngularSpeed) < acc * 2)
					AngularSpeed = 0;
			}

			// Reducing max rotation speed to maintain control
			if (AngularSpeed > 0.75)
				AngularSpeed = 0.75f;
			else if (AngularSpeed < -0.75)
				AngularSpeed = -0.75f;

		}

		/// <summary>
		/// Set a movement state
		/// </summary>
		/// <param name="moving">The new movement state</param>
		public void Move(Moving moving)
		{
			m_moving = moving;
		}

		/// <summary>
		/// Set a rotation state
		/// </summary>
		/// <param name="rotating">The new rotation state</param>
		public void Rotate(Rotating rotating)
		{
			m_rotating = rotating;
		}


		/// <summary>
		/// Check if the actor is currently rotating, and the direction they are rotating.
		/// </summary>
		/// <returns></returns>
		public Rotating IsRotating()
		{
			return m_rotating;
		}

		/// <summary>
		/// Check if the actor is currently moving, and the direction they are moving.
		/// </summary>
		/// <returns></returns>
		public Moving IsMoving()
		{
			return m_moving;
		}
	}
}