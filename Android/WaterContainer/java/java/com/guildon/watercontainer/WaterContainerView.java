package com.guildon.watercontainer;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by Chris on 21/Apr/2018.
 * <p>
 * Java version of the View which displays the wall configuration,
 * and the water it stores, graphically.
 */
public class WaterContainerView extends View {

	/**
	 * Paint object used to draw a grid
	 */
	private final Paint linePaint = new Paint();

	/**
	 * Paint object used to draw a wall
	 */
	private final Paint wallPaint = new Paint();

	/**
	 * Paint object used to draw water
	 */
	private final Paint waterPaint = new Paint();

	/**
	 * Layout of the walls and water
	 */
	private char[] layout;

	/**
	 * Width of the layout
	 */
	private int layoutWidth = 0;

	/**
	 * Height of the layout
	 */
	private int layoutHeight = 0;

	/**
	 * Width and height of a "cell" in the layout
	 */
	private final int cellSize = 20;


	public WaterContainerView(Context context, AttributeSet set) {
		super(context, set);

		wallPaint.setColor(Color.GRAY);
		waterPaint.setColor(Color.CYAN);
		linePaint.setColor(Color.BLACK);

		linePaint.setStrokeWidth(2);
	}

	/**
	 * Set the layout to show in this View
	 *
	 * @param mLayout : The layout to show in the view
	 * @param mWidth  : Width of the layout
	 * @param mHeight : Maximum height of the layout
	 */
	public void setLayout(char[] mLayout, int mWidth, int mHeight) {
		layout = mLayout;

		layoutWidth = mWidth;
		layoutHeight = mHeight;
	}


	@Override
	protected void onDraw(Canvas canvas) {

		if (layout == null)
			return;

		// Pass through the layout, rendering each cell
		for (int i = 0; i < layoutHeight; i++) {
			for (int j = 0; j < layoutWidth; j++) {
				switch (layout[((layoutHeight - 1 - i) * layoutWidth) + j]) {
					case '#':
						canvas.drawRect(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize, wallPaint);
						break;
					case 'w':
						canvas.drawRect(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize, waterPaint);
						break;
				}
			}
		}

		// Draw a grid to show each cell
		for (int i = 1; i < layoutHeight; i++)
			canvas.drawLine(0.0f, i * cellSize, layoutWidth * cellSize, i * cellSize, linePaint);

		for (int i = 0; i < layoutWidth; i++)
			canvas.drawLine(i * cellSize, 0.0f, i * cellSize, layoutHeight * cellSize, linePaint);
	}
}
