/*
 *		ShipBehaviour.cs
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

public class ShipBehaviour : MonoBehaviour
{

	Animator jumpAnim;

	// Use this for initialization
	void Start ()
	{
		jumpAnim = GetComponent<Animator> ();
	
	}
	
	// Update is called once per frame
	void Update ()
	{
		
	}

	public void Jump ()
	{
		// On win condition, have the ship play the Jump animation (speeds off into the distance)
		jumpAnim.SetBool ("jump", true);
	}

	public void Fail ()
	{
		// On lose condition, have the ship play the Fail animation (drops out of the screen)
		jumpAnim.SetBool ("fail", true);
	}
}
