package com.guildon.watercontainer

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View

/**
 * Created by Chris Allen on 14/Sep/2018.
 *
 * Kotlin version of the View which displays the wall configuration,
 * and the water it stores, graphically.
 */
class WaterContainerView(context: Context, attrs: AttributeSet) : View(context, attrs) {

	/**
	 * Paint object used to draw a grid
	 */
	private val linePaint = Paint()

	/**
	 * Paint object used to draw a wall
	 */
	private val wallPaint = Paint()

	/**
	 * Paint object used to draw water
	 */
	private val waterPaint = Paint()

	/**
	 * Width of the layout
	 */
	private var layoutWidth: Int = 0

	/**
	 * Height of the layout
	 */
	private var layoutHeight: Int = 0

	/**
	 * Layout of the walls and water
	 */
	private lateinit var layout: Array<Char>

	/**
	 * Width and height of a "cell" in the layout
	 */
	private val cellSize = 20.0f

	init {
		linePaint.color = Color.BLACK
		linePaint.strokeWidth = 2.0f

		wallPaint.color = Color.GRAY
		waterPaint.color = Color.CYAN
	}

	/**
	 * Set the layout to show in this View
	 *
	 * @param mLayout : The layout to show in the view
	 * @param mWidth : Width of the layout
	 * @param mHeight : Maximum height of the layout
	 */
	fun setLayout(mLayout: Array<Char>, mWidth: Int, mHeight: Int) {
		layout = mLayout

		layoutWidth = mWidth
		layoutHeight = mHeight
	}


	override fun onDraw(canvas: Canvas) {

		// Pass through the layout, rendering each cell
		for (i in 0 until layoutHeight) {
			for (j in 0 until layoutWidth) {
				when (layout[((layoutHeight - 1 - i) * layoutWidth) + j]) {
					'#' -> canvas.drawRect(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize, wallPaint)
					'w' -> canvas.drawRect(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize, waterPaint)
				}
			}
		}

		// Draw a grid to show each cell
		for (i in 1 until layoutHeight)
			canvas.drawLine(0.0f, i * cellSize, layoutWidth * cellSize, i * cellSize, linePaint)

		for (i in 0 until layoutWidth)
			canvas.drawLine(i * cellSize, 0.0f, i * cellSize, layoutHeight * cellSize, linePaint)
	}


}