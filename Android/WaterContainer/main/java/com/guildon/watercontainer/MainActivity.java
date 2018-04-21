package com.guildon.watercontainer;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.TextView;


import java.util.Random;

/**
 * App to analyse a wall configuration to find out how much water it would hold
 *
 * Rules:
 * Volume is calculated as 1*1 squares
 * Water runs off the front and end of the configuration
 *
 * @author Chris Allen
 */
public class MainActivity extends AppCompatActivity {

	final int mMaxHeight = 9;
	final int mMaxWidth = 9;

	int[] mHeightmap = null;

	char[][] mLayout = null;

	WaterContainerView mContainerView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Toolbar toolbar = findViewById(R.id.toolbar);
		setSupportActionBar(toolbar);

		mContainerView = findViewById(R.id.containerView);

		loadContent();
		analyse();
	}

	/**
	 * Create a menu to appear in the action bar
	 *
	 * @param menu The menu to create
	 */
	@Override
	public boolean onCreateOptionsMenu(Menu menu){

		MenuInflater inflater = getMenuInflater();

		inflater.inflate(R.menu.menu_main, menu);

		return super.onCreateOptionsMenu(menu);
	}

	/**
	 * Detect and handle when an option is selected from the menu
	 *
	 * @param item	The item selected
	 * @return	True if handled correctly, false if not
	 */
	@Override
	public boolean onOptionsItemSelected(MenuItem item){
		switch(item.getItemId()){
			case R.id.action_random:	// The Randomise option
				genConfig();	// regenerate the configuration
				return true;

			default:
				return super.onOptionsItemSelected(item);	// returns false
		}
	}

	/**
	 * Randomise the configuration
	 */
	public void genConfig(){
		Random random = new Random();	//Create a new randomiser object

		// set the seed to current time to ensure different values each run
		random.setSeed(System.currentTimeMillis());

		// create the heightmap if necessary
		mHeightmap = new int[mMaxWidth];

		mLayout = new char[mMaxHeight][mMaxWidth];

		// randomly generate the values (between 1 and max height)
		for(int i = 0; i < mMaxWidth; i++)
			mHeightmap[i] = random.nextInt(mMaxHeight) + 1;

		// show the configuration
		showConfig();

		analyse();	// analyse the volume
	}

	protected void showConfig(){
		// show the configuration
		TextView textview = findViewById(R.id.textConfiguration);

		StringBuilder config = new StringBuilder("Your configuration is: ");

		for(int i = 0; i < mHeightmap.length; i++){
			config.append(String.valueOf(mHeightmap[i]));

			if(i < mHeightmap.length - 1)
				config.append(" | ");
		}

		textview.setText(config.toString());

		for(int height = mMaxHeight; height >= 1; height--){

			for(int j = 0; j < mMaxWidth; j++)
			{
				if(mHeightmap[j] >= height)
					mLayout[mMaxHeight - height][j] = '#';
			}
		}
	}

	/**
	 * Initial creation of the configuration.
	 */
	protected void loadContent(){
		mHeightmap = new int[mMaxWidth];        // gives volume of 10

		// Set everything to 1 to begin with
		for(int i = 0; i < mMaxWidth; i++)
			mHeightmap[i] = 1;

		mHeightmap[0] = 2;
		mHeightmap[1] = 5;
		mHeightmap[2] = 1;
		mHeightmap[3] = 2;
		mHeightmap[4] = 3;
		mHeightmap[5] = 4;
		mHeightmap[6] = 7;
		mHeightmap[7] = 7;
		mHeightmap[8] = 6;

		/*heightmap = new int[]{	// gives volume of 3
				2, 1, 1, 2, 3, 4, 7, 5, 6
		};*/

		/*heightmap = new int[]{	// should be 15
				5, 4, 7, 2, 3, 4, 7, 5, 7
		};*/

		mLayout = new char[mMaxHeight][mMaxWidth];

		showConfig();
	}

	/**
	 * Analyse the configuration for the volume of water it can hold.
	 *
	 * Models water filling up.
	 *
	 * For each height, find an instance of a wall at that height, then find two larger walls either side
	 * Fill the space between those two walls with water up to the current height + 1, adding to the volume
	 *
	 * Repeat until the highest wall is analysed, then report the final volume
	 *
	 */
	protected void analyse(){
		int volume = 0;

		// work upwards, from smallest wall to largest.  In both directions, find the next wall equal to and higher, then fill

		// find the highest and lowest walls
		int highest = Integer.MIN_VALUE;
		int lowest = Integer.MAX_VALUE;

		for (int aHeightmap : mHeightmap) {
			if (aHeightmap > highest)
				highest = aHeightmap;
			if (aHeightmap < lowest)
				lowest = aHeightmap;
		}

		// If the walls are the same size, return 0 as the only value
		if(highest == lowest){
			Log.i("RESULT", "Volume = 0");

			TextView textview = findViewById(R.id.textResult);

			textview.setText("Calculated volume is 0");

			return;
		}

		// Now we have the highest and lowest, find the volume
		for(int height = lowest; height <= highest; height++){

			// find an instance of the current height
			for(int j = 0; j < mMaxWidth; j++){
				if(mHeightmap[j] == height){
					// skip this height if it's the same as the last height (avoids filling twice)
					if(j >= 1 && mHeightmap[j] != mHeightmap[j - 1]){
						Log.d("ANALYSE", "Found an instance of " + String.valueOf(height) + " at " + String.valueOf(j));

						// look right for a higher wall
						for(int right = j + 1; right < mMaxWidth; right++){
							if(mHeightmap[right] > height){
								Log.d("ANALYSERIGHT", "Found a wall at " + String.valueOf(right));

								// look left for a higher wall
								for(int left = j - 1; left >= 0; left--){
									if(mHeightmap[left] > height){// fill up between walls at left and right
										Log.d("ANALYSELEFT", "Found a wall at " + String.valueOf(left));

										// We need to add to the volume the amount that will fill up to the lowest wall
										int add;

										Log.d("ANALYSIS", String.valueOf(mHeightmap[left]) + " to " + String.valueOf(mHeightmap[right]));

										if(mHeightmap[right] < mHeightmap[left])
											add = mHeightmap[right] - height;
										else
											add = mHeightmap[left] - height;

										Log.d("ANALYSIS", "add = " + String.valueOf(add));

										for(int h = height; h < Math.min(mHeightmap[left], mHeightmap[right]); h++) {
											for (int k = left + 1; k < right; k++)
												mLayout[(mMaxHeight - 1) - h][k] = 'w';
										}

										volume += add * ((right - left) - 1);
										left = 0;	// break the left looking loop
									}
								}
								right = mMaxWidth;	// break the right-looking loop
							}
						}
					}
				}
			}
		}

		// report the final volume
		Log.i("RESULT", "Volume is " + String.valueOf(volume));

		TextView textview = findViewById(R.id.textResult);

		textview.setText("Calculated volume is " + String.valueOf(volume));

		mContainerView.setLayout(mLayout);
		mContainerView.postInvalidate();
	}
}
