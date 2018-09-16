package com.guildon.watercontainer

import android.app.Activity
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.widget.TextView
import java.util.*

/**
 * Created by Chris Allen on 14/Sep/2018.
 *
 * Water Container Activity written in Kotlin.
 *
 * Water Container is a small project undertaken as part of a previous job application.
 * The requirements of the App were specified by the employer, and this project was written
 * to fulfil those requirements.
 *
 * Originally the project was written in Java, but I have decided to add a Kotlin version, in
 * order to learn the language.  I also added a graphical representation of the result, whereas
 * the original had a text-based display
 *
 * Water Container analyses a certain layout of walls, and calculates how much water could be
 * stored by that configuration, if water were to be poured over the structure.
 *
 * Rules:
 * Volume is calculated as 1*1 squares
 * Water runs off the front and end of the configuration
 */
class MainActivity : Activity() {

	/**
	 * 	Maximum height a wall can be
	 */
	private val maxHeight = 9

	/**
	 * Width of the structure
	 */
	private val maxWidth = 9

	/**
	 * View to show the result of the analysis
	 */
	private lateinit var containerView: WaterContainerView

	/**
	 * Heights of the walls in the structure
	 */
	private lateinit var heightmap: IntArray

	/**
	 * Text based layout of the structure, showing the walls and the stored water
	 */
	lateinit var layout: Array<Char>

	/**
	 * Random number generator to create a random wall configuration
	 */
	private val random = Random(System.currentTimeMillis())


	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)

		setContentView(R.layout.activity_main)

		setActionBar(findViewById(R.id.toolbar))

		// Set up the initial height of the walls
		heightmap = intArrayOf(2, 5, 1, 2, 3, 4, 7, 7, 6)

		initLayout()

		containerView = findViewById(R.id.containerView)

		showConfig()

		analyse()
	}


	override fun onCreateOptionsMenu(menu: Menu): Boolean {
		val inflater = menuInflater

		inflater.inflate(R.menu.menu_main, menu)

		return super.onCreateOptionsMenu(menu)
	}


	override fun onOptionsItemSelected(item: MenuItem?): Boolean {

		if (item != null) { // Check to prevent warning about item being null

			when (item.itemId) {
				R.id.action_random -> {
					genConfig()
					return true
				}
			}
		}

		return super.onOptionsItemSelected(item)
	}

	/**
	 * Initialise the layout array, blanking it out to be filled in
	 * with a new wall configuration
	 */
	private fun initLayout() {
		// Create an empty structure (to be filled in later)
		layout = Array(maxHeight * maxWidth) { ' ' }

		// Mark the parts of the structure where the walls would be
		for (i in 0 until maxWidth) {
			for (h in 1..heightmap[i]) {
				layout[((h - 1) * maxWidth) + i] = '#'
			}
		}
	}

	/**
	 * Generate a new configuration of walls randomly
	 */
	private fun genConfig() {
		val temp = Array(maxWidth) { random.nextInt(maxHeight) + 1 }

		heightmap = temp.toIntArray()

		initLayout()

		showConfig()

		analyse()
	}

	/**
	 * Show the current wall configuration as a list of heights
	 */
	private fun showConfig() {
		val textConfig = findViewById<TextView>(R.id.textConfiguration)

		val builder = StringBuilder("Your Configuration is: ")

		for (height in heightmap)
			builder.append(height.toString()).append("    ")

		textConfig.text = builder.toString()
	}

	/**
	 * Analyse the configuration for the volume of water it can hold.
	 * <p>
	 * Models water filling up.
	 * <p>
	 * For each height, find an instance of a wall at that height, then find two larger walls either side
	 * Fill the space between those two walls with water up to the current height + 1, adding to the volume
	 * <p>
	 * Repeat until the highest wall is analysed, then report the final volume
	 */
	private fun analyse() {

		// Also measuring the volume of water stored
		var volume = 0

		// Storage for highest and lowest wall heights
		var highest = Int.MIN_VALUE
		var lowest = Int.MAX_VALUE

		val textVolume = findViewById<TextView>(R.id.textResult)

		// Find the smallest and highest walls
		for (item: Int in heightmap) {
			if (item > highest)
				highest = item

			if (item < lowest)
				lowest = item
		}

		// If all walls same size, no water can be stored
		if (highest == lowest) {
			textVolume.text = "Calculated volume is 0"

			return
		}

		// work upwards, from smallest wall to largest.
		// In both directions, find the next wall equal to and higher, then fill

		// For all possible heights in this configuration
		for (height: Int in lowest..highest) {

			for (j in 0 until maxWidth) {

				// find an instance of the current height
				if (heightmap[j] == height) {

					// Skip height if it is the same as the previous height
					// (prevents filling the same space twice)
					if (j >= 1 && heightmap[j] != heightmap[j - 1]) {

						// Find a higher wall to the left and right of this one
						val left = search(j, height, -1)
						val right = search(j, height, 1)

						// If higher walls found, fill space between walls, and add to total volumne
						if (left >= 0 && right >= 0) {
							val add = Math.min(heightmap[left], heightmap[right]) - height

							fill(left, right, height)

							volume += add * ((right - left) - 1)
						}
					}
				}
			}
		}

		// report the final volume
		textVolume.text = "Calculated volume is $volume"

		containerView.setLayout(layout, maxWidth, maxHeight)
		containerView.postInvalidate()
	}

	/**
	 * Fill up the space between two walls with water
	 *
	 * @param left : Left hand wall column index (0-indexed)
	 * @param right : Right hand wall column index (0-indexed)
	 * @param height : height to fill the water up to
	 */
	private fun fill(left: Int, right: Int, height: Int) {
		for (h in height until Math.min(heightmap[left], heightmap[right])) {
			for (i in left + 1 until right) {
				layout[(h * maxWidth) + i] = 'w'
			}
		}
	}

	/**
	 * Search from a starting point, in a direction, to find a higher wall
	 *
	 * @param column : The starting point for the search (not included).  0-indexed
	 * @param height : The height of the starting point
	 * @param direction : Direction of travel for the search. -1 is left, +1 is right
	 *
	 * @return The 0-indexed column index which has a higher wall than the original, or -1 if no results
	 */
	private fun search(column: Int, height: Int, direction: Int): Int {
		when (direction) {
			// Searching left
			1 -> {
				if (column == maxWidth - 1)
					return -1

				for (found in column + 1 until maxWidth) {
					if (heightmap[found] > height)
						return found
				}
				return -1
			}

			// Searching right
			-1 -> {
				if (column == 0)
					return -1

				for (found in column - 1 downTo 0) {
					if (heightmap[found] > height)
						return found
				}
				return -1
			}

			// Deal with invalid direction
			else -> return -1
		}
	}
}

