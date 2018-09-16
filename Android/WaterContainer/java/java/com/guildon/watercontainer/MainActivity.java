package com.guildon.watercontainer;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.TextView;

import java.util.Locale;
import java.util.Random;

/**
 * Created by Chris Allen.
 * <p>
 * Water Container Activity written in Java.
 * <p>
 * Water Container is a small project undertaken as part of a previous job application.
 * The requirements of the App were specified by the employer, and this project was written
 * to fulfil those requirements.
 * <p>
 * Originally the project was written in Java.  I also added a graphical representation of the result, whereas
 * the original had a text-based display, as well as making a few code changes, which do not affect
 * the final outcome of the analysis.
 * <p>
 * Water Container analyses a certain layout of walls, and calculates how much water could be
 * stored by that configuration, if water were to be poured over the structure.
 * <p>
 * Rules:
 * Volume is calculated as 1*1 squares
 * Water runs off the front and end of the configuration
 */
public class MainActivity extends Activity {

	/**
	 * Maximum height a wall can be
	 */
	private final int maxHeight = 9;

	/**
	 * Width of the structure
	 */
	private final int maxWidth = 9;

	/**
	 * Random number generator to create a random wall configuration
	 */
	private final Random random = new Random(System.currentTimeMillis());

	/**
	 * Heights of the walls in the structure
	 */
	private int[] heightmap = null;

	/**
	 * Text based layout of the structure, showing the walls and the stored water
	 */
	private char[] layout = null;

	/**
	 * View to show the result of the analysis
	 */
	private WaterContainerView mContainerView;


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		setActionBar(findViewById(R.id.toolbar));

		heightmap = new int[]{
				2, 5, 1, 2, 3, 4, 7, 7, 6
		};

		initLayout();

		mContainerView = findViewById(R.id.containerView);

		showConfig();

		analyse();
	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();

		inflater.inflate(R.menu.menu_main, menu);

		return super.onCreateOptionsMenu(menu);
	}


	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
			case R.id.action_random:
				genConfig();
				return true;

			default:
				return super.onOptionsItemSelected(item);
		}
	}

	/**
	 * Initialise the layout array, blanking it out to be filled in
	 * with a new wall configuration
	 */
	private void initLayout() {
		layout = new char[maxHeight * maxWidth];

		for (int i = 0; i < maxWidth; i++) {
			for (int h = 1; h <= heightmap[i]; h++)
				layout[((h - 1) * maxWidth) + i] = '#';
		}
	}

	/**
	 * Generate a new configuration of walls randomly
	 */
	public void genConfig() {
		heightmap = new int[maxWidth];

		for (int i = 0; i < maxWidth; i++)
			heightmap[i] = random.nextInt(maxHeight) + 1;

		initLayout();

		showConfig();

		analyse();
	}

	/**
	 * Show the current wall configuration as a list of heights
	 */
	protected void showConfig() {
		TextView textConfig = findViewById(R.id.textConfiguration);

		StringBuilder config = new StringBuilder("Your Configuration is: ");

		for (int aHeightmap : heightmap)
			config.append(String.valueOf(aHeightmap)).append("    ");

		textConfig.setText(config.toString());
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
	private void analyse() {

		// Also measuring the volume of water stored
		int volume = 0;

		// Storage for highest and lowest wall heights
		int highest = Integer.MIN_VALUE;
		int lowest = Integer.MAX_VALUE;

		final TextView textVolume = findViewById(R.id.textResult);

		// Find the smallest and highest walls
		for (int item : heightmap) {
			if (item > highest)
				highest = item;

			if (item < lowest)
				lowest = item;
		}

		// If all walls same size, no water can be stored
		if (highest == lowest) {
			textVolume.setText("Calculated volume is 0");

			return;
		}

		// work upwards, from smallest wall to largest.
		// In both directions, find the next wall equal to and higher, then fill

		// For all possible heights in this configuration
		for (int height = lowest; height <= highest; height++) {

			// find an instance of the current height
			for (int j = 0; j < maxWidth; j++) {

				if (heightmap[j] == height) {

					// Skip height if it is the same as the previous height
					// (prevents filling the same space twice)
					if (j >= 1 && heightmap[j] != heightmap[j - 1]) {

						int left = search(j, height, -1);
						int right = search(j, height, 1);

						if (left >= 0 && right >= 0) {
							int add = Math.min(heightmap[left], heightmap[right]) - height;

							fill(left, right, height);

							volume += add * ((right - left) - 1);
						}
					}
				}
			}
		}

		// report the final volume
		textVolume.setText(String.format(Locale.UK, "Calculated volume is %d", volume));

		mContainerView.setLayout(layout, maxWidth, maxHeight);
		mContainerView.postInvalidate();
	}

	/**
	 * Fill up the space between two walls with water
	 *
	 * @param left   : Left hand wall column index (0-indexed)
	 * @param right  : Right hand wall column index (0-indexed)
	 * @param height : height to fill the water up to
	 */
	private void fill(int left, int right, int height) {
		for (int h = height; h < Math.min(heightmap[left], heightmap[right]); h++) {
			for (int i = left + 1; i < right; i++)
				layout[(h * maxWidth) + i] = 'w';
		}
	}

	/**
	 * Search from a starting point, in a direction, to find a higher wall
	 *
	 * @param column    : The starting point for the search (not included).  0-indexed
	 * @param height    : The height of the starting point
	 * @param direction : Direction of travel for the search. -1 is left, +1 is right
	 * @return The 0-indexed column index which has a higher wall than the original, or -1 if no results
	 */
	private int search(int column, int height, int direction) {
		switch (direction) {
			case -1:    // Searching left
				if (column == 0)
					return -1;

				for (int found = column - 1; found >= 0; found--) {
					if (heightmap[found] > height)
						return found;
				}

				break;

			case 1:     // Searching right
				if (column == maxWidth - 1)
					return -1;

				for (int found = column + 1; found < maxWidth; found++) {
					if (heightmap[found] > height)
						return found;
				}

				break;

			default:    // Deal with invalid direction
				return -1;
		}

		return -1;
	}
}
