/*
 *		EVController.cs
 *
 *	Author: Chris Allen
 *	Copyright Chris Allen 2019, all rights reserved
 *
 *	This file forms part of the Escape Velocity Project.
 *	It is intended to form part of my portfolio, for demonstration purposes ONLY.
 *
 *	You may NOT edit/alter this file in any way.
 *	You may NOT make any copies of this file for purposes other than its original intention (i.e. for demonstration purposes).
 *	You may NOT use or claim this file as your own work, either partially or wholly
 *
 *	This file is provided as-is.  No support will be provided for editing or using this file beyond its original intention.
 */

using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.SceneManagement;


[System.Serializable]
class Boundary
{
	public float xMin, xMax, yMin, yMax;
}

/**
 * 
 * Game Controller
 * 
 * Controls Obstacle spawning, and Ship movement
 * 
 */
public class EVController : MonoBehaviour
{
	// Player's ship
	public GameObject ship;

	// Box collider specifying boundry
	public Collider boundsCollider;

	// Scrollbar to represent min, current and max velocity
	public Scrollbar uiScrollbar;

	// Space-effect particle system
	public ParticleSystem system;

	// min and max speeds
	public float[] speeds;

	// Amount the speed changes on collision with an object
	public float speedStep = 2.0f;

	// speed of ship's movement when using arrow keys
	public float shipSpeed = 1.0f;

	// How fast the ship is currently travelling
	public float levelSpeed = 10f;

	// List of objects that control spawning obstacles
	public GameObject[] spawners;

	// Text showing how fast ship is travelling
	public Text speedText;

	// End of level text
	public Text winLoseText;

	// Text showing game is paused
	public Text pauseText;

	// Ship's Rigidbody for movement
	Rigidbody shipBody;

	// 2D rectangle specifying the bounds of ship movement
	Boundary bounds;

	// How far away the objects spawn from the ship
	float spawnZ;

	// In game, while the level is not complete
	bool inGame = true;

	// Paused flag - true when game is to be paused
	bool paused = true;

	// Target position of the ship - follows the mouse cursor, or a touch point
	Vector2 target;

	// Use this for initialization
	void Start()
	{
		Random.InitState((int)System.DateTime.Now.Ticks);

		shipBody = ship.GetComponent<Rigidbody>();

		Bounds b = boundsCollider.bounds;

		bounds = new Boundary();

		target = new Vector2(0, 0);

		// Set up the boundary, prevents the ship moving beyond the screen
		Vector3 min = b.min;
		Vector3 max = b.max;

		bounds.xMin = min.x;
		bounds.xMax = max.x;

		bounds.yMin = min.y;
		bounds.yMax = max.y;

		winLoseText.text = "";
		pauseText.text = "Paused";
		Time.timeScale = 0;
	}

	// Update is called once per frame
	void Update()
	{
		// Toggle paused state
		if (Input.GetKeyDown(KeyCode.Space))
		{
			paused = !paused;

			if (paused)
			{
				Time.timeScale = 0;
				pauseText.text = "Paused";
			}
			else
			{
				Time.timeScale = 1;
				pauseText.text = "";
			}
		}

		// Close game on Escape pressed
		if (Input.GetKeyDown(KeyCode.Escape))
			Application.Quit();

		if (inGame)
		{
			/*
			// Controls for toouchscreens
			if (Input.touchCount > 0)
			{
				Touch touch = Input.GetTouch(0);

				Vector3 touchPos = new Vector3(touchPos.x = touch.position.x, touchPos.y = touch.position.y, touchPos.z = 10);

				switch (touch.phase)
				{
					case TouchPhase.Began:
					case TouchPhase.Moved:
						target = Camera.main.ScreenToWorldPoint(touchPos);
						break;
				}
			}*/

			// Get the mouse cursor location as a world point, set that as the new target
			Vector3 mousePos = Camera.main.ScreenToWorldPoint(new Vector3(Input.mousePosition.x, Input.mousePosition.y, 10));

			target = new Vector2(mousePos.x, mousePos.y);

			Vector2 dist = new Vector2(target.x, target.y) - new Vector2(shipBody.position.x, shipBody.position.y);

			dist = Vector2.ClampMagnitude(dist, 1.0f);

			dist *= Time.deltaTime * shipSpeed;

			// Move towards the target, but keep within the boundarys
			shipBody.MovePosition(shipBody.position + new Vector3(dist.x, dist.y, 0));

			shipBody.position = new Vector3(
				Mathf.Clamp(shipBody.position.x, bounds.xMin, bounds.xMax),
				Mathf.Clamp(shipBody.position.y, bounds.yMin, bounds.yMax),
				shipBody.position.z
			);

			// Change particle effect speed based on current ship speed
			ParticleSystem.MainModule main = system.main;

			main.startSpeed = levelSpeed / 2.0f;

			speedText.text = "Level Speed: " + levelSpeed.ToString();

			uiScrollbar.value = (levelSpeed - speeds[0]) / (speeds[1] - speeds[0]);

			inGame = (levelSpeed != speeds[1]);
		}
	}

	public void Collide(Collider other)
	{
		// avoid collisions after game's end
		if (!inGame)
			return;

		// On collide with an Asteroid, reduce speed
		if (other.CompareTag("Asteroid"))
		{
			levelSpeed -= speedStep;

			// If speed drops below minimum speed, game ends in a loss
			if (levelSpeed <= speeds[0])
			{
				levelSpeed = speeds[0];
				EndLevel(false);
			}
			return;
		}

		// On collide with a Boost, increase speed
		if (other.CompareTag("Boost"))
		{
			levelSpeed += speedStep;

			// If speed goes above maximum speed, game ends in a win
			if (levelSpeed >= speeds[1])
			{
				levelSpeed = speeds[1];
				EndLevel(true);
			}
		}
	}

	void EndLevel(bool success)
	{
		// Turn off the spawners
		foreach (GameObject g in spawners)
			g.GetComponent<SpawnerBehaviour>().Deactivate();

		// check end condition and report to player
		if (success)
		{
			ship.GetComponentInChildren<ShipBehaviour>().Jump();
			winLoseText.text = "Level Complete! Resetting in 5 seconds...";
		}
		else
		{
			ship.GetComponentInChildren<ShipBehaviour>().Fail();
			winLoseText.text = "Level Failed! Resetting in 5 seconds...";
		}

		// Reset the game in 5 seconds
		Invoke("Reset", 5.0f);
	}

	void Reset()
	{
		// Reload the game scene, resetting the game
		SceneManager.LoadScene(0, LoadSceneMode.Single);
	}
}
