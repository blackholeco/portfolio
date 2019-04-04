/*
 *		SpawnerBehaviour.cs
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

/*
 *	Controls the behaviour of Spawner objects, which create new obstacles
 *	or pickups.
 */
public class SpawnerBehaviour : MonoBehaviour
{
	public GameObject asteroid;
	public GameObject boost;
	public Collider boundsCollider;
	public EVController controller;

	Boundary bounds;
	float spawnZ;

	// Use this for initialization
	void Start ()
	{

		Bounds b = boundsCollider.bounds;

		bounds = new Boundary ();

		Vector3 min = b.min;
		Vector3 max = b.max;

		bounds.xMin = min.x;
		bounds.xMax = max.x;

		bounds.yMin = min.y;
		bounds.yMax = max.y;

		spawnZ = transform.position.z;

		// Sets a timer to call Create sometime between 1.5 ans 5.5 seconds
		Invoke ("Create", Random.Range (1.5f, 5.5f));
	}

	public void Activate ()
	{
		gameObject.SetActive (true);

		Invoke ("Create", Random.Range (1.5f, 5.5f));
	}

	public void Deactivate ()
	{
		CancelInvoke ("Create");

		gameObject.SetActive (false);
	}


	void Create ()
	{
		// Only run if this object is active
		if (gameObject.activeInHierarchy) {
			float x = Random.Range (bounds.xMin, bounds.xMax);
			float y = Random.Range (bounds.yMin, bounds.yMax);

			// Create a new instance of Boost or Asteroid, randomise its starting position and rotation
			GameObject go;

			if (Random.value >= 0.5f)
				go = Instantiate (boost, new Vector3 (x, y, spawnZ), Quaternion.identity) as GameObject;
			else
				go = Instantiate (asteroid, new Vector3 (x, y, spawnZ), Quaternion.identity) as GameObject;

			ObstacleBehaviour behaviour = go.GetComponent<ObstacleBehaviour> ();
			behaviour.controller = controller;

			// Sets a timer to call this function again sometime between 1.5 ans 5.5 seconds
			Invoke ("Create", Random.Range (1.5f, 5.5f));
		}

	}
}
