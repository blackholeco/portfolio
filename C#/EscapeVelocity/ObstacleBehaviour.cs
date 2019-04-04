/*
 *		ObstacleBehaviour.cs
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

/*
 *	Controls the behaviour of an Obstacle or Pickup (since they both
 *	do the same thing in the scene)
 */
public class ObstacleBehaviour : MonoBehaviour
{
	// Game controller
	public EVController controller;

	Rigidbody body;

	bool collided = false;

	Quaternion rotation;	// Rotation quaternion, makes the object rotate while in the scene

	// Use this for initialization
	void Start ()
	{
		body = GetComponent<Rigidbody> ();

		// Create a random rotation quaternion
		rotation = Quaternion.AngleAxis (
			Random.Range (0.25f, 2.5f),
			new Vector3 (
				Random.Range (-1, 1),
				Random.Range (-1, 1),
				Random.Range (-1, 1)
			)
		);
	}

	// Update is called once per frame
	void Update ()
	{
		// Moves the object towards the camera
		body.MovePosition (transform.position + new Vector3 (0, 0, -controller.levelSpeed * Time.deltaTime));
		body.MoveRotation (transform.rotation * rotation);
	}

	void OnTriggerEnter (Collider other)
	{
		// Check for collision with the Player, and destroy this object
		if (other.CompareTag ("Player") && collided == false) {
			controller.Collide (GetComponent<Collider> ());
			Destroy (this.gameObject);
			collided = true;
			return;
		}

		// On collision with the Obstacle catcher behind the camera, destroy this object
		if (other.CompareTag ("Catcher"))
			Destroy (this.gameObject);
	}
}
